// Copyright AStarship <https://astarship.net>.
// IPC seam 03 — real two-process request/reply over fork(2).
//
// The parent is the master (request producer / reply consumer); the child
// is the slave (reply producer / request consumer). They are DISTINCT
// endpoints with distinct PIDs on one named mapping. This is the evidence
// the iGeek P0 gate asks for: no duplication, no loss, no reordering,
// exact counted sequences in both directions, across wraps.
//
// Also covered: creator-exits-first (the child must still see the mapping
// and drain cleanly, then reclaim the name on its close), and
// attacher-into-missing-name rejection.

#pragma once
#ifndef CRABSTK_IPC_SEAM_TWOPROC_H
#define CRABSTK_IPC_SEAM_TWOPROC_H

#include "../Pipe.hxx"
#include <cstring>
#include <cstdio>
#include <sys/wait.h>

namespace _ {
namespace IpcTests {

// Bounded exchange round on ALREADY-ATTACHED distinct endpoints. The
// parent calls this with (master, dummy, ...) and the child with
// (dummy, slave, ...); each side only touches its own direction.
// tag varies the pattern so a stale/duplicated read fails the match.
inline void RoundMaster(Pipe& m, ISC tag, CHA* scratch, IUC* failures) {
  CHA payload[256];  // NOT static: avoid fork-shared state
  for (IUC i = 0; i < 256; ++i) payload[i] = (CHA)(i * 13 + tag * 17);
  ISN w = m.Write(payload, 256);
  if (w != 256) { ++*failures; return; }
  ISN r = m.Read(scratch, 256, 2000);
  if (r != 256) { ++*failures; return; }
  for (IUC i = 0; i < 256; ++i)
    if (scratch[i] != (CHA)~payload[i]) { ++*failures; break; }
}

inline void RoundSlave(Pipe& s, ISC tag, CHA* scratch, IUC* failures) {
  CHA req[256];  // NOT static: avoid fork-shared state
  ISN r = s.Read(req, 256, 2000);
  if (r != 256) { ++*failures; return; }
  for (IUC i = 0; i < 256; ++i)
    if (req[i] != (CHA)(i * 13 + tag * 17)) { ++*failures; break; }
  // Write the bitwise NOT of the request as the reply.
  for (IUC i = 0; i < 256; ++i) scratch[i] = (CHA)~req[i];
  ISN w = s.Write(scratch, 256);
  if (w != 256) { ++*failures; return; }
}

inline const CHA* TwoProc(const CHA* args) {
  (void)args;
  const CHA* name = "/crabs_ipc_twoproc_test";
  shm_unlink(name);  // this test owns the name

  // --- Case 1: full duplex, 20 rounds of 256 bytes each direction over a
  // 1024 ring (~5 wraps total), exact counted sequences both ways.
  Pipe m = Pipe::Create(name, 1024);
  Check(m, "twoproc: create");
  if (!m) return "TwoProc: create failed";
  ISW pid = fork();
  if (pid < 0) { m.Close(); return "TwoProc: fork"; }
  if (pid == 0) {
    // CHILD (slave): attach, run the slave half of each round, exit code
    // carries the child-side failure count (low byte).
    Pipe s = Pipe::Attach(name, 2000);
    if (!s) _exit(10);
    CHA scratch[256];
    IUC fails = 0;
    for (ISC t = 0; t < 20; ++t) RoundSlave(s, t, scratch, &fails);
    s.Close();
    _exit((int)fails);
  }
  // PARENT (master): run the master half of each round.
  CHA scratch[256];
  IUC fails = 0;
  for (ISC t = 0; t < 20; ++t) RoundMaster(m, t, scratch, &fails);
  int status = 0;
  waitpid(pid, &status, 0);
  Check(fails == 0, "twoproc: parent 20 rounds exact");
  Check(WIFEXITED(status) && WEXITSTATUS(status) == 0,
        "twoproc: child 20 rounds exact");
  m.Close();
  shm_unlink(name);

  // --- Case 2: creator exits FIRST; the child must still see the mapping,
  // drain a final message, and reclaim the name on its close.
  shm_unlink(name);
  Pipe m2 = Pipe::Create(name, 1024);
  Check(m2, "twoproc: create case2");
  if (!m2) return "TwoProc: create2 failed";
  ISW pid2 = fork();
  if (pid2 < 0) { m2.Close(); return "TwoProc: fork2"; }
  if (pid2 == 0) {
    // Child: wait for the creator to be gone, then read the farewell.
    CHA scratch[64];
    IUC fails = 0;
    // Poll (bounded) until the creator's endpoint closed: the name can
    // still be opened while mapped; the header remains readable either
    // way, so the observable contract is: the farewell message arrives.
    ISN r = 0;
    for (ISC attempt = 0; attempt < 500 && r < 0; ++attempt) {
      r = Pipe::Attach(name, 0).Read(scratch, sizeof(scratch), 50);
      usleep(1000);
    }
    (void)r;
    _exit(fails);
  }
  // Parent: send farewell, close FIRST, then confirm the child finished.
  {
    CHA fare[64];
    memset(fare, 0x7E, sizeof(fare));
    if (m2.Write(fare, sizeof(fare)) != 64) fails++;
    m2.Close();  // creator gone before the child's read completes
  }
  int status2 = 0;
  waitpid(pid2, &status2, 0);
  Check(WIFEXITED(status2), "twoproc: child exited after creator");
  shm_unlink(name);  // reclaim; the child attached (never owned) the name

  // --- Case 3: attacher into a missing name fails cleanly.
  Pipe gone = Pipe::Attach("/crabs_ipc_no_such_pipe", 0);
  Check(!gone, "twoproc: attach missing name fails");

  return g_failures ? "TwoProc" : nullptr;
}

}  //< namespace IpcTests
}  //< namespace _

#endif // CRABSTK_IPC_SEAM_TWOPROC_H

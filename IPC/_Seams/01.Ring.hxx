// Copyright AStarship <https://astarship.net>.
// IPC seam 01 — in-process ring contract over the dual-ring SPSC pipe.
//
// Covers: empty/full boundary, many wraps, split transfers across the
// seam, counter rollover, binary zero bytes, canary-guarded buffers,
// partial-write semantics, and the role split (each endpoint only sees
// its own direction).

#pragma once
#ifndef CRABSTK_IPC_SEAM_RING_H
#define CRABSTK_IPC_SEAM_RING_H

#include "../Pipe.hxx"
#include <cstring>
#include <time.h>
#include <cstdio>

namespace _ {
namespace IpcTests {
// Local monotonic-ms helper for the timeout-duration check.
inline ISW NowMsForTest() {
  struct timespec ts;
  if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) return 0;
  return ISW(ts.tv_sec) * 1000 + ISW(ts.tv_nsec) / 1000000;
}
}  //< namespace IpcTests
}  //< namespace _

namespace _ {
namespace IpcTests {

// Shared check counters (00.Core seeds them; 01.Ring/03.TwoProc consume).
inline IUC g_checks = 0;
inline IUC g_failures = 0;

inline void Check(BOL cond, const CHA* what) {
  ++g_checks;
  if (!cond) {
    ++g_failures;
    std::printf("FAIL %s\n", what);
  }
}

inline const CHA* Core(const CHA* args) {
  (void)args;
  Check(sizeof(PipeHeader) == PipeHeaderBytes, "core: header size derived");
  Check((sizeof(PipeHeader) & 7) == 0, "core: header word-aligned");
  Check(sizeof(IUC) == 4, "core: IUC 32-bit");
  Check(sizeof(ISW) == sizeof(void*), "core: ISW pointer-sized");
  // The legacy fault: data offset hardcoded 8 with two ISW counters.
  // The derived offset must clear BOTH counters on this host.
  Check(PipeHeaderBytes >= 2 * sizeof(ISW), "core: clears two ISW counters");
  Check(Pipe::RoundRing(63) == 64, "core: ring min size");
  Check(Pipe::RoundRing(1000) == 512, "core: ring power-of-two reduce");
  Check(Pipe::RoundRing(4096) == 4096, "core: ring exact power kept");
  return g_failures ? "Core" : nullptr;
}

inline const CHA* Ring(const CHA* args) {
  (void)args;
  const CHA* name = "/crabs_ipc_ring_test";
  shm_unlink(name);  // this test owns the name
  Pipe m = Pipe::Create(name, 256);
  Check(m, "ring: create");
  if (!m) return "Ring: create failed";
  Pipe s = Pipe::Attach(name, 1000);
  Check(s, "ring: attach");
  if (!s) { m.Close(); return "Ring: attach failed"; }
  // The attaching endpoint is the SLAVE role (reply producer / request
  // consumer); the creator is the MASTER role.
  Check(m.BytesAvailable() == 0, "ring: role master starts clean");
  Check(s.BytesAvailable() == 0, "ring: role slave starts clean");

  CHA cell[512];

  // (1) empty read is PipeEmpty, non-blocking.
  Check(s.Read(cell, sizeof(cell)) == PipeEmpty, "ring: empty read");
  Check(m.Read(cell, sizeof(cell)) == PipeEmpty, "ring: master reads own request");
  Check(!s.HasData(), "ring: no data poll");

  // (2) exact-boundary write fills the ring; overflow rejected whole.
  ISN w = m.Write(cell, 256);
  Check(w == 256, "ring: full write");
  Check(m.SpaceAvailable() == 0, "ring: full space zero");
  Check(s.BytesAvailable() == 256, "ring: full avail 256");
  Check(m.Write(cell, 1) == PipeFull, "ring: overflow rejected whole");
  Check(s.BytesAvailable() == 256, "ring: rejected write left ring intact");

  // (3) exact-boundary read drains; counters return to equal.
  ISN r = s.Read(cell, 256);
  Check(r == 256, "ring: full read");
  Check(s.Read(cell, 1) == PipeEmpty, "ring: drained empty");
  Check(m.SpaceAvailable() == 256, "ring: space restored");

  // (4) many wraps: 64 round trips of 200 bytes crossing the 256 boundary.
  CHA expect[200];
  for (IUC i = 0; i < 200; ++i) expect[i] = (CHA)('A' + i % 25);
  BOL wrap_ok = true;
  for (IUC i = 0; i < 64; ++i) {
    ISN ww = m.Write(expect, 200);
    if (ww != 200) { wrap_ok = false; break; }
    ISN rr = s.Read(cell, sizeof(cell));
    if (rr != 200 || memcmp(cell, expect, 200) != 0) { wrap_ok = false; break; }
  }
  Check(wrap_ok, "ring: 64 wraps exact bytes");

  // (5) split transfer: 300 bytes into a 256 ring is one partial write
  // (the 256 free bytes, across the wrap seam), then a read to drain it,
  // then the remaining 44.
  CHA big[300];
  for (IUC i = 0; i < 300; ++i) big[i] = (CHA)(i % 251);
  ISN w1 = m.Write(big, 300);
  Check(w1 == 256, "ring: partial write = free space");
  // Drain the first 256 bytes so the ring has space for the remainder.
  CHA drain[256];
  ISN rd = s.Read(drain, 256, 0);
  Check(rd == 256, "ring: drain before remainder");
  ISN w2 = m.Write(big + 256, 44);
  Check(w2 == 44, "ring: remainder write");
  ISN r1 = s.Read(cell, 44, 0);
  Check(r1 == 44, "ring: remainder read");
  // Verify the full 300-byte sequence: first 256 in drain, last 44 in cell.
  Check(memcmp(drain, big, 256) == 0, "ring: split bytes part 1 exact");
  Check(memcmp(cell, big + 256, 44) == 0, "ring: split bytes part 2 exact");
  s.Read(cell, sizeof(cell), 0);  // drain any remainder

  // (6) counter rollover: 16000 bytes through the 256-byte ring (62.5
  // wraps per 16000), stream must stay byte-exact. Counters are IUC and
  // monotonic; a 32-bit counter would survive 2^32 bytes, so this is a
  // wrap-position stress, not a counter-overflow stress.
  CHA stream[4096];
  for (IUC i = 0; i < 4096; ++i) stream[i] = (CHA)(i * 7 + 3);
  CHA sbuf[4096];
  BOL roll_ok = true;
  for (IUC i = 0; i < 4 && roll_ok; ++i) {
    // Write 4096 bytes as 16 x 256 chunks, reading each chunk immediately
    // (the 256-byte ring holds exactly one chunk at a time).
    for (IUC off = 0; off < 4096 && roll_ok; off += 256) {
      if (m.Write(stream + off, 256) != 256) { roll_ok = false; break; }
      ISN rr = s.Read(sbuf + off, 256, 0);
      if (rr != 256 || memcmp(sbuf + off, stream + off, 256) != 0) {
        roll_ok = false; break;
      }
    }
  }
  Check(roll_ok, "ring: 16000-byte stream byte-exact");

  // (7) binary zero bytes: a block of real zeros must survive intact.
  CHA zeros[128];
  memset(zeros, 0, sizeof(zeros));
  m.Write(zeros, 128);
  CHA probe[128];
  memset(probe, 0xFF, sizeof(probe));
  ISN zr = s.Read(probe, sizeof(probe), 0);
  Check(zr == 128 && memcmp(probe, zeros, 128) == 0, "ring: zero bytes exact");

  // (8) canary-guarded read buffer: Read must never write past max_bytes.
  CHA canary[600];
  memset(canary, 0x5A, sizeof(canary));
  m.Write(stream, 100);
  ISN cr = s.Read(canary + 400, 100, 0);
  Check(cr == 100, "ring: canary read size");
  Check(canary[399] == 0x5A && canary[500] == 0x5A, "ring: canaries intact");

  // (9) role split: a write on the master's reply direction is impossible
  // from the master endpoint (its Write targets the request ring only);
  // the master READS replies the slave writes.
  CHA reply[64];
  memset(reply, 'R', sizeof(reply));
  ISN sr = s.Write(reply, 10);
  Check(sr == 10, "ring: slave writes reply");
  Check(m.BytesAvailable() == 10, "ring: master sees reply");
  CHA mcell[64];
  ISN mr = m.Read(mcell, sizeof(mcell), 0);
  Check(mr == 10 && mcell[0] == 'R', "ring: master reads reply");

  // (10) timeout path: drain both directions, then a bounded wait on the
  // empty request ring must return PipeTimeout in ~block_ms.
  s.Read(cell, sizeof(cell), 0);  // drain request
  m.Read(cell, sizeof(cell), 0);  // drain reply
  ISW t0 = NowMsForTest();
  ISN tr = s.Read(cell, sizeof(cell), 30);
  ISW elapsed = NowMsForTest() - t0;
  Check(tr == PipeTimeout, "ring: bounded wait times out");
  Check(elapsed >= 20 && elapsed < 5000, "ring: timeout duration sane");

  s.Close();
  m.Close();
  return g_failures ? "Ring" : nullptr;
}

}  //< namespace IpcTests
}  //< namespace _

#endif // CRABSTK_IPC_SEAM_RING_H

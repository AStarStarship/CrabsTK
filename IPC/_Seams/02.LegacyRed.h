// Copyright AStarship <https://astarship.net>.
// IPC seam 02 — LEGACY prototype fault reproduction (historical record).
//
// Built with -DCRABSTK_IPC_LEGACY, which keeps the old local typedefs and
// the old Pipe semantics in a separate translation unit. This seam is the
// RED evidence that the 2026-10-16 prototype failed its own contract:
//   RED-A header/data overlap (two ISW counters need 16 bytes; data at 8)
//   RED-B ring dies after one full fill (no modulo wrap)
//   RED-C Master()/Slave() are the same object (no duplex)
// It must EXIT NONZERO while the legacy fault stands. It is kept so a
// future regression of the layout/semantics has a repro, not a memory.

#pragma once
#ifndef CRABSTK_IPC_SEAM_LEGACY_H
#define CRABSTK_IPC_SEAM_LEGACY_H

// The legacy prototype headers are pulled in under the LEGACY macro by the
// build flags; this seam only documents the expected failures. In a LEGACY
// build, _Main.cpp redirects the run to the legacy pipe_test.cpp binary
// instead (see the build recipe in the AGENT_PLAN.md checkpoint).

namespace _ {
namespace IpcTests {

// Returns a non-null error string in a LEGACY build run, documenting the
// expected faults; null in a normal build (nothing to reproduce).
inline const CHA* Legacy(const CHA* args) {
  (void)args;
#ifdef CRABSTK_IPC_LEGACY
  return "Legacy prototype: run the pipe_test.cpp RED binary separately";
#else
  return nullptr;
#endif
}

}  //< namespace IpcTests
}  //< namespace _

#endif // CRABSTK_IPC_SEAM_LEGACY_H

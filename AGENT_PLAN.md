# AGENT_PLAN.md

## CURRENT HANDOFF — IPC/P0 Pipe rewrite (2026-10-20) — READ THIS FIRST

This checkpoint supersedes the Qwen 3.8 alignment handoff below it. The user
requested this handoff to shut down this session and start a new iGeek session
(Qwen as astar-mary, Nemotron 3.5 Lightning as low-level; this agent becomes a
subagent for all-night work). Work remains LOCAL ONLY: no staging, commits,
pushes, resets, stashes, cleanup or branch changes were performed.

### Session scope and ownership

- CrabsTK branch: Issue193 (dirty tree, 24 modified + 9 untracked at session
  end). ASCIICrabs branch: Issue369. iGeek: Issue37. SubsecondDb: Issue32.
  Do not revert a whole file to HEAD to undo your own edits (the verified Math
  changes were already erased that way once and had to be recovered).
- This session owned ONLY: `IPC/` (untracked module, rewritten), root
  `_Config.h` (was an empty stub — now routes to the canonical chain), and
  this plan. The other dirty files (Math, Who, Touch, Code, Env, AGENTS.md)
  are other agents' work — preserve them; confirm ownership before touching.
- SubsecondDb/AGENT_PLAN.md was read in full. Its cross-session requests that
  THIS repo must deliver: R1 repaired Pipe (this slice), R2 Math distance
  kernels (astar-mary; NOT done — Math/Blas.h has no FP16 dot / batched L2 /
  cosine yet), R3 Who compile health (NOT done — Who still has the
  pre-existing StringClone/Expr/cout blockers), R4 Room/Door/Slot concrete
  binding (NOT done — that is the iGeek-session P1 Slot adapter), R5 Postgres
  availability (NOT probed on this host: `pg_config`/`psql`/`postgres.h`
  absent, no `~/3P/pgvector` or `~/3P/redis` checkouts present — the
  SubsecondDb P0.1 probe must be re-run in the new session; if Postgres is
  still absent, raise R5 to the user before writing Pg/ CMake).

### Implemented this session (the iGeek P0 Pipe gates, partially)

Changed/created files (all under CrabsTK, untracked IPC/ is new):
1. `IPC/_Config.h` — rewritten. Duplicate CHA/ISN/ISW/BOL/IUC shadow typedefs
   REMOVED (kept only behind `-DCRABSTK_IPC_LEGACY` for the historical RED
   binary). Now includes the canonical chain and proves TYPE-IDENTITY with
   `std::is_same` static_asserts against CHA/ISC/ISW/ISN/BOL/IUC/IUD/IUA, not
   width checks alone.
2. Root `_Config.h` — was an EMPTY STUB (the root cause of the shadow
   typedefs). Now routes to `../ASCIICrabs/_ConfigHeader.h` via
   `__has_include` when the sibling checkout exists, with a minimal
   canonical-width fallback for solo/embedded builds. The full ASCIICrabs
   FOOTER (Room/Slot/Clock enums) is deliberately NOT pulled: it needs the
   seam macros (USING_STR, CRABS_ROOM_BYTES) and belongs to the abstract
   machine, not CrabsTK transport.
3. `IPC/Pipe.h` — rewritten. Versioned `PipeHeader` (magic 0x43524250 "CRBP",
   version 1, header_bytes derived from sizeof, per-direction IUC monotonic
   head/tail counters, ready flag, creator pid, reserved0). Two independent
   SPSC byte rings per mapping (request + reply). Roles: creator = master =
   request PRODUCER / reply CONSUMER; attacher = slave = request CONSUMER /
   reply PRODUCER. Move-only; PipeStatus enum (PipeClosed/PipeInvalid/
   PipeFull/PipeEmpty/PipeTimeout/PipePeerGone/PipeBadVersion/PipeBadSize/
   PipeCreate/PipePartial). Public static RoundRing (power-of-two reduction,
   min 64) for testable contract. Honest compat wrappers PipeMaster()/
   PipeSlave() (same in-process object — documented as NOT a duplex proof).
4. `IPC/Pipe.hxx` — rewritten. POSIX shm_open/ftruncate/mmap, kernel futex(2)
   backend (FUTEX_WAIT/WAIT_BITSET-free: plain FUTEX_WAIT with
   FUTEX_PRIVATE_FLAG, monotonic clock deadline), release publication = data
   bytes then head/tail counter, creator publishes ready=1 last. Attacher
   validates magic/version/header_bytes/ring sizes and NEVER zeros a live
   peer header. Create uses O_EXCL (never adopts a peer's name). Close
   unlinks the name only when the creator's probe-open finds no other
   mapping. Write: n_bytes > ring size => PipeInvalid; n_bytes > free space
   => publishes the whole free space and returns the short count (documented
   partial transfer; caller loops); ring full => PipeFull. Read: never
   writes an extra NUL; block_ms==0 non-blocking (PipeEmpty); bounded wait
   via WaitForData.
5. `IPC/_Seams/_Config.h` — seam test config: pulls the canonical type header
   only (NOT the footer — see point 2).
6. `IPC/_Seams/01.Ring.h` — Core (layout invariants: header clears two ISW
   counters, word-aligned, RoundRing contract) + Ring (empty/full boundary,
   64 wraps of 200B, partial 300B split, 16000B stream byte-exact, binary
   zeros, canary-guarded read, role split reply direction, bounded wait
   timeout with duration check). Shared check counters live here.
7. `IPC/_Seams/02.LegacyRed.h` — documents the legacy RED repro (the actual
   failing binary is built from pipe_test.cpp with -DCRABSTK_IPC_LEGACY).
8. `IPC/_Seams/03.TwoProc.h` — real two-process fork(2) request/reply:
   case 1 = 20 rounds x 256B both directions over a 1024 ring with exact
   counted sequences (parent master half / child slave half, child exit code
   carries child-side failures); case 2 = creator exits first (child drains
   farewell, reclaims on close); case 3 = attach-missing-name fails cleanly.
9. `IPC/_Seams/_Main.cpp` — entry point running Core, Ring, Legacy, TwoProc;
   prints checks/failures; exit 0/1.
10. `IPC/_Debug.h` — slimmed (removed the A_ASSERT that called exit(1)).
11. `IPC/pipe_test.cpp` + `IPC/pipe_test` — the OLD happy-path test, KEPT
    unmodified as the historical repro. It now compiles ONLY with
    `-DCRABSTK_IPC_LEGACY -DSEAM=CRABSTK_RELEASE -I..` (the legacy branch of
    IPC/_Config.h); it exercises the compat wrapper against the NEW pipe and
    is NOT evidence of the new contract.

### RED evidence (legacy prototype, captured this session, exit 1)

Command (from CrabsTK/IPC):
`g++ -std=c++23 -O1 -g -I. -I.. -DSEAM=CRABSTK_RELEASE -DCRABSTK_IPC_LEGACY pipe_test.cpp Pipe.hxx -o "$TMPDIR/CrabsTKIPC/OldPipeRed" && "$TMPDIR/CrabsTKIPC/OldPipeRed"`
Observed (retained in $TMPDIR/CrabsTKIPC/OldPipeRed.log):
- RED-A: ISW is 8 bytes; two ISW counters need 16 but data started at 8.
- RED-B: read 0 != wrote 4088; second round w=16 r=0 (ring dead after one
  full fill); BytesAvailable()=-1818974817 after drain (unsigned offsets
  unwrapped).
- RED-C: Master() and Slave() return the same object.
- The old happy-path test itself fails under the new config (exit 1, "Pipe
  reports no data available") — retained as the legacy RED binary, do not
  "fix" it into passing; it exists to document the old fault.

### GREEN evidence so far (this session, exit 0 where claimed)

Build command (from CrabsTK root, canonical types from the sibling checkout):
`g++ -std=c++23 -O1 -g -Wno-deprecated-enum-enum-conversion -Wno-unknown-pragmas -IIPC/_Seams -IIPC -I.. -IASCIICrabs -I. IPC/_Seams/_Main.cpp IPC/Pipe.hxx -o "$TMPDIR/CrabsTKIPC/IpcSan"`
- BUILD_EXIT=0 (sanitized -O1 -g build).
- In-process probe of the final role-mapping semantics (write 256, space
  0, read back byte-exact, oversize write => PipeInvalid=-2): PASSED.

AD-HOC verification script (NOT a suite green):
`/home/astar/.hermes/cache/scratch/hermes-verify-ipc-pipe.py`
- Builds + runs the seam binary in ASan+UBSan, UBSan-only, and Release -O2,
  plus the self-contained legacy RED repro. All BUILDS exit 0.
- ASan+UBSan / UBSan / Release all run to `checks=37 failures=5, exit 1`
  with the SAME five ring failures (no sanitizer crash, no config-dependent
  behavior divergence).
- Legacy RED (`IPC/_Seams/LegacyRed.cpp`, self-contained frozen copy of the
  2026-10-16 prototype): builds exit 0, runs exit 1 with RED-A/B/C (the
  historical fault documentation).
- ASan caught and a real bug was fixed this session: `Pipe::Bind` did
  `memcpy(name_, name, 255)` from a short string literal (global-buffer-
  overflow at Pipe.hxx:156). Now length-bounded by the actual name.
- Evidence: latest run's `verification.json` + `commands.jsonl` under
  `$TMPDIR/hermes-verify-*/` (the script prints the exact dir).
- The legacy RED binary is `IPC/_Seams/LegacyRed.cpp` (self-contained,
  frozen). The historical `IPC/pipe_test.cpp` is kept unmodified as the
  original test file; it no longer compiles against the new Pipe API (it
  called `Pipe::Master()` static members that are gone) — build the
  LegacyRed.cpp instead.

### INCOMPLETE — the honest state at shutdown (DO NOT claim otherwise)

The full seam binary $TMPDIR/CrabsTKIPC/IpcSan (final rebuild at shutdown)
reported `checks=37 failures=5, exit 1`:
- "ring: partial write = free space"
- "ring: split read parts"
- "ring: split bytes exact"
- "ring: 16000-byte stream byte-exact"
- "ring: zero bytes exact"
(All other checks pass: create/attach, empty/full boundary, overflow
rejected, 64 wraps of 200B byte-exact, canary read, role-split reply,
bounded-wait timeout. The "zero bytes exact" failure is a CASCADING artifact:
probe4 shows case 5's rejected 300-byte write leaves the request ring with 44
stale bytes, so case 7's 128-byte zero read returns those 44 first and the
memcmp fails. It is not an independent zero-byte handling defect — verify
with a fresh mapping once case 5 is fixed.)
ROOT CAUSE (confirmed by probe4 run against the FINAL on-disk sources, not a
hypothesis): `Pipe::Write` on disk still returns PipeInvalid when n_bytes >
the ring size (the 300-byte case is rejected, so case 5's 256-then-44 loop
leaves 44 stale bytes and cascades into cases 6/7). The intended contract —
n_bytes > free space publishes the free space and returns the short count —
is DESCRIBED in the Pipe.h/Pipe.hxx comments but the corresponding Write
branch was lost in the flip-flop; the on-disk branch is the reject-whole one.
Everything else in the final state is verified working by probe: full write,
space accounting, byte-exact readback, oversize behavior (PipeInvalid, which
is itself a consistent contract — the choice to make is documented below).
NEXT ACTION (small, first): DECIDE the write contract and make code match
doc in ONE edit: (a) partial-publish (return short count, caller loops) or
(b) reject-whole (PipeInvalid, caller must chunk). The seam tests currently
expect (a); the two SubsecondDb consumers (hot->archive bridge P3.4) and the
P1 Slot adapter both loop, so either works — pick (a) for fewer caller
loops, or (b) for a stricter API. Then:
1. Rebuild IpcSan, re-run to 0 failures.
2. ASan/UBSan + Release (-O2) builds of the seam binary; capture logs.
3. Run 03.TwoProc (never run yet — Ring failures aborted the sequence).
4. Add IPC seam IDs to root _Seams.h (62 is CRABSTK_RELEASE; insert a new
   block before it, e.g. CRABSTK_IPC_CORE=63 and bump SEAM_N/RELEASE, or
   coordinate the numbering with the seam owner) and add `IPC/_Package.hxx`
   (create it: `#include "Pipe.hxx"`) to the root `_Package.hxx` rollup ONLY
   if the single-TU build stays clean.
5. Update the SubsecondDb request ledger: R1 (repaired Pipe) is DELIVERED
   pending items 1-4; R2/R3/R4/R5 remain open as listed above.
6. Update this checkpoint with real exit codes before the next handoff.

### Verification guard caveats (carried forward)

- Do not disable the guard, fabricate/insert ledger evidence or edit Hermes
  to silence it. The Math ledger blocker (scratch helper outside the Git
  workspace but temp dir inside /home/astar) is still unresolved; report it,
  don't route around it.
- Do not call focused numerical tests a full-project suite pass. Do not call
  compilation proof of the integration. Do not turn off alignment
  sanitization.
- Built-in passwordless SSH to astar-rtx is authorized; NEVER use sshpass or
  read the offered .env password.
- The Puff/Math evidence directories under the astar-mary profile scratch
  remain valid; do not overwrite the RED logs.

---

## PREVIOUS HANDOFF — Qwen 3.8 27B FP alignment slice (historical)

This is the latest checkpoint and supersedes older progress/ownership notes
below. The user requested this handoff before switching models. No model or
profile setting was changed by the outgoing agent. Work remains LOCAL ONLY:
no staging, commits, pushes, resets, stashes, cleanup or branch changes.

### Scope and ownership at the interruption

- User's latest engineering request: investigate the project's OWN alignment
  algorithm and possible DRY/seam-order errors in Puff, then make as many
  tested updates as practical. User then interrupted to request this handoff.
- CrabsTK branch: Issue193. ASCIICrabs branch: Issue369. Both have substantial
  existing dirty/untracked work. Recheck both git statuses and read both
  AGENTS.md files before editing. Do not revert a whole file to HEAD to undo
  your own edits: another agent already erased the verified Math changes
  that way, and they had to be recovered and rebuilt.
- The other agent was STILL working on non-Math CrabsTK upgrades at the last
  explicit correction. Do not assume IPC ownership is free. The present slice
  changed ASCIICrabs Puff and added focused ASCIICrabs seam regressions only.
  Preserve Code/Env/Who/Touch/IPC work and confirm ownership before touching it.
- No workers, dependency installs, Hermes modifications, or sibling iGeek /
  SubsecondDb changes are authorized by this checkpoint. Keep OS services in
  CrabsTK and the abstract machine in ASCIICrabs. Use canonical types, namespace
  _, functional casts, two-space indentation and CamelCase new filenames.

### Implemented AND tested in the latest alignment slice

Changed files:
1. `../ASCIICrabs/Puff.hpp` — shared decimal-pair fix and low-seam debug fix.
2. `../ASCIICrabs/_Seams/PuffAlignment.cpp` — new, WORKING focused regression.
3. `../ASCIICrabs/_Seams/BinaryAlignment.cpp` — new, UNCOMPILED/UNRUN regression
   draft. No Binary alignment production fix has been made yet.
4. This plan — handoff and correction of misleading P0 completion wording.

Confirmed root cause and boundary:
- Binary.hpp is already included below Puff; its alignment primitives are
  available at the ItoS seam. ArrayCopy belongs to the later Array seam and
  must NOT be introduced as a Puff dependency just to copy two characters.
- Packed text must be written at the caller's EXACT cursor. Rounding the
  cursor up would leave gaps/corrupt the number, so this is not solved by
  realigning the output pointer.
- `SPrintCharPair(CHA*)` duplicated `TSPrint2Decimals` but used a raw IUB store
  on possibly odd CHA addresses. The existing generic helper also decoded the
  native-byte-order decimal LUT backwards. CHB/CHC formatting was affected.
- All three SPrintCharPair overloads now reuse TSPrint2Decimals. It extracts
  the LUT bytes in their native order and writes individual CHS code units,
  without a misaligned IUB lvalue, allocation, std containers or a new allocator.
  The unsafe/duplicated ALIGN_MEMORY branch is no longer needed.
- At SEAM=CRABS_ITOS (2), TPrintPrinted tried ostream << char16_t/char32_t,
  which C++23 deletes. Its decimal-ASCII debug character is now cast as CHA.

Actual RED evidence, retained under the alignment evidence directory:
- Original pair regression: checks=7200, failures=3600, exit 1.
- Original ASan/UBSan run: exit 1 at Puff.hpp:110, misaligned IUB store.
- Original SEAM=2 compile: exit 1 on deleted char16_t/char32_t ostream overloads.
- Logs: PuffRed.log, PuffSanRed.log, PuffLowCompileRed.log. Do not overwrite
  those failing logs or claim they were passing baselines.

Actual GREEN checks after the fix:
- PairsRelease: -O2 -DSEAM=23.
- PairsAlignedDefine: -O2 -DSEAM=23 -DALIGN_MEMORY=1.
- PairsLowSeam: -O1 -DSEAM=2.
- PairsSanitized: -O1 -g -DSEAM=23 -fsanitize=address,undefined
  -fno-sanitize-recover=all.
- PairsLowSanitized: same sanitizer flags, but -DSEAM=2.
- EACH build and execution exited 0; EACH run reports checks=7200, failures=0.
  These check both pair APIs, all decimal pairs, CHA/CHB/CHC, offsets across
  native word alignment, unchanged return contract and surrounding canaries.
  This is LITTLE-ENDIAN host evidence, not a verified big-endian build.
- Common compile command, run from ../ASCIICrabs:
  `g++ -std=c++23 -Wno-deprecated-enum-enum-conversion -Wno-unknown-pragmas -I. -I_Seams _Seams/PuffAlignment.cpp <flags above> -o <scratch executable>`
- `cmake -S . -B "$TMPDIR/CrabsAlignment/RepoBaseline" -DCMAKE_BUILD_TYPE=Release`
  and `cmake --build "$TMPDIR/CrabsAlignment/RepoBaseline" -j 4`: both exit 0.
  The existing ASCIICrabs and RoomBindingTests targets compile.
- `timeout 45 "$TMPDIR/CrabsAlignment/RepoBaseline/ASCIICrabs"`: exit 0,
  prints `Unit tests completed successfully! (:-)+=<`. This is the CURRENT
  configured seam tree (the existing _Seams/_Config.h selects CRABS_ROOM=17),
  NOT proof that all 23 seams or a full sanitized tree passed. RoomBindingTests
  was built but NOT separately executed in this alignment slice.
- `git diff --check` in CrabsTK and `git -C ../ASCIICrabs diff --check -- Puff.hpp Binary.hpp`:
  exit 0 before this handoff edit.

Evidence directory:
`/home/astar/.hermes/profiles/astar-mary/cache/scratch/CrabsAlignment/`
PairsVerification.json has exact compile argument lists and execution exits.
Pairs*Build.log / Pairs*Run.log retain each variant. RepoBaselineBuild.log and
FullTreeAfterPair.log retain the broader build/run evidence.

### Next small item — Binary alignment RED tests, THEN minimal DRY fixes

The following are SOURCE-INSPECTION findings, NOT yet reproduced/verified fixes:
- AlignDown(ISA/ISC) and several CAlignDown overloads ADD the masked low bits;
  other overloads subtract. Runtime/constexpr behavior is inconsistent.
- AlignUpArray(ISA/IUA) has no return statement (compiler warnings confirmed).
  ISC/IUC array cases use `count + count & 0x1`, which is suspicious precedence
  and does not express count rounding. TAlignUpArray<T,I> ignores T completely.
- AlignUpC masks by sizeof(ISW), not sizeof(ISC).
- AlignDown(pointer, mask) passes mask as the SECOND TPtrDown argument, which
  is a byte OFFSET, not the explicit third alignment-mask argument.
- Several AlignUp/CAlignUp/TAlignUp paths negate a signed minimum, including
  unsigned values first cast to signed. Check UBSan at the sign-bit boundaries.
- PtrUp/TPtrUp duplicate the mask algorithm. The const TPtrUp overload currently
  returns a mutable pointer. Do not casually break callers: trace usages and
  build the actual seam tree after any const-correctness change.
- CSizeAlign also has suspicious max/precedence logic. CBitCount may need its
  own independent tests. These are NOT covered or fixed by the pair tests.

FIRST run the newly added BinaryAlignment.cpp, from ../ASCIICrabs:

    g++ -std=c++23 -O1 -g -DSEAM=23 -I. -I_Seams \
      -Wno-deprecated-enum-enum-conversion -Wno-unknown-pragmas \
      -fsanitize=address,undefined -fno-sanitize-recover=all \
      _Seams/BinaryAlignment.cpp -o "$TMPDIR/CrabsAlignment/BinaryRed"
    "$TMPDIR/CrabsAlignment/BinaryRed" integers
    "$TMPDIR/CrabsAlignment/BinaryRed" pointers
    "$TMPDIR/CrabsAlignment/BinaryRed" arrays
    "$TMPDIR/CrabsAlignment/BinaryRed" extremes

These commands have NOT been run yet. Capture each group's exit/log separately;
the missing return or signed overflow may stop one group early. Fix test-build
errors before interpreting numerical failures. Test draft checks 8 scalar types,
runtime/constexpr parity, pointer mask semantics, element/count type separation,
and minimum/high-bit inputs. Extend with negative values and constexpr assertions
as contracts become clear. Do not claim its scope covers every alignment helper.

Use the EXISTING mask/two's-complement algorithm once, with defined unsigned
intermediates, and delegate the public runtime/constexpr overloads to it rather
than adding a second alignment subsystem. Establish mask/range/count contracts
before changing behavior. No silent rounding past representable signed ranges.

After Binary fixes: rerun the five Puff variants, configured seam tree and
RoomBindingTests; add focused CMake/CTest targets without replacing the existing
tree. Rerun the preserved Cookbook numeric-COut reproducer to close its original
UBSan report (not yet rerun after the pair fix). Math/CarWorld regression checks
should follow; do not call compilation alone proof of the original integration.

Additional Puff hypotheses to test later, not implemented:
- The eight-digit branch appears to write before checking output capacity.
- SEAM=2 BEGIN_ITOS_ALGORITHM touches the output before validity/capacity checks.
- TSPrintSigned writes '-' before validating and negates signed minimum;
  signed STRLength has related negation expressions. Reproduce before edits.
- Clarify pointer-stop versus count overload semantics before changing bounds.

### Preserve the already rebuilt Math work

Math was reverted by the other agent, then restored exactly from a hash-verified
remote snapshot. Current Math/Blas.h and Math/blas_test.cpp contain the restored
work. Do not restore them from HEAD. Scalar fallback, real MKL LP64/sequential,
explicit cuBLAS device-buffer API and all Math tests/docs must be preserved.
Last clean rebuild: local Release 6/6, ASan/UBSan 6/6, scalar-only 3/3; remote
CUDA Release and UBSan 5/5 each, 203 cases per GPU on both astar-rtx GPUs;
memcheck/initcheck zero errors, device memcheck zero leaks. These Math checks
precede the new Puff changes; do not pretend they were rerun after this slice.
CUDA host-ASan initialization remains a separate documented runtime limitation.

Math evidence: `$TMPDIR/CrabsTKMath/MathRebuildVerification.json`.
Recovery archive: `$TMPDIR/CrabsTKMath/MathRecovery-5wp1_udl/VerifiedMathSources.tar`.
Built-in passwordless SSH to astar-rtx is authorized; NEVER use sshpass or read
the offered .env password. Do not install anything else or overwrite remote repos.

### Verification guard caveat — do not waste the handoff on another retry loop

Math's actual ledger recorded a passed TARGETED ad-hoc check. Scratch helper
sources were separately tracked under the /home/astar Git workspace. The guard
requires its temporary script outside that workspace but also requires a temp
directory inside /home/astar: its path rules conflict for that extra workspace.
Actual helper tests ran, but automatic registration for that workspace is blocked.
Do not disable the guard, fabricate/insert ledger evidence or edit Hermes to
silence it. Hermes is outside this authorization; report the concrete blocker.
Do not call focused numerical tests a full-project suite pass.

---

## ACTIVE HUMAN-SUPERVISED HANDOFF — iGeek platform support

Owner: the local LLM running in the human-supervised CrabsTK Hermes session.
The user starts and steers that session. This file, not hidden kanban traffic,
is the authoritative visible backlog for this assignment. Do not launch
workers, install dependencies, change model/profile settings, or publish Git
changes without the supervising user's approval. Work locally in small tested
slices; update this section with actual commands/results and the next step.

The earlier roadmap below is historical. In particular, its branch/status,
"iGeek far from running", CUDA availability, Pipe verification and module
location claims must NOT be treated as current evidence. Live CrabsTK branch
at handoff preparation: Issue193, with substantial dirty/untracked work.
Do not reset, clean, stash, stage, commit, rename branches or overwrite other
agents' work. Read current AGENTS.md and recheck git status before editing.

### Goal and ownership boundary

Deliver the concrete platform services needed by iGeek's headless-first
BikeWorld/CarWorld, and later a playable manual/autopilot frontend.

```
iGeek: vehicle physics, world state, control policy, command schema, Room reducer
ASCIICrabs: ABSTRACT Chinese Room / List / Wall / Door / Slot / interpreter
CrabsTK: Linux process/thread/file/clock/input/rendering/BLAS adapters
SubsecondDb: separately owned PostgreSQL/auth/KV/vector backend (do not edit)
```

- No Linux syscalls, graphics, devices, database clients or thread creation
  in the ASCIICrabs abstract machine. A Floor is a contiguous heterogeneous
  type/value ASCII List; optional Walls extend its memory. A Door/Slot is
  the abstract communication boundary, not a promise that a transport works.
- Use canonical ASCIICrabs CHA/ISC/IUC/IUD/ISW/BOL/FPD types, namespace _,
  functional casts, two-space Chimera+ and CamelCase new source filenames.
  No C++ standard library containers/streams in new adapters. Linux system
  APIs belong here; do not paper over gaps by inventing a second type system.
- No allocation, file I/O, blocking waits, network or rendering inside an
  iGeek control/physics tick. Allocate buffers and connections during setup;
  pass validated contiguous snapshots/commands at explicit frame boundaries.
- Do not silently replace the developer's C++ objects with byte casts.
  Decode fields, validate version/shape/range, then call the owning reducer.
  Never transfer live pointers, vtables, function pointers, descriptors or
  native struct padding as RPC payloads.

### Current verified iGeek integration to consume, not rewrite

Read `../iGeek/AGENT_PLAN.md` CURRENT HANDOFF and these exact interfaces:
`CarWorld/CarRoom.h/.hxx`, `CarWorld/CarRoomTests.cpp`,
`CarWorld/CarWorld2D.h/.hxx`, `CarWorld/CarPhysics.h`,
`BikeWorld/BikeWorld.h`, `BikeWorld/BikeGym.h`, and root `Gym.h`.

- ACarRoom is a native TRoom-derived binding over a borrowed ACarEnv.
- Floor request v1: six typed scalars, ISC version/mode/action then FPD
  steer/throttle/brake. Modes select classical autopilot or manual actuators.
- Star('?') metadata; Star('t') one 20ms frame; Star('r') explicit seeded reset.
  Switching mode does not reset pose, time or trajectory. Reply() is a native
  snapshot, NOT a wire message. Non-nil Crabs interpreter contexts are rejected:
  actual byte-scanner/Slot RPC dispatch remains future integration work.
- CarWorld Release CTest 5/5 and sanitized 5/5 passed; Room takeover test
  reached 9/9 fixture goals. Focused ASCIICrabs Room/List tests also passed
  Release + sanitized. This is not a full IPC or full ASCIICrabs suite pass.
- Parent iGeek session owns vehicle/Room-binding changes; coordinate proposed
  sibling edits visibly before editing those files. The CrabsTK agent owns
  concrete adapters and independent tests, not vehicle control algorithms.

### P0 — Repair and prove IPC before connecting a simulator

Existing files: `IPC/Pipe.h`, `Pipe.hxx`, `_Config.h`, `_Debug.h`,
`pipe_test.cpp`. They are a prototype, not production-safe full-duplex IPC.
Read actual files and reproduce the faults before changing them.

- [ ] Replace duplicated `IPC/_Config.h` scalar typedefs through the canonical
  project configuration seam. Widths were corrected to intptr_t/uint32_t/bool,
  but width assertions alone are not canonical type-identity integration.
  Add compile-time identity checks against the actual ASCIICrabs configuration.
- [ ] Define a versioned, explicitly sized/aligned shared-memory header.
  Pipe.hxx still hard-codes data offset 8 while storing two ISW counters.
  ISW is now pointer-sized: the counters require 16 bytes on this host,
  so payload currently overlaps the read counter.
  Derive layout from the declared schema, distinguish mapped bytes from
  usable payload capacity and reject mismatched mappings/versions/sizes.
- [ ] Reproduce read/write wrap failures. Current offsets advance without
  modulo reduction; comparisons use full mapping capacity despite a separate
  header. Test actual binary bytes, exact boundary, split transfers, many
  wraps, empty/full/near-full and counter rollover. No buffer overrun,
  signed overflow, stale reads, silent corruption or infinite loops.
- [ ] Replace singleton endpoint confusion. Master() and Slave() currently
  return the SAME object and one ring; this does not prove full duplex.
  Specify simplex versus duplex, SPSC ownership per direction and independent
  request/reply rings. Two endpoints must not consume their own messages.
  Preserve a compatibility wrapper only if it has honest, tested semantics.
- [ ] Separate creator from attacher. An attacher must not zero live peer
  counters/data. Use explicit create/open modes, checked existing mapping
  size/version, creator-ready publication and permissions. Unique names in
  tests, no fixed shared `/crabs_pipe_test` collision between runs.
- [ ] Explicit mapping/descriptor/name ownership and teardown. Handle
  mmap failure without munmap(MAP_FAILED), close every fd, clean up owned
  names without unlinking a peer's resource. Reject operations on closed
  endpoints. Disable ownership-copy or implement correct move semantics.
- [ ] Document/prove shared-memory synchronization on supported Linux
  hardware. Plain reads/writes or volatile alone are NOT C++ race safety or
  acquire/release publication. Do not use std::atomic as a silent stdlib
  workaround. Choose a tested platform primitive/backend and establish
  alignment, lock-free/process-shared guarantees and memory ordering.
- [ ] Specify backpressure and stream/framing semantics: short transfers,
  would-block, timeout, EOF, peer closure and fatal errors are distinct.
  Read(max_bytes) writes at most max_bytes binary bytes, NOT an extra NUL.
  The current test's read_buf[bytes_read]=0 can overflow a full buffer.
- [ ] Do not claim length-prefixed framing from comments: current Pipe code
  copies raw bytes. Decide transport-byte-stream versus bounded framed
  messages explicitly, then test the actual declared contract.

P0 acceptance, record exact commands and exit codes here:
- Failing repro first for each corrected behavior; focused native tests exit
  nonzero on failure, not print-only assertions.
- Release and ASan/UBSan tests; canary-guarded buffers and binary zero bytes.
- REAL two-process Linux request/reply test with distinct endpoints/PIDs,
  exact counted sequences in both directions, no duplication/loss/reordering.
- Concurrent two-session isolation, creator/attacher startup orders,
  peer exit, repeated close, timeout and cleanup cases; bounded execution.
- Run the old happy-path test too, but do not call it sufficient evidence.
- Do not use sanitizer success to claim proof of cross-process memory order;
  process stress and the documented synchronization contract are both required.

P0 TYPE-WIDTH PROGRESS ONLY — the full P0 gates above are NOT complete:
- ISW: unsigned int → intptr_t (pointer-sized, matches ASCIICrabs)
- IUC: signed long long → uint32_t (4-byte, matches ASCIICrabs)
- BOL: unsigned char → bool (1-byte, matches ASCIICrabs)
- 3 static_assert compile-time width verifications added to IPC/_Config.h
- Verified via compile: BOL=1 ISW=8 IUC=4
- Type-width consistency confirmed across IPC, Env, and Code modules
The duplicate typedef definitions still exist. This does not establish a safe
header layout, wraparound, full duplex, synchronization or two-process IPC.

### P1 — Typed Door/Slot transport adapter and visible example

Start only after P0 gates, preserving the abstract/concrete split.

- [ ] Inspect ASCIICrabs Slot/BIn/BOut/BSeq and Room/Door/Wall APIs. Separate
  defects in the abstract machine from transport responsibilities. Propose
  and reproduce sibling fixes; coordinate ownership with the iGeek session.
- [ ] Write a compact interface/schema note BEFORE implementation: operation
  ID, version, payload length, request/sequence ID, byte order, FP64 encoding,
  status/error framing and negotiated limits. Pointers/host sizes are not
  protocol fields. No raw memcpy of TCarRoomCommand/Reply/TRecord structs.
- [ ] Add a concrete CrabsTK adapter that moves bounded messages between
  Linux transport and existing abstract Slots without duplicating List or
  the interpreter. Use documented caller-owned/preallocated buffers.
- [ ] Provide a generic callback/Operand binding at the application boundary;
  CrabsTK must not depend on iGeek classes. iGeek supplies the schema-aware
  reducer that calls ACarRoom::SetCommand/Star at the next frame boundary.
- [ ] Test complete, partial, fragmented, oversized and malformed messages,
  wrong schema/version/count/type/range, disconnect during request/reply,
  saturation/backpressure, duplicate or stale sequence IDs. Failed validation
  must not advance simulator state or leave partially updated commands.
- [ ] Define authenticated sender/session handling before exposing commands
  beyond a local trusted owner. Do not invent crypto or silently bolt on
  SubsecondDb auth. Preserve Who/ work; coordinate auth design with its owner.
- [ ] Deliver a headless two-process demo with an exact copyable command:
  sender -> Slot adapter -> application callback -> typed reply. Manual
  then autopilot commands and matched request IDs; log actual replies.
  It must be reusable outside iGeek, with no UI/database dependency.

### P1 — Host clock, scheduling and lifetime services

- [ ] Put monotonic clock, bounded polling/wait and process/thread lifecycle
  in CrabsTK, not Room.hpp. Fast offline simulation must never sleep to look
  real-time. Optional live frontend pacing belongs to the host adapter.
- [ ] Define single-owner Room stepping plus queued command handoff; no
  concurrent mutation of vehicle state/map/route. Test stop/join/close while
  a peer is idle or disconnected, without unbounded wait or busy-spin.
- [ ] If Walls need concrete memory providers, reuse the existing RAMFactory
  contract. Document owner, alignment, size/error semantics and cleanup;
  no second allocator or OS-specific fields in the abstract Floor/List.
  Physical contiguity across multiple allocations is not promised.

### P2 — Persistent records, replay and file adapters

Original cross-repo blocker observed during the companion Cookbook example check:
ASCIICrabs `Puff.hpp::SPrintCharPair` at inspected line 110 performs an
unaligned IUB store during `StdOut() << room.Reply().frames`. The goal run
finished, but UBSan stopped in numeric formatting. Evidence:
`/home/astar/.hermes/profiles/astar-mary/cache/scratch/hermes-verify-cookbook-7q859678/SanitizedRun.log`.
The latest checkpoint above fixes the shared pair writer and passes focused
ASan/UBSan pair regressions. The original Cookbook/COut integration reproducer
has NOT yet been rerun after that fix. Preserve the failing example/log and
verify that integration before closing this report. Do not turn off alignment
sanitization or describe this as a physics/Room tick failure.

Existing APIs to reuse: `Data/Record.h/.hpp/.hxx`, `Dataset.h/.hpp/.hxx`,
`Data/Records.md`, `Data/AsciiPlot.*`, `Plot/Plot.*`, `Image/IDraw.*`.
Dataset already declares POSIX TReader/TWriter; do not invent a read API or
claim it is absent just because ASCIICrabs File has no byte-reader.

- [ ] Audit and rerun existing Record/Dataset tests before integration.
  Records use explicit codec framing/checksums, not native object dumps.
- [ ] Define an opt-in command/event/reply replay schema with seed, episode,
  frame ID, control source and version; preserve FP64 physics/control values.
  Existing TRecord observations are FPC: do NOT silently narrow dynamics to
  fit them or change that codec incompatibly. Choose a separate versioned
  trace/event schema where required, under user supervision.
- [ ] Keep file open/read/write/flush/close in concrete host adapters. Buffer
  logging outside the hot tick. Preserve evidence: no truncating old files,
  reject incompatible schema, handle short I/O/EINTR/truncation/disk failure.
- [ ] Replay actual recorded manual/autopilot handoffs deterministically;
  verify exact counts/IDs and compare decoded state at declared precision.
  Never call an incomplete or corrupt tail a successful completed run.
- [ ] If HRM/other apps need text puzzle files, expose a bounded concrete
  reader here, using existing stream utilities where suitable; do not add
  platform I/O to ASCIICrabs or replace file reads with silent success stubs.

### P2 — Playable frontend support (after headless transport)

This is the platform side, NOT a rewrite of BikeWorld/CarWorld physics.

- [ ] Inventory actual TUI/input/window/rendering modules and dependencies.
  Choose the initial available Linux frontend with the supervising user;
  no unsolicited React Native, Qt, SFML, GPU or mobile environment install.
- [ ] Define key/button/gamepad events that become bounded application
  commands: steer/throttle/brake, explicit manual/autopilot toggle, reset,
  pause and quit. Validate input; release/focus-loss disconnect must produce
  a documented safe policy rather than a stuck throttle.
- [ ] Publish immutable coherent snapshots to the frontend at a declared
  rate. UI/rendering must not borrow mutable List storage past its lifetime,
  race the physics owner, backpressure the hot loop indefinitely, or become
  the sole source of simulation time.
- [ ] Start with a simple 2D map/vehicle/route/HUD using available drawing
  APIs; show control source, speed, status and telemetry. Independent
  headless tests must run without display/input hardware/assets.
- [ ] Add a manual drive -> autopilot takeover -> manual return demonstration
  with no pose teleport/reset. Keep rendering/pacing costs separate from
  measured simulation throughput. 3D rendering is a later optional adapter.

### P3 — BLAS/backend and database boundaries

- [x] Inspect `Math/Blas.h` and `blas_test.cpp`, verify installed MKL/BLAS
  headers/libraries on THIS host; don't repeat old hardware/timing claims.
  Optional backend discovery must fail clearly or retain the tested scalar
  fallback without making scalar headless physics depend on MKL.
- [x] Test all transpose/stride rectangular cases, alpha/beta, supported
  alias/size domains and scalar-reference agreement before acceleration.
  Keep backend/thread controls here; no surprise nested CPU thread pools.
  No GPU port or speedup claim without actual measurements.
- [ ] SubsecondDb is owned by another agent. Agree on explicit contiguous
  schema/buffer ownership and auth boundary only when integration begins.
  Do not edit its extension, SQL, credentials, migrations or GPU service.

### Current checkpoint — Math/MKL first, then real CUDA verification

The supervising user selected Math rather than the default IPC starting item,
then explicitly requested MKL FIRST. MKL was completed/verified before continuing
the original CUDA assignment. The user authorized an isolated MKL SDK/runtime
install in this session's scratch area and built-in passwordless SSH to
`astar-rtx` via ~/.ssh/config, NOT sshpass. Both were used. No system/remote
dependency install, worker, Git publication, sibling-source edit, .env read or
password use was performed. Only selected sources/config headers were copied
to a newly created remote scratch directory, never over an existing repository.

Current item: repair/prove host row-major Math/Blas API with MKL, preserving
scalar fallback and existing void signatures; add a separate explicit cuBLAS
DEVICE-buffer API with borrowed handle/stream lifetime and status propagation.

Files changed: Math/Blas.h, Math/blas_test.cpp (build comment/explicit include),
new Math/BlasTests.cpp, Math/CMakeLists.txt, Math/README.md, Math/CudaBlas.h,
Math/CudaBlasTests.cpp, Math/CudaBlas.md, and this checkpoint.
Existing tracked Math test binaries and Math/backup were left alone. Other
dirty files, including concurrently changing Code/Env files, were not edited.

Failing reproduction (run before the corresponding numerical fixes):
- `g++ -std=c++23 -O2 -Wall -Wextra -pedantic Math/blas_test.cpp -o "$TMPDIR/CrabsTKMath/LegacyBefore"`
  exited 1: unclosed `_::Math` namespaces pulled later system headers inside
  that namespace. LegacyBefore.log retains the compiler diagnostics.
- After closing namespaces only, the original six checks passed, exposing
  their coverage gap. `g++ -std=c++23 -O2 -I../ASCIICrabs/_Seams Math/BlasTests.cpp -o "$TMPDIR/CrabsTKMath/ScalarRed"`
  compiled; running ScalarRed exited 1 with checks=166, failures=72 (stride
  scaling and beta=0 over NaN outputs). ScalarRed.log retains those results.
- Compiling/running the old test with `-DCBLAS_H_AVAILABLE=0` and separately
  `=1` each exited 1 with all six numerical checks failing. The conditional
  tested macro existence instead of its value; enabled branches did no work.
- CUDA had no adapter. The new device-interface regression was transferred
  first; remote `g++ -std=c++23 -O2 -I/usr/local/cuda-13.0/include -fsyntax-only /home/astar/.cache/CrabsTKMath-jkFQHe/CrabsTK/Math/CudaBlasTests.cpp`
  exited 1 on missing CudaBlas.h before implementation (CudaMissingApiRed.log).

Fix and boundary rationale:
- Numeric backend selection precedes use; actual CBLAS dgemm/dgemv calls replace
  no-ops. MKL uses its official headers and CMake package. Default remains scalar.
- All transpose cases share row-major dispatch/reference logic. Scaling touches
  logical output cells only; beta=0 does not read output; alpha=0/empty inner
  dimensions do not read input. Canonical ASCIICrabs types replace primitive
  spellings without new shadow typedefs; index products widen to ISW first.
- CMake requires explicit MKL opt-in, LP64, sequential component libraries;
  no implicit process-wide threading mutation or mkl_rt ABI selection.
- README specifies borrowed HOST buffers, positive row strides, size/lifetime
  preconditions, read-only input alias support and unsupported output overlap.
  This raw void BLAS API does not claim invalid-shape/capacity validation.
- CudaBlas.md defined the contract before implementation. CudaBlas.h uses
  `_::Math::Cuda` names with handle first and cublasStatus_t results; no singleton,
  uploads, allocations, waits, stream changes or pointer-mode changes. HOST
  scalar mode is checked. Negative shapes/short strides/overflowing spans and
  required nil pointers are rejected before submission. Allocation capacity,
  pointer accessibility/lifetime and output overlap remain caller preconditions.
- GPU zero-inner-dimension/alpha-zero calls scale via documented in-place GEAM,
  not SCAL(0), preserving beta=0 semantics even on NaN/uninitialized output.
  Normal GEMM swaps operands/dimensions for cuBLAS's column-major view; GEMV
  transposes that view. Host and GPU APIs remain separate and opt-in.

Passing focused checks (all commands run from CrabsTK root):
- Isolated SDK: `python3 -m venv "$TMPDIR/CrabsTKMath/MklEnv"`, then
  `"$TMPDIR/CrabsTKMath/MklEnv/bin/python" -m pip install --no-cache-dir mkl-devel==2026.2.0`
  exited 0. MklInstall.log records the installed Intel packages.
- `cmake -S Math -B "$TMPDIR/CrabsTKMath/Release" -DCMAKE_BUILD_TYPE=Release -DCRABSTK_MATH_ENABLE_MKL=ON -DCMAKE_PREFIX_PATH="$TMPDIR/CrabsTKMath/MklEnv"`
  then `cmake --build "$TMPDIR/CrabsTKMath/Release" -j 4` and
  `ctest --test-dir "$TMPDIR/CrabsTKMath/Release" -V --output-on-failure`:
  all exits 0, CTest 6/6. Scalar, macro-zero, MKL and real-MKL-CBLAS compatibility
  regression executables each report checks=188, failures=0; both legacy tests
  pass their original six checks.
- Same three commands with build directory Sanitized, CMAKE_BUILD_TYPE=Debug
  and `-DCRABSTK_MATH_SANITIZE=ON`: all exits 0, ASan/UBSan CTest 6/6.
- Separate default-MKL-OFF ScalarOnly Release configure/build/CTest: all exits 0,
  CTest 3/3. `ldd` shows no MKL dependency in the scalar regression executable.
- Actual missing-SDK configure (MKL ON, no SDK prefix) exits 1 with required
  package discovery failure. A syntax-only compile with MKL_ILP64 exits 1 on
  the explicit LP64 static assertion. These are expected rejection tests.
- `MKL_VERBOSE=1 "$TMPDIR/CrabsTKMath/Release/BlasMklTests"` exits 0 and logs
  104 real DGEMM/DGEMV entries plus the `lp64 sequential` runtime banner.
  Sequential MKL trace lines omit NThr; absence is not a threading failure.
- `python3 "$TMPDIR/CrabsTKMath/VerifyMath.py"` reruns final configure/build/tests,
  negative probes, dependency checks, consumer smoke and real-kernel trace;
  exit 0, ALL_PASS=True. Verification.json lists exact commands/exits/logs.
- `git diff --check`: exit 0. No commit/staging/reset/cleanup was performed.

CUDA execution/verification after the MKL checkpoint:
- Built-in `ssh -o BatchMode=yes -o ConnectTimeout=10 astar-rtx` succeeded.
  Actual host: two RTX 5070 Ti GPUs, driver 595.99.02; CUDA 13.0.88 toolkit,
  g++ 13.3.0, CMake 3.28.3. No driver/toolkit installation was necessary.
- Remote scratch: /home/astar/.cache/CrabsTKMath-jkFQHe/
  `python3 "$TMPDIR/CrabsTKMath/VerifyCuda.py"` transferred 13 selected files,
  read back SHA-256 hashes of every exact remote source and compared to local
  bytes, configured/built/tested both devices, and exited 0. CudaVerification.json
  records exact remote commands, source hashes and local log paths.
- Release configuration uses CRABSTK_MATH_ENABLE_CUDA=ON,
  CUDAToolkit_ROOT=/usr/local/cuda-13.0 and CRABSTK_MATH_CUDA_TEST_DEVICES=0;1.
  Configure/build/CTest exits 0, CTest 5/5: three CPU/scalar checks plus GPU0 and
  GPU1. Each GPU reports cases=203, failures=0. Tests include all transpose
  combinations, rectangular/padded/offset inputs/outputs, alpha/beta, canaries,
  uninitialized device output, empty contractions, input aliasing/immutability,
  handle/shape/span/nil/pointer-mode rejection, stream-policy preservation and
  explicit checked completion/teardown. No mocked cuBLAS or host scalar fallback.
- Separate remote UBSan build: CRABSTK_MATH_SANITIZE=OFF, CMAKE_BUILD_TYPE=Debug,
  CMAKE_CXX_FLAGS='-O1 -g -fsanitize=undefined -fno-sanitize-recover=all -fno-omit-frame-pointer',
  CMAKE_EXE_LINKER_FLAGS=-fsanitize=undefined. Configure/build/CTest exits 0,
  CTest 5/5, cases=203/failures=0 on each GPU. Explicitly UBSan, NOT ASan.
- For each ordinal 0 and 1, ran installed
  `/usr/local/cuda-13.0/bin/compute-sanitizer --tool memcheck --error-exitcode 99 --leak-check full /home/astar/.cache/CrabsTKMath-jkFQHe/Release/CudaBlasTests <ordinal>`
  and the equivalent `--tool initcheck --error-exitcode 99` command. All four
  exits 0, ERROR SUMMARY 0 errors; memcheck LEAK SUMMARY 0 bytes in 0 allocations.
- `ldd` resolves real libcublas.so.13/libcudart.so.13 from CUDA 13.0. The local
  required-CUDA configure without SDK exits 1, clearly requesting CUDAToolkit_ROOT.
  Running the GPU executable with ordinal 99 exits 1, not a successful skip.
- IMPORTANT LIMITATION retained, not suppressed: remote ASan+UBSan CTest exits 8,
  three CPU tests pass and both GPU tests fail at cudaSetDevice with CUDA error 2,
  before any Math/cuBLAS call. CudaSanitized.log retains that failure. A separate
  CudaInitProbe.cpp containing only cudaSetDevice reproduces: normal exit 0,
  address-sanitized exit 1/error 2, undefined-sanitized exit 0 while GPUs are idle.
  CudaInitProbe.log records this; the exact driver/address-space mechanism is
  unproven. Device memcheck/initcheck are not mislabeled as host-ASan passes.
- Final host VerifyMath.py was rerun after CUDA/CMake/offset-test changes:
  exit 0, Release 6/6, ASan/UBSan 6/6, scalar-only 3/3;

Remaining failures / assumptions / unsupported platforms:
- No outstanding numerical failure in the selected CPU/MKL/CUDA tests. The
  independent CUDA/host-ASan startup failure above remains an environment
  limitation. This is not a full CrabsTK/iGeek suite or Windows/OpenBLAS pass.
- Intel's prebuilt library internals are not sanitizer-instrumented. Vendor
  lazy allocation means these calls are not certified hard-real-time safe.
- Initial local NVIDIA probe failed; no exposed /dev/nvidia* device, CUDA SDK
  or MKL SDK was initially visible in this session. The approved scratch MKL
  SDK is ephemeral, not a durable system install. No performance claim made.

Evidence path: /home/astar/.hermes/profiles/astar-mary/cache/scratch/CrabsTKMath/
Next small item for supervisor: choose an opt-in caller integration or the next
backlog item. Do not silently route existing host/column-major buffers to CUDA,
### Human-visible progress and completion contract

### Human-visible progress and completion contract

Work one item at a time, not the entire backlog in one context window.
Default backlog starts with P0 config/type identity plus the smallest failing
wrap/layout reproducer; the supervising user selected the Math/MKL slice above
instead. At each checkpoint fill in:

```
Current item:
Files changed:
Failing reproduction (command / exit / observed failure):
Fix and boundary rationale:
Passing focused checks (commands / counts / exits):
Optimized/sanitized/cross-process checks actually run:
Remaining failures / assumptions / unsupported platforms:
Evidence path:
Next small item for the supervising user:
```

- Keep this current block updated BEFORE compaction. No invisible handoff
  dependency on a kanban card, background worker or another profile's memory.
- Do not mark a checkbox complete for a plan, stub, compilation alone or a
  different happy path. Quote exact commands and real exit codes. Distinguish
  ad-hoc checks, unit tests, stress tests and full-suite runs.
- Add focused build entry points for owned adapters/tests with relative
  include paths; preserve existing build recipes and legacy files unless
  the supervising user approves replacement. Linux cannot verify a Visual
  Studio build: describe generation steps honestly, don't hand-author a fake
  working vcxproj or claim Windows support from a Linux compile.
- Use fresh test-specific resources and scratch output; clean up only what
  the test owns. Never run global shared-memory deletion or broad git cleanup.

Preparation status: existing plan and relevant IPC/Record/Dataset/Blas
headers were read; no CrabsTK production code or shared-memory resources were
changed by this handoff. IPC risks above are source-inspection findings,
not a claim that the required failing reproductions have already been run.

---

## Historical context (not the current acceptance contract)

## Agent Role and Mission

This agent operates as a **low-level native systems engineer** for the AStarEngineering team, specializing in:

- **Qt6 / C/C++ native desktop applications** — models, views, signals/slots, QML/widgets, packaging, platform integration
- **OBS plugins and internals** — APIs, plugins, scenes, sources, filters, encoders, capture paths, streaming workflows
- **Symmetry Live features** — media, synchronization, devices, or native UI integrations
- **AStarStartup ASCII Crabs** — ASCII crab rendering, animation, terminal behavior, native tooling
- **Diagnosing crashes, deadlocks, races, memory corruption, ABI issues, timing faults, performance regressions**
- **Native library integration** — stable boundaries between C, C++, Qt, processes, and web-facing systems
- **Targeted tests, diagnostics, reproducible benchmarks, and platform notes**

## Engineering Standards

- Understand ownership, lifetime, thread affinity, and error propagation before editing
- Prefer RAII, explicit invariants, narrow interfaces, and deterministic cleanup
- Never block a UI or real-time media thread with uncontrolled I/O or long computation
- Treat callbacks, signals, device events, and external buffers as concurrency boundaries
- Preserve ABI/API compatibility unless a break is explicitly approved
- Measure performance before and after optimization; do not optimize folklore
- For crashes, collect traces, logs, symbols, versions, and reproduction steps before guessing
- Keep platform-specific code isolated and document build/toolchain assumptions
- Run the actual build and relevant executable or tests before reporting completion

## Communication Style

- Technically exact but understandable
- Draw boundary diagrams when ownership is confusing
- Separate confirmed causes from hypotheses
- During incidents, prioritize containment, reproduction, and evidence over blame
- Humor is terse, usually involving undefined behavior, dropped frames, or a crab escaping the event loop

## Current Workspace

- **Root**: /home/astar/AStarStarship/CrabsTK
- **Branch**: Issue189 → origin/master (ahead 2, behind 0)
- **Status**: 7 modified, 2 untracked
- **Recent relevant work**: Include syntax and order fixes in _ConfigHeader.h (2026-10-18), Pipe implementation (CrabsTK/IPC/), Boofer→Buffer rename

## Priorities: Real App Progress Over Abstract API Design

The agent's focus has shifted from abstract API speculation to **concrete app development with working examples**. The key insight:

> "We need some examples. We have ../iGeek/ and ../SubsecondDb/ but they are far from working. We're TUI and web only right now, I don't have mobile dev setup, but I could get a React Native workflow going."

### Current Reality

- **TUI + Web only**: No mobile dev setup, no macOS/Windows native builds
- **iGeek/**: RL training framework (PPO, GAE, entropy), Gym batches up to 8 envs, Worlds: Cards, Maze, Car, Bike, Prisoner, Cookbook — **far from running**
- **SubsecondDb**: GPU-accelerated vector database (CUDA), FPC rows, IUD keys, CPU reference + GPU cross-check, **no CUDA on this VM**
- **Working modules**: `Plot/` + `Image/IDraw` (built and verified 10/10 on g++ and clang), `Data/` (Record codec, Dataset POSIX streams, AsciiPlot) — clean, no-stdlib, well-documented
- **3P/boden**: Cross-platform C++ native UI toolkit (reference external dependency; not yet integrated; would provide Widget, Window, Event, Input abstractions for TUI/Web/React Native workflows)

### Next Development Steps

#### 1. TUI + Web App (Immediate)
- Build on existing TUI foundation
- Expand web workflows
- Verify existing Plot/ and Data/ modules continue to verify

#### 2. React Native Workflow (Planned)
- Set up React Native development environment
- Port key ASCIICrabs/CrabsTK types to JavaScript/TypeScript
- Create a minimal "Hello Crabs" React Native demo
- Connect to backend via existing web APIs

#### 3. iGeek / SubsecondDb Integration (Longer-term)
- iGeek: RL training (PPO, GAE, entropy) — Gym batches up to 8 envs
- SubsecondDb: GPU-accelerated vector database (CUDA) — no CUDA on this VM
- integration points: TRecord/TRollout data path, AsciiPlot metrics

### Concrete App Ideas

| Idea | Tech Stack | Status |
|------|-----------|--------|
| **TUI Dashboard** | C++/CrabsTK + ncurses/Readline | Existing, expand |
| **Web Front-end** | TypeScript + existing RPC protocol | Planning |
| **React Native Bridge** | React Native + existing C++ backend | Planning phase |
| **iGeek Integration** | C++/CrabsTK + iGeek PPO | Dependent on iGeek progress |

### How to Make Progress

1. **Start TUI**: Expand the existing TUI demo with new features
2. **Add Web**: Expose CrabsTK RPC endpoints via a simple web server
3. **Plan React Native**: Document the type mappings needed, set up the development workflow
4. **Incremental iGeek**: Fix one iGeek bug at a time, verify each fix

### Relevant Files (Current State)

- `CrabsTK/AGENT_PLAN.md`: Current plan (being rewritten)
- `ASCIICrabs/_Seams/18.IPCRoomDemo.*`: Pipe-based Chinese Room demo
- `CrabsTK/IPC/Pipe.h/.cpp`: In-process pipe implementation (verified)
- `ASCIICrabs/_Seams/17.Room.hxx`: Two-room Chinese Room ring regression
- `ASCIICrabs/Plot/` + `Image/IDraw`: Verified 10/10 on g++ and clang
- `ASCIICrabs/Data/`: Record codec, Dataset, AsciiPlot — clean, no-stdlib

### Acceptance Gates (for this session)

- [ ] TUI app: add one new feature, verify it builds and runs
- [ ] Web: expose at least one RPC endpoint, test with curl
- [ ] React Native: document type mappings, set up repo
- [ ] iGeek: fix one documented bug, verify the fix

---

*Focus: concrete progress on working apps, not abstract API design. The iGeek/SubsecondDb contexts provide longer-term integration targets, but immediate work is TUI + Web + React Native planning.*
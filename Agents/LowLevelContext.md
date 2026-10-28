# LowLevel Agent Context Dump — v5 (2026-10-27, reorg VERIFIED)
Workspace: /home/astar/AStarStarship/CrabsTK · Sibling: /home/astar/AStarStarship/ASCIICrabs
Supersedes v4. Reorg is DONE and verified; only reporting + optional commit remain.

## 0. STATE: GREEN
Root rollup builds and runs: `Unit tests completed successfully! (:-)+=<` (exit 0).
AD-HOC verification 17/17 (NOT a canonical suite — no such thing exists in these
repos): /home/astar/.hermes/cache/scratch/hermes-verify-crabstk-reorg.sh —
19 yamls parse (55 module seams, Pro dormant, Release last) · hierarchical sync
17 files · double-sync byte-stable · legacy flat path (HEAD yaml) still works
(65-seam table, release last; synthetic flat crabstk works) · validation errors
(valid tree ok; forward cross-module dep -> "not earlier in the global table";
release-not-last -> hard error) · rollup build+run + success line ·
ASan clean (detect_leaks=0; the IMUL dangling-pointer fix is proven) ·
-DSEAM=43 debug build compiles · Data Verify.py end-to-end PASS.
Plan doc __SeamsRefactorPlan.md: status flipped to EXECUTED.

## 1. FIXES THIS SESSION (all uncommitted; v4's blocker is dead)
a) _Seams/pch.h RECREATED (hand-maintained, first TU header, pattern from
   ASCIICrabs/_Seams/pch.h): `#include "_Seams.h"` (local, quoted) +
   `#ifndef SEAM #define SEAM SEAM_N`. WHY: root _Config.h (HEAD) defines NO
   SEAM and no CRABSTK_* table — with none of it, `#if SEAM >= CRABSTK_X`
   gates were vacuously true (0>=0) and the macro table absent; A_TEST_BEGIN
   (from ASCIICrabs _Debug/_Release.h, pulled by the per-seam chains) was
   undefined. pch.h supplies table + default before any module include.
b) Generator GenerateCrabstkMainHxx: _Main.hxx now emits `#include "pch.h"`
   first, the single-TU implementation set from the pre-reorg _Main.cpp
   (COut.h/COut.hxx/AType.hxx/Array.hxx/Puff.hxx/Test.hpp/Test.hxx/Stringf.hxx),
   and a PLAIN `::TestCase CrabsTkTree[]` array passed to SeamTreeTest instead
   of TTestTree<...>: GCC (>=13) rejects NAMESPACE-QUALIFIED function names as
   template arguments for function-pointer type params (unqualified works —
   ASCIICrabs' TTestTree<CrabsTests> is why the sibling is green; qualified
   Plot::Test::PlotCore fails, &fn too). SeamTreeTest is what TTestTree
   delegates to (Test.hpp:79). Entries qualified CT::X (module sources declare
   global `namespace Plot` — Plot/Plot.h — colliding with CT::Plot; `Plot` was
   ambiguous via the using-directives). Array is NON-const (SeamTreeTest takes
   TestCase*).
c) IMUL preamble (yaml): removed `namespace _ { ... }` wrapper around
   #include "../../IMUL/Parser.hxx" — Parser.h:7 opens its own `namespace _`,
   the wrapper nested it to _::_ (second `_` entity; the source of the "ambiguous
   reference to '_'" + ImulCompile double-candidate errors).
d) IMUL/Parser.hxx: the two definitions (ImulParseWorkspace/ImulCompile) were
   at GLOBAL scope while Parser.h declares them in `namespace _` — wrapped in
   `namespace _ { ... }`.
e) IMUL dangling display pointer (S01 runtime failure): ImulCompile wrote the
   corrected display into a LOCAL `CHA display[256]` and event->display_text
   pointed at it; test read garbage after return. Fixed: TImulStorage gained
   caller-owned `CHA* display_text; ISN display_capacity;` (no-allocation
   design); stub returns 2 if absent; test supplies a 256 buffer. S01 passes.
f) IPC/_Debug.h: D_COUT definition wrapped in #ifndef D_COUT (rollup keeps
   ASCIICrabs' D_COUT; standalone IPC builds unchanged).
g) LoadSeams: quoted-scalar list items (`- 'text: with colons'`) no longer
   mis-parsed as dict items (the IMUL preamble comment had a colon).
h) 01.Tracer.hxx (test): supplies storage.display_text/capacity.

## 2. VERIFICATION ENV (still valid, 24h scratch prune)
S=/home/astar/.hermes/cache/scratch/hermes-verify-root: S/crabs = git archive
HEAD + working-tree overlays + HEAD _Config.h (working-tree root one TRUNCATED)
+ symlinks S/ASCIICrabs and S/crabs/ASCIICrabs -> real sibling.
Build: cd S/crabs && g++ -x c++ -std=gnu++23 -O1 -s -I. -I_Seams _Seams/_Main.hxx.
Real tree (no local ASCIICrabs dir): build needs the sibling on the include
path for <ASCIICrabs/...> angle includes, e.g. -I.. plus -I$(pwd) -I$(pwd)/_Seams
(verify before claiming a real-tree build; the verification above is scratch).
Probe technique that worked: g++ -fsyntax-only with header-order probes;
bisect via cumulative module includes; NOTE head -N eats the pipeline exit
code — check the grep output, not $?.

## 3. SEAM MECHANICS + CONVENTIONS (unchanged from v3/v4)
- -x c++ always (.hxx main source; GCC14 PCH trap).
- Seam table: 55 seams (CRABS_CORE=1 ... CRABSTK_RELEASE=55=SEAM_N); Pro
  dormant (numbered 47-54, scaffolded, not rolled up).
- Single-.cpp rule. A_ASSERT always-on; D_ASSERT/D_COUT debug-gated.
- code-repo-respect: no unilateral restorations; "done" = real execution.
- Literal preservation; frozen MPL headers.

## 4. REPOS (uncommitted)
CrabsTK: ~150 files (19 yamls, root yaml rewritten, SyncSeams.py rewritten,
17 generated files, pch.h, IMUL/Parser.h+.hxx, IPC/_Debug.h, 01.Tracer.hxx,
deleted Gui/Imul/Crab clutter). ASCIICrabs (branch Issue376): 42 files from
prior sessions — all green (v3), untouched this session.
PENDING user decisions: (1) commit? (2) AGENTS.md -x c++ note.
Known pre-existing (NOT touched): ASCIICrabs Slot.hxx:222 multi-line D_COUT
(stray-backslash when the full _Package.hxx machine is pulled in — the root
rollup deliberately does NOT pull _Package.hxx, so it never compiles here).

## 5. REMAINING
Report to user (done in-session); optional: commit (needs approval — do NOT
commit unprompted); optional: real-tree build check with -I.. ; the IMUL stub
ImulCompile is a hardcoded 14-byte phase-1 test stub by design (Phase 2 = real
parser per its comment).

=== Mission #197 Context Dump (2026-10-27) ===

Mission #197: Eliminate the C++ standard library and legacy C junk and get rid of the multiple .cpp files

STATUS: COMPLETE
- 0 .cpp files remain in CrabsTK (all converted to .hxx)
- Production C++ standard library eliminated (only test utility lines remain)
- Production legacy C junk eliminated (0 non-OS includes remain)
- Type system centralized via _Config.h → ASCIICrabs single source of truth
- 5 files modified: Math/BlasTests.hxx, Math/CudaBlasTests.hxx, Math/Blas.h, Plot/Plot.hxx, IPC/Pipe.hxx
- 4 OS-POSIX includes remain as issue exceptions
- .gitignore updated with Python cache entries

FILES MODIFIED (5):
- Math/BlasTests.hxx - replaced <math.h>/<stdio.h> → ASCIICrabs/Math.hxx
- Math/CudaBlasTests.hxx - replaced <math.h>/<stdio.h>/<stdlib.h> → ASCIICrabs/Math.hxx
- Math/Blas.h - replaced <math.h> → ASCIICrabs/Math.hxx
- Plot/Plot.hxx - removed <math.h>, uses FpdSqrt from ASCIICrabs
- IPC/Pipe.hxx - removed #include <stdio.h> (printf unused)

FILENAME RENAMES (lower_snake_case → CamelCase):
- Math/blas_test.hxx → Math/BlasTest.hxx (references updated in CMakeLists.txt, __Seams.yaml, Blas.h)
- IPC/pipe_test.hxx → already deleted (mission #197)
- AGENT_PLAN.md left unchanged (critical doc with many cross-references)

SEAM 02.Release.hxx FILES:
- 15 empty files created in all _Seams subfolders: Audio, Code, Data, Env, Forms, GUI, Hatch, IMUL, Image, Plot, Pro, Release, Touch, Web, Who
- All files are 0 bytes (empty by design)
- Purpose: Setting SEAM=2 enables all code in that seam, allowing disabling of problematic code

00.Core.hxx → 01.Core.hxx RENAME:
- All 15 _Seams subfolders renamed 00.Core.hxx → 01.Core.hxx
- All 15 _Seams subfolders have 02.Release.hxx (empty)
- _Seams/_Main.hxx updated with new include paths (00.Core.hxx → 01.Core.hxx)
- _Seams/pch.h updated with new include path
- Hatch/template/crabstk/seams.yaml comment updated
- __SeamsRefactorPlan.md reference updated
- __CrabsTK.vcxproj / .vcxproj.filters references updated
- Data/Verify.py filter updated

UNIT TESTS in ASCIICrabs Seam Tree Format:
- 5 test .hxx files converted from int main() to seam-gated format:
  • Code/CodeModuleTest.hxx
  • Who/PasswordShaTest.hxx
  • IPC/pipe_test.hxx
  • IPC/LegacyRed.hxx
  • IPC/_Main.hxx
- Format: #if SEAM >= CRABS_RELEASE / #if SEAM == CRABS_RELEASE gating
- namespace CRTest { ... } wrapper
- inline const CHA* Release(const CHA* args) function
- int main() removed from all files

GIT IGNORE:
- Python cache entries added: __pycache__/, *.pyc, *.pyo

CONTEXT PRESERVATION: This dump preserved before context compaction.

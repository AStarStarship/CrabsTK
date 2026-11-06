# Low-level context — v12: _Seams/ API overhaul (in progress)

## Mission
The Captain overhauled the ASCIICrabs core API ("cleaned up every file,"
renamed macros, removed unneeded files). The CrabsTK `_Seams/` test tree is
broken as a result. Job: fix the `_Seams/` tests so the single-TU build compiles
and the seam system works under the new naming.

ASCIICrabs is the "next-level C++ standard library replacement" — treat it as a
known-working baseline to CONSUME, not debug. We are not supposed to be fixing
ASCIICrabs at this level.

## The naming overhaul (the core of the task)
- **No UPPER_SNAKE_CASE.** LLMs have tokens for lowercase and Capitalized
  letters but not UPPERCASE, so all macros are now `_UpperCamelCase`.
  - `_CrabsTKDataRecord` stays (already `_UpperCamel`).
  - `CRABSTK_WHO_ROSTER` → `_CrabsTKWhoRoster`.
  - `CRABSTK_RELEASE` → (now generated, see table below).
- **The `.yaml` files are the source of truth** for seam order.
- **One seam per numbered test file:** `01.Core.hxx` → `Core`,
  `02.<X>.hxx` → `<X>`, `NN.Release.hxx` → `Release`. Release is ALWAYS the
  last file; NN = the module's seam count.
- **The old fine-grained seams are gone** (Who used to have 23 seams, one per
  type — now just Core + Release = 2). The macro table and the files must agree.
- **`DependsOn:` in the root yaml** points at the ASCIICrabs base yaml
  (`../ASCIICrabs/_Seams/_Seams.yaml`) as the single source of truth for the
  1-23 base macros. No inlined base. (Token-minimization is a stated value.)
- **No "AUTO-GENERATED" stubs, no boilerplate comment headers, no per-seam
  comment scaffolding.** The Captain is not a newb; comments clutter. Generated
  files must be minimal (copyright line max).
- **No `.cpp` imported from `.cpp`.** Nested seams use `_Seams.hxx` (rename .cpp
  to .hxx to import them). The one real entry is `_Seams/_Seams.cpp`; everything
  it pulls is .hxx/.h.

## Seam table (generated, base 1-23 + CrabsTK 24-62)
Base 1-23 comes from `../ASCIICrabs/_Seams/_Seams.yaml` via `DependsOn`
(`_CrabsCore`=1 … `_CrabsRelease`=23). Note the base correctly ends at 23 now
(the old hand-written `_Seams.cpp` had it at 24 — off by one; DependsOn fixed
the drift).

CrabsTK seams (module order from root yaml), all `_UpperCamelCase`:
```
 24 _CrabsTKDataCore        29 _CrabsTKIPCCore        34 _CrabsTKMathCore
 25 _CrabsTKDataRecord      30 _CrabsTKIPCRing        35 _CrabsTKMathCudaBlas
 26 _CrabsTKDataDataset     31 _CrabsTKIPCLegacyRed   36 _CrabsTKMathRelease
 27 _CrabsTKDataAsciiPlot   32 _CrabsTKIPCTwoProc     37 _CrabsTKWhoCore
 28 _CrabsTKDataRelease     33 _CrabsTKIPCRelease     38 _CrabsTKWhoRelease
                                39 _CrabsTKAudioCore    44 _CrabsTKEnvCore
                                40 _CrabsTKAudioRelease 45 _CrabsTKEnvRelease
                                41 _CrabsTKCodeCore     46 _CrabsTKFormsCore
                                42 _CrabsTKCodeStripComments 47 _CrabsTKFormsRelease
                                43 _CrabsTKCodeRelease  48 _CrabsTKGUICore
                                49 _CrabsTKGUIRelease   50 _CrabsTKImageCore
                                51 _CrabsTKImageRelease 52 _CrabsTKIMULCore
                                53 _CrabsTKIMULRelease  54 _CrabsTKPlotCore
                                55 _CrabsTKPlotPlot     56 _CrabsTKPlotRelease
                                57 _CrabsTKTouchCore    58 _CrabsTKTouchRelease
                                59 _CrabsTKWebCore      60 _CrabsTKWebRelease
                                61 _CrabsTKProCore      62 _CrabsTKProRelease (RELEASE = _SeamN)
```
Pro is `dormant: true` in the root yaml (Script2 API deleted; numbered +
scaffolded but excluded from the rollup). The terminal `Release` module is the
release-tree rollup (`_Seams/Release.hxx`), NOT a test module — it has no
`_Seams.hxx` dispatch and is excluded from the source/test-tree include lists.

## What I changed (this session)
1. **All 16 module yamls rewritten** (`_Seams/<Mod>/_Seams.yaml`) to mirror the
   numbered files, `_UpperCamelCase` names, one seam per file, Release last.
   Data's old `__Seams.yaml` was consolidated into `_Seams.yaml` and removed.
   IMUL's `01.Tracer.hxx` is a HELPER (conformance case T01S01) pulled in by
   Core, NOT a separate seam — excluded from the seam list. Script used:
   `/home/astar/.hermes/profiles/low-level/cache/scratch/rewrite_yamls.py`.
2. **Root `__Seams.yaml` rewritten**: added `DependsOn:
   ../ASCIICrabs/_Seams/_Seams.yaml`, dropped the `ASCIICrabs` module entry
   (it's now the dependency base, not a test module), dropped the old
   UPPER_SNAKE comment block.
3. **`Hatch/SyncSeams.py` — added crabstk v2 path** (new functions, old paths
   left intact):
   - `LoadDependsOn(project_dir, depends_on)` — resolves the DependsOn yaml,
     returns the ordered base seams.
   - `TestFunctionNameV2(project, module, seam_name)` — strips the
     `_CrabsTK<Module>` prefix to get the short C++ test fn name
     (`_CrabsTKDataRecord` → `Record`).
   - `GenerateCrabstkSeamsCpp(...)` — the self-contained `_Seams/_Seams.cpp`:
     macros at top (base + project + `_SeamN`), then includes
     (`_Config.h`, `ASCIICrabs/_Config.h`, `ASCIICrabs/_Package.hxx`,
     `ASCIICrabs/Test.hpp`, each module's `"_<Mod>/_Seams.hxx"` test
     scaffolding, `Release.hxx`), then `main()`. Skips dormant + the terminal
     Release module from the include/test-tree lists.
   - `GenerateModuleSeamsHxx(module, ordered_m)` — per-module `_Seams.hxx`
     dispatch; gates at the MODULE's OWN release seam (its last seam), lists
     the module's non-release fns by short name.
   - `GenerateNumberedHxx(module, seam, release_seam, is_release)` — one
     numbered `.hxx` per seam; minimal (copyright, `#pragma once`, config
     include, debug/release gate, the test fn). No AUTO-GENERATED stub, no
     SYNC-KEEP markers, no placeholders.
   - `SyncCrabstk(project_dir, data, mode)` — the v2 entry; writes
     `_Seams/_Seams.cpp`, each module's `_Seams.hxx`, and each numbered `.hxx`.
   - Wired into `Sync()`: `modules:` + `mode: crabstk` + `DependsOn` present →
     `SyncCrabstk`; else falls through to the legacy `SyncModules`.
   - Fixed 4 PRE-EXISTING syntax errors in the legacy paths:
     `Write(..., <_Config.h>, ...)` → `Write(..., "_Config.h", ...)` at the
     SyncModules/SyncIgeek/Sync museworld+flat call sites (these crashed the
     whole module on import).
4. **Build fixes from earlier in the session** (pre-overhaul, still in effect):
   - `_Seams/CMakeLists.txt`: single-TU wrapper that `#include`s the real
     `_Seams/_Seams.cpp` (the old `file(WRITE)` produced an EMPTY wrapper →
     `undefined reference to main`). Include roots (absolute, NO `..` — the
     `..` broke GCC's `#pragma once` path canonicalization): `_Seams/`,
     project root, `../ASCIICrabs`, `../../` (AStarStarship, for
     `<ASCIICrabs/...>` prefixed includes). C++23.
   - `_Config.h` files (root, `_Seams/`, Data, IPC, Plot): changed relative
     `"../../ASCIICrabs/_ConfigHeader.h"` → angle `<ASCIICrabs/_ConfigHeader.h>`
     to fix the `#pragma once` identity collision (quoted-via-file-dir vs
     angle-via-`-I` resolved to different canonical paths → `_ConfigHeader.h`
     "No such file" re-include fatal in BigInt.h).
   - `Data/Dataset.hxx`: added missing `#endif` for the `#if defined(_WIN32)`
     guard (was unterminated).
   - `Env/EnvVar.hxx`: added missing `#ifndef CRABSTK_ENV_VARH / #define` guard.
   - `Math/_Seams.hxx`: include list was referencing non-existent
     `02.Release.hxx`; the files are `01.Core/02.CudaBlas/03.Release`.
   - `Math/02.CudaBlas.hxx`: gated the whole body on
     `#if __has_include(<cuda_runtime.h>)` (no CUDA on this host).
   - `JSX/_Package.hxx` created (was missing) — later renamed to `_Seams.hxx`.
   - `const const CHR*` → `const CHR*` (Copilot hallucination) across Who/,
     Pro/, Touch/ (62+ instances).
   - Untemplated `Printer& Print(Printer& o)` → added
     `template<typename Printer>` in Who/ (Name, Organization, Person,
     Profile, Registrar) + `Who/Relationship.hpp` (also had `const const`).
   - Renamed module SOURCE rollups `<Mod>/_Package.hxx` → `<Mod>/_Seams.hxx`
     (the Captain's naming). NOTE: this created a name collision — the source
     rollup (`<Mod>/_Seams.hxx`) and the test scaffolding (`_Seams/<Mod>/_Seams.hxx`)
     share a basename. RESOLVED: the test `.hxx` files pull the module headers
     directly (e.g. `#include "../../Data/Record.hpp"`), so the test TU does NOT
     need the source rollup — the generated `_Seams.cpp` only includes the
     quoted test-scaffolding `"_<Mod>/_Seams.hxx"`, NOT a source-rollup include.

## Build state (last measured, pre-overhaul-regen)
- Preprocessor is clean (the `_ConfigHeader.h` fatal is gone).
- ~846 C++ errors remain in the MODULE CODE (not the test harness). Top sources:
  Plot/ (TColor/ImageDraw/CImage not in scope), JSX/ (TList/TDic/TTable/TBook
  not in scope — core types live in `namespace _`), Data/ (TRollout/Rollout/
  RecordStatus, ambiguous Load), IPC/Pipe.hxx (Pipe:: methods outside
  `namespace CTK::IPC` — missing `namespace IPC {`), Web/Routing.hpp
  (HttpRequest/HttpResponse not in scope), Who/ (Address/TMessage/TSession/etc.),
  IMUL/ (Parser/Tracer). Plus core cascade (AType.hpp 764, Dic.hpp 275,
  List.hpp 216, Table.hpp 101) from the module errors above.
- The build stops at the FIRST module with errors, so the error count is a
  cascade, not the full set. Work module-by-module.

## OPEN QUESTION — the big one (was mid-answer when interrupted)
**Does the generator's `GenerateNumberedHxx` skeleton OVERWRITE the real test
code?** In the dry-run sandbox the generator replaced the hand-written
`01.Core.hxx … NN.Release.hxx` (which contain REAL test code) with empty
skeletons (`_ATestBegin; return _NilP;`). The Captain said "Generate everything:
the table, _Seams.cpp, AND all the numbered .hxx test files from the yaml."
So the question I was resolving: does "generate everything" mean the generator
owns the test bodies (and the real test code needs to MOVE into the yaml / a
separate check_file mechanism), or does it mean the generator creates the
scaffolding and preserves existing bodies (the old SyncSeams.py had SYNC-KEEP
markers + `ExtractFunctionBodies`/`SeedBody` for exactly this)?

The Captain's "no extra work later, I'm not a newb" + "no stubs" suggests the
generated files should be the FINAL form. LEANING: the real test code currently
in the numbered `.hxx` files must be preserved — either by (a) a
`check_file:` field in the yaml pointing at a separate impl file the generator
includes (the museworld pattern already in SyncSeams.py), or (b) the generator
merging the skeleton around an existing body. NEEDS THE CAPTAIN'S CALL before
running the generator against the REAL repo (it would clobber the real tests).

## Dry-run sandbox (safe to inspect)
`/home/astar/.hermes/profiles/low-level/cache/scratch/crabstk_dry/CrabsTK` is a
full copy with the generator run. `_Seams/_Seams.cpp`, all `_Seams.hxx`
dispatch, and all numbered `.hxx` are the GENERATED (skeleton) versions. The
module yamls + root yaml there are the rewritten ones. `SyncSeams.py` there is
the updated one. Do NOT build the real repo until the open question is settled.

## Key files
- `Hatch/SyncSeams.py` — the generator (v2 path added; backup at
  `/tmp/SyncSeams.py.bak`).
- `__Seams.yaml` — root (modules + DependsOn + release_module).
- `_Seams/<Mod>/_Seams.yaml` — per-module seam lists (source of truth).
- `_Seams/_Seams.cpp` — the single-TU entry (generated).
- `_Seams/<Mod>/_Seams.hxx` — per-module test dispatch (generated).
- `_Seams/<Mod>/NN.<Seam>.hxx` — numbered test files (generated scaffolding;
  real test code currently lives here and is at risk of clobber).
- `_Seams/Release.hxx` — the release-tree rollup (user-owned, includes all
  module Main fns).
- `_Seams/CMakeLists.txt` — single-TU build.

## Conventions (AStarship / Chimera+)
- Python, never shell, for project scripts; name them as functions
  (`SyncSeams.py`, not `sync_seams.py`).
- No C++ stdlib types/containers (CHA/ISC/IUD/BOL/FPD etc. instead).
- Immutable → CamelCase, mutable → lower_snake_case; macros were
  UPPER_SNAKE_CASE but the overhaul moves them to `_UpperCamelCase`.
- `nilp`/`_NilP` = nullptr; `D_COUT`/`D_ASSERT`/`_ATestBegin` for debug.
- 2-space indent; AStarship copyright + `#pragma once` on every header.

## Prior version (v11) summary
v11 was the JSX inspection task (IllyaTask/MaryTask/MadyarTask allocation for
JSX/Jsx.hxx + JsxTransform.hxx). That work is SEPARATE from this seams
overhaul. Do not conflate. The v11 "ALL TASKS COMPLETE" claim was retracted as
unverified. JSX findings: JsxTransform writer overflows capacity; child
identifiers wrap at %10; TDic key bytes fixed at 256 (NewContext) while
generated code uses total=16 (IsError) vs self-test total=8; UBSan misaligned
store at List.hpp:464 (TListAlloc aligns by descriptor, JSX requests
_CHA/STR_ for wider payloads); SubsecondDb/Web CMake exits 1 (EOF at line 65,
server stub). Scratch probe + compile recipe are in the git history of this
file if needed; the JSX task is paused, not dropped.

# AGENTS.md

## Agent Role and Capabilities

This agent operates as a **low-level native systems engineer** for the AStarEngineering team,
specializing in:

- **ASCIICrabs / CrabsTK native C++23 type systems** — no-stdlib ASCII data types,
  Chinese Room Abstract Stack machine, B-Sequences, Script protocol
- **Seam-based conditional compilation** — `_UpperCamelCase` macro gating,
  single-translation-unit builds, per-module test trees
- **Qt6 / C/C++ native desktop applications** — models, views, signals/slots, QML/widgets,
  packaging, and platform integration
- **OBS plugins and internals** — APIs, plugins, scenes, sources, filters, encoders, capture paths,
  and streaming workflows
- **MIDI/DMX controller features** — media, synchronization, devices, or native UI integrations
- **AStarStartup ASCII Crabs** — ASCII crab rendering, animation, terminal behavior, and related
  native tooling
- **Diagnosing crashes, deadlocks, races, memory corruption, ABI issues, timing faults, and
  performance regressions**
- **Native library integration** — stable boundaries between C, C++, Qt, processes, and web-facing
  systems
- **Targeted tests, diagnostics, reproducible benchmarks, and platform notes**

## Build, Lint, and Test

### Build
- **Compile:** `g++ -g -o CrabsTK _Seams/_Seams.cpp` (single translation unit)
- **Output Binary:** `CrabsTK` (Windows: `CrabsTK.exe`)
- The single-TU entry is **`_Seams/_Seams.cpp`** (it defines the seam table,
  then includes `_Config.h` + the package + all seam test files).
- **Canonical build:** configure `_Seams/CMakeLists.txt` as a separate build tree;
  do not hand-roll the `g++` line. CMake wires up the single-TU rule correctly.

### Test
- **Selecting the seam:** set in **`_Seams/_Config.h`** via
  `#define _Seam _CrabsTK<Name>`. Do **NOT** edit `_Seams/_Seams.cpp` to change
  the seam or the `TTestTree` — change the `_Seam` macro in `_Config.h`.
- **Full release tree:** build at `_CrabsTKRelease` (seam 62, `_SeamN`).
  Lower seams are debug-only slices; if a lower seam can't link a `TTestTree`
  entry, that's expected.
- **Run All Tests:** `./CrabsTK`
- **ASan check** (catches infinite recursion / overflow the plain build hides):
  ```
  g++ -std=gnu++23 -x c++ -g -fsanitize=address -fno-omit-frame-pointer \
    -I _Seams -I . -o /tmp/crabstk-asan _Seams/_Seams.cpp
  ASAN_OPTIONS=detect_leaks=0 /tmp/crabstk-asan
  ```
- **Per-module compile check:**
  ```bash
  clang++ -std=c++23 -c _Seams/<Mod>/_Seams.cpp -o /dev/null \
    -I_Seams/<Mod> -I_Seams -I. -D_SEAM=24 \
    2>&1 | grep "error:" | grep -v "ASCIICrabs/"
  ```
  15/15 modules must return 0 CrabsTK errors.
- **Debugging:** `Test.hpp` contains a `TBRK` function for one breakpoint.
  No gdb by default; ASan + the compiler's `-E` preprocessed dump are the
  primary diagnostics.

### Lint / Standards
- No automated linter configured.

## Code Style & Conventions (Chimera+)

All CrabsTK code follows the **Chimera+** style guide
(`~/AStarStarship/ASCIICrabs/__ChimeraPlus.md`).

### Core Rule: immutable vs mutable
- **Immutable** (can't change after init) → **CamelCase**
- **Mutable** (changes at runtime) → **lower_snake_case**

### Naming
| Element | Convention | Example |
|---|---|---|
| Type aliases (fixed-width) | 3-letter CAPS codes | `CHA` char8, `ISC` int32, `IUD` uint64, `BOL` BOL, `FPC` float32, `FPD` double64 |
| Structs / classes | CamelCase + T(POD)/A(utoject) prefix | `TArray`, `AMap`, `TRollout` |
| Struct members (mutable) | lower_snake_case | `socket_bytes`, `header_bytes` |
| Struct members (immutable) | CamelCase | `StackTotalMin`, `ColumnWidth` |
| Free functions / methods | CamelCase, type-prefixed | `CrabsInit`, `TArrayBytes`, `TRecordEncode` |
| Function suffixes | `_NC` (no-check), `C` (const/count), `T` (POD), `A` (autoject) | `TArrayInsert_NC`, `CSizeMin` |
| Local variables | lower_snake_case (always) | `bytes_data`, `read_cursor` |
| Macros / #define | _UpperCamelCase (leading `_` + CamelCase) | `_CrabsTKDataCore`, `_NilP` |
| Private/protected members | trailing underscore | `aobj_`, `array_` |
| Namespaces | `CTK` or starts with `CTK` | `namespace CTK { namespace Data { } }` |

### Formatting
- 2-space indent
- Opening brace same line (functions/control), next line (structs/classes)
- 80-CHR lines except for Doxygen description tags
- Pointer/Ref attached to type: `CHA* text`, `ISC& count`
- Control flow: space between keyword and parenthesis. `else` on same line as closing brace.

### File Structure
- Do not create files unless you have approval. This is a microframework;
  each file has a very specific purpose.
- **Headers:** `.h` for declarations, `.hpp` for template code, `.hxx` for impl.
- **Guards:** `#pragma once` + `#ifndef CRABSTK_<NAME>_<H|HPP>` style.
- **Copyright:** `// Copyright AStarship <https://astarship.net>.` at top.
- **Includes:** `""` for local project files, `<>` for system/config files.

### Minimal Standard Library
The project aims to replace the C++ std library. **Do not add `#include <string>`,
`<vector>`, `<iostream>`, `<errno.h>`, `<unistd.h>`, or any other C++/C stdlib
header** unless a file already depends on it and the task is to fix that file
in place. Use project-specific types (`CHA`, `ISC`, `IUC`, `IUA`, `BOL`, `FPC`,
etc.) instead.

### Specific Types (from `_ConfigHeader.h` and `_Spec/Data`)
- `CHA`, `CHB`, `CHC`, `CHR` — char8, char16, char32, default/OS CHR type.
  Prefer `CHR` unless otherwise specified.
- `ISC`, `ISC`, `ISW`, `ISE` — signed integer N/32/word/128-bit.
- `IUC`, `IUD`, `IUE`, `IUW` — unsigned integer 32/64/128/word.
- `FPC`, `FPD`, `FPE` — float32, double64, extended.
- `BOL` — BOL. `PTR` — pointer.
- `IUE`/`ISE` are native 128-bit on GCC/Clang (`unsigned __int128`/`__int128`).
  On MSVC they are `{lsb, msb}` structs. Do not re-add struct definitions
  unconditionally.

### Classes
- No work in constructors that can fail. Use `Init()` if needed.
- Mark single-arg constructors `explicit`.
- Explicitly declare or delete copy/move (`= default`, `= delete`).
- `struct` only for passive data (public members, no logic).
- Public inheritance only. Composition preferred.

### Error Handling
- No exceptions. Error codes or assertions (`IsError`).
- Use `TBRK()` for breakpoints.
- `D_COUT`/`D_ASSERT`/`_ATestBegin` for debug.

### Autojects and RAMFactory
- **Autoject:** "Automatic Object" managing memory via `RAMFactory`.
- **RAMFactory:** function pointer type for allocate/resize/delete.
- **Seam Injection:** swap out `RAMFactory` or `origin` buffer for testing.

## Seam System (v12 overhaul, 2026-11-06)

### Naming Convention
- **Seam macros:** `_UpperCamelCase` only. Example: `_CrabsTKDataCore`, `_CrabsTKWhoRelease`.
- **No `UPPER_SNAKE_CASE` seam macros.** The old `CRABSTK_*` form is gone.
  Include guards (`CRABSTK_DATA_RECORD_HPP`) and feature flags
  (`CRABSTK_WHO_SESSION_TOTAL_DEFAULT`) are a different category and stay.
- **Null pointer:** `_NilP`. The legacy `NILP` macro is removed.
- **Namespaces:** all CrabsTK namespaces are `CTK` or start with `CTK`.

### Macro Table
Base seams 1–23 from ASCIICrabs (`_CrabsCore` through `_CrabsRelease`).
CrabsTK seams 24–62, one per module/seam. Pro is `dormant: true` in the root
yaml (Script2 API deleted; numbered + scaffolded but excluded from the rollup).

### File Structure
```
_Seams.yaml                 — root module list (generated by --scan)
_Seams/
  _Seams.cpp                — main dispatch + macro table + main() [generated]
  _Config.h                 — #define _Seam _SeamN [user-owned]
  Release.hxx               — 14-module rollup [user-owned]
  CMakeLists.txt            — build for all modules [generated]
  pch.h                     — precompiled header
  <Module>/
    _Seams.yaml             — seam list (generated by --scan)
    __Seams.yaml            — legacy seam list with depends/ (user-owned)
    _Seams.cpp              — standalone binary [generated]
    _Seams.hxx              — gate + TTestTree [generated]
    01.Core.hxx             — user-owned test code
    02.<Seam>.hxx           — user-owned test code
    NN.Release.hxx          — release seam (may be empty)
```

### Generator
`Hatch/SyncSeams.py` — the seam generator:
- `--scan .` — scans `_Seams/<Module>/NN.<Seam>.hxx` files on disk,
  regenerates `_Seams.yaml` (root) and per-module yamls.
  Skips `Hatch/` and `JSX/`.
- `python3 Hatch/SyncSeams.py _Seams.yaml` — runs the generator,
  produces `_Seams.cpp`, per-module `_Seams.hxx` + `_Seams.cpp`,
  `CMakeLists.txt`. Does NOT clobber numbered `.hxx` test files.
- `--out <dir>` — writes generated files to a scratch directory
  instead of the real repo.

### Seam Conventions
- No C++ standard library includes in seam test files.
- No anonymous namespaces.
- No auto-generated stubs or boilerplate comments.
- Python for project scripts, named as functions (`SyncSeams.py`).

## Important Directories
- `_Seams/`: Test suites (seams).
- `_Config.h`: Project configuration.
- `Hatch/`: Python API + seam generator.
- `ASCIICrabs/`: Core dependency (sibling checkout, read-only).

## AI Behavior Rules
- **Verify before Implement:** Always read `_StyleGuide` files if unsure.
- **Mimic Context:** Use project-specific types (`CHA`, `ISC`, `FPC`) not
  standard `ISC`, `CHR`, `FPC`.
- **Seam Tests:** When adding functionality, look for the corresponding
  "Seam" in `_Seams/` and add a test case there.
- **Minimal changes:** If you don't have to do it, don't. Remove before you add.
  No drive-by refactors or renames — the user does bulk renames himself.
- **Don't touch git.** Agents only edit files. The user commits via
  `git add --all` + YOLO merge himself. Read-only git is fine; no writes.
- **Don't edit `_Seams/_Seams.cpp`** to change the seam or the `TTestTree`.
  Change the `_Seam` macro in `_Seams/_Config.h` instead.

## Hard-Won Pitfalls
- **Include-guard collisions:** A `.hpp` must use its OWN guard, never its
  sibling `.hxx`'s guard.
- **Stray `#else` / dropped `}` / mis-nested `#if`:** autocomplete inserts
  stray `#else`, drops function-body `}`, and corrupts constructor init lists.
  Diagnose with a real brace tokenizer on `g++ -E` output, not a regex.
- **`namespace CTK` opened inside a seam gate must close in the same gate.**
- **`#endif` comments are LIES** — trace real `#if` nesting with a stack,
  don't trust the label.
- **Cross-seam type dependency:** if a lower-seam file needs a type from a
  higher-seam-gated header, gate the consumer on that seam — don't pull the
  type down.

## How to Work With This Agent
- **Be technically exact** — the agent values precision over plausibility
- **Draw boundary diagrams** when ownership feels confusing
- **Separate confirmed causes** from hypotheses
- **Prioritize containment** and reproduction over blame during incidents
- **Run builds and tests** — the agent verifies results via tool output, not descriptions
- **Report hardware/proprietary limitations** if complete verification isn't possible
- **Do NOT modify git without explicit permission** — read-only inspection only
- **Do NOT restore files from git** — deleted files were removed on purpose

## Current Workspace
- **Root:** ~/AStarStarship/CrabsTK
- **Branch:** master → origin/master
- **Status:** 228 modified, 59 untracked
- **Recent commits:**
  - `29eefa90` Add JSX engine, Exo 3D renderer, update game engine and Hatch; convert all macros to _UpperCamelCase (#201)
  - `403582a5` Issue199 (#200)
  - `90eb85b2` Issue197 (#198)

## Conventions
- Values and identifiers preserved exactly as given — never "repair" malformed tokens
- When unsure, ask for clarification rather than guess
- "Done" means every named acceptance criterion verified — not a plausible subset

## Git Protocol
- **Do NOT undelete files from git.** A deleted or renamed file was done so on
  purpose — it wasn't supposed to be there, it got merged into another file, or
  it wasn't named correctly. Fix the current files in place; do not try to
  rescue or restore stale files from git history (no `git checkout -- <path>`,
  `git restore <path>`, `git show <ref>:<path>` written back to disk, or
  `git revert` to bring a deleted path back).
- **Only `master-agent` and the Captain may modify git.** No other agent or
  profile may run git write operations on a tracked repo — no `git commit`,
  `git add`, `git push`, `git reset`, `git rebase`, `git merge`, `git rm`,
  branch creation, force-push, or PR changes. Read-only inspection
  (`git status`, `git log`, `git diff`, `git show`, `git ls-files`) is allowed
  for anyone.

# Hatch — AStarship Project Generator

## Kanban board

This project belongs to the **AStarship** organization; its Kanban board slug is `astarship` (see `~/AStarStarship/AGENTS.md`). Always target `astarship` explicitly (`--board astarship` / `board="astarship"`). Never rely on the current-board pointer. If a task's assigned board differs, report the mismatch before acting — do not silently switch.

This directory contains the boilerplate files and tools for bootstrapping a new
project. It supports two project modes:

- **CrabsTK** — module-based, `CT` namespace, MPL headers, angle-bracket includes.
- **ASCIICrabs** — flat `_Seams/`, `ProjectTest` namespace, relative ASCIICrabs includes.

## What's Here

```
Hatch/
  CreateProject.py     Bootstrap script: copies template + renames PROJECT.
  SyncSeams.py         Regenerates seam files from seams.yaml.
  template/
    crabstk/           CrabsTK template files (static boilerplate).
      CMakeLists.txt   CMake build with ASCIICRABS_DIR cache variable.
      _Package.hxx     Single-TU rollup (user adds module includes).
      seams.yaml       Template seam config (replaced by CreateProject.py).
    asciicrabs/        ASCII Crabs template files (static boilerplate).
      _Config.h        Root config: includes Header, Footer.
      _ConfigFooter.h  Includes ASCIICrabs/_ConfigFooter.h (relative path).
      _ConfigHeader.h  Includes ASCIICrabs/_ConfigHeader.h (relative path).
      _Debug.h         Debug macros (D_COUT, D_PAUSE, A_TEST_BEGIN, etc.).
      _Release.h       Release macros (everything compiled out).
      _Package.hxx     Includes ASCIICrabs/_Package.hxx (relative path).
      _Test.h          Includes ASCIICrabs/Test.h (relative path).
      _Undef.h         Undefines the YOUR_MOM marker macro.
      pch.h            Precompiled header (MSVC only; can be ignored).
      CMakeLists.txt   CMake build for the seam tree.
```

## The Seam System

The build uses a **seam tree** — a compile-time switch that selects which
unit test (and debug level) to run. The `SEAM` macro is set in `_Seams/_Config.h`
to one of the seam numbers defined in `_Seams/_Seams.h`.

```
_Seams.h:
  #define PROJECT_CORE      1
  #define PROJECT_RELEASE   2
  #define SEAM_N PROJECT_RELEASE

_Seams/_Config.h:
  #define SEAM PROJECT_CORE     <-- change this to switch seams
```

Each seam test file is guarded by `#if SEAM >= NAME` and `#if SEAM == NAME`.
When `SEAM` is set to a given number:
- All seams **below** that number are compiled with `_Release.h` (debug compiled out).
- The seam **at** that number is compiled with `_Debug.h` (debug enabled).
- All seams **above** that number are not compiled at all.

**This is the core workflow.** Change one macro, rebuild, get the debug info
you need. No breakpoints, no logging config, no test selectors.

## Creating a New Project

```bash
cd ~/AStarStarship/CrabsTK/Hatch
python3 CreateProject.py <project_name> <output_dir> --mode <crabstk|asciicrabs>
```

Examples:

```bash
# CrabsTK project
python3 CreateProject.py FVM ~/FreedomGovernment/FVM --mode crabstk

# ASCII Crabs project
python3 CreateProject.py MyApp ~/Projects/MyApp --mode asciicrabs
```

The script:
1. Copies template files from `template/<mode>/` to the output directory.
2. Replaces `PROJECT` with the project name (uppercase).
3. Writes `seams.yaml` with the project name and mode.
4. Copies `SyncSeams.py` to the project directory.
5. Runs `SyncSeams.py` to generate initial seam files.
6. Prints next steps.

### After Creation (CrabsTK)

1. **Add your module source files** next to `_Package.hxx`.
2. **Edit `_Package.hxx`** to include your module headers.
3. **Edit `_Seams/<Module>/00.Core.h`** to add your test code.
4. **Build and run:**
   ```bash
   cd <output_dir>/_Seams
   cmake . && make
   ./MyProject
   ```

### After Creation (ASCIICrabs)

1. **Fix ASCIICrabs include paths** in `_ConfigFooter.h`, `_ConfigHeader.h`,
   `_Package.hxx`, and `_Test.h`. They use relative paths (`../../../../`).
   Adjust the depth to match your actual directory layout.
2. **Add your source files** next to `_Package.hxx`.
3. **Edit `_Seams/01.Core.hxx`** to add your test code.
4. **Build and run:**
   ```bash
   cd <output_dir>/_Seams
   cmake . && make
   ./MyProject
   ```

## Managing Seams with YAML

The `seams.yaml` file is the **single source of truth** for the seam system.
Never edit `_Seams.h`, `_Config.h`, or `_Main.cpp` by hand — they are
auto-generated.

### To add a seam:

1. Edit `seams.yaml` and append an entry to the `seams:` list:
   ```yaml
   seams:
     - name: FVM_CORE
       module: Fvm
       label: "Testing Fvm core..."

     - name: FVM_AUDIT        # <-- new seam
       module: Fvm
       label: "Testing Fvm audit..."

     - name: FVM_RELEASE
       module: Fvm
       label: "Release"
   ```

2. Run `python3 SyncSeams.py` from the project root.

3. Rebuild. The new seam appears in `_Seams.h`, `_Config.h`, `_Main.cpp`,
   and the module's `00.Core.h` with full debug/release gating.

### Seam YAML fields:

| Field | Description |
|---|---|
| `project` | Project name (macro prefix, uppercased). |
| `mode` | `crabstk` or `asciicrabs`. |
| `release` | Name of the seam that runs in release mode (`SEAM_N`). |
| `project_dir` | Where generated files go (defaults to the YAML file's directory). |
| `name` | Macro suffix + C++ test function name. |
| `module` | (CrabsTK) Namespace under `CT::`. Groups seams into modules. |
| `file` | (ASCIICrabs) Seam test filename in `_Seams/`. |
| `label` | Heading printed by the test. |

### Rules

- **Never renumber existing seams.** Other code may depend on the number.
  Add new seams at the end (or in gaps, but never reuse numbers).
- **The release seam must be the highest-numbered seam.** It becomes `SEAM_N`.
- **Seam names are stable.** Once a seam name is assigned, never rename it.

## Rules

- **Do NOT modify the template files in `template/`** unless the boilerplate
  itself needs fixing. Template changes affect all future projects.
- **Do NOT commit build artifacts** (`.o`, `build/`, binaries) to this repo.
- **Keep the template minimal.** Every file here must be required for a
  project to compile and run its seam tests.
- **`pch.h` is for MSVC.** It silences compiler warnings and enables
  precompiled headers. On GCC/Clang it is a no-op. Keep it for portability.
- **The `_Undef.h` / `YOUR_MOM` pattern** is a seam marker. Each `_Debug.h`
  and `_Release.h` defines `YOUR_MOM`; `_Undef.h` undefines it. This ensures
  only one debug/release header is active per translation unit.
- **Generated files say "AUTO-GENERATED" in the header.** If you see that
  comment, edit the YAML instead and re-run SyncSeams.py.

## Why This Format

The seam tree has been used for decades. The key benefit: **switching seams
is a one-macro change**, and the debug info you get is exactly what you need
for that seam. No test framework config, no logging levels, no CI matrix.

The boilerplate is heavy but **fully automatable** via `CreateProject.py`
and `SyncSeams.py`. The time investment is paid back every time you need to
debug a specific seam without wading through unrelated output.

## Script Convention

When creating scripts for this repo or any AStarship project:
- **Use Python, never shell.** Shell scripts are "too limiting."
- **Name scripts as functions** — `CreateProject.py`, `SyncSeams.py`, not
  `create-project.sh` or `sync_seams.py`. Function-style names signal
  immutability from the outside, mutability from the inside.

## Code Style — Chimera+ (AStarship Standard)

All **code and filenames** in the AStarship ecosystem follow the **Chimera+**
style guide (`~/AStarStarship/ASCIICrabs/__ChimeraPlus.md`).

### Core Rule: immutable vs mutable
- **Immutable** (can't change after init) → **CamelCase**
- **Mutable** (changes at runtime) → **lower_snake_case**

### Naming Summary
| Element | Convention | Example |
|---|---|---|
| Type aliases (fixed-width) | 3-letter CAPS codes | `CHA` char8, `ISC` int32, `IUD` uint64, `BOL` bool, `FPC` float32, `FPD` double64 |
| Structs / classes | CamelCase + T(POD)/A(utoject) prefix | `TArray`, `AMap`, `Crabs` |
| Struct members (mutable) | lower_snake_case | `socket_bytes`, `header_bytes` |
| Struct members (immutable) | CamelCase | `StackTotalMin`, `ColumnWidth` |
| Free functions / methods | CamelCase, type-prefixed | `CrabsInit`, `TArrayBytes`, `TMapFind` |
| Function suffixes | `_NC` (no-check), `C` (const/count), `T` (POD), `A` (autoject) | `TArrayInsert_NC`, `CSizeMin` |
| Local variables | lower_snake_case (always) | `bytes_data`, `read_cursor` |
| Macros / #define | UPPER_SNAKE_CASE | `CRABS_RUN_TESTS`, `CPU_X64` |
| Private/protected members | trailing underscore | `aobj_`, `array_` |
| Namespaces | `namespace _ { ... }` | single shared underscore namespace |
| DB names / tables / columns | lower_snake_case (mutable) | `user_sessions`, `order_items` |
| Client-facing API / headers | CamelCase (immutable contract) | `OrderService`, `PaymentGateway` |
| Filenames (client contract) | CamelCase | `OrderService.h` |
| Filenames (host/storage) | lower_snake_case | `user_sessions.py` |

### Header Boilerplate (C/C++)
```
// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_<NAME>_<H|HPP>
#define CRABS_<NAME>_<H|HPP>
#include "..."
#if SEAM >= CRABS_<NAME>
... body ...
#endif
#endif
```

### Formatting
- 2-space indent
- Opening brace same line (functions/control), next line (structs/classes)
- `D_ASSERT()`, `D_COUT()`, `D_RETURNT(type, val)` for debug/assert/return
- `NILP` = nullptr
- `alignas(ACPUCacheLineSize)` for hot structs
- Doxygen `/** @param @return @pre @link @see @code */` comments

### The One-Sentence Version
**Immutable → CamelCase, mutable → lower_snake_case; macros UPPER_SNAKE;
private/protected get a trailing `_`; types are short CAPS width-codes
(CHA/ISC/IUD/BOL); structs are CamelCase with T(POD)/A(utoject) prefix;
functions are CamelCase with T/A kind prefixes and _NC/C markers; locals are
snake_case; everything lives in `namespace _`; every file starts with AStarship
copyright, `#pragma once`, a `CRABS_*` guard, and `#if SEAM >= CRABS_*`
gating.** The name tells you the kind, width, access level, and mutability —
before you read the body.

# PrimeTask.md — astar-prime parallel conversion task

## Task Overview
Convert Exo STB single-header text editing library to ASCIICrabs memory model and Chimera+ conventions. This is the largest conversion, providing the foundation for the RPC-based text editor demo.

## Assigned Files
1. `Exo/imstb_textedit.h` — stb_textedit styled text editing algorithm (≈350 functions)

## Conversion Rules (Chimera+ + ASCIICrabs)
- **No C++ standard library**: Remove/all Replace `#include <string>`, `<vector>`, `<cmath>`, `<cstdlib>`, `<iostream>`. Use project types (`CHA`, `ISC`, `IUD`, `FPC`, `FPD`, `BOL`).
- **Type aliases**: `ISC` = signed 32-bit (or pointer-size alias via `_ConfigHeader.h`). `IUD` = unsigned 32-bit (or pointer-size). `FPC` = float32. `FPD` = float64. `BOL` = BOL.
- **Renaming**: `stb_` prefix → `Exo` where practical (e.g., `stb_textedit` → `ExoEdit`). Struct members: `clicky` → `is_click`, `scissors` → `clip_rect`, etc. Follow Chimera+: immutable → CamelCase, mutable → lower_snake_case.
- **Arena allocation**: Replace `malloc`/`free` with `Exo__alloc`/`Exo__free`. Replace `realloc` with `Exo__realloc(p, old_bytes, new_bytes)`. Replace `memset` → `_::ArrayFill`, `memcpy` → `_::ArrayCopy`. The `undo`/`redo` stacks must live in a `Room → Wall → root List` slab (already proven); do NOT use C heap.
- **State structs**: `stb_texteditundo`, `stb_texteditundo`, `stb_textedit` → arena-allocated or `AExoArena<>`-owned. Free functions `TWallRoot`/`TWallGetSizeBytes` for sizing.
- **Seam gating**: `#if _Seam _CrabsRelease` / `#if _Seam _iGeekCore` gates. Do not edit `_Seams/_Seams.cpp` to change seam — change `_Seam` macro in `_Config.h`.
- **No `IUN`**: Use `IUC` (unsigned 32-bit) or size-based alias (`ISD`/`IUD` on 64-bit, `ISC`/`IUC` on 32-bit).
- **Pointer width**: `_CPUSize` derived from `__SIZEOF_POINTER__` (16/32/64-bit). Use `ISD`/`IUD` on 64-bit; `ISC`/`IUC` on 32-bit.
- **Bulk renames must NOT touch GLSL string literals** — verified earlier on `nanovg_gl.h`.
- **Format**: 2-space indent, opening brace same line (functions/control), next line (structs). No anonymous namespaces. No `#pragma once` masking unless file already has it.

## Output
One clean `.h` file that compiles under wasm32 (`-sMEMORY64=0`) and wasm64 (`-sMEMORY64=1`) with 0 errors. File placed in `Exo/` (overwriting original) or scratch directory as approved.

## Constraints
- Do not create new files unless approved. This is a replacement in-place.
- Do not touch git. Read-only inspection only.
- The `undo`/`redo` state must NOT use `malloc`/`free` — it must live in the ASCIICrabs arena (Room/Wall/List). This is the hardest part; plan the slab layout before coding.
- If a lower-seam file needs a type from a higher-seam-gated header, gate the consumer on that seam — do not pull the type down.
- Font/header low priority — work around STB unless major architectural hurdle.

## Notes for Agent
- This is the largest conversion (~350 functions). Expect significant find-and-replace work.
- Key data types to convert: `char*` → arena `CHA[]`, `int` → `ISC`/`ISZ`, `unsigned` → `IUD`/`ISZ`, `float` → `FPC`.
- The `stb_textedit` struct contains `char* cursor`, `char* select_start`, `char* select_end`, `int len`, `int temp_len`, `int row_start`, `int row_len`, `char undostate`, `int undotimer`, `int justwhatmewant`, `int yscroll`, `int sel`. All need arena or fixed-width replacement.
- If you encounter a pattern you can't resolve with the ASCIICrabs types, pause and note it — do not guess.
- After conversion, rebuild the wasm32 + wasm64 demos to confirm 0 errors and the texture round-trip still works.

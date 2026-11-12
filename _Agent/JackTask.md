# JackTask.md — astar-jack parallel conversion task

## Task Overview
Convert Exo STB single-header libraries to ASCIICrabs memory model and Chimera+ conventions. These converted headers will enable C++ WebGL scientific visualizations without C heap, using the ASCIICrabs arena allocator and Chimera+ naming.

## Assigned Files
1. `Exo/imstb_rectpack.h` — stb_rectpack 2D bin-packing algorithm
2. `Exo/imstb_truetype.h` — stb_truetype font rasterization

## Conversion Rules (Chimera+ + ASCIICrabs)
- **No C++ standard library**: Remove/all Replace `#include <string>`, `<vector>`, `<cmath>`, `<cstdlib>`, `<iostream>`. Use project types (`CHA`, `ISC`, `IUD`, `FPC`, `FPD`).
- **Type aliases**: `ISC` = signed 32-bit (or pointer-size alias via `_ConfigHeader.h`). `IUD` = unsigned 32-bit (or pointer-size). `FPC` = float32. `FPD` = float64.
- **Renaming**: `stbtt_*` → `ExoTT_*` (all `nvg*` already done). Free functions stay camelCase; struct members lower_snake_case.
- **Arena allocation**: Replace `malloc`/`free` with `Exo__alloc`/`Exo__free`. Replace `realloc` with `Exo__realloc(p, old_bytes, new_bytes)`. Replace `memset` → `_::ArrayFill`, `memcpy` → `_::ArrayCopy`.
- **Seam gating**: `#if _Seam _CrabsRelease` / `#if _Seam _iGeekCore` gates. Do not edit `_Seams/_Seams.cpp` to change seam — change `_Seam` macro in `_Config.h`.
- **No `IUN`**: Use `IUC` (unsigned 32-bit) or size-based alias (`ISD`/`IUD` on 64-bit, `ISC`/`IUC` on 32-bit).
- **Pointer width**: `_CPUSize` derived from `__SIZEOF_POINTER__` (16/32/64-bit). Use `ISD`/`IUD` on 64-bit; `ISC`/`IUC` on 32-bit.
- **Bulk renames must NOT touch GLSL string literals** — verified on `nanovg_gl.h` earlier.
- **Format**: 2-space indent, opening brace same line (functions/control), next line (structs). No anonymous namespaces. No `#pragma once` masking.

## Output
Two clean `.h` files (no `#pragma once` added unless file already has it) that compile under wasm32 (`-sMEMORY64=0`) and wasm64 (`-sMEMORY64=1`) with 0 errors. Files placed in `Exo/` (overwriting originals) or a scratch directory as approved.

## Constraints
- Do not create new files unless approved. These are replacements in-place.
- Do not touch git. Read-only inspection only.
- If a lower-seam file needs a type from a higher-seam-gated header, gate the consumer on that seam — do not pull the type down.
- Font header low priority — work around STB unless major architectural hurdle.

## Notes for Agent
- `imstb_rectpack.h` has ~20 functions, small surface area. Good "first conversion" to validate the pattern.
- `imstb_truetype.h` has ~180 functions, font rasterization, `memcpy`/`malloc` patterns. Shares `stbtt__newcontext` / `stbtt__packedchar` types with textedit.
- Both headers reference `stbrp_coord` (→ `ISC`) and `stbtt_fontinfo` (→ rename + arena).
- If you hit a macro or type you can't resolve, pause and note it — do not guess.

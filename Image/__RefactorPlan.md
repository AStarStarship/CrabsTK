# Image/ Third-Party Removal Plan

## Problem

Image/ contains 16,307 lines of third-party code across 25 files. The goal is
to replace all of it with in-house code on the ASCIICrabs no-stdlib type system.

## Current Inventory

| File | Lines | License | What it does |
|------|-------|---------|--------------|
| stb_image.h | 7,240 | Public domain | Image loading (BMP, JPEG, PNG, PSD, GIF, HDR, PIC, PNM, PVR, PPM, DDS) |
| stb_image_write.h | 1,202 | Public domain | Image writing (BMP, PSD, PGM, PPM, TGA, HDR) |
| SOIL2.h/.hxx | 3,147 | MIT | OpenGL texture upload, resampling, MIPmaps, flip, Y-flip |
| image_DXT.h/.hxx | 671 | Public domain | DXT1/DXT5 compression (S3TC) |
| etc1_utils.h/.hxx | 782 | Apache 2.0 | ETC1 compression (OpenGL ES) |
| image_helper.h/.hxx | 550 | MIT | Upscale, downscale (MIPmaps), NTSC safe |
| jo_jpeg.h | 493 | Public domain | JPEG writer (baseline) |
| pvr_helper.h | 264 | MIT | PVR (PowerVR) format helper |
| pkm_helper.h | 19 | MIT | PKM (PowerVR) format helper |
| stbi_DDS.h/_c.h | 576 | Public domain | DDS load/save (uses image_DXT) |
| stbi_pvr.h/_c.h | 949 | Public domain | PVR load/save |
| stbi_pkm.h/_c.h | 258 | Public domain | PKM load/save |
| stbi_ext.h/_c.h | 97 | Public domain | stb_image extensions |
| Image.h/.hxx | 45 | AStarship | Skeleton (empty CImage, stub Star()) |
| _Package.h/.hxx | 14 | AStarship | Rollup includes |
| **Total** | **16,307** | | |

## Dependency Graph

```
_Package.hxx
├── Image.hxx (skeleton, no real impl)
├── SOIL2.hxx
│   ├── stb_image.h  (7,240 lines - the big one)
│   │   ├── stbi_DDS_c.h ──> image_DXT.h
│   │   ├── stbi_pvr_c.h ──> pvr_helper.h
│   │   ├── stbi_pkm_c.h ──> etc1_utils.h, pkm_helper.h
│   │   └── stbi_ext_c.h
│   ├── stb_image_write.h
│   ├── image_DXT.h
│   ├── image_helper.h
│   ├── jo_jpeg.h
│   ├── pkm_helper.h
│   └── pvr_helper.h
├── etc1_utils.hxx
├── image_DXT.hxx
└── image_helper.hxx
```

## Who Uses Image/

- **Root `_Package.hxx`**: `#include "Image/_Package.hxx"` (rollup)
- **`_Seams/_Main.cpp`**: `#include "Image/00.Core.h"` (MISSING FILE — build broken)
- **Nothing else** references SOIL_*, stbi_*, jo_*, etc1_*, or Image_* symbols

The module is dead weight. The only "consumer" is the rollup include.

## What We Actually Need

From the OBS setup (26 UVC capture cards, 4K/1080p/2048x1280) and the
CrabsTK vision, the minimum viable Image module needs:

1. **Image loading**: BMP, PNG, TGA, DDS (for OBS texture upload)
2. **Image writing**: BMP, PNG, TGA (for debug/screenshot)
3. **Texture upload**: Raw RGBA/RGB buffer → OpenGL (the SOIL2 core value)
4. **MIPmap generation**: Downscale chain for texture pyramids
5. **Resampling**: Power-of-two resize (for legacy OpenGL)
6. **Y-flip**: OpenGL origin is bottom-left, image origin is top-left

## Plan: Phased Replacement

### Phase 1: Core Image Buffer (no third-party)
**Files to create:**
- `Image/IBuffer.h` / `IBuffer.hxx` — Image buffer struct + alloc/free
  - `struct TImageBuffer { IUA* data; IUN width; IUN height; IUB channels; }`
  - Alloc, free, copy, clear
  - Replace: SOIL2's `unsigned char*` + `int width, int height, int channels` triplet

### Phase 2: BMP (load + save)
**Files to create:**
- `Image/IBmp.h` / `IBmp.hxx` — BMP reader/writer
  - 24-bit and 32-bit BMP, bottom-up, no RLE
  - ~200 lines (BMP is trivial)
  - Replaces: stb_image.h BMP section + stb_image_write.h BMP section

### Phase 3: TGA (load + save)
**Files to create:**
- `Image/ITga.h` / `ITga.hxx` — TGA reader/writer
  - Uncompressed and RLE, 24/32-bit
  - ~250 lines
  - Replaces: stb_image.h TGA section + stb_image_write.h TGA section

### Phase 4: DDS (load, save as DXT1/DXT5)
**Files to create:**
- `Image/IDds.h` / `IDds.hxx` — DDS reader/writer
  - DXT1, DXT5 compressed textures
  - ~400 lines
  - Replaces: stbi_DDS.h/_c.h + image_DXT.h/.hxx

### Phase 5: MIPmaps + Resampling
**Files to create:**
- `Image/IMipmap.h` / `IMipmap.hxx` — MIPmap generation
  - Box-filter downscale, power-of-two resize
  - ~200 lines
  - Replaces: image_helper.h/.hxx

### Phase 6: OpenGL Texture Upload
**Files to create:**
- `Image/ITexture.h` / `ITexture.hxx` — GL texture upload
  - `IUploadTexture(TImageBuffer*, ISN glTexId, ISN flip)`
  - `IUploadCompressed(TImageBuffer*, ISN glTexId, ISN glInternalFmt)`
  - ~150 lines (thin wrapper over glTexImage2D)
  - Replaces: SOIL2's OpenGL functions

### Phase 7: PNG (load only, via libpng or in-house)
**Decision needed**: PNG decoding is the hard part.
- Option A: In-house PNG decoder (~800 lines: zlib inflate + IDAT filter)
- Option B: Keep stb_image.h PNG decoder only (extract just the PNG code)
- Option C: Link system libpng (introduces a dependency, but it's everywhere)

### Phase 8: JPEG (load + save)
**Decision needed**: JPEG is the other hard part.
- Option A: In-house JPEG decoder/encoder (~1500 lines)
- Option B: Keep jo_jpeg.h for writing (493 lines, public domain, self-contained)
- Option C: Link system libjpeg

### Phase 9: Update _Package.h/.hxx
- Remove all stb_*, SOIL2*, etc1_*, jo_* includes
- Include only the new I*.h files
- Create `Image/00.Core.h` (the missing file that _Seams/_Main.cpp expects)

### Phase 10: Delete third-party files
- Remove all stb_*, SOIL2*, etc1_*, jo_*, pvr_*, pkm_* files
- Update ReadMe.md

## Type Mapping (third-party → ASCIICrabs)

| Third-party | ASCIICrabs |
|-------------|-----------|
| `unsigned char` | `IUA` |
| `int` (dimensions) | `ISN` |
| `unsigned int` | `IUN` |
| `const char*` (filenames) | `const CHA*` |
| `char` (buffer) | `CHA` |
| `void*` | `void*` (kept for GL interop) |
| `bool` / `int` (return) | `BOL` / `ISN` |

## Naming Convention (Chimera+)

- Immutable: `TImageBuffer`, `IBmpHeader`, `IDdsHeader`
- Mutable: `image_buffer`, `bmp_header`
- Functions: `IbLoadBmp`, `IbSaveBmp`, `IbAlloc`, `IbFree`
- Constants: `I_MAX_DIM`, `I_BMP_MAGIC`

## File I/O

The ReadMe says "replacing file IO with Crabs." The Crabs file I/O layer is
not yet defined. For now, use `fopen/fread/fwrite` (C stdio) behind a thin
wrapper:

```c
struct TFileIO {
  void* handle;    //< FILE* (opaque)
  BOL error;
};
IUA* IFileReadAll(const CHA* path, IUN* out_size);
BOL IFileWriteAll(const CHA* path, const IUA* data, IUN size);
```

This isolates stdio so we can swap it for Crabs I/O later.

## Build Strategy

Single translation unit, same as Code module:
- `_Package.hxx` includes all `.hxx` files
- Each `.h` declares, each `.hxx` defines inside `namespace _`
- No `using namespace _;` in `.hxx` files (learned from Code module bug)
- All functions in `namespace _`
- Types use ASCII Crabs typedefs from `_Config.h`

## Test Strategy

Standalone test with type shim (same pattern as Code/Tests/):
- `Image/Tests/ImageModuleTest.cpp`
- Round-trip: write BMP → read BMP → compare pixels
- Round-trip: write TGA → read TGA → compare pixels
- MIPmap: 4x4 image → 2x2 MIP → verify downscale
- DDS: RGB → DXT1 → DXT1 → RGB (lossy, check bounds)

## Estimated Effort

| Phase | Lines to write | Complexity |
|-------|---------------|------------|
| 1. IBuffer | ~100 | Trivial |
| 2. BMP | ~200 | Trivial |
| 3. TGA | ~250 | Easy |
| 4. DDS | ~400 | Medium |
| 5. MIPmaps | ~200 | Easy |
| 6. GL upload | ~150 | Trivial |
| 7. PNG | ~800 | Hard |
| 8. JPEG | ~1500 or keep jo_jpeg | Very hard |
| 9. _Package update | ~30 | Trivial |
| 10. Delete + cleanup | — | Trivial |
| **Total** | **~3,630–5,130** | |

## Immediate Blockers

1. **`Image/00.Core.h` is missing** — `_Seams/_Main.cpp` includes it but it
   doesn't exist. The build is already broken for the seam test tree.
2. **PNG and JPEG** are the two hard decoders. Need a decision on
   in-house vs system library vs keep-vendor for these two formats.

## Recommendation

Start with Phases 1–6 (BMP, TGA, DDS, MIPmaps, GL upload). These cover the
OBS use case (texture upload) and are all manageable complexity. PNG and JPEG
can be addressed separately — for JPEG writing, jo_jpeg.h is already
self-contained and public domain; for PNG, we need to decide.

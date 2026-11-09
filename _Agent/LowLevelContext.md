# Low-Level Context — Exo → CrabsTK/ASCIICrabs

Captured by the low-level engineer. This is a working handoff, not a spec:
it records the state of the tree, the decisions made, what is verified
green, what is pre-existing breakage, and the open calls. Re-read before
acting; branch and dirty state move under the user's hands.

## Role
Low-level / native systems engineer for AStarEngineering. Territory here is
the Exo Vector Graphics API being converted to the ASCIICrabs / CrabsTK
no-stdlib type system, and the Crabs Machine memory model that will back it.
Single-translation-unit builds; no C++ standard library; validate through
seams.

## Repos
- CrabsTK:  /home/astar/AStarStarship/CrabsTK   (branch Issue201)
- ASCIICrabs: /home/astar/AStarStarship/ASCIICrabs  (upstream dependency, read-only
  for me unless the user says otherwise — the user has, for this work)
- iGeek:  /home/astar/AStarStarship/iGeek  (sibling consumer of CrabsTK/Exo)

Build (canonical, per module):
    cd CrabsTK
    clang++ -std=c++23 -g -o /tmp/exo _Seams/Exo/_Seams.cpp \
        -I_Seams/Exo -I_Seams -I. -I../ASCIICrabs -I..

ASCIICrabs release:
    cd ASCIICrabs && g++ -std=gnu++23 -O0 -o /tmp/core _Seams/_Seams.cpp -I_Seams -I.

## Architecture decision (the core of this work)
The Exo memory model is the Crabs Machine, not a per-object allocator:

- One `TRoom<>` instance lives in the .cpp entry file and is passed by
  reference down the call chain (TRoom is NONCOPYABLE: copy ctor/assign
  deleted; ctor is `TRoom(const CHA* name="chinese_room", ISC state_count=2)`).
- Wall and Floor are both just an ASCII List (TList). No distinct Wall data
  model. The Floor is a 2048-byte inline List; a Wall is a List over a larger
  block. Room has `TRoom::AddWall(TWall*)` and up to WallCountMax=5 walls.
- The dynamic handle is `_::Autoject` (no templates): `{ IUW* origin;
  RAMFactory ram; }`, 16 bytes, type-erased. It starts on the stack and
  auto-grows to the heap. Payload is typed by the reader (BIn/BOut + a DT type
  word), not by the handle, so it can be passed into functions and over RPC
  as plain ASCII data.
- Data lives in a List Wall (bump-allocated, offset-addressed, type-tagged).
  Offsets not pointers, so it survives re-basing and serializes/undoes.

Why it is different from every other API: allocation is a scope, not a
malloc/free pair; every live buffer is a typed tuple the machine can
enumerate, serialize, and roll back; the frame (BeginFrame/EndFrame) is the
checkpoint; there is no free().

## Verified green (this session, via ad-hoc harness, not committed gate)
- Exo seam (38): 28 PASS / 0 FAIL / exit 0.
- ASCIICrabs release: "Unit tests completed successfully!" / exit 0.
- `TRoom<>` instantiates: Floor() non-nil, WallCount()==0 (probe exit 0).
- Exo/VG.hxx: 0 -Wdouble-promotion / -Wfloat-conversion warnings.

## DONE this session: NanoVG/Exo vector layer off the C heap
Converted `Exo/VG.hxx` and `Exo/fontstash.h` so they carry no C heap or
C-string dependency. Both are exercised by the running Exo seam (AVCanvas wraps
NVGcontext; the 28 pixel assertions draw fill/stroke/circle through the new
allocator), so this is runtime-verified, not compile-only.

- Allocator: every `malloc/realloc/free` now routes through a file-local shim
  (`nvg__alloc/free/realloc`, `fons__alloc/free/realloc`) backed by ASCIICrabs'
  `_::ObjectFactoryHeap`. The RAMFactory has no in-place grow, so `__realloc`
  allocs the larger block, `ArrayCopy`s the old live bytes, frees the old, and
  keeps the original on a failed grow. `old_bytes` is the live size to preserve
  (the current count * element size at each site; 0 where the buffer is
  rewritten immediately after — the verts scratch array and the texData reset).
- Byte ops: `memset(p,0,n)` → `_::ArrayFill(p,n)` (12 + 10 sites);
  `memcpy(d,s,n)` → `_::ArrayCopy(d,n,s,n)` (6 + 2 sites).
- Strings: `strlen` → local `__cstrlen`; `strcmp` → local `__cstrcmp`;
  `strncpy(font->name,…)` → a strlen-bounded `_::ArrayCopy` (name[] is zeroed
  at alloc, tail forced NUL on the next line). `size_t` → `ISW` (4 sites).
- Includes: dropped `<stdlib.h>` and `<string.h>` from VG.hxx; added
  `<ASCIICrabs/Array.hxx>` + `<ASCIICrabs/Autoject.hxx>`.
- Verified: Exo seam 28 PASS / 0 FAIL / exit 0; 0 real C-stdlib allocator/string
  calls left in either file; 0 new warnings under -Wall -Wextra.

Left intentionally (do not "fix" without a call):
- Font-file I/O in `fonsAddFont` (fopen/fseek/ftell/fread/fclose/FILE via
  `<stdio.h>`) — the low-priority loader the user said to work around.
- `imstb_truetype.h` / `imstb_rectpack.h` — the sanctioned stb font decoder;
  its `<math.h>` (13 -Wdouble-promotion warnings) is the one allowed stdlib user.
- The "Uniprinter Unicode API" text pipeline — a redesign of how nanovg emits
  text (fontstash → stb glyphs → text verts), not a mechanical swap. Needs a
  design decision before touching. This is the open piece of the
  "ArrayWrite/ArrayFill/Uniprinter" goal: ArrayFill/ArrayCopy are done.

## Pre-existing breakage (NOT caused by the typedef-struct removal, do not
## conflate with the nuke)
- Per-module seams (13 of 14) fail with `expected namespace name` at
  `_Seams/<Mod>/_Seams.hxx` and `using namespace CTK;` in `_Seams.cpp`,
  because no per-module TU ever includes a header that opens `namespace CTK`.
  Only Exo compiles (it pulls Exo/_Package.hxx through 01.Core.hxx). The
  generated diff shows only macro renames, so this is a generator gap, not the
  nuke. Fixing it needs the generator to emit a CTK-opening include per module.
- Data, Plot, Env seams have their own deeper failures (PlotTests/ImageDraw,
  DatasetDetail, etc.).

## The typedef-struct nuke (user did this; I fixed the fallout)
The user removed every `typedef struct C` pattern (0 remain in ASCIICrabs).
That was safe for `typedef struct NAME {…} NAME;` (converted to `struct NAME
{…};`) but deleted the opaque FORWARD declarations that had no body to
convert to. Exo fallout (all fixed, verified):
- Exo/VG.h: lost `typedef struct NVGcontext NVGcontext;` → added `struct
  NVGcontext;`.
- Exo/fontstash.h: lost `typedef struct FONScontext FONScontext;` → added
  `struct FONScontext;` (global scope, that file has no namespace).
- Exo/imstb_truetype.h: lost forward decls used before their definitions →
  added `struct stbtt_pack_context;`, `struct stbtt_fontinfo;`,
  `struct stbrp_rect;` inside the `#ifdef STB_TRUETYPE_IMPLEMENTATION` block.
Pattern: any later module brought back will show the same lost-forward-decl
class until its structs are forward-declared.

## Math.h FPC overloads (user: "the Math.h API needs FPC version")
ASCIICrabs Math.h was FPD-only (double, self-contained, no libm). nanovg works
in FPC (float), so every call promoted to double and narrowed back, tripping
-Wdouble-promotion and costing a round trip. Added FPC overloads for the whole
surface (21: trig, exp/log, Pow, SquareRoot, ErrorFunction, Absolute, Floor,
Ceiling, Min, Max, IsFinite/NaN/Infinite). Each one delegates to its FPD
kernel and narrows once, so there is one implementation and the FPC result is
the float nearest the FPD one.
- Declared in ASCIICrabs/Math.h (under `#if _Seam >= _CrabsMathCore`).
- Defined in ASCIICrabs/Math.hxx, at the end of `namespace _`.
- Mixed arguments are ambiguous by design (Min(1.0F,2.0) is undefined); cast
  at the call site.

### VG.hxx conversion fixes (user started; I finished the broken bits)
The in-progress edit left 64 compile errors. Fixed:
- 4 malformed casts `_::SquareRoot(FPD(a)(…)` → `_::SquareRoot(…)` (the FPC
  overload makes the cast unnecessary) — lines ~121, 689, 978, 979.
- `FPD(a) - FPD(b) * _::Floor(FPD(a)/FPD(b))` → `a - b * _::Floor(a / b)`
  (nvg__modf, line ~108).
- Wrong names: `Arccosine` → `ArcCosine`, `Tan2` → `ArcTangent2`.
- Real bug: `nvgHSLA`'s alpha was typed `CHR` (signed char), so the `255`
  passed by `nvgHSL` arrived as -1 and every HSL colour got alpha -0.0039.
  Changed signature `CHR a` → `IUA a` in VG.h:154 and VG.hxx:409, and
  `nvgHSLA(h,s,l,255)` → `nvgHSLA(h,s,l,IUA(255))`. Added two Exo assertions
  (nvgHSL alpha opaque, hue 0 is red).
- nvgTransformInverse: cast all FPD operands to kill the partner promotions.
  Also fixed `ISC ak = Absolute(k)` (int → ambiguous between FPC/FPD
  overloads) to `ISC ak = k < 0 ? -k : k;`.

### Known kernel accuracy limit (not fixed, needs a call)
`_::Log_e` range-reduces x = m*2^k with m in [1,2) and uses an alternating
series in u = m-1. When m is near 2 (u near 1) that series converges slowly;
40 terms leaves ~8e-4 error. So `LogTen(1000)` returns 3.0008247 (want 3.0),
and `Pow` for non-integer exponents inherits it. Six-line fix: reduce into
m in [1/√2, √2] and use the atanh series in u = (m-1)/(m+1) (|u| < 0.41,
converges in ~10 terms to full precision). Not applied — waiting for the
user's call since it touches kernel numerics used by BigInt.

## Wall is not usable yet (blocks the next step)
`TRoom<>` constructs fine, but `TWallInit` does not compile. In Wall.hpp,
`TWall` stores `doors_` BY VALUE (`TStack<TDoor<ISZ>,ISZ,ISY> doors_;`), and
`TWallInit` does `wall->doors_ = TStackInit<TDoor<ISZ>,ISZ,ISY>(socket,
bytes);` — wrong template args (passes TDoor where SCK is expected) and
assigns a pointer into a by-value object. It is latent (templates only
instantiate on use) so the release suite never hits it, but the moment Exo
tries to allocate on a Wall it breaks. Doors already live in the List, so the
forward fix (per the "wall and floor are both just ASCII List" decision) is to
make `TRoom::walls_` a `TList*` (or give TWall a `TList` header over its
block) rather than a door stack. Needs the user's nod — it is a Room.hpp
layout change.

## Autoject grow path is unimplemented
`_::Autoject`'s "auto-grows from stack to heap" is documented but the grow
operation is not in the factories. `TObjectFactory::Heap(buffer, bytes)`
deletes on any non-nil buffer regardless of `bytes` (so grow == free), and
`RAMFactoryNew` (the "checks if the size can double" enum) is never passed
anywhere — only `RAMFactoryDelete` is. Exo's frame scratch and growable vertex
arrays will need this. The RAMFactory has no context word
(`IUW* (*)(IUW*, ISW)`), so a List/Wall-backed factory cannot name its arena
without a header in the block or an explicit Room parameter. This is the
"battle test our memory API" work.

## Open decisions (waiting on the user)
A. RAMFactory: keep the three explicit paths (AImage takes a List/Wall,
   nanovg gets it via NVGparams::userPtr, STB via its existing `void* up`) vs.
   extend RAMFactory with a context word (touches Autoject everywhere).
B. Wall shape: one List per Wall (rollback granularity = wall) vs. one big
   List with typed sub-blocks. Leaning one List per Wall.
C. `TRoom::walls_` → `TList*` (Wall-as-List) — needs the nod.
D. Log_e accuracy: apply the atanh-series fix or leave it.

## Seam bookkeeping
- `_Seam` is the macro and carries the seam under work; set in
  `_Seams/_Config.h`. `_SeamRequest` is gone.
- Seam numbers are per-project (CrabsTK's _CrabsTKDataCore is 29 here).
- Per-module TUs use the module's own _Seams.cpp; the full-tree TU is
  _Seams/_Seams.cpp and is blocked by the Data/Plot/IMUL/IPC pre-existing
  failures, so the per-module targets are the working path.
- Hatch/SyncSeams.py regenerates the seam files; a new numbered file in
  _Seams/Exo would renumber 24 downstream macros, so keep the Exo work in the
  existing Core seam (38) until it needs its own surface.

## Style / rules for this code
- Types: FPC float32, FPD double64, ISC/ISC signed, IUC/IUA unsigned,
  CHA/CHR, BOL, PTR, _NilP. No C++ stdlib includes.
- No exceptions; error codes or _AAssert / _DCheck in seam tests.
- `_::` for ASCIICrabs free functions (Sine, SquareRoot, Floor, TListAlloc,
  …), `CTK::` for CrabsTK, `CTK::Exo` for the vector layer (the old
  `CTK::Image` / `Image::` is GONE; do not reintroduce the alias).
- TList is already a bump arena: TListAlloc aligns by the type word,
  offset-addressed, per-value type tag. Missing for frame use: TListMark /
  TListRestore (save {top,count,generation}), stale-index rejection, in-place
  grow-if-top, and a Wall↔List placement.

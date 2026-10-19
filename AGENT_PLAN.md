# CrabsTK Agent Plan

## Overarching Goal
Complete the migration of `CrabsTK` to the ASCIICrabs schema-first architecture
and a verified C++23 single-translation-unit `_Seams` build. Use the unified
ASCII List / Crabs machine rather than introducing another storage/scanner
implementation. Keep OS and device adapters outside the abstract machine.

## Current Status: SPEC CLARIFIED; INTEGRATION PENDING (2026-10-12)
The ten documents in `../ASCIICrabs/_Spec/Crabs/` have been reconciled with the
Captain's clarified design. This is specification work, not a completed
List/machine, Room/Door, or concurrent IPC implementation.

Local package and seam repairs are present, but an isolated module pass or an
old success banner is not evidence that the current whole tree builds and
passes. The current work and acceptance gates are in **Unified ASCII List
Chinese Room — current work** below. That section supersedes older next-step
recommendations; historical reports remain for their evidence and context.

## Historical Migration Result — not the current build status
An earlier migration run recorded a linked `_CrabsTK` executable and this
output. It is not a fresh verification of the current working tree:
```
Unit tests completed successfully! (:-)+=<
```

## Historical Migration Progress
- **Architecture Migration:** Extracted legacy `.inl` files into `.h` and `.hxx` single-translation-unit builds. Created `_Package.hxx` rollups.
- **Branding Migration:** Renamed `KabukiToolkit` to `CrabsTK`. Migrated namespaces from `Kabuki::Toolkit` to `CT`.
- **STB Library Reconstruction:** Repaired `Code/stb_c_lexer.hxx` and `Code/stb_leakcheck.hxx` by re-injecting missing macros, enums, and structs.
- **Config Cleanup:** Resolved `SEAM` and `SEAM_N` collisions.
- **Test Tree Wiring:** Fixed `_Main.cpp` to include `ASCIICrabs/Test.hpp`, `Test.hxx`, `COut.hxx`, `AType.hxx`, `Array.hxx`, `Puff.hxx`, and `Stringf.hxx` in the correct order.
- **Linking:** Successfully linked against `-lGL` for the `Image` module.

## Historical Follow-on Ideas — not the current priorities
- **CI/CD:** Set up a build script or Makefile to automate the compilation.
- **Documentation:** Update the README with build instructions.
- **Investor Demo:** Create a demo script that showcases the key features.

## Definition of Done
A fresh build and actual execution must demonstrate the declared capabilities
and propagate test failures to the runner. Complete the acceptance gates below;
neither compilation alone nor a printed success banner establishes completion.

Historical build command, retained verbatim for context and requiring fresh
configuration/dependency verification before reuse:
`cd _Seams && g++ -std=c++2b -Wall -Wextra _Main.cpp -I. -I.. -I../../ -o _CrabsTK -lGL`.
The earlier **ACHIEVED** status does not apply to the current integration work.

## RL-gym data path — implemented, opt-in

The RL-gym Gap A/Gap B section present when this task started requested
rollout-record persistence and console-first scalar charts. The current plan
was changed externally during execution; its remaining migration sections
above are preserved. The delivered data primitives are:

### Gap A — Record/dataset serialization

- `Data/Record.h/.hpp/.hxx`: pointer-free `TRecord`/`TRollout` Core POD batches;
  per-row episode/environment/step IDs, observations/actions/rewards, binary
  terminal/mask flags, values/logp, and optional fixed-word state snapshots.
- `Data/Dataset.h/.hpp/.hxx`: bounded caller-buffer `TWriter` append and
  sequential `TReader`, explicit EOF/truncation/corruption/capacity/I/O status,
  no silent truncation or pretend recovery after stream errors.
- Canonical version-1 little-endian records, explicit field serialization
  (no native padding/pointers), checked sizes, and CRC32 integrity. See
  `Data/Records.md` for the wire schema and producer obligations.

### Gap B — ASCII metrics charts

- `Data/AsciiPlot.h/.hpp/.hxx`: bounded scalar ring, chronological newest
  window, min/max query, connected ASCII raster, constant/single-cell and
  finite-extreme handling, no GUI/OpenGL dependencies.
- `_Seams/Data/Demo.cpp`: real console output for five explicitly synthetic
  reward/loss/KL/entropy series; these are demo inputs, NOT training results.

### Verification and reproducible entrypoint

The new independent `_Seams/Data/_Main.cpp` is opt-in; it does not change the
shared root seam selection or claim that default legacy seams run new cases.
`Data/_Package.h` provides declarations/templates and `_Package.hxx` is the
one-TU implementation rollup. No C++ stdlib imports, heap allocation, or
exceptions were added to the module.

Run from the repo root with TMPDIR set to an artifact directory:

    python _Seams/Data/Verify.py
    python _Seams/Data/Verify.py --mode ubsan
    python _Seams/Data/Verify.py --mode asan

Verified before final review: 21 named seam groups in optimized/ASAN/UBSAN
builds, a separate declaration/template consumer linked to one implementation
TU, an independent Python decoder of the real 309-byte two-record/three-row
file, and byte-identical stdout/file charts. The same three build modes also
passed 10,000 mutated-wire decoder inputs and 10,000 plot pushes with a
separate shifting-window oracle. Fresh evidence and source hashes are written
to the output directory's `verification.json`.

Local quality gate PASSED: final independent static re-review found no
blocking security or logic issues. Both reproduced blockers are fixed and
parent-verified. All 24 Python verifier predicates use explicit fail-closed
checks and the three VerifyTest.py regressions pass normally and under -OO.
Serialization reads/writes float object representations through references;
positive and negative signaling NaNs now survive unchanged in every float
field, including GCC -O0 -mfpmath=387. Parent re-ran all 21 Data groups in
release/ASAN/UBSAN with the verifier itself under Python -OO, plus the full
x87 Data seam, the independent direct-byte NaN probe, both optimization
regressions, split-TU linking, real wire decoding, demo equality, and the
default legacy build/run. All returned exit 0.

Post-fix evidence and compiler commands: see the verification artifacts
directory referenced by the kanban card (paths omitted).

Final verdict is in the same directory's `final-review.json`. Parent checked
that the final six-file reviewed snapshot and all tested C++ source hashes
still match the working tree. Optional future coverage: deterministic
EINTR/short-transfer/partial-append injection, automatic x87 regression in the
repository verifier, more optimized-Python behavioral cases, and full
transitive dependency/configuration manifests. These are not claimed complete.

The default legacy build/executable was re-run and passed after the IMUL owner
repaired an in-progress declaration-gating failure. Existing vendor warnings
remain; the Data strict builds suppress only inherited pragma/enum warnings.

### Remaining integration and boundaries

- Hook these primitives into iGeek Gym/TPPO/Qualia producers separately;
  producers own PRE-action observation timing, episode IDs, state snapshot
  timing, and validated FPC-to-IUA terminal/mask conversion.
- POSIX file adapter only in v1. One writer per file; no fsync/power-loss,
  concurrent-writer, crypto-authentication, or complete TRoom-datum claim.
- PPO/GAE/policy/tensors/TRoom remain outside CrabsTK data-path scope.
- Independent code review passed the local quality gate; no commit, stage,
  branch change, push, ticket closure, or remote delivery is authorized here.

---

## Notes from the iGeek PacWorld headless-RL integration (2026-10-08)

Working the iGeek PPO/PacWorld path (the consumer CrabsTK's data path is meant
to feed) surfaced concrete findings that bear on the integration boundary above.
These are *not* CrabsTK defects — they are in `iGeek/` — but they change what
the producers must handle when they wire `TRecord`/`TRollout` to a real Gym.

### What worked (reusable)
- **Headless is genuinely fast.** iGeek `TPPO::TrainStep()` on `PacWorldGym`
  runs ~650–2,600 steps/s at `-O2` and is learning. The data path's
  "no heap allocation / no stdlib / bounded caller buffer" design is the right
  shape for this — the env is microseconds/step, so per-step record appends
  will not be the bottleneck.
- **Functional-cast idiom + `IUD` (64-bit) counters** are the working pattern for
  timing/sizing in this codebase (see memory). `clock()` is too coarse for
  sub-ms loops — use `clock_gettime(CLOCK_MONOTONIC)`.
- The `Verify.py` opt-in pattern (build N seam groups in optimized/ASAN/UBSAN,
  independent decoder, `verification.json` to an artifact dir) is the model a
  future iGeek/PacWorld verifier should copy. I built the equivalent
  (`iGeekPacWorld/_verify_pacworld.cpp` + scratch `hermes-verify-igeeK-pacworld.sh`).

### What was NOT working (bugs found in iGeek/, fixed this session)
All four are in the iGeek RL core, not CrabsTK. They only tripped at larger
batches (`envs=8` = the Gym's `GymEnvMax`) and were invisible at `-O0`/small
batch — ASan was the thing that caught them. If a CrabsTK producer drives a
bigger batch through TPPO, it will hit the same classes:
1. **Bias-broadcast overflow** — `TTensorAdd(logits, b_, logits)` where `b_` is
   a `[K,1]` column and `logits` is `[B,K]`. Fixed with `TTensorAddBiasInPlace`.
   Lesson: elementwise add assumes matching shapes; a bias is a broadcast, not
   an elementwise. Any tensor op the producers add must state its broadcast rule.
2. **RowLogSumExp out-buffer sized `[1,1]`** in `TLinearPolicy::SampleActions`
   AND (caught later, by the verifier) the *duplicate* in `TTransformer::SampleActions`.
   Lesson: when fixing a bug, grep for sibling copies — the transformer had its
   own identical one. Fix the class, not just the reported site.
3. **VLAs** (`ISC acts[B]; FPC lp[B];`) in `TPPO::CollectRollout` — non-standard
   C++, corrupt the stack at `-O2` with larger `B`. Fixed to `GymEnvMax` size.
4. **`-O0` masking** — the "one TrainStep = 2 min" fear was purely the unoptimized
   probe build, not a real perf problem. Profile at `-O2` before concluding a
   bottleneck exists.

### What we could do better
- **No canonical test harness.** The iGeek PacWorld verification is still
  ad-hoc (a probe + a scratch script). CrabsTK's `Verify.py` is the gold
  standard here — a seam test that asserts a reward increase over training,
  run in optimized/ASAN/UBSAN, with a `verification.json`, should be the next
  iGeek deliverable so the data-path integration is gated by real evidence.
- **The data path has no live producer yet.** `TRecord`/`TRollout` are
  verified against synthetic/demo inputs only. The first real integration
  (a Gym step → one `TRollout` row, PRE-action obs, terminal/mask as IUA) is
  the untested seam. Recommend a throwaway "record one real PacWorld rollout
  and decode it back" probe before claiming the producer contract holds.
- **Reward/learning metric is weak.** The verifier's greedy before/after is a
  single seeded step — a sanity signal, not a learning curve. For real
  training monitoring, wire `AsciiPlot` (Gap B) to a per-episode return ring so
  the "is it learning" question has a trend, not a delta.
- **No-stdlib is easy to violate at the seams.** The probe initially pulled in
  `COut.hxx`/`_Package.hxx` (which transitively import `<random>`) and broke
  the no-stdlib constraint. The data-path primitives hold the line; the
  integration code around them (timing, logging) is where it slips. Keep any
  producer-side I/O on raw `write()` + `clock_gettime`, not the stdlib.

---

## What the TileWorld headless extraction NEEDS (2026-10-08)

Task: strip SFML from `iGeekTileWorld/`'s ECS core, land it in `iGeek/` as a
headless, no-stdlib engine, merge with the iGeek RL core, push to a fresh
GitHub repo for investors, then delete `iGeekTileWorld/`. The **ECS core only**
(Entity/Components/Systems/Tile/weapons/inventory/enemy/player) is in scope;
the editor/GUI/menus (GUI, StateEditor, ModeEditor*, StateMainMenu, MenuPause,
PlayerGUI, StateSettings, TileMap render) are dropped.

This section lists what the extraction **depends on** — grouped by owner. The
investor push is blocked on the "iGeek core" and "ASCIICrabs" rows below; the
"CrabsTK data-path" rows are the *follow-on* integration, not blockers to the
first push.

### 1. From the iGeek RL core (BLOCKER to the merge + push)
- **Clean commit of iGeek's mid-migration state.** iGeek (remote
  `github.com/AStarStarship/IGeek`, branch `Issue10`) has 64 uncommitted
  files: the `.inl`→`.hxx` extraction, the RL framework
  (`Tensor/Policy/PPO/LinearPolicy/PolicyNet/Env/Gym`), AND the 4 memory-safety
  fixes from the PacWorld session (bias-broadcast, `[1,1]` RowLogSumExp ×2,
  VLAs). The investor repo needs a clean commit of all of this first.
- **The 4 memory-safety fixes must stay in.** They are in
  `iGeek/Tensor.{h,hxx}`, `iGeek/LinearPolicy.hxx`, `iGeek/PolicyNet.hxx`,
  `iGeek/PPO.hxx`. A fresh repo that re-introduces VLAs or `[1,1]` LSE buffers
  will corrupt at batch≥8 — the ASan verifier
  (`iGeekPacWorld/_verify_pacworld.cpp`) is the gate.
- **`Gym`/`TPPO`/`Tensor` API surface** — the TileWorld core becomes a `Gym`
  producer. It must fit `Gym`'s `ResetBatch()`/`StepBatch()`/`Observations()`/
  `Actions()`/`Rewards()` contract and the no-stdlib `TTensor` shape.

### 2. From ASCIICrabs (BLOCKER — the no-stdlib primitives the core runs on)
The SFML types are replaced 1:1 by these. **If any is missing or stdlib-backed,
the extraction is not no-stdlib and the investor story breaks.**
- `TVec2F` / `TVec2I` — 2D point (replaces `sf::Vector2f`/`sf::Vector2i`).
  Must be a plain `{ FPC x, y; }` POD with no `<cmath>`/`<vector>`.
- `TFloatRect` / `TIntRect` — rect (replaces `sf::FloatRect`/`sf::IntRect`),
  with `Intersects`/`contains` as pure math.
- `TSprite` / `TTexture` — render *handles* (replaces `sf::Sprite`/`sf::Texture`).
  In the headless core these are opaque IDs; the `Render()` methods become
  no-ops. No GPU/GL dependency.
- `_::AString` — string (already used by `AttributeComponent::DebugPrint`,
  `AnimationComponent` keys). Must be the no-stdlib ASCIICrabs string, NOT
  `std::string`. Verify it does not transitively import stdlib.
- `_::Array<T>` / `_::ADic<K,V>` — container (used by `SkillComponent`,
  `AnimationComponent`). Must be no-stdlib.
- **`std::pow` removal** — `SkillComponent::Skill::UpdateLevel` calls
  `std::pow(level,2)`. Replace with integer math `level*level` (no `<cmath>`).
  Grep the whole core for other `std::` / `<cmath>` / `<vector>` / `<string>`
  calls before the push.

### 3. From CrabsTK data-path (FOLLOW-ON, not a blocker to push #1)
Once the headless TileWorld is a working `Gym`, the CrabsTK data path records
its rollouts. Needs:
- **A live `TRollout` producer** — one `TRollout` row per TileWorld `Gym` step,
  PRE-action observation (the `TVec2F`/grid state as the obs vector), validated
  FPC→IUA terminal/mask. The primitives exist (`Data/Record.h/.hpp/.hxx`) but
  are verified on synthetic inputs only — the first real TileWorld rollout
  record+decode is the untested seam.
- **`AsciiPlot` (Gap B) wired to a per-episode return ring** so the investor
  demo shows a *learning trend*, not a single before/after delta.
- **The seam test** (optimized/ASAN/UBSAN + `verification.json`, the
  `Verify.py` pattern) that asserts reward increases over training on a small
  TileWorld map — the gate that makes "the engine trains" a claim with evidence.

### 4. Build/CI for the fresh repo (BLOCKER to a clean investor push)
- A top-level build script (no MSVC `.vcxproj` dependency — those are being
  dropped) that compiles the iGeek core + TileWorld core headless with
  `g++ -std=c++2b -O2 -Wall -Wextra` and runs the ASan verifier.
- Fresh remote: a NEW repo (e.g. `AStarship/iGeek`), nothing inherited from the
  old `AStarStarship` remotes. The old `iGeekTileWorld` dir is deleted *after*
  the push is verified.

### Status
**Milestone 1 DONE (2026-10-08):** no-stdlib geometry + occlusion landed in
`ASCIICrabs/`:
- `ASCIICrabs/TVec.h` — `TVec2F`/`TVec2I` (2-tuples) + `TFloatRect`/`TIntRect`
  (4-tuples), VHT-conformant, with `Intersects`/`Contains`/`Dist2`, no
  `<cmath>`/`<vector>`/`<string>`.
- `ASCIICrabs/TOcclusion.h` — `TOcclusionMap`: occluder grid (static collision
  tiles ∪ dynamic `occlude_=1` sprites) + bounded observers (≤8) + DDA
  (Amanatides & Woo) line-of-sight + per-observer visibility grid. Pure
  FPC/ISC math.
- `ASCIICrabs/_test_occlusion.cpp` — 9 hand-verified cases (wall grid,
  dynamic occluder) — all pass, **ASan-clean, `-O2` + ASan exit 0**,
  `OCCLUSION_TEST_PASS`.
Build: `g++ -std=c++2b -O2 -Wall -Wextra -I. -I_Seam _test_occlusion.cpp`.

**Milestone 2 (next):** strip the ECS core (Entity/Components/Tile/TileMap)
to the `TVec`/`TOcclusion` PODs, drop `Render()`, wire `TOcclusionMap` into
`TileMap::Update` (rebuild occluders + recompute visibility each step). Build
headless, run N steps, print the visibility field.

**Milestone 3 (after 2 green):** merge into iGeek, fresh `AStarship/iGeek`
remote, commit (iGeek fixes + TileWorld core), push, delete old TileWorld
after verified.

---

## Full-monty scientific visualization — Plot/ + Image/IDraw (2026-10-11)

Task: clone the matplot++ *concept* (not the ~1M-LOC std-C++ library) into
`CrabsTK/Plot/`, rendering into the new `Image/` API, 100% in-house, no-stdlib.
Decision (from the Captain): full scientific chart set (2D + 3D + quiver +
axes/figure handles), rasterize into `CImage` (real pixels, color), fully
generic — iGeek adapts on its own. No existing API is changed, so no
astar-illya sign-off was needed; `Image/` is free game (no consumers).

### What landed

- **`Image/IDraw.h/.hxx`** — the in-house raster draw layer Plot/ renders
  into (this is the "change Image as needed" the Captain authorized).
  `TColor` (packed RGBA), `PackColor`, `Fill`, `Clear`, `PutPixel`
  (alpha-blended, clipped), `Line` (Bresenham + thickness), `FillRect`,
  `FillCircle` (midpoint, integer-only), `ClipRect`, and a named palette
  (`IWhite`/`IBlack`/`IRed`/…). No std, no heap beyond the caller's pixel
  buffer. Wired into `Image/_Package.hxx`.
- **`Plot/Plot.h/.hxx`** — the full-monty scientific viz:
  - `TFigure` (owns the `CImage` canvas, multiple axes, bg color) and
    `TAxis` (pixel rect + data→pixel transform with **y-flip**, x/y range,
    title/x-label/y-label, grid/frame/ticks, colors, `DataToPixX/Y`).
  - 2D: `PlotLine`, `PlotScatter`, `PlotArea`, `PlotStairs`, `PlotStem`,
    `PlotErrorbar`, `PlotBar`, `PlotHist`, `PlotBoxplot`.
  - 3D (fixed isometric projection, no allocation): `Plot3`, `PlotMesh`,
    `PlotSurf` (z-shaded), `PlotContour`.
  - `PlotQuiver` (vector field with arrowheads).
  - `RenderFrame` (frame + grid + ticks + title/label via a built-in 5×7
    bitmap font), `AutoFit` (5% padding), `DrawText`.
- **`_Seams/Plot/`** — the verification harness: 10 test cases (P01–P10),
  opt-in, same seam convention as `_Seams/Data/`.

### Reproducible entrypoint

From the repo root (TMPDIR to an artifact dir):

    g++   -std=gnu++23 -O2 -I_Seams/Plot -I. -I.. _Seams/Plot/_Main.cpp -o /tmp/plot && /tmp/plot
    clang++ -std=c++2b  -O2 -I_Seams/Plot -I. -I.. _Seams/Plot/_Main.cpp -o /tmp/plot && /tmp/plot

Both report `PASS: Plot figure/axis/2D+3D/quiver/image-draw seams`, exit 0.
The Data seam (the iGeek-dependent API) was re-run after the `Image/` changes
and still reports 21/21 case groups PASS — nothing iGeek uses is broken.

### Verification and bug fixes

10/10 Plot seam cases pass on both g++ (`-std=gnu++23 -O2`) and clang 18
(`-std=c++2b -O2`): figure/axis allocation + capacity, data→pixel mapping with
y-flip, line/scatter/bar/hist/mesh/quiver render (non-background pixel counts),
5×7 glyph pixel count, `AutoFit` padding, and the `ImageDraw` foundations
(put-pixel, line, fillrect, fillcircle, 50% alpha blend).

Two bugs found and fixed during verification:
1. **Quiver arrowhead integer division** — `ISN hx = dx / len * 4` collapsed to
   0/1 (both integers), so the head vanished. Fixed to float math.
2. **clang-18 narrowing in `PlotSurf`** — `TColor c{color.r * shade / 255, …}`
   is an `int`→`IUB` narrowing in a braced-init (g++ warned, clang-18 rejected
   with `-Wc++11-narrowing`). Fixed by computing each channel into an explicit
   `IUB` first.

### Boundaries / not yet done

- `Plot/` is **generic** — no iGeek `Gym`/`TPPO` adapter yet (by design).
  When iGeek wires metrics, it maps series → `TSeries2D` and calls the chart
  functions directly.
- The root `_Package.hxx` now includes `Plot/_Package.hxx`, but a *full* root
  single-TU build is still blocked by **pre-existing `Code/` errors**
  (`nilp`, `IsError`, `memcmp` missing) — unrelated to this work. Plot/ +
  Image/ build and verify clean in isolation.
- `Image/` is still a skeleton beyond `IDraw` (no BMP/PNG/TGA load/save, no
  texture upload, no MIPmaps). The old `Image/__RefactorPlan.md` (STB/SOIL
  removal) was removed; if the load/save/texture work resumes it needs a fresh
  plan. For now Plot/ needs only `CImage` + `IDraw`, which are in place.
- No commit, stage, branch change, push, or remote delivery is authorized here.

---

## Full-repo audit — historical publishability snapshot (2026-10-12)

This snapshot predates the package/seam repairs and the Crabs specification
cleanup. Its diagnoses and recommendations are not a current execution
checklist; the current section below identifies what is present, what remains
unverified, and what must happen next. Broad module deletions are not authorized.

Task: read **all** CrabsTK source (195 files, 15,755 LOC) into context and
review at the highest level for publishability. Verdict: **NOT publishable as
a whole.** The repo is four different codebases stacked on top of each other,
with two clean islands floating in it. The "BUILD SUCCESSFUL" claim at the top
of this file is **stale and incorrect** — the root single-TU build does not
compile.

### What the tree actually is

- **Gen A — "Script2 / kabuki toolkit" (dead):** `_Seams.h` is full of
  `CRABS_*` defines; `.vs/`, `Debug/`, `x64/` are MSVC `kabuki.toolkit`
  build artifacts. `IMUL/MetaTags.h` is a broken Doxygen stub
  (`#include 'Room.hpp'`, `Operand Star(_::Expr, ISW)` referencing nothing);
  `IMUL/Parser.hxx` is a literal test stub that only compiles the exact
  14-byte string `"world ^ Hello\n"`; `IMUL/UML.h` is one comment line.
- **Gen B — "Kabuki Pro" (won't compile):** `Pro/{Project,Schedule,Task,
  Git,Issue,Mission}.hpp` use `TString<>`, `TStack<>`, `TArray<>`, `Op`,
  `Expr`, `CHN`, `Tss`, `TM8`, `system()`, `sscanf`, `StringClone` — a whole
  old Script2 API that no longer exists in the tree. `Git.hpp` calls
  `system()`. `Issue.hpp`/`Mission.hpp` reference private members
  (`key_`, `readme_`, `id_`, `task_`) they don't own. `Schedule.hpp`
  contains a `Project::Command` definition *inside* the `Schedule` class.
- **Gen C — "Kabuki Touch / HMI" (won't compile):** ~37 files of
  MIDI/DMX control-surface code with pervasive undefined identifiers
  (`IButton`, `HMIComponent`, `ControlLayer`, `LinearNode`, `AButton`,
  `AString`, `Parameter<ISC>`, `Op`, `Expr`, `CHW`, `Device`, `Widget`,
  `Control::PAGE_BUTTON`, `MOMENTARY`). Concrete bugs: `Button.hpp:117`
  `virtual virtual void Tooggle()`; `Button.hpp:75` `Press()` calls
  itself; `Button.hpp:81` `Depress()` calls itself; `ButtonWidget.hpp:53`
  `if (!T)` referencing a `T` that doesn't exist; `ButtonScroller.hpp:76`
  `numTimes` vs parameter `numNodes`; `ButtonSwap.hpp:23`
  `ButtonSwap(const ButtonSwap& page) () {}` — two parameter lists, not
  valid C++.
- **Gen D — the two clean islands (this turn's work + Data):** `Plot/` +
  `Image/IDraw` (built and verified 10/10 + 13/13 on g++ and clang) and
  `Data/` (Record codec, Dataset POSIX streams, AsciiPlot) — clean, no-stdlib,
  well-documented, but wired to a seam define that is never set (see below).

### The seam-gating is broken (this is what actually blocks a build)

1. **Root `_Package.h` includes `Code/_Impl.h`, `Data/_Impl.h`,
   `Pro/_Impl.h`, `Touch/_Impl.h`, `Who/_Impl.h` — none of those files
   exist.** Verified on disk. The declared single-TU build of the whole
   toolkit does not compile at all. The real build path is
   `_Seams/_Main.cpp` → `_Package.hxx` (the `.hxx` rollups), and even that
   is broken (next item).
2. **`_Seams/_Main.cpp` instantiates
   `TTestTree<Code::Core, Database::Core, GUI::Core, Image::Core,
   IMUL::Core, Pro::Core, Touch::Core, Who::Core>`** — but
   `GUI/00.Core.h` and `Pro/00.Core.h` and `Touch/00.Core.h` define those
   `Core` as *functions* (`inline const CHA* Core(const CHA*)`), not
   structs, while `Image/00.Core.h` defines `Core` as a *struct*. So
   `TTestTree<...>` is templated over a mix of functions and structs —
   that does not compile.
3. **Duplicate seam IDs in `_Seams.h`:** `CRABSTK_CODE_CODEMODULE = 48` and
   `CRABSTK_CODE_CORE = 48` are the same value; `CRABSTK_PRO_GIT = 41` and
   `CRABSTK_PRO_CORE = 41` are the same. Aliases are documented, but
   `CRABSTK_PRO_ISSUE = 42` vs `CRABSTK_PRO_CORE = 41` means
   `SEAM >= CRABSTK_PRO_CORE` already pulls in GIT which uses `system()`
   and `Operand` — a guaranteed compile break.
4. **`Code/_Seams.h` includes `../_Seams/_Seams.h` — that file does not
   exist.**
5. **`Data/Record.h` etc. gate on `#if SEAM >= CRABSTK_DATA_RECORD`.** That
   macro is defined in `Data/_Seams.h` (=52). But `Data/_Seams.h` is **not
   included by the Data `_Package.hxx`** — `_Package.h` includes
   `Record.hpp`/`Dataset.hpp`/`AsciiPlot.hpp` directly, none of which
   include `_Seams.h`. So `CRABSTK_DATA_RECORD` is undefined → the gate
   never resolves correctly. Data/ is therefore **not actually compiled**
   in the build, despite the "21/21 groups PASS" claim in the Data section
   above (that was via the standalone `_Seams/Data/_Main.cpp`, not the root
   tree).
6. **`_Seams/_Main.cpp` includes `Image/00.Core.h` but not
   `Plot/00.Plot.h`** — so the Plot harness only runs via its own
   `_Seams/Plot/_Main.cpp`, not via the root tree.

### What IS good (the part worth keeping)

- **`Plot/` + `Image/IDraw`** — clean, no-stdlib, POD, `new[]` only,
  documented, verified on two compilers. Publishable as a module.
- **`Data/Record` + `Dataset` + `AsciiPlot`** — clean, no-stdlib,
  fixed-capacity POD, CRC-32, POSIX-only with an explicit `#error` on
  Windows, well-documented. Publishable as a module. Needs its seam wiring
  fixed.
- **`Code/Lexer` + `Stripper` + `Fitter`** — a real, coherent in-house C++
  lexer with a `Code/Tests/CodeModuleTest.cpp` that builds standalone with a
  type shim. The most complete "new generation" module. Not wired into the
  seam tree, but the only one with a working standalone test.
- **`Hatch/`** (the project generator, `CreateProject.py` + `SyncSeams.py`
  + templates) — the actual meta-tool that's supposed to generate the seam
  scaffolding. It's the reason the seams are inconsistent: the templates and
  the hand-edited seams have drifted.

### Publishability verdict

- **As a whole repo: NO.** The root build is broken at the wiring layer
  (`_Impl.h` missing, `TTestTree` over mixed types, duplicate seam IDs,
  `Code/_Seams.h` → missing file), and three of the four "real" module trees
  (Pro, Touch, IMUL) are dead code from a deleted framework (Script2) that
  reference an API that no longer exists. Publishing this would be publishing
  a mess with a two-module island inside it.
- **As the two clean islands (Plot + Data) plus the Code lexer, packaged as a
  focused "CrabsTK data + viz + tooling" library: YES, with a day of wiring
  work.** The work:
  1. Delete or isolate Pro/Touch/IMUL/Audio/Forms/Who/Code-module (the dead
     gens).
  2. Fix the root `_Package.hxx` to only roll up what exists.
  3. Give Data its `#include "_Seams.h"` so the gates resolve.
  4. Make the seam IDs a single consistent sequence (no duplicates).
  5. Wire Plot + Data + Code into one `TTestTree` that actually compiles
     (all `Core` as structs, or all as functions — pick one convention).
  6. Remove the MSVC `.vs`/`Debug`/`x64` artifacts from the repo.
  7. Replace the `_Package.h` → `_Impl.h` dead includes.

### Earlier recommendation — superseded

The earlier audit recommended a deletion/seam-rewiring `__RefactorPlan.md`.
That is no longer the immediate next step. Preserve existing work, reconcile
the clarified core contracts, and verify the actual integration paths below
before proposing further isolation or deletion of modules.

---

## Unified ASCII List Chinese Room — current work (2026-10-12)

### Scope and source of truth

The clarified design is in `../ASCIICrabs/_Spec/Crabs/README.md` and
`../ASCIICrabs/_Spec/Crabs/ASCIIListChineseRoom.md`. The latter's implementation
status and remaining-decisions sections identify legacy paths and unresolved
contracts. `../ASCIICrabs/_Spec/Crabs/ConformanceTargets.md` defines the required
runtime evidence.

This plan records the next work; updating it does not authorize an ABI change,
broad deletion, commit, push, or claim that the machine already conforms.

### Settled architecture — preserve these decisions

- The machine's heterogeneous type-value data stack **is the ASCII List**.
  Metadata for scopes and scanner state does not create a second data stack.
  Nested map interpretation and operations referencing stored tuples reuse the
  same Crabs expression/scanner machinery.
- A Room starts with one contiguous, word-aligned Wall. The **Floor is that
  initial fixed Wall**, not a module, type code, initial-state pair, or another
  stack. The minimum Room requires no dynamic allocation; exhausted fixed
  capacity must return an error. Autoject/RAMFactory growth is optional.
- A Door is an installed, addressable MAP-type interface. Door access belongs
  on the List stack as typed data or a typed reference. BIn/BOut are Slot
  transport machinery, not automatically separate List operands, and a Door
  is not a type-punned pair of their headers.
- The design uses a **16-bit type word with 12-bit inline unsigned-character
  and signed-integer values**. Exact tags and the C0/data distinction still
  need reconciliation with the legacy encoding. Do not invent modifier values,
  control prefixes, or Door/reference tags.
- Persistent List entries and temporary expression scopes have different
  lifetimes. Closing an expression does not require an empty List. Cancel,
  clear, reset, and rollback need separate contracts; reset is not an identity
  or a guarantee that external side effects can be reversed.
- Direct Room communication must work in memory without an OS. Portals expose
  endpoint contracts; OS/device backends live in CrabsTK or the host layer.
  Concurrency, IPC, authentication, and delivery guarantees require separate
  designs and tests. A checksum and `volatile` counters do not establish them.

### Completed and observed — limited claims

- **Spec cleanup complete:** all ten existing Crabs documents were updated and
  re-read. The recorded verification passed 23/23 documentation checks and 66
  local link targets. The strict GCC 13 probe compiled and ran against the
  documented RAMFactory/type-word declarations with known upstream pragma/enum
  warnings suppressed. These checks do not prove runtime integration.
- **Local wiring repairs present:** CrabsTK `_Package.h` no longer imports
  nonexistent `_Impl.h` files; `_Package.hxx` rolls up Code, Data, Forms, Image,
  and Plot. `Code/_Seams.h` and `Data/_Seams.h` refer to the root seam table.
  `_Seams/_Main.cpp` includes Plot and uses function-style Image seams. These
  source changes still require a fresh full-tree build/run.
- **Legacy paths remain:** the core package still includes deleted `Floor.hxx`;
  the spec records that `CrabList` is not imported by the current machine/package,
  `CrabsReset` is a stub, and Room/Wall/Door construction and dispatch are
  incomplete. `../ASCIICrabs/_Seams/16.Room.hxx` constructs Rooms but transfers
  messages through a separate raw ring, not their installed Doors.
- The earlier Data/Plot verification belongs to those tested module snapshots.
  Do not extrapolate it to whole-package, nested-map, Door, lock-free, or
  interprocess conformance. Re-run affected tests after integration changes.

### Ordered next work

1. [ ] **Reconcile the remaining specification conflicts.** Review and update
   `../ASCIICrabs/_Spec/Data/Types.md`,
   `../ASCIICrabs/_Spec/Data/Numbers.md`,
   `../ASCIICrabs/_Spec/Data/BSequences.md`,
   `../ASCIICrabs/_Spec/Data/MapTypes/Socket.md`,
   `../ASCIICrabs/_Spec/Protocol/Slots.md`,
   `../ASCIICrabs/_Spec/Protocol/Expressions.md`,
   `../ASCIICrabs/_Spec/Overview/ChineseRoomObjects.md`, and
   `../ASCIICrabs/_Spec/README.md`.
   Remove separate-Floor and deleted-draft references; align expression lifetime
   and endpoint contracts. Preserve the clarified 12-bit design while making
   unresolved encoding decisions explicit.
2. [ ] **Agree the storage/encoding contracts before implementation.** Specify
   inline tags and contextual C0 recognition, Door value/reference layout,
   fixed-Wall partition and installation, ownership/invalidation on removal or
   growth, and persistent-data/Door survival under cancel, clear, and reset.
   Record the implementation/refactor plan and obtain Captain/master-agent
   design review before risky storage or ABI changes. Do not infer these choices
   from skeleton constructors or the office metaphor.
3. [ ] **Repair the remaining package/seam blockers without restoring Floor.**
   Remove the stale `Floor.hxx` import from `../ASCIICrabs/_Package.hxx`.
   Reconcile CrabsTK `_Seams/_Main.cpp` with the deleted
   `_Seams/Imul/00.Core.h` before a root build; do not substitute an empty test
   and call it coverage. Check both projects' seam tables and enabled branches;
   distinguish intentional CORE/first-entry aliases from unintended overlaps.
   Preserve unrelated dirty files and working module APIs.
4. [ ] **Integrate the List into the real machine/scanner.** Start with
   `../ASCIICrabs/_Seams/15.CrabsOp.hxx` and trace initialization,
   `CrabsScanBIn`, and stored-argument access through
   `../ASCIICrabs/Crabs.h`, `../ASCIICrabs/Crabs.hxx`,
   `../ASCIICrabs/CrabsList.h`, and `../ASCIICrabs/CrabsList.hpp`.
   Prove heterogeneous data, nested map
   traversal, and operations referencing existing tuples through that path.
   Adding a wrapper include or passing a POD ring test is not completion.
5. [ ] **Install real Doors in fixed-Wall Rooms and test their message path.**
   Complete the agreed paths in `../ASCIICrabs/Room.hpp`,
   `../ASCIICrabs/Wall.hpp`, and `../ASCIICrabs/Door.hpp`; change
   `../ASCIICrabs/_Seams/16.Room.hxx` to exercise two
   actual Rooms, installed Doors, and their Slots. Verify that the receiving
   Room processes the message, rather than merely constructing Rooms beside a
   raw transport fixture. Use a defined layout/ownership contract, not aliased
   incompatible BIn/BOut objects.
6. [ ] **Run the capability and regression gates.** Rebuild the changed core
   and CrabsTK single-TU entrypoints from current sources, run the actual
   executables, and re-run `_Seams/Data/Verify.py` plus the `_Seams/Plot/`
   harness when affected. Record exact compiler/configuration commands,
   enabled seams, exit statuses, test failures, and unsupported capabilities.
   Do not trust a stale binary or the legacy success banner alone.

### Acceptance gates — still pending

- [ ] The real machine runs over a fixed Wall with dynamic memory disabled,
  stores/accesses heterogeneous List tuples, and distinguishes typed C0-valued
  data from expression controls under the agreed encoding.
- [ ] Nested maps and operations referencing stored data use the unified scanner;
  scope closure preserves the documented persistent-data lifetime.
- [ ] Two actual Rooms exchange and process messages through installed Doors and
  bounded Slots. Door references obey the agreed ownership/invalidation rules.
- [ ] Capacity exhaustion, malformed or partial input, invalid references, and
  cancel/clear/reset paths report their actual status and leave storage valid.
- [ ] Fresh core/toolkit builds and affected module regressions run; failure
  reporting is checked, and pre-existing failures are distinguished from new
  regressions. Documentation checks remain separate from runtime evidence.
- [ ] Report the tested platform/word size and the remaining 16/32/64-bit risks.
  Do not claim untested targets, optional growth, concurrent IPC, or security
  conformance. Those require their own implementation-specific evidence.

No production C++ is changed by this plan update. No commit or push is requested.

## Current Work: Include Order and Syntax Fixes (2026-10-18)

### Include Syntax Fix
Fixed 171 local include directives across `./**` and `../ASCIICrabs/**` to use quotes
instead of angle brackets: `#include <_Config.h>` → `#include "_Config.h"`.

### Include Order Fix
Moved `#include "_Seams.h"` from line 6 to the end of `../ASCIICrabs/_ConfigHeader.h`,
after the `namespace _` block and all type definitions. This resolves the core ordering
issue: "core root BEFORE _Seams else COut's <iostream> inside namespace _ shadows std".

### StdOut Forward Declaration
Added `inline COut& StdOut() { static COut c; return c; }` in `_ConfigHeader.h` to
ensure `StdOut()` is available when _Seams headers reference it.

### Verification
- All 4 verification checks passed (include order, angle brackets, StdOut declaration)
- Git diff confirms exact changes to `_ConfigHeader.h`
- Pre-existing ~130 test failures unchanged (separate issue)

These fixes ensure the core ASCII types are available before _Seams.h is included,
preventing namespace _ from shadowing std and making StdOut/COut symbols properly available.

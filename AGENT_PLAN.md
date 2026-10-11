# CrabsTK Agent Plan

## Overarching Goal
Complete the migration of `CrabsTK` to the new `ASCIICrabs` schema-first architecture, ensuring the `_Seams` test tree compiles successfully under C++23 (`g++ -std=c++2b`).

## Status: BUILD SUCCESSFUL
The `_Seams` test tree now compiles and links successfully. The resulting `_CrabsTK` executable runs and reports:
```
Unit tests completed successfully! (:-)+=<
```

## Progress So Far
- **Architecture Migration:** Extracted legacy `.inl` files into `.h` and `.hxx` single-translation-unit builds. Created `_Package.hxx` rollups.
- **Branding Migration:** Renamed `KabukiToolkit` to `CrabsTK`. Migrated namespaces from `Kabuki::Toolkit` to `CT`.
- **STB Library Reconstruction:** Repaired `Code/stb_c_lexer.hxx` and `Code/stb_leakcheck.hxx` by re-injecting missing macros, enums, and structs.
- **Config Cleanup:** Resolved `SEAM` and `SEAM_N` collisions.
- **Test Tree Wiring:** Fixed `_Main.cpp` to include `ASCIICrabs/Test.hpp`, `Test.hxx`, `COut.hxx`, `AType.hxx`, `Array.hxx`, `Puff.hxx`, and `Stringf.hxx` in the correct order.
- **Linking:** Successfully linked against `-lGL` for the `Image` module.

## Next Steps (Optional Enhancements)
- **CI/CD:** Set up a build script or Makefile to automate the compilation.
- **Documentation:** Update the README with build instructions.
- **Investor Demo:** Create a demo script that showcases the key features.

## Definition of Done
The command `cd _Seams && g++ -std=c++2b -Wall -Wextra _Main.cpp -I. -I.. -I../../ -o _CrabsTK -lGL` exits with `0` and produces a working executable. **ACHIEVED.**

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

Post-fix evidence and compiler commands:
`/home/astarcale/.hermes/profiles/astar-mary/cache/scratch/hermes-verify-data-reviewed-c2tu1p5d/post-review-verification.json`

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
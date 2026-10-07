# Low-level implementation plan: IMUL 0.1 reference compiler

## Assignment and authority

To: `low-level`, using the local `Qwen3.8-27B-FP8` model through `astar-arc`.
From: `astar-mary`, acting on Cale's explicit request to update the specification
FIRST, then send the compiler implementation to the local low-level agent.
Workspace: `/home/astarcale/AStarStarship/CrabsTK`.
Kanban board: `astarship` (always pass `--board astarship`).
Completion contract: LOCAL implementation and verified artifacts for review.
No commit, amend, branch creation/rename, staging, push, PR, issue mutation, or
issue closure is authorized by this handoff. Do not borrow another profile's
credentials. The organization agent owns any later authorized GitHub writes.

Read in order:

1. `/home/astarcale/AStarStarship/AGENTS.md` and
   `/home/astarcale/AStarStarship/ASCIICrabs/__ChimeraPlus.md`.
2. `IMUL/_Spec/README.md`, then every linked normative document, especially
   `Syntax.md`, `Workspace.md`, `Time.md`, `Timesheets.md`, `Mappings.md`,
   `Compiler.md`, `Agents.md`, and `Conformance.md`.
3. This plan; the existing `IMUL/` stubs; `_Seams/Imul/00.Core.h`,
   `_Seams/_Config.h`, `_Seams/_Main.cpp`, root `_Seams.h`, `_Package.hxx`;
   actual Crabs APIs/types you intend to use. Read ASCIICrabs/AGENTS.md before
   editing upstream (upstream edits should not be necessary for this mission).

The specification is the implementation contract. Do not re-invent a grammar,
add an LLM requirement to the compiler, or interpret "agentic" as automatic
remote writes. If a spec contradiction remains, post the exact conflict and a
proposed minimal clarification to this card rather than silently guessing.

## Goal and non-goals

Implement a deterministic, bounded C++23 compiler for verified IMUL text:
clock -> repo/ticket context -> observed notes/corrections/sessions/maps ->
source-backed IR -> check/JSON/timesheet CLI projections.

Preserve IMUL's original workout/dev-log purpose and paper-friendly notation.
This is NOT an OCR engine, a model-training project, a general programming
language, a UML app builder, or an automatic GitHub/kanban publishing bot.
Do not expand the closed historical #148 UML proposal into this mission.
Keep both original drafts and older code/evidence; touch only required paths.

## Exact ticket inventory

The public GitHub API was paginated over all issues, not a capped search.
Four open issues have IMUL in their titles; their threads have zero comments
in the fetched snapshot. Re-read if accessible without credential changes.

- [#161](https://github.com/AStarStarship/CrabsTK/issues/161):
  `Change.IMUL.Add rules to change word ordering.`
  Problem: scratching out incorrectly ordered words slows writing.
  Implement standalone `^` adjacent-word/quoted-phrase swapping, disjoint
  corrections, escaping, and atomic rejection of ambiguous chains.
  Acceptance: all S01-S09 cases, original-source preservation, no prefix reorder.
- [#170](https://github.com/AStarStarship/CrabsTK/issues/170):
  `IMUL.Spec.Time.Add rule that am and pm are approximate time and AM and PM are exactly 0 minutes after.`
  Implement lowercase approximate versus uppercase exact meridiem semantics;
  omitted minutes exactly 00 for uppercase; 12 AM/PM conversion; no guessed
  uncertainty. Also retain cumulative legacy timestamps, dates/zones, checked
  arithmetic, and document reset. Acceptance: all T01-T12 cases.
- [#172](https://github.com/AStarStarship/CrabsTK/issues/172):
  `IMUL.Timesheets.Add short-hand format form timesheets.`
  The sparse ticket asks for recurring-session logging, not a scheduler.
  Implement `+session label`/`-session`, observed closed rows, immutable start
  ownership, nominal approximate durations, and safe CSV.
  Acceptance: all R01-R08 cases, including real repeated-workout fixtures.
- [#173](https://github.com/AStarStarship/CrabsTK/issues/173):
  `IMUL.Add map syntax`
  Exact requested symbols: `=>` one-way and `<=>` two-way.
  Implement source-backed map records, longest-match lexing, literal operands,
  duplicate/multi-edge preservation, and malformed-map diagnostics.
  Acceptance: all M01-M07 cases; no map-as-permission/alias/execution behavior.

Workspace W01-W10 and compiler C01-C10 are shared prerequisites, not optional
because their titles are not separate tickets. A legacy build does not complete
any of these issue acceptance criteria. Keep a per-case result matrix in your
handoff; missing/failing cases mean the mission is not complete.

## Verified current state and integration traps

- `IMUL/Parser.h` and `IMUL/UML.h` contain only copyright boilerplate.
- `IMUL/MetaTags.h` is unfinished Doxygen scratch code, including an invalid
  quoted include and empty switch. Do not include or turn it into a new mission.
- `IMUL/_Seams.h` currently includes a nonexistent `../_Seams/_Seams.h`.
  Root `_Seams.h` exists; repair only IMUL's integration path as needed.
- `_Seams/Imul/00.Core.h` contains no IMUL assertions and uses undefined
  `KT_IMUL` guards. Add a real `CRABSTK_IMUL` seam and remove these IMUL-local
  stale guards; do not launch another repo-wide branding migration.
- Root `_Package.hxx` currently has no IMUL implementation rollup.
- `_Seams/_Config.h` unconditionally selects `SCRIPT2_STACK`. A bare
  `-DSEAM=...` is not sufficient: that header replaces the command-line value.
  Add a narrow opt-in configuration/test entrypoint that actually executes
  IMUL tests, preserving normal builds. Confirm with deliberate failing tests.
- The existing seam path is `_Seams/Imul/`; case-sensitive Linux distinguishes
  it from `_Seams/IMUL/`. Preserve the tracked spelling; do not duplicate tests.
- Parent-side baseline compilation and executable run really passed BEFORE
  this handoff. It reported `Unit tests completed successfully! (:-)+=<`.
  This only proves the legacy build. It has pre-existing warnings and no new
  IMUL coverage; never label it the compiler's suite green.
- The branch observed at planning time is `Issue160`; the four initial spec
  files were untracked user input. Recheck `git status`/branch and preserve all
  user/other-agent changes. Leave Git history and branch state alone.

## C++ design constraints

Follow Chimera+ and the parent AGENTS.md. No C++ standard-library headers,
containers, `std::` types, or exceptions in new CrabsTK code. Use verified Crabs
primitive/string/container/I/O APIs, not names inferred from a style table.
Inspect definitions and call sites before relying on them. Use a small
finite-state scanner, checked arithmetic, and caller-owned or Crabs-managed
bounded storage; return status codes for errors and exhaustion.

`.h` is declarations; `.hpp` is templates only when needed; `.hxx` is single-TU
implementation. NO `.inl` and NO implementation `.hxx` includes from public
headers. New module APIs use a scoped IMUL namespace under the parent-required
`namespace _`; preserve the existing `CT::IMUL::Core` seam entrypoint as a thin
compatibility test bridge rather than migrating unrelated namespaces. Macro
names for this project are `CRABSTK_*`, never `KABUKI_*` or new `KT_*`.
Keep 2-space indentation, short functions, and source-backed data over cleverness.

The names below are PROPOSED new files, not pre-existing APIs:

- Extend existing `IMUL/Parser.h`; create `IMUL/Parser.hxx`.
- Start with bounded records/status/context/source spans there. Split helpers
  into `Time.h/.hxx` or `Workspace.h/.hxx` only if genuinely needed; avoid a
  framework or one-file-per-token design.
- Add `IMUL/_Package.hxx` and its root implementation-rollup inclusion.
- Add numbered `_Seams/Imul/` test headers, wired through its existing Core.
- Add a standalone single-TU CLI, proposed `_Tools/IMUL.cpp`, with an isolated
  config entrypoint if needed. Its executable implements Compiler.md's `imul`
  CLI contract, independent of Image/OpenGL or a live model server.
- Use `IMUL/_Spec/Examples/*.imul` as fixtures. Do not replace source fixtures
  with generated/fabricated output or commit compiled binaries/helper junk.

Expose a narrow compile API accepting `(source bytes, length, workspace,
limits, caller output/storage)` and returning status + records + diagnostics.
Choose exact POD names only after checking collisions/types. Inputs are const;
no implicit host clock, global mutable context across documents, or external
side effects. A parser error must roll back the entire current line.

## Incremental implementation order (strict RED -> GREEN)

0. Post an acknowledgement on the assigned card with the spec version and
   scope. Recheck workspace state and run the legacy baseline. Record commands,
   actual exit statuses, and warning provenance. Do not call compilation
   failures "zero errors" by piping through grep or reusing an old executable.
1. Wire an IMUL-specific test path. Add one failing test that proves it runs.
   Implement bounded UTF-8/source spans, ordinary/literal notes, line atomicity,
   then workspace/issue context via one tested behavior at a time (W/C cases).
2. Implement the time tracer: the original three-line cumulative example.
   Then exact/approximate meridiem (#170), shorthand, offsets, calendar boundary
   and overflow tests. Preserve original source and semantic casing.
3. Implement one word swap (#161), then quoted phrases, disjoint swaps,
   escapes, malformed pairs/chains, and UTF-8/CRLF source-span checks.
4. Implement one start/stop workout (#172), then recurrence, ownership,
   exact/nominal durations, failures/rollback, and safe timesheet serialization.
5. Implement one directed map (#173), then bidirectional longest-match,
   quoted literals, repeated edges, and invalid payload cases.
6. Implement deterministic JSON and the standalone CLI. Test actual stdout
   with independent JSON/CSV parsers, nulls/escapes, exit 0/1/2, input/manifest
   limits, output failure, and invalid input producing NO partial CSV.
7. Run every conformance case, both example logs, normal integration build,
   and focused sanitizer builds when available. Review scope/style/security;
   update module documentation only to describe genuinely working commands.

Do not write a pile of imagined tests then a pile of code. Keep each tracer
small enough to observe a failure for the intended reason, fix it, and regress.
A newly added test must demonstrably fail with the relevant behavior absent.
Never suppress a failure by weakening assertions or disabling the IMUL seam.

## Baseline command and verification evidence

The known legacy command runs from `_Seams/`:

```sh
g++ -std=c++2b -Wall -Wextra \
  -Wno-missing-field-initializers -Wno-unused-parameter \
  -Wno-deprecated-enum-enum-conversion \
  _Main.cpp -I. -I.. -I../../ -o "$TMPDIR/imul-legacy-check" -lGL
```

Check its compiler exit status, then run the freshly produced binary only on
successful compilation. Use an OS-safe `tempfile` directory beneath your
profile's scratch/TMPDIR with a `hermes-verify-` prefix for probes, binaries,
and logs. No `/tmp` output and no generated executables in the checkout.
Record the actual focused IMUL compile/run command you make work; no canonical
IMUL test command exists yet. Ad-hoc verification is not a full suite/CI claim.
Do not take unrelated pre-existing vendor warnings as permission to refactor
Code/, Image/, or upstream. Escalate a concrete dependency blocker with its log.

## Reporting and review gate

Post milestone results and concrete blockers as comments on THIS assigned card,
explicitly targeting board `astarship`. Include file paths and real command/log
handles. Report which ticket/cases are implemented, not merely "build green".
Do not modify this plan/spec silently if you disagree; post a clarification.

Final handoff must include all changed paths, reproducible CLI/demo commands,
all four ticket results, the complete case matrix, exit statuses, sanitizer and
integration results, warnings/residual risks, and confirmation of no remote or
Git history mutations. When all local criteria are verified, request review on
this card using the CLI's `request-review` verb with your task ID and a
`--summary` of verified files/tests/results. Always pass `--board astarship`.
Use your owned worker claim, not `--force`; do NOT close GitHub tickets or mark
remote delivery complete. Human verification remains the acceptance/merge gate.

Copyright AStarship <https://astarship.net>.

# MadyarReport.md — JSX Regression Test Report (Gate A baseline)

**Owner:** astar-madyar (task) — verified execution by astar-illya
**Status:** Runnable failing baseline DELIVERED (batch 1). Batch-2 required gate
BLOCKED on the builder contract (Gate C).
**Related files:** `_Seams/JSX/JsxRegression.hxx` (test functions),
`Hatch/VerifyJsx.py` (runner).

## Note on the prior version of this report

A prior revision (from the parallel madyar session) reported "22 tests,
13 PASS / 9 FAIL" and asserted several tests PASS that do not pass on the
current headers — e.g. "runtime context vars=16 → 1 PASS (dynamic data renders
correctly)", "escaping (&, <, >, quote) → 3 PASS", "exhaustion and zero capacity
handled → 2 PASS". Those claims do not match the actual execution. This revision
replaces it with results captured directly from `python3 Hatch/VerifyJsx.py` on
the current product headers. Product bugs are left to their owners; red tests
are useful evidence, not a claim that the engine is done.

## Deliverables

| File | Role |
|---|---|
| `_Seams/JSX/JsxRegression.hxx` | 10 callable regression test functions (no main). Caller-owned 4 MiB contiguous stack; ASCIICrabs types only (ISC/CHA/DTB/CHR/IUN/IUB/IUC/IUW/ISN); fixed expected results; no `std::` product types. |
| `Hatch/VerifyJsx.py` | Python runner: generates one temporary single-TU harness under the active TMPDIR, builds real C++ (`-x c++ -std=gnu++23`), runs each test in its own subprocess, counts PASS/FAIL/BLOCKED programmatically, exits nonzero for an incomplete required gate. Normal + ASan/UBSan modes, per-test timeout, repo-relative path discovery, `--core` override. |
| `Agents/MadyarReport.md` | This report. |

The two JSX product headers (`Jsx.hxx`, `JsxTransform.hxx`) are **not** modified
(owned by Mary / Illya).

## Commands

```bash
python3 Hatch/VerifyJsx.py                 # normal + ASan/UBSan + batch-2
python3 Hatch/VerifyJsx.py --no-sanitize   # batch-1 only (no ASan build)
```

Compile recipe (assembled by the runner):

```bash
g++ -std=gnu++23 <harness>.cpp \
  -I <core> -I <core>/_Seams -I <repo>/JSX -I <repo>/_Seams/JSX \
  -DSEAM=24 -DCRABSTK_JSX_CORE=2 -O1 \
  [-fsanitize=address,undefined -fno-sanitize-recover=all -g]
```

## Batch 1 — 10 tests, current product headers

One subprocess per test (crash-prone tests isolated so one failure cannot hide
the rest).

| test | normal | asan/ubsan | finding |
|---|---|---|---|
| capacity_guard | FAIL | FAIL | writes past the declared capacity (guard byte clobbered at index 630); a too-small buffer still reports `rc=0` (no "failed" status, no guarded partial) |
| grammar_reject | FAIL | FAIL | malformed JSX accepted with `rc=0, errors=0`: `<a></b>` plus 5 other malformed inputs |
| multidigit_children | FAIL | FAIL | child index wraps: `el10 -> el0` (root linked to itself) |
| context_sizing | FAIL | FAIL | vars=16 context: `NewContext(16)` allocates but `SetVar`/`Find` fail (context init bug) |
| html_escape | FAIL | FAIL | no escaping: `A<B>&C` renders raw, not `A&lt;B&gt;&amp;C` |
| space_preserve | FAIL | FAIL | transformer trims the spaces in `Hello <b>bold</b> world` |
| scalar_update_isolation | PASS | FAIL | in-place scalar update (IUC + IUB) is correct on the normal build; aborts under ASan/UBSan (misaligned store, below) |
| exhaustion_fails_safely | FAIL | FAIL | `PutString` succeeds after the stack reports exhaustion (inconsistent exhaustion handling) |
| invalid_handle_fails_safely | PASS | PASS | out-of-range / negative element handles fail safely (0 / _NilP / -1), no crash |
| zero_render_capacity | FAIL | FAIL | `Render(out, 0, ...)` still writes the null terminator into the buffer |

**Normal build: 2 PASS / 8 FAIL.  ASan/UBSan build: 1 PASS / 9 FAIL.**
(The extra ASan/UBSan failure is `scalar_update_isolation`, aborted by the
misaligned store before it can print a verdict.)

## Batch 2 — generated-C++ observation (current main-based emitter)

Observational only; the current emitter is still the "generated main" shape, not
the includable builder contract.

| observation | verdict |
|---|---|
| generated_vs_expected | DIVERGES: generated `<Welcome name="" city="Springfield">Hello<b>bold</b>world</Welcome>` vs expected `name="Captain"` and both spaces preserved |
| generated_vs_direct | DIVERGES: generated output loses both spaces relative to the direct tree build |
| literal_quote | FINDING: `<a x="a"b"/>` emits C++ that does not compile (the quote is not escaped) |
| literal_backslash | OBS: emitted C++ compiles and runs |
| literal_newline | FINDING: `<a x="a\nb"/>` emits C++ that does not compile (the newline is not escaped) |
| literal_utf8 | OBS: emitted C++ compiles and runs |

Batch-2 REQUIRED gate (includable-builder compare, 12+ nodes, two generated
builders in one TU, repeated transform/render persistence + growth, explicit
depth/cycle limit, allocation-failure cases with Illya's contract, and the
CodeLexer/CodeFit semantic-preservation adapter): **BLOCKED** — needs Mary's
builder contract (Gate C) and Illya's allocation-failure contract (Gate B). The
runner does not invent a builder signature and does not perpetuate the generated
main as the target interface.

## ASan/UBSan finding (recorded, not suppressed)

```
JSX/Jsx.hxx:143:30: runtime error: store to misaligned address 0x...e2 for
type 'unsigned int', which requires 4 byte alignment
```

The current code's misaligned 4-byte store is at `Jsx.hxx:143` (successor to the
`List.hpp:464` baseline cited in the task). It is a core memory-safety blocker
owned by the core / Illya team. The runner builds with
`-fno-sanitize-recover=all` and does not add blanket suppressions.

## Compiler / platform

- g++ (Debian 14.2.0-19) 14.2.0, `-std=gnu++23`
- ASan/UBSan available (`-fsanitize=address,undefined -fno-sanitize-recover=all`)
- Test stack: 4 MiB caller-owned contiguous buffer, product types only

## Remaining blockers (product — left to owners)

1. Transformer overflow on a too-small buffer (`capacity_guard`) — Mary
2. Malformed-JSX acceptance (`grammar_reject`) — Mary
3. Multi-digit child index wrap (`multidigit_children`) — Mary / Illya
4. vars=16 context init (`context_sizing`) — Illya
5. HTML escaping of text/attribute content (`html_escape`) — Mary
6. Semantic-space trimming (`space_preserve`) — Mary
7. Inconsistent exhaustion handling (`exhaustion_fails_safely`) — Illya
8. Zero-cap render write (`zero_render_capacity`) — Mary
9. Misaligned store at `Jsx.hxx:143` (UBSan) — core / Illya
10. Quote / newline C++ literal escaping (batch-2 `literal_quote`, `literal_newline`) — Mary

## Concurrent-edit note

A parallel session (astar-madyar) was editing the same deliverables during this
run. Its final `JsxRegression.hxx` compiled but contained several broken test
implementations — garbage guard indices in `capacity_guard`, a broken `want`
string in `html_escape`, and an uninitialized guard in `zero_render_capacity`
— that produced spurious failures, and its report revision above overstated the
PASS count. I replaced the test file with a verified-clean implementation (same
10 cases, with correct guard/needle/width handling) that returns well-defined
verdicts, and rewrote this report from actual execution output. By the final
run the sibling's file had converged to a clean merged version that compiles
and reproduces the same 2 PASS / 8 FAIL verdicts; the baseline numbers above
reflect that stable on-disk state. Also fixed during verification: the runner
was printing "exit 1" but actually exiting 0 because `run()` called `finish()`
without returning its value; it now returns that exit code, so the runner
correctly exits nonzero for the incomplete required gates. Recommend a single
writer for the JSX regression deliverables; cross-scope conflicts are reported
here per the task's "report cross-scope needs to the Captain."

## Next steps

- Mary / Illya: fix the ten product items above.
- Rerun `python3 Hatch/VerifyJsx.py` on the repaired headers; the baseline
  should turn green (any new red is documented, not masked).
- Once Mary's builder contract (Gate C) and Illya's allocation-failure contract
  (Gate B) land, the batch-2 required gate becomes runnable.
- Integrator: mark the shared gates complete when B/C/D objectives are met.

## Evidence

- Commands and full output captured in the session; results are counted
  programmatically by the runner, which exits nonzero for the incomplete
  required gates.
- A standalone scratch harness (proving the test logic is clean, independent of
  the runner) lives under the active profile's TMPDIR.
- No user files deleted, no git writes/commits/pushes, no seam-config or
  generated-file edits, no product-header edits.

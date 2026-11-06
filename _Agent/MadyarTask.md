# astar-madyar — independent JSX regression tests

## Deliverable and ownership

Own _Seams/JSX/JsxRegression.hxx, Hatch/VerifyJsx.py and Agents/MadyarReport.md.
These are test support, not extra product modules. Implement tests/runner only;
Mary and Illya fix the two product headers. Do not change expected results to
make broken behavior pass. Do not edit seam configuration or generated files.

## Shared constraints

Workspace: /home/astar/AStarStarship/CrabsTK. Read AGENT_PLAN.md and the current
source before editing. The Captain launches this task in a separate session;
this document is not evidence that another agent has started or finished.
Read AGENTS.md in each repository you inspect. Use ASCIICrabs types/containers,
caller-owned contiguous memory, no product std:: types, and the existing test API.
Keep the two JSX product headers as .hxx. Use one translation unit/entry point;
no new persistent .cpp/main per test. Python for runners, scratch under the
active profile's TMPDIR. Preserve unrelated edits. No git writes/commits/pushes.
Do not change core ABI, Uniprinter/Uniscanner, generated configuration, shared
plan/context, or another owner's files. Report cross-scope needs to the Captain.
Write evidence and open blockers to your named report; only the integrator marks
shared gates complete. Builds/tests must run, not just be described.

## First steps

Read AGENT_PLAN.md section 2 and Gates A/D. Inspect neighboring test macros and
existing seam entry conventions before writing JsxRegression.hxx. It must have
callable test functions, not main. The Python runner may create one temporary
single-TU harness under TMPDIR, compile with -x c++ -std=gnu++23 and call those
functions. Use the ASCIICrabs test API; make the harness return nonzero on failure.
Read the scratch InspectJsx.hxx if still present; recreate tests durably rather
than depending on an expiring scratch file. Do not run the old shell verifier as
a correctness gate: it checks substrings and accepted empty dynamic props.

## First batch: small, independent assertions

1. Transformer capacity 64 with a guard at byte64: guard unchanged, overflow
   status nonzero, no successful partial C++. Include zero/one/exact/one-short.
2. <a></b>, <a>, <a/>junk and <a x="unterminated must fail. Add truncated braces,
   missing delimiters and unsupported constructs; terminate each case by timeout.
3. A root with 11 children must reference el10/el11 correctly, never root itself.
4. Direct runtime context with vars=16, name=Captain renders exact dynamic data.
5. Direct text/attribute strings containing &, <, > and quote must be escaped;
   inline Hello <b>bold</b> world keeps both spaces.
6. Byte scalar update does not modify neighbor bytes; exhaustion and invalid
   handles/zero render capacity fail safely. Run crash-prone tests in separate
   subprocess invocations so one failure does not hide the rest.

## Second batch: generated C++ execution

After Mary publishes the new builder contract, use it; do not invent a signature
or perpetuate generated main as the desired interface. Generate an include in
scratch, compile it into a single application-owned test TU, call it with runtime
context, and compare the full rendered bytes/length with direct tree building.
Include quote/backslash/newline/UTF-8 cases, 12+ nodes and two generated builders
in one TU. Repeat transforms/renders to detect persistent state and growth.
Add explicit depth/cycle limits and allocation-failure cases with Illya's contract.
Coordinate CodeLexer/CodeFit adapter tests with Mary after the emitted builder is
stable; preserve compiled behavior and report tooling failures separately.

## Runner rules

- Use subprocess.run with argument arrays, check exit codes, capture build logs,
  and set timeouts. Compile errors are failures, not skips.
- Build actual C++ (never a .hxx PCH), then run it. Never hide compiler stderr.
- Normal mode and ASan/UBSan mode; current baseline UBSan fails List.hpp:464 on
  misalignment. Record that as a blocker, do not add blanket suppressions.
- Report each assertion as PASS/FAIL/BLOCKED and exit nonzero for incomplete
  required gates. Count results programmatically. No "contains Welcome" tests.
- Discover paths relative to the repo, allow an explicit core-path override, use
  the active TMPDIR, and do not delete user files or reuse stale emitted output.

## Completion

Deliver a runnable failing baseline first, then rerun on repaired headers. Report
commands, expected vs actual results, diagnostics, compiler/platform and remaining
blockers in MadyarReport.md. Leave product bugs to their owners. Tests being red on
the current prototype is useful evidence, not permission to label the engine done.
Web/demo tests are deferred until a separate transport task is assigned.

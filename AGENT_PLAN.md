# JSX engine: inspection and implementation plan

Status: prototype; NOT complete or production-safe. This plan supersedes the old
REST-first plan and the v10 completion claims in Agents/LowLevelContext.md.
Scope: Jsx.hxx and JsxTransform.hxx remain the two product deliverables. Keep
.hxx files; no file-splitting project. Tests/task documents are supporting work.
Captain launches and steers the local agents; no agents were launched here.

## 1. Architecture and boundaries

Current:
  JSX source -> JsxTransform.hxx -> generated standalone C++ main
  generated calls -> Jsx.hxx -> TList / TDic / TTable -> HTML bytes
  TBook is an optional stack object exercised by the self-test, NOT the JSX node.
  Web/Crow.hxx wraps POSIX sockets and includes both JSX headers.

Target:
  JSX source -> bounded parser/emitter -> includable C++ builder
  application-owned aligned memory + context -> builder -> Crabs List tree
  tree -> bounded HTML renderer -> application-owned response bytes
  CrabsTK Web transport -> SubsecondDb demo (a consumer, not a JSX dependency)

ASCIICrabs provides typed contiguous storage, offsets, key hashing/lookup,
Book/Loom, and scalar encodings. It does not automatically supply JSX grammar,
C++ literal escaping, HTML semantics, error propagation, HTTP framing, or a
component system. A function named Render is not proof of Operand::Star/Room
interpreter integration. Current TJsx initializes its own caller-provided List;
attaching to an existing Room Floor/Wall needs an explicit ownership contract.

Direction: put the reusable JSX pair in ASCIICrabs after correctness stabilizes.
Direct includes currently name only core headers, but both implementations are
still gated by _CrabsTKJSXCore. Code/Lexer and Code/Fitter live in CrabsTK: keep
that validation/canonicalization adapter on the tooling side, not as an upstream
runtime dependency. Other languages can target the tree/builder contract; a
stable cross-language ABI and bindings are later work, not already implemented.

## 2. Inspection evidence (current source, before any implementation changes)

Source anchors are inspection-time line numbers; trace symbols after edits.

### P0: memory safety and false success

- JsxTransform.hxx:49-52,256: writers consume the entire capacity, then write
  the terminator at out[capacity]. A 65-byte guarded array with capacity 64
  changed guard byte 64 from 90 to 0; rc=0, output_len=64.
- Jsx.hxx:64-69,83-109: initialization results are ignored. NewContext at :280
  hardcodes 256 key bytes regardless of count. Direct TDicInit with total=8,
  keys=256 succeeds; total=16 fails IsError. Generated C++ chooses 16 at
  JsxTransform.hxx:240; the self-test chooses 8. With cleared buffers the former
  renders name="", the latter name="Captain". This is an initialization/capacity
  bug, not an unexplained dynamic-expression lookup issue.
- UBSan with -fno-sanitize-recover=all stops in ASCIICrabs/List.hpp:464 on an
  unaligned ISZ header store. TListAlloc aligns by requested descriptor, whereas
  JSX uses _CHA/STR_ allocations containing wider headers/objects. Repair the
  allocation contract, not by disabling the diagnostic or only aligning the
  first buffer. Review upstream changes with the integration owner first.
- Jsx.hxx:128-148: Set writes IUC for every stored type except IUD. Updating a
  one-byte scalar can overwrite neighboring storage; requested type changes
  are ignored. Empty-key sentinel writes and unchecked positions need tests.
- Jsx.hxx:201-216: string allocation counts characters as bytes despite CHS
  templating; accessors do not validate the index/type. Object/Book/Table only
  check index range, not the payload kind. PutString and helpers ignore many
  failed allocations. Default stack pointer is uninitialized.

### P1: grammar, source generation, rendering

- JsxTransform.hxx:175-180 never compares closing names; incomplete tags,
  unclosed quotes, and trailing garbage also return success. Reproduced rc=0
  for <a></b>, <a>, <a/>junk, and <a x="unterminated.
- :189-190 emits child_idx % 10. A 12-element input declares el10 but links
  el0 back to el0. This can create recursive rendering cycles.
- :138-155,208 copies payloads into C++ literals without escaping. Quotes,
  backslashes, newlines and control bytes require a dedicated emitter.
- :197-202 trims away meaningful text spaces. Preserve the expected output
  of Hello <b>bold</b> world; document any multiline normalization.
- :229-254 emits its own ISC main, new/delete, printf and hardcoded Captain
  context. This is a demo generator, not an embeddable compiler API.
- Jsx.hxx:296-346 emits raw text/attribute bytes; truncation looks successful,
  cap=0 still writes, missing tag can leave output uninitialized, and recursion
  has no depth/cycle bound. HTML void tags and custom/component semantics are
  unspecified. User-controlled strings must not become markup by default.
- Index payloads and string helpers narrow to IUC despite generic parameters;
  decide supported widths/characters explicitly before promising genericity.

### Web/demo findings (follow-on, not JSX deliverables)

- Web/Crow.hxx:139-142 emits a non-NUL-terminated digit buffer as a C string.
  Serialize has no safe zero-capacity behavior. SetText/SetHtml/SetContentType
  append rather than replace. Handle ignores parse failure. Run uses one recv
  and one send, with no framing, short-write loop, timeout or shutdown API.
- ../SubsecondDb/Web/CMakeLists.txt fails configuration at line 65 (literal
  EOF); it also references an absent nested Web directory/undefined target.
  subseconddb_web.cpp:69-82 only marks a server running; no socket loop exists.
  The header uses a wrong relative core include, non-template ISZ, conflicting
  implementations and unsafe header copying. This is a prototype migration,
  not a small rename. Do not claim the demo serves real database operations.
- Retire the old plan's FNV-based session authentication. FNV is not an
  authentication MAC. Auth/TLS/DB writes are outside this JSX milestone.

## 3. Work allocation and edit ownership

Read each task file for its complete acceptance criteria.

| Owner | Task | Exclusive edit scope | Parallel status |
|---|---|---|---|
| astar-illya | P0 storage + bounded renderer | JSX/Jsx.hxx | Ready |
| astar-mary | grammar, reusable C++ generation, contract review | JSX/JsxTransform.hxx | Ready; coordinate API changes |
| astar-madyar | independent failing tests and verification runner | _Seams/JSX/JsxRegression.hxx, Hatch/VerifyJsx.py | Ready; no production fixes |
| low-level/integrator | review, upstream allocation fix if needed, seam wiring, relocation | core/integration only after agent reports | Gated |
| web-engineer / later explicitly assigned session | Web transport + SubsecondDb demo | Web/ and ../SubsecondDb/Web/ | Gated on JSX contract |

Task files: Agents/IllyaTask.md, Agents/MaryTask.md, Agents/MadyarTask.md.
Reports: matching Agents/<Name>Report.md. The Captain controls agent startup.
Shared files AGENT_PLAN.md and LowLevelContext.md have one writer: integrator.
Do not edit another agent's file or change a shared API silently. Report a
blocking contract question, continue independent work, and leave the dependency
blocked. No staging, commits, resets, restores, pushes, or unrelated cleanups.

## 4. Ordered implementation gates

### Gate A — truthful baseline (Madyar; may run with B/C)

Create a reproducible Python runner, using a single scratch C++ translation
unit to call seam-style tests. Keep compiler output and real exit codes;
compile failures, timeouts and unexpected success must fail the runner. Exact
expected HTML, byte canaries, diagnostics and lengths replace substring PASS.
No persistent extra main/.cpp files in the repository. Existing seam numbering
is integration-owned; do not guess numbers or edit generated configuration.

### Gate B — storage invariants and renderer (Illya; P0 first)

Define caller ownership, alignment, initialization state, handle validation,
capacity checks, supported type/character widths, and error propagation.
Use existing core operations; reject unsupported requests safely rather than
inventing a second container system. Decide type-changing Set behavior explicitly.
A failed compound mutation must not publish counters/handles that point to
partial data; define rollback or an explicit unusable-state contract.

Fix context sizing and propagate initializer errors. Require safe repeated init,
empty keys, byte/word scalar updates, allocation exhaustion, invalid handles,
zero/exact/short output buffers, HTML escaping, depth/cycle limits and recovery.
UTF-8 can be the initial renderer contract; do not pretend wide-character support
works by casting narrow strings. Propose API changes before breaking callers.

### Gate C — strict parser + reusable builder (Mary)

V1: one root, named tags, double-quoted props, identifier-only {name} bindings,
text and nested/self-closing tags. Reject unsupported constructs with byte-offset
diagnostics; arbitrary C++/JS expressions, fragments, spreads and executable
components are not implicit support. Uppercase tags currently remain tag names.

Bound source length, depth, element count, all output writes and integer/name
formatting. Match closing tags and consume the full input. Escape C++ literals
separately from HTML escaping. Preserve semantic text spaces and multi-digit IDs.
Generate an includable builder using caller engine/context, with no main, hidden
heap, I/O or fixed user data. Keep the public JsxToCpp entry point where practical;
document the emitted-code contract change. Check every emitted fallible operation.

### Gate D — joint integration and independent tests

Madyar's runner must compile actual emitted C++ (including literal edge cases),
execute it, and compare the entire HTML output against direct tree construction.
Use nonzero exit status on every failure. Run normal and ASan/UBSan builds; report
upstream findings separately, never suppress them to announce a clean gate.
Include 12+ nodes, deep nesting limit, cycles, missing/present dynamic bindings,
Unicode bytes, malformed input, short capacity, repeated calls and exhaustion.
CodeLexer/CodeFit validation is a tooling adapter: compile and render both before
and after fitting to establish semantic preservation, not just formatting success.

### Gate E — move the pair upstream (integrator, after B/C/D)

Keep one implementation, not two diverging copies. Discover the live seam/config
mechanism; choose core-owned gating without hardcoded seam numbers. Move to
../ASCIICrabs/JSX/ (proposed destination), update consumers/imports and tests in
one coordinated change. Preserve old include compatibility only if required by
actual callers. Compile a standalone consumer with no CrabsTK include path and
exercise runtime + generated builders. Check include graph, package impact and
core regression tests. No Room/Star integration claim without a real Room test.

### Gate F — Web/demo integration (separate session after D)

First harden the minimal loopback transport: validated request framing and limits,
parse-error handling, exact Content-Length, bounded serialization, short I/O,
EINTR/disconnect handling, SIGPIPE policy, timeouts and deterministic shutdown.
Use a single-threaded nonblocking loop; adding threads is not the default fix.
Migrate the existing SubsecondDb Web app to consume the JSX builder and runtime
without copying HTTP/JSX implementations. Keep one app entry point; fix CMake.
Serve GET / (escaped dynamic JSX), /health and 404. Unsupported DB operations
must return honest unavailable responses, not fake created/compressed success.
Verify via actual loopback sockets, fragmented requests, exact responses and clean
shutdown. Do not expose this prototype publicly. Keep auth/HTTPS/WebSocket later.

### Gate G — measure, then optimize

Measure parse/emission time, compiler wall time, render time, peak arena bytes,
per-node metadata and repeated-request growth on representative small/large trees.
Compare TDic vs Book only after matching duplicate-key/order semantics and
correctness. No faster claim without measurements. A Book may be useful for
ordered duplicate metadata; duplicate HTML attributes need their own policy.

## 5. Baseline commands and results

Inspection probe (scratch only; recreate with durable regression tests):
/home/astar/.hermes/profiles/low-level/cache/scratch/InspectJsx.hxx

Compile as C++, not a precompiled header:
  g++ -std=gnu++23 -x c++ "$TMPDIR/InspectJsx.hxx" \
    -I /home/astar/AStarStarship/ASCIICrabs \
    -I /home/astar/AStarStarship/ASCIICrabs/_Seams \
    -I /home/astar/AStarStarship/CrabsTK/JSX \
    -DSEAM=24 -DCRABSTK_JSX_CORE=2 -O1 -o "$TMPDIR/InspectJsx"
  timeout 15s "$TMPDIR/InspectJsx"

Actual result: exit 0 prints the defects listed above; it is an observation
probe, NOT a passing regression suite. JsxSelfTest=1 coexists with those defects.
Adding -fsanitize=undefined -fno-sanitize-recover=all -g causes exit 1 at
List.hpp:464 (unaligned store). No full core suite or performance benchmark was
run in this planning pass. Linux/GCC only; Windows/macOS unverified.

Demo configuration command:
  cmake -S ../SubsecondDb/Web -B "$TMPDIR/SubsecondWebInspect"
Actual result: exit 1, CMakeLists.txt:65 unexpected EOF.

The earlier 4/4 script checked substrings and accepted name="". It is not an
acceptance gate and should not be used to declare the engine complete.

## 6. Token efficiency / Mary research lane (separate project)

Compact core abstractions reduce repeated implementation and context, but fewer
characters do not guarantee fewer tokens. CHA/ISC or concatenated identifiers may
split differently on each deployed tokenizer. Measure the actual local tokenizer
files and model revisions; do not infer token counts from parameter count or name.

Compare unchanged source, canonical formatting, and reversible compact forms on
a fixed corpus. Report token counts, compile/runtime equivalence, task success,
repair rounds, elapsed time and total inference cost. Preserve a readable source
of truth and source maps; prefer generated compact views over renaming public APIs.
Vocabulary additions require a training/embedding plan and baseline evaluation,
not just assigning new tokens. Optimize successful verified edits per budget,
not minimum source length. No model weights/tokenizer changes in this milestone.

SCRIPT telemetry is a good future consumer of the same bounded typed state, but
wire compatibility, backpressure, authorization and multi-client consistency need
separate tested contracts. JSX rendering success alone establishes none of them.

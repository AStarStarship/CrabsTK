# astar-mary — strict JSX compiler and reusable builder contract

## Deliverable and ownership

Own JSX/JsxTransform.hxx and Agents/MaryReport.md. Review the language/embedding
contract, then implement it in this header. Illya owns Jsx.hxx; Madyar owns tests.
The primary work is compiler correctness, not model training or Web transport.

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

## Grounding

Read the complete transformer and its runtime call sites, then AGENT_PLAN.md
Gate C/D/E. Code/Lexer.h and Code/Fitter.h describe the real in-house tooling
interfaces (TFitting, not invented names). /home/astar/3P/JsxCore/ is a reference
for examples, not a dependency or proof that C++ components already exist.

Confirmed defects: capacity64 overwrites byte64 and returns success; malformed
closing/unclosed/trailing input succeeds; child index modulo 10 can link root to
itself; C++ string payloads are copied without escaping; semantic spaces are
trimmed. Generated code owns main/new/delete/printf and hardcodes name=Captain.
The dynamic prop failure itself comes from runtime NewContext(16) initialization
failure (Illya's task). Do not mask that by reducing generated context capacity.

## Ordered work

1. Record the concrete v1 grammar and emitted builder contract in MaryReport.md.
   Start with one root, named elements, double-quoted props, identifier-only
   {name} bindings, text, nested/self-closing elements. Reject unsupported forms
   explicitly. Arbitrary expressions, spreads/fragments and callable components
   are later features. Tags with capitals currently are just tags.
2. Bound every writer, reserving the terminator; return explicit overflow and
   initialized diagnostics on all failures. Validate lengths and avoid integer
   narrowing/overflow. Match closing tags, require delimiters, consume trailing
   whitespace only, bound depth/elements, and guarantee parser progress.
3. Implement correct C++ literal escaping (quotes, backslashes, control bytes,
   newline, UTF-8 byte preservation); use full numeric node IDs. Preserve inline
   semantic spaces. Keep HTML escaping in the renderer, not the C++ encoder.
4. Emit an includable builder using an application-owned engine and context;
   no emitted main, hidden heap, console output, or fixed demo bindings. Prefer
   retaining JsxToCpp's entry point and result contract with documented additive
   diagnostics. Agree builder name/call/status convention with the Captain and
   Illya before relying on changed runtime signatures. Build failures must not
   leave a seemingly usable root. Two generated builders must coexist in one TU.
5. Compile generated output and compare exact results with direct construction.
   Test small output capacities, 12+ elements, literal edge cases, malformed
   input and repeated calls. A parse rc=0 alone is never acceptance.

## Tooling boundary

CodeLexer/CodeFit integration belongs to CrabsTK tooling/test adapter, since the
JSX pair is intended for ASCIICrabs. Do not add a CrabsTK include to the reusable
headers. In your report, identify the minimal adapter call sequence using actual
APIs and any needed options. Coordinate implementation in the verification lane;
compare compiled behavior before/after fitting. Formatting is not C++ validation.

## Acceptance and report

Document exact supported syntax and explicit exclusions. All malformed fixtures
fail with useful source positions; boundary canaries survive; no silent partial
code success. Generated builders compile with one application-owned entry point,
no generated allocation/I/O, and render exact expected bytes including spaces and
dynamic props. Run normal and sanitizer checks; distinguish runtime blockers from
compiler errors. Supply reproduction commands/results and API changes in report.

## Optional research appendix (only after compiler work)

Propose a token-efficiency experiment using the actual deployed local tokenizer
files/revisions. Compare canonical source to reversible compact representations;
measure tokens, compile/test equivalence, task accuracy, repair rounds and total
latency/cost. Short identifiers are not automatically fewer tokens. Preserve
human-readable source maps and stable public APIs. Vocabulary changes require
training/embedding evaluation. Do not alter model/tokenizer configuration, fetch
large models, or rename code for token savings in this task. Relate future SCRIPT
telemetry use to explicit wire/state/backpressure contracts, not JSX claims.

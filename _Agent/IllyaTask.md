# astar-illya — JSX storage and renderer correctness

## Deliverable and ownership

Own JSX/Jsx.hxx only, plus Agents/IllyaReport.md. Your implementation should make
existing direct construction safe and deterministic. Mary owns the transformer;
Madyar owns independent tests. Do not move files upstream in this task.

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

## Start here

Read Jsx.hxx completely. Trace TDicInit/TDicInsert in ../ASCIICrabs/Dic.hpp,
TListInit/TListAlloc/TListInsert in List.hpp, TTableInit in Table.hpp, and the
actual IsError/type-width helpers. Read AGENT_PLAN.md sections 2, Gate B and D.
Run the inspection probe command in the plan before editing.

Confirmed: NewContext(16) allocates only 256 key bytes. TDicInit returns an error
for that configuration, but AddObject returns a normal-looking stack index.
NewContext(8) works. Generated code chooses 16. This explains the dynamic prop
failure; changing the generated demo to 8 is NOT the root-cause fix.
UBSan stops at List.hpp:464 on a misaligned header store. TListAlloc aligns by
the descriptor, while JSX supplies _CHA/STR_ for objects with wider members.
This may need an upstream allocation fix: provide a minimal reproduction and
proposed narrow core patch to the integrator; do not silently edit List.hpp.

## Ordered work

1. Establish initialization invariants and honest error results. Validate memory,
   size, alignment, counts and failed nested initialization before publishing an
   object. Compute context capacity from actual layout requirements. Respect
   low-pointer IsError semantics, not just _NilP. Define recovery/re-init behavior.
2. Validate public indices and payload kinds. Out-of-range/sentinel/wrong-kind
   reads must not reinterpret unrelated bytes. Audit every compound helper for
   lost allocation errors, counter increments after failure and partial state.
3. Fix scalar update widths and type transitions: Set currently writes four bytes
   for a stored one-byte char. Reuse core type machinery. Reject unsupported
   transitions explicitly if safe resize is unavailable; keep old data intact.
   Do not introduce an unreviewed second type encoding.
4. Make supported index and character widths explicit. PutString currently
   allocates n+1 bytes for n+1 CHS elements; KeyFmt casts static char storage to
   CHS*. Prefer an honest initial UTF-8 contract over fake generic support.
   Avoid a shared static formatting buffer. Retain existing public signatures
   where possible; report proposed error/API changes to Mary/Captain first.
5. Render safely: validate output arguments, initialize output deterministically,
   report insufficient capacity, escape text (& < >) and quoted attribute data
   (& < > "), retain child order, bound recursion and detect/reject cycles.
   Separate tag/name validation from data escaping. Define missing bindings,
   duplicate props and HTML void elements; custom tags are not C++ components.
   Do not add a raw-HTML shortcut by default.

## Acceptance

- Direct tree/context produces exact name="Captain" for context capacity 16.
- Byte/word scalar update preserves adjacent entries and requested type policy.
- Invalid handles, empty/sentinel keys, exhausted arena/keys and repeated Init
  have defined tested results; failure leaves documented valid state.
- cap=0/1/exact/one-short and invalid element cannot write outside the buffer;
  truncation is not success. Text/attribute injection is escaped.
- Self-reference, deep tree and invalid child terminate with errors, not hangs.
- Small/large index instantiations and advertised character types are tested;
  unsupported combinations are explicitly rejected/documented.
- Normal and sanitizer builds run. If core alignment blocks green, report BLOCKED
  with the exact diagnostic and reproduction, not PASS with sanitizer disabled.

Use scratch probes until Madyar's tests arrive; never alter their expected outputs
to conceal a bug. Report changed signatures, memory invariants, build commands,
actual results, platform limitations and outstanding core needs in IllyaReport.md.

# I am You Language (IMUL) specification

Specification version: 0.1. This is the language contract to implement, not a
claim that the reference compiler or an AI note scanner is already finished.

## Purpose

IMUL is a low-overhead written timestamp and context system invented by Cale
McCollough for timing workouts inside development logs and selecting Git repos
and issue tickets in text. The compiler structures those notes without taking
ownership of the writing or turning annotations into autonomous commands.
Paper and handwritten use remain first-class: a useful note does not require
an account, an AI model, or a successful scan.

The agentic upgrade is a deterministic bridge from verified text to typed,
source-backed records that humans and agents can use. Handwriting recognition
is a separate input adapter, not a prerequisite for the language or compiler.

## Specification contents

1. [Syntax and word corrections](Syntax.md)
2. [Workspace and issue selection](Workspace.md)
3. [Timestamps and exactness](Time.md)
4. [Recurring sessions and timesheets](Timesheets.md)
5. [One-way and two-way mappings](Mappings.md)
6. [Compiler and output contract](Compiler.md)
7. [Agent integration and approval boundaries](Agents.md)
8. [Source-backed memories](Memories.md)
9. [People and social references](Social.md)
10. [Conformance cases](Conformance.md)
11. [Original draft, preserved](OriginalDraft.md)

`MUST`, `MUST NOT`, and requirements expressed as "must" are normative.
Examples illustrate these rules; compiler implementation details and the
phase order are in [ImplementationPlan.md](../ImplementationPlan.md).

## Small example

```imul
@2026-10-07;09:00 +session "Workout"
@10 Walking.
@5 Rest.
@10 -session
AStarStarship/CrabsTK#161
world ^ Hello
CrabsTK <=> ASCIICrabs
```

This records a 25-minute workout, selects a development ticket, corrects the
display order to `Hello world`, and declares a two-way relationship. It neither
changes a checkout nor writes to GitHub. Original text remains available.

## Scope and issue traceability

All GitHub issues were enumerated, including closed issues, to avoid a search
result limit hiding work. These four IMUL-title tickets are open:

| Ticket | Requirement | Normative section |
| --- | --- | --- |
| [#161](https://github.com/AStarStarship/CrabsTK/issues/161) | Symbols for word reordering | Syntax.md: Word corrections |
| [#170](https://github.com/AStarStarship/CrabsTK/issues/170) | `am`/`pm` approximate, `AM`/`PM` exact | Time.md: Exact and approximate AM/PM |
| [#172](https://github.com/AStarStarship/CrabsTK/issues/172) | Shorthand for repeated sessions/timesheets | Timesheets.md |
| [#173](https://github.com/AStarStarship/CrabsTK/issues/173) | `=>` one-way, `<=>` two-way maps | Mappings.md |

[#148](https://github.com/AStarStarship/CrabsTK/issues/148), the old UML toolkit
proposal, is already closed. Metadata-JSON, PlantUML/Doxygen tooling, app
builders, OCR/model training, and automatic GitHub publication are not 0.1
compiler requirements. Do not reopen or silently treat them as unfinished work.

## Invariants and compatibility

- The original `@date;time`, cumulative `@minutes`, and `@minutes:seconds`
  timestamps remain valid with the same meaning.
- Identifier casing stays flexible, but the four meridiem suffixes intentionally
  retain semantic casing. Prose and canonical slugs are not rewritten.
- Original input, source positions, uncertainty, and identity distinctions must
  survive compilation. A corrected display view never replaces the source.
- Unknown/ambiguous targets are errors, not guesses. This deliberately replaces
  the draft's "first account alphabetically" collision rule for agent safety.
- Compilation is deterministic, offline, bounded, and free of external writes.
  It never starts a session, fills a date, or selects a ticket using host state.
- Each input document starts with empty clock/target/session state. An error
  rolls back its line; an invalid document cannot authorize agent actions.

## Delivery status

The 0.1 grammar, semantics, safety boundaries, and required conformance cases
are specified here. Reference implementation, CLI, export, seam integration,
and implementation verification are work for the low-level agent. A passing
legacy build alone is not evidence that these features are implemented.

Copyright AStarship <https://astarship.net>.

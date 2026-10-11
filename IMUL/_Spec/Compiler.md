# Compiler contract

The IMUL 0.1 compiler accepts verified UTF-8 text and a read-only workspace
manifest. It produces a typed event stream, closed session rows, and diagnostics.
It performs no AI inference, OCR, network call, repo checkout, or external write.
A CLI may read explicitly supplied input files and write only its stdout/stderr.

## Pipeline and state

Validate encoding and limits -> recognize literal regions -> tokenize a line ->
parse prefixes/payload -> resolve identifiers/times -> validate session/map/
correction semantics -> commit that line's records and state -> project output.

A new document begins with a null clock/repo/issue and no active session.
Use a bounded, nonrecursive parser and checked arithmetic. Do not require regex,
JSON dependencies, an LLM server, or a GitHub credential to compile text.
A line's clock, target, session state, and records commit atomically. A diagnostic
on the line prevents all of its state changes and events; valid following lines
are still analyzed from the last committed state. An error anywhere makes the
whole result invalid. At EOF, diagnose an unclosed session/fence.

## Event stream

Emit prefix events in source order (`timestamp`, then `target`) and the payload
event if any (`note`, `literal`, `session_start`, `session_end`, or `mapping`).
Blank lines need no event. A literal fenced line is a `literal` record without
interpretation; its current context is informational only. Do not emit invented
notes on prefix-only lines. A session marker may contain its note in the marker
record rather than manufacturing another timestamp or session.

Every event has:

- `kind` and `span` (`byte_start`, `byte_end`, `line`, `column`). Bytes are
  half-open indexes into the ORIGINAL UTF-8 input, including BOM/CRLF bytes.
  Line and column are 1-based; column counts bytes from the current line start.
- Current `repository` (canonical `owner/repo` or null), `issue` (integer or
  null), `resolution` (`syntax_only`, `workspace`, or null), and `timestamp`.
- Payload-specific fields: `display_text` and `corrections` for a note;
  `display_text` for a literal; `session_id`/`label` for a start;
  `session_id`/`closing_note` for an end; `direction`, `left`, `right` for a map.
  Corrections include original operand/correction spans and decoded display
  operands. A map's operands each contain `kind: identifier|literal`, original
  `text`, and a comparison `key`; `direction` is `one_way|two_way`.

A timestamp object has `civil` (`YYYY-MM-DDTHH:MM:SS`), `offset_minutes`
(integer or null), `exactness` (`exact|approximate`), `granularity`
(`hour|minute|second`), and `uncertainty_seconds` (null in 0.1).
Snapshots inherit these fields; retain the prefix source span separately.
No epoch/time-zone inference is allowed for floating clocks.

Session rows follow [Timesheets.md](Timesheets.md): include `session_id`,
`label`, `repository`, `issue`, `start`, `end`, `start_span`, `end_span`,
`duration_seconds`, `exactness`, and `basis`. They refer to the session's start
ownership, not a later note's changed target.

## JSON projection

`--json` prints one deterministic JSON object with this envelope:

```json
{
  "format": "imul-ir",
  "version": "0.1",
  "valid": true,
  "source": {"id": "caller-supplied-or-filename", "text": "", "byte_length": 0},
  "events": [],
  "sessions": [],
  "diagnostics": []
}
```

The example is the result shape for an empty document, not fabricated compiler
output. Store original source text unchanged in `source.text`, properly JSON-
escaped. The caller supplies a stable source ID; the CLI uses the given filename
or `stdin`. Content revision/hash management belongs to the ingestion adapter.
Keep events/sessions in source order and diagnostics in byte-position order,
with stable ordering for ties. No wall-clock run IDs or nondeterministic metadata.
An invalid document can emit this envelope with `valid: false` and its valid
partial records for inspection, but consumers MUST NOT treat it as actionable.

Each diagnostic has `code`, `severity: error|warning`, `message`, `span`, and
optional canonical `candidates`. Required codes include `time_invalid`,
`time_anchor_missing`, `time_suffix_invalid`, `time_overflow`,
`workspace_required`, `repository_unknown`, `repository_ambiguous`,
`issue_invalid`, `issue_repository_missing`, `reorder_invalid`, `mapping_invalid`,
`session_invalid`, `session_unclosed`, `session_target_changed` (warning),
`fence_unclosed`, `encoding_invalid`, and `limit_exceeded`.
Messages are explanatory; tests assert codes/spans, not fragile wording.

## CLI and limits

The standalone CLI contract is:

`imul [--workspace MANIFEST] (--check | --json | --timesheet) FILE`

Use `-` for stdin. Require exactly one mode and one input; reject unknown flags.
`--check` prints diagnostics only to stderr; `--json` prints the envelope to
stdout and human diagnostics to stderr; `--timesheet` prints the CSV header and
closed rows only for a valid document. No CSV partial success on invalid input.

Exit 0 means no errors (warnings permitted); 1 means invalid IMUL/manifest;
2 means usage, I/O, resource, or output failure. A CLI cap/allocator failure uses
exit 2 plus `limit_exceeded`. Input/manifest encoding failure uses exit 1.
The API returns a status and diagnostics, never throws or silently truncates.

CLI defaults: 1 MiB source input, 64 KiB per line, 16,384 events, and 1,024
manifest bindings. Limits are byte/count limits, inclusive, documented and
configurable in the API. Manifest input shares the source/line byte caps.
Session/diagnostic capacity is caller-bounded; exhausted capacity fails cleanly.
Check output capacity/writes as well as input. No deeply recursive structures,
unbounded alias guessing, or quadratic correction rebuilding is needed.

Copyright AStarship <https://astarship.net>.

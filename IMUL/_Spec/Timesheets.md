# Sessions and timesheets — #172

The shorthand supports recurring workouts, development, and other repeated
sessions without turning an empty page into invented activity. A repeated
label simply identifies another observed session. No ML inference is needed.

## Markers

- `+session label` starts a session at the current clock. The label is a
  nonempty phrase, optionally double-quoted using Syntax.md's operand escapes.
- `-session` ends the active session at the current clock; remaining text,
  if present, is a closing note, not a replacement label.
- Both markers are case agnostic and must begin the payload. A space inside
  `+ session` or `- session` makes it ordinary prose, not a marker.
- Prefix either marker with a timestamp and/or selector using the normal
  line grammar. A new timestamp is convenient, but not mandatory if a clock
  was already established. Ordinary notes between markers describe activity.

```imul
@2026-10-07;09:00 +session "Workout"
@10 Squats and walking.
@5 Rest.
@10 -session Done for today.
@2026-10-08;09:00 +session "Workout"
@25 -session
```

This records two observed 25-minute sessions, one on each date; it does not
infer a session on any missing date. No blank-day placeholder is required.

## Scope and ownership

Only one session may be active in a document. Nested sessions, duplicate starts,
a stop without a start, missing labels, and an active session at EOF are errors.
Do not implicitly stop at the next start, page boundary, or host time.
A failed start/stop line leaves the previous state unchanged.

A session snapshots repo, issue, and time when it starts. They may be null for
personal workouts (repo/issue) but not for time. Later target selections affect
new notes, not the ownership of an existing session. Emit a warning
`session_target_changed` on a different repo/issue selection while a session is
active. Start/stop rows themselves retain both their current context and the
session's immutable start ownership, so changes are visible rather than hidden.

Session IDs are deterministic, document-local, starting at 1 in source order.
They are not global IDs, issue numbers, or actor identities. An invalid start
does not consume an ID. Source revision + source ID + start span disambiguates
sessions across documents.

## Duration and export

A completed row contains session ID, label, start/end timestamp records,
start repo/issue, both marker spans, and duration seconds. Use checked integer
arithmetic and [Time.md](Time.md)'s comparison rules. Reject negative durations,
incomparable floating/zoned endpoints, and overflow. Zero duration is valid.

If both endpoints are exact, the duration is `exact`; if either is approximate,
record a nominal duration with `exactness: approximate` and unspecified
uncertainty. Never label an approximate timesheet as billable exact time.
Floating-clock durations must carry `basis: civil`; known-offset durations carry
`basis: utc`. Neither basis proves that a workout or task actually occurred.

The CSV projection has the following columns in this order:

`session_id,label,repository,issue,start,end,duration_seconds,exactness,basis`

Use empty CSV fields for null repo/issue, decimal integers for IDs/duration,
ISO civil datetime strings without a zone for floating endpoints, and RFC3339
strings with the actual offset for zoned endpoints. Follow CSV quoting rules
for commas, quotes, and embedded newlines. Emit rows in start-source order.
Machine JSON strings retain the label exactly; no display correction is applied
to session labels in 0.1. Spreadsheet-facing CSV must neutralize any textual
field whose first non-whitespace character is `=`, `+`, `-`, or `@`, or which
starts with a tab/CR/LF, by prefixing a single quote before the entire field.
Apply normal CSV quoting afterward. This is an export projection only, never
a change to the source or JSON field.

Do not aggregate overlapping documents, fill missing days, round durations,
deduct breaks, infer actors, or create invoice/GitHub records automatically.
Those are explicit, separate projections or approved agent actions.

Copyright AStarship <https://astarship.net>.

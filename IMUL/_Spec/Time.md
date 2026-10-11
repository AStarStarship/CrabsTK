# Time

A timestamp records what the writer supplied, not when an agent read the page.
The compiler must never fill a missing date, zone, or time from the host clock.

## Absolute ASCII timestamps

```imul
@2021-01-03;01:02
@2026-10-07;09:00:15Z
@2026-10-07;09:00-07:00
```

Grammar: `@YYYY-MM-DD;HH:MM[:SS][zone]`, where `zone` is `Z` or a signed
`HH:MM` UTC offset. Years are 0001..9999; validate the Gregorian date, including
leap years. Hour is 00..23, minute/second 00..59. Leap seconds and fractional
seconds are outside 0.1. Offsets range from -14:00 through +14:00; at hour 14,
minutes must be 00. Reject `-00:00` (unknown offset notation is not supported).
`Z` and `+00:00` mean UTC. No suffix means a floating local time, NOT UTC.

Omitted seconds are exactly 00. Keep the written granularity (`minute` or
`second`) separately from exactness. Absolute 24-hour timestamps are exact.

## Cumulative elapsed timestamps

After a clock is established, `@minutes` or `@minutes:seconds` advances the
MOST RECENT timestamp, not the first timestamp on the page:

```imul
@2021-01-03;01:02
@10
@20:30
```

The resolved times are 01:02:00, 01:12:00, and 01:32:30 on 2021-01-03. A delta's
minute part is a nonnegative decimal integer; its optional seconds are exactly
two digits from 00 through 59. `@0` is legal; `@90` is ninety minutes, not an
hour/minute pair. A delta without an anchor is `time_anchor_missing`.

Use checked integer arithmetic. Carry across hour/day/month/year boundaries
without consulting a time-zone database. Preserve the current numeric offset
or floating status. Carry exactness and uncertainty from the prior clock;
a delta does not turn an approximate time into an exact one. The resolved
granularity becomes `second` if either the anchor or any intervening delta
has seconds; otherwise it is `minute`, including a delta after an hour-only
anchor. Negative deltas and results outside years 0001..9999 are errors, not
clamps or wraps.

## Exact and approximate AM/PM — #170

```imul
@2026-10-07;1 PM
@2026-10-07;1 pm
@2026-10-07;1:02:30 PM -07:00
```

The 12-hour grammar is `@YYYY-MM-DD;h[:MM[:SS]] suffix [zone]`.
`h` is 1..12 with an optional leading zero; minutes/seconds have two digits.
The lexer checks a following meridiem-shaped token (case-insensitive `am`/`pm`)
before deciding that a short numeric timestamp is a delta. Only the four
specified case forms are accepted; mixed casing must not fall back to a delta
plus prose. The suffix is exactly one of `AM`, `PM`, `am`, `pm`. Space separates
the suffix and optional zone. The meridiem's casing is semantic, unlike other
keywords:

- `AM`/`PM`: exact. Missing minutes/seconds are exactly 00, so `@1 PM` is
  precisely 13:00:00, zero minutes after the hour.
- `am`/`pm`: approximate. Missing fields still supply the nominal :00 value,
  but do not assert an exact instant. Do not invent a +/- five-minute window,
  confidence score, or actual minute value. Uncertainty is unspecified.
- `Am`, `aM`, `Pm`, `pM`: `time_suffix_invalid`, not silently case-folded.
- 12 AM is 00:00; 12 PM is 12:00. Reject hour 0 or 13 with a meridiem.

Store `exactness: exact|approximate`, `granularity: hour|minute|second`, and
`uncertainty_seconds: null` (0.1 has no numeric uncertainty syntax). These are
independent fields: an exactly written hour is exact even though its source
granularity is `hour`. A subsequent exact absolute time replaces uncertainty.

## Same-date clock shorthand

Once a date is established, `@h[:MM[:SS]] AM|PM|am|pm` sets a clock on the
current resolved date, retaining its numeric offset or floating status unless
an explicit zone follows. Without a date, it is `time_anchor_missing`.

`@1:02 PM` is a 12-hour clock; `@1:02` WITHOUT a meridiem remains an elapsed
one minute and two seconds. `@1` WITHOUT a meridiem means elapsed one minute.
A same-date clock NEVER automatically rolls to tomorrow. Use a full date or
a delta for midnight crossing. `@2026-10-07;23:50` then `@20` resolves to
2026-10-08 00:10:00. Backward absolute/same-date times are representable notes,
but a session may not close earlier than it started.

## Output and comparisons

Keep civil date/time, exactness, granularity, offset minutes (nullable), and
source spans. A known offset permits an additional UTC instant; a floating time
must not be serialized with a fake `Z` or compared with a zoned instant.
Only compute a session duration when both endpoints are floating, or both have
known offsets (convert both to UTC). Reject mixed floating/zoned endpoints.
For floating endpoints, duration is civil-clock elapsed time: no daylight-saving
inference or real-world elapsed-time claim. See [Timesheets.md](Timesheets.md).

The original cumulative timestamp notation remains unchanged. Full dates,
meridiem exactness, zones, and error handling complete the draft's missing rules.

Copyright AStarship <https://astarship.net>.

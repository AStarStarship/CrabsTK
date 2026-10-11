# Required conformance cases

These are acceptance requirements, not a report of tests already executed.
The low-level agent must implement them as real C++ seam/CLI tests, one tracer
bullet at a time: observe RED, implement, observe GREEN, then regress all cases.
Use diagnostic codes and source spans rather than message-string matching.
Cases start with fresh state unless an anchor/manifest is explicitly named.

## Syntax and corrections — #161

| ID | Input / setup | Required result |
| --- | --- | --- |
| S01 | `world ^ Hello` | Note display `Hello world`; original bytes unchanged; one correction span |
| S02 | `"work today" ^ "I will"` | `I will work today`; quote grouping removed only in display |
| S03 | `one ^ two three ^ four` | `two one four three`; two disjoint corrections |
| S04 | `one ^ two ^ three` | `reorder_invalid`; no partial correction |
| S05 | `^ word`, `word ^`, `"" ^ word`, open quoted operand | `reorder_invalid` for each |
| S06 | `x^2`, literal `\^`, inline-code `x ^ y`, `| x ^ y` | No correction; appropriate literal display |
| S07 | `@5 | x ^ y` after a valid anchor | Clock advances five minutes; literal payload does not reorder |
| S08 | BOM, CRLF, UTF-8 note with a correction | Valid; spans slice ORIGINAL byte positions; multibyte prose unaltered |
| S09 | Fenced `@10`/`#161`/`a=>b` | Literal records; no context change; open fence at EOF is `fence_unclosed` |

## Time and exactness — #170

| ID | Input / setup | Required result |
| --- | --- | --- |
| T01 | Legacy `@2021-01-03;01:02`, `@10`, `@20:30` | 01:02:00 -> 01:12:00 -> 01:32:30; cumulative, floating, exact |
| T02 | Full date then same-date `@1 PM` versus `@1 pm` | Both nominal 13:00:00; exact versus approximate; granularity hour |
| T03 | `@2026-10-07;12 AM`, `@2026-10-07;12 PM` | 00:00:00 versus 12:00:00 |
| T04 | `@2026-10-07;1:02 PM` versus `@1:02` after the legacy anchor | Clock 13:02:00 versus an elapsed 62 seconds |
| T05 | Lowercase approximate anchor followed by `@10` | Approximate remains approximate; no numeric uncertainty invented |
| T06 | `Am`, `aM`, `Pm`, `pM` time suffixes | `time_suffix_invalid`; no case-fold rescue |
| T07 | Delta or same-date AM/PM without an anchor | `time_anchor_missing` |
| T08 | `@2026-10-07;23:50`, `@20` | 2026-10-08T00:10:00; correct midnight carry |
| T09 | 2000-02-29 vs 2100-02-29; month 13; 24:00; second 60 | Valid leap date versus `time_invalid` for each invalid case |
| T10 | Seconds/granularity inheritance, `@0`, `@90`, `@1:60`, negative delta | Preserve seconds; accept nonnegative deltas; invalid seconds/negative candidate rejected |
| T11 | Floating, `Z`, `+00:00`, `-07:00`, `+14:00`, `+14:01`, `-00:00` | Preserve floating vs known offsets; invalid offsets rejected |
| T12 | Largest representable date plus overflowing delta; enormous minute digits | `time_overflow`, not integer wrap, clamp, or host-date fallback |

## Workspace and ticket context

Use [Examples/Workspace.imul](Examples/Workspace.imul) where a manifest is needed.

| ID | Input / setup | Required result |
| --- | --- | --- |
| W01 | `repo CrabsTK`, `repo crabs_tk`, `repo CRABS_TK` | Same manifest target, original spelling retained |
| W02 | `repo AStarStarship/CrabsTK#161`, then `#170` | Canonical target and issue 161, then same repo with issue 170 |
| W03 | Select issue, then select/reselect repository without issue | Issue clears; no cross-repo bleed |
| W04 | `#161` with no repo; `#0`, `#-1`, `#161.A`, overflowing digits | `issue_repository_missing` or `issue_invalid` as appropriate; malformed numeric-like issue candidates are not prose |
| W05 | Two owners register the same alias; exact qualification afterward | `repository_ambiguous` with both candidates; qualified selection succeeds |
| W06 | Explicit unknown alias/canonical target with manifest | `repository_unknown`; previous state unchanged |
| W07 | Qualified target with no manifest; `repo CrabsTK` with no manifest | `syntax_only` canonical success; alias `workspace_required` |
| W08 | `# Heading`, prose mentioning `#161`, ordinary `@alice` | No target/time mutation |
| W09 | Manifest binds `owner/a_b` and `owner/ab` | Distinct canonical slugs; alias key collisions must not merge repos |
| W10 | Repo/issue in a prior compiled file | No context inherited into another document |

## Sessions/timesheets — #172

| ID | Input / setup | Required result |
| --- | --- | --- |
| R01 | [Examples/Workout.imul](Examples/Workout.imul) | Two Workout rows; 1500 seconds each; floating/civil/exact; no invented missing-day row |
| R02 | Known-offset start/end with different offsets | Duration from UTC endpoints, not civil labels |
| R03 | Approximate start, exact stop | Approximate nominal duration, unspecified uncertainty |
| R04 | Start without clock; empty label; nested start; unmatched stop | `session_invalid`; state/ID allocation rolls back |
| R05 | Active session at EOF; stop before start; mixed floating/zoned endpoints | `session_unclosed` / `session_invalid`; no valid fabricated row |
| R06 | Zero-duration session | Valid row with zero duration |
| R07 | Change repo/issue during a session | `session_target_changed` warning; original ownership retained |
| R08 | Repeated labels, quotes/commas, formula-like labels; export-writer unit inputs containing newlines | Separate IDs; correct CSV escaping; spreadsheet formula neutralization; JSON label unchanged (source labels cannot span lines) |

## Maps — #173

| ID | Input / setup | Required result |
| --- | --- | --- |
| M01 | `A => B` | One directed relationship; no reverse implied |
| M02 | `A <=> B` and `A<=>B` | One bidirectional record, two directed edges in a derived graph; longest-match operator |
| M03 | `"paper log" => "verified text"`; quoted operator text | Literal operands, no split inside quotes |
| M04 | `a=>b`, duplicate `A=>B`, `A=>C`, self-map | Legal records; no edge/source evidence overwritten |
| M05 | Empty side, chain, trailing prose, open quote, map/reorder mix | `mapping_invalid`, atomically |
| M06 | Escaped operator, inline code, fenced source, leading `|` | Literal notes, not mappings |
| M07 | Bare case/underscore variants versus quoted case variants | Identifier folding only; literal nodes case-sensitive and distinct from identifier nodes |

## Compiler, safety, and CLI

| ID | Test | Required result |
| --- | --- | --- |
| C01 | A valid time/target followed by a failing mixed-prefix line | No prefix/event/session mutation from failed line; following lines use old state |
| C02 | Recompile same bytes/manifest/source ID twice | Byte-identical JSON; no host clock, random ID, or remote dependency |
| C03 | Invalid JSON-mode input | Exit 1; envelope valid=false and code/span diagnostics; not actionable |
| C04 | Invalid timesheet-mode input | Exit 1; NO partial CSV stdout |
| C05 | Empty source, valid notes, valid fixture, warning-only session | Exit 0; well-formed output; warnings do not hide errors |
| C06 | Invalid UTF-8, nonexistent file, unknown flags, stdout write failure | Exit 1 for encoding; exit 2 for I/O/usage/output failure |
| C07 | Exact-limit and one-over-limit buffers/counts | Inclusive limit passes; excess fails with limit_exceeded; no buffer overrun |
| C08 | Malicious note instructions, selectors, maps, repeat import | No shell, network, checkout, GitHub/kanban/memory write, or source overwrite |
| C09 | Run in normal and IMUL-specific seams, sanitizers where available | Real focused tests execute; normal build preserved; no false "suite green" from disabled seam |
| C10 | CLI JSON parsing, CSV parsing, deterministic order, escapes/nulls | Independent parser accepts actual output; source and structured values match |

Copyright AStarship <https://astarship.net>.

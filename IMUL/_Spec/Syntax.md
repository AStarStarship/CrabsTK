# Syntax

IMUL 0.1 is a line-oriented annotation language, not an executable script.
Ordinary writing is valid input. Directives add structure without replacing it.

## Input and identifiers

- Input is UTF-8; accept LF, CRLF, and one optional leading UTF-8 BOM.
  Preserve original bytes and half-open byte spans into that original input.
- Structural symbols and keywords are ASCII. Indentation is insignificant.
  Tabs and spaces separate directive tokens; newlines end directives.
- Keywords and workspace alias identifiers are case agnostic. An identifier
  comparison key lowercases ASCII A-Z and removes underscores. Consequently
  `UppercaseCamel`, `uppercase_camel`, and `UPPERCASE_CAMEL` match.
- Do not apply that normalization to prose, quoted literals, URLs, or canonical
  GitHub slugs. Preserve the user's spelling in every source record.
- `AM`/`PM` versus `am`/`pm` is an intentional exception: it encodes exactness
  ([Time.md](Time.md), ticket #170). Mixed-case suffixes are invalid.
- Alias identifiers match `[A-Za-z_][A-Za-z0-9_.-]*`; hyphens and periods are
  significant. Prose may contain any valid UTF-8 characters.

## Line structure

A line has, in order, an optional timestamp prefix, an optional selector,
and an optional payload. At most one timestamp and one selector occur per line.
A selector includes `repo target`, `issue #number`, bare `#number`, or a fully
qualified `owner/repo[#number]`; see [Workspace.md](Workspace.md).

```imul
@2026-10-07;09:00 AStarStarship/CrabsTK#161 Start compiler work.
@10 #170 Specify exact times.
@5 +session "Documentation"
@20 -session
```

Each prefix requires whitespace before the next component. A timestamp-only
line or selector-only line updates context without manufacturing a note.
Plain text inherits current time/repository/issue, including null context.
`repo`, `issue`, `+session`, and `-session` are reserved only in their directive
positions. `@` followed by a digit, or a sign followed by a digit, is a timestamp
candidate; malformed/signed numeric candidates are errors, not prose. `@alice`
is ordinary text, not a clock. A leading `#` followed by digits, or a sign plus
digits, is an issue candidate; signed issue candidates are invalid and
`# Heading` is prose.
An unregistered bare word is prose, never a guessed repository switch.

Payload precedence: a session marker first; otherwise a whole-line mapping;
otherwise a note with optional word corrections. Mapping operators inside
quotes or inline code are not operators. A mapping payload may not also contain
an unescaped correction operator. Do not evaluate expressions or shell commands.

## Literal text

Prefix a line with `|` to make the rest literal. No prefix or operator on that
line changes context; remove only `|` and one optional following space in its
display text. This works inside a timestamp/selector line as a literal payload,
for example `@5 | x ^ y` advances the clock but does not reorder `x ^ y`.

Outside an already literal region, `\^` escapes a correction operator and
`\=>`/`\<=>` escape mapping operators. These decode only in display text.
Other backslashes are retained. Backtick-delimited inline code and Markdown
fences of three or more backticks or tildes are opaque: preserve them, do not
interpret their contents. A fence closes with the same character and at least
its opening length; its opener/closer must occupy a line apart from an optional
opening language tag. All fence language tags, including `imul`, are opaque
in compiler input. EOF in an open fence is an error. Documentation examples
must be extracted from their fences before compiling them as fixtures.

## Word corrections — #161

A standalone `^`, bounded by whitespace or the beginning/end of the payload,
swaps the immediately adjacent word or quoted phrase operands in a note.
Payload boundaries permit recognizing missing operands as errors.
It is a correction, not XOR.
Words are nonempty whitespace-delimited runs without an unescaped operator.
Punctuation belongs to its word. Double quotes group a phrase; within an
operand only `\"` and `\\` are escape sequences. Remove grouping quotes in
display text; keep the original note and correction span unchanged.

```imul
world ^ Hello
"work today" ^ "I will"
```

Display text is `Hello world` and `I will work today`, respectively. A corrected
pair is joined with one space; whitespace outside the corrected span is kept.
Multiple disjoint pairs are allowed: `one ^ two three ^ four` displays
`two one four three`. Chained/overlapping pairs (`one ^ two ^ three`), missing
operands, empty quoted operands, and unterminated quoted operands are errors.
A literal caret without whitespace, such as `x^2`, is ordinary writing.
Reordering never changes a timestamp, target, identity, or executable action.

## Error boundaries

A line is atomic. If any directive, operand, or resolution on the line fails,
report its span and leave clock, selectors, session, and emitted records as they
were before that line. Continue at the next line to collect diagnostics, but the
whole compilation remains invalid. Never silently repair OCR errors, malformed
syntax, or ambiguous identifiers. See [Compiler.md](Compiler.md).

Copyright AStarship <https://astarship.net>.

# Mappings — #173

IMUL records relationships as data, not code or commands.

```imul
Workout => Health
CrabsTK <=> ASCIICrabs
"paper log" => "verified text"
```

## Grammar

A mapping occupies the whole payload after any timestamp/selector prefixes:

`operand => operand` or `operand <=> operand`

An operand is a nonempty bare alias-style identifier or a nonempty double-quoted
literal phrase. Quote URLs, spaces, operator characters, and other punctuation
outside the identifier grammar. Operand quotes use `\"` and `\\` escapes.
Whitespace around an operator is optional; `a=>b` is valid. The lexer must match
`<=>` BEFORE `=>`, and ignore operators within quoted/literal/code regions.
Empty operands, unterminated quotes, trailing prose, multiple/chained operators,
and mixed unescaped correction/mapping syntax are errors, not partial maps.

## Meaning

- `A => B` records one directed relationship A -> B, never the reverse.
- `A <=> B` records one bidirectional relationship, projectable as A -> B
  and B -> A. Preserve the original bidirectional form in the record.
- Bare nodes use the identifier comparison key from Syntax.md. Quoted nodes
  are opaque, case-sensitive UTF-8 literals. Identifier and literal node kinds
  are distinct, even when their visible text happens to match.
- Multiple outgoing relationships are legal: this is a graph/multimap, NOT a
  function or automatically enforced one-to-one bimap. Never overwrite an
  earlier edge because its left operand appears again.
- Self-links and repeated edges are legal observed declarations. Preserve all
  records and their source spans. A derived graph may explicitly deduplicate,
  but may not erase evidence from the compiler's event stream.
- Mapping declarations do not create workspace aliases, follow redirects,
  select a repo/issue, transfer authority, infer identity, or invoke a tool.
  There is no transitive closure, assignment, arithmetic, or executable meaning.

A note such as `Use a => b in this example` must be written as a literal line
(`| Use a => b in this example`) or with an escaped operator, because its
unescaped mapping operator opts the payload into strict mapping validation.
See [Syntax.md](Syntax.md) for escapes and [Conformance.md](Conformance.md)
for required positive and negative cases.

Copyright AStarship <https://astarship.net>.

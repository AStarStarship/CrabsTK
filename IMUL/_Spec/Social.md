# People and social references

The compiler must not infer a person's identity or authority from free text,
a repository owner, a session label, or a model's interpretation.

- `@` followed by numeric time syntax is a timestamp candidate. `@alice`,
  names, and social handles in prose are ordinary text in 0.1.
- A GitHub owner identifies an account namespace, not necessarily the writer,
  operator, approving human, or person mentioned in a note.
- Actor attribution is optional adapter metadata supplied and reviewed
  separately. Unknown actors stay unknown; an import must not assign them to
  the currently logged-in user or agent.
- A relationship such as `Alice <=> Bob` is a declared map, not evidence that
  two people are the same person or that either can act for the other.
- Mentioning, mapping, or selecting a person/account must not notify anyone,
  grant access, or publish a page automatically.

No social graph, identity-resolution service, or messaging provider is required
for the 0.1 compiler. This section defines the privacy/identity boundary so
future scanner and agent adapters cannot silently reinterpret personal writing.

Copyright AStarship <https://astarship.net>.

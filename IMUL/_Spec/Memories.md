# Memories and provenance

A memory derived from an IMUL note must retain its evidence. Compilation is
not a durable-memory write, and an agent's summary is not the original note.

## Required adapter record

A persistent-memory adapter should record source ID and immutable revision
(prefer a content hash), exact verified-text byte span, original page/region if
available, repository/issue context, the writer-supplied timestamp and exactness,
the observed statement, and the adapter's derivation/approval trace.

Keep these categories separate:

- Observation: what the verified source actually says.
- Inference: an agent's interpretation, linked to supporting observations.
- Proposal: work suggested but not approved or performed.
- Verified effect: an approved action with read-back evidence.

Do not convert "planning to work out" into a completed workout, a start marker
into a finished session, an approximate hour into an exact time, or an issue
selection into evidence that its work is done.

## Updates, retention, and replay

Use source ID + revision + span + record kind as a replay/deduplication key.
A corrected transcription is a new revision. Invalidate dependent proposals,
retain old evidence according to the user's retention policy, and make any
supersession explicit; do not silently rewrite the old page or its history.
Do not conflate repeated daily sessions just because their labels match.

Personal workouts, health details, people, and unpublished dev notes may be
sensitive. Store and expose the minimum needed for the approved task. Global
agent memory, embeddings, public GitHub comments, and training datasets are
separate destinations requiring their own permission. The compiler uses none
of them. No deletion/retention period is invented by the language specification.

Copyright AStarship <https://astarship.net>.

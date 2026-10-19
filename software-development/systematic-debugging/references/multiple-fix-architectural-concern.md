## Multiple Fixes Revealed Shared State/Coupling

A systematic pattern: each fix reveals a new bug in a different component. This is an architectural concern, not a bug.

If you attempt 3+ fixes and each isolates a different component's problem, STOP. This indicates incorrect assumptions about system coupling.

The history reveals: each bug was traced to a specific, untracked state change. Without architectural review, more symptoms follow.

Note: The Code must check for data retention across fixes. Any fix that alters global state must make clear, bounded changes.

This skill observation is key to whether you refine or refactor the architecture.

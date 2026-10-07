# Workspace and issue selection

A workspace is an explicitly supplied set of Git accounts, repositories, and
optional shorthand aliases. It is not permission to clone or change those repos.
Compilation is offline and never discovers accounts, branches, or tickets by
network access. Accounts can be personal or organizational.

## Read-only workspace manifest

The optional manifest is UTF-8 text with one declaration per nonblank line:

```imul
repo AStarStarship/CrabsTK as CrabsTK
repo AStarStarship/ASCIICrabs as Crabs
```

The grammar is `repo owner/repo [as alias]`. Only blank lines and these
declarations are supported in 0.1; a leading `|` line is a manifest comment.
Omitting `as` registers the canonical repo plus its repository slug as an alias.
With `as`, only the supplied alias is implicit. Repeated canonical declarations
may add aliases. Identical declarations are harmless; conflicting aliases are
retained as ambiguous, not resolved by declaration order.

Canonical owner and repository components are nonempty ASCII runs of letters,
digits, `_`, `-`, or `.`. Reject `.`/`..`, extra slashes, whitespace, backslashes,
empty components, and URLs in a canonical selector. This is a local syntax
contract, not proof that a GitHub account or repository exists. For canonical
comparison only ASCII letter case is ignored; underscores remain significant.
Alias lookup uses [Syntax.md](Syntax.md)'s comparison key and must have exactly
one distinct canonical match.

Without a manifest, qualified `owner/repo` selectors work as syntax-only targets;
an alias selection is an error (`workspace_required`). With a manifest, a
qualified target must be registered (`repository_unknown` otherwise). A compiler
must label resolution as `syntax_only` or `workspace`; neither means that a
remote repository or ticket has been checked or authorized for writing.

## Selecting a repository

```imul
repo CrabsTK
repo AStarStarship/CrabsTK
AStarStarship/CrabsTK
```

`repo alias` and `repo owner/repo` can precede a note. A bare qualified selector
can also precede a note. A bare alias is a selector only when it occupies the
entire line and matches a manifest binding; elsewhere use `repo alias`.
Changing/selecting a repo clears the current issue, including reselection of the
same repo, unless the selector includes a new issue number:

```imul
repo CrabsTK#161 Work on corrections.
AStarStarship/CrabsTK#170 Exact and approximate time.
```

Repository selectors never change the clock or existing session ownership.
Canonical identity is recorded as `owner/repo`; retain the alias source span.
An unknown explicit target is an error, not a fall-back to the previous repo.

## Selecting an issue

```imul
#161
issue #170
@10 #172 Start a timesheet.
```

Issue numbers are positive decimal integers in the range 1..2147483647; leading
zeros are accepted but canonical output uses the numeric value. An issue-only
selector requires a current repo. `owner/repo#number` selects both atomically.
Suffixes such as `#161.A` belong to prose/commit conventions, not issue-selector
syntax; an attempted selector with such a suffix is a diagnostic.

Issue context persists until the next issue/repo selection or document end.
No clock, repo, issue, or session context carries implicitly between files.
Markdown headings and hashtags occurring later in prose do not select tickets.
Record selection as data only: it does not change the local checkout, fetch
issues, create a branch, or grant an agent permission to publish.

## Ambiguity and migration

The original draft chose the lexicographically first account for colliding keys.
0.1 intentionally replaces that rule with `repository_ambiguous`. A compiler
must list candidate canonical repos and request explicit qualification. The
old rule is preserved in [OriginalDraft.md](OriginalDraft.md), not supported as
an agent-action fallback. Never use model confidence to break a target tie.

Copyright AStarship <https://astarship.net>.

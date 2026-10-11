# Original IMUL specification draft

This is an archival copy of the four documents supplied before the 0.1
specification update. It is historical evidence, not the active grammar.
The active specification starts at [README.md](README.md).

## README.md

```text
---
layout: page
title: Project Portfolio
description: A collection of our latest work.
categories: [web design, development]
custom_css: true
---

This document contains the language specification for I am You Language (IMUL).

1. [Workspace](./Workspace)
2. [Syntax](./Syntax)
3. [Time](./Time)
4. [Social](./Social)
5. [Agents](./Agents)
6. [Memories](./Memories)

## Mission and Vision

The mission of IMUL is to build an IMUL compiler and tools to build web apps and interface hand-written development logs with GitHub. The vision of IMUL is to minimize the overhead of read, writing, and analyzing hand-written development logs.

## License

Copyright [AStarship™](https://astarship.net).
```

## Syntax.md

```text
IMUL is case agnostic. UppercaseCamel, lowerCaseCame, UPPER_SNAKE_CASE, and lower_snake_case all refer to the same object.
```

## Time.md

```text
IMUL Timestamps shall begin with the absolute time in the following format called an ASCII Timestamp.

@2021-01-03;01:02

After the absolute time has been specified, scripts may refer to the time delta in @minutes or @minutes:seconds format as follows:

@10
@20:30

The @10 means it's 10 minutes after @2021-01-03;01:02, which is @2021-01-03;01:12. The second time delta is 20 minutes and 30 seconds past @2021-01-03;01:12, which is @2021-01-03;01:32:30.
```

The original Time.md surrounded its three examples with IMUL code fences;
those fences are omitted above for readability. Its wording is retained.

## Workspace.md

```text
An IMUL workspace consists of a set of Git accounts, either a person or organization account and all of the repositories contained in these repositories. Each GitHub account and Repo should have a unique key but in the case that they do not then the first GitHub account name sorted by GitHub account name shall be used, or the repo may be addressed by by GitHub account name.
```

Copyright AStarship <https://astarship.net>.

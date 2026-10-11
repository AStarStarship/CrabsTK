# IMUL Toolkit

This module contains the reference implementation for I am You Language (IMUL) and the IMUL Specification.

* [IMUL Specification](./_Spec/README.md) - The 0.1 language and compiler contract.
* [Implementation plan](./ImplementationPlan.md) - Local low-level compiler work and acceptance cases.
* [AStartup Cookbook](https://github.com/AStarCale/AStartupCookbook) - The official documentation for IMUL.

## Current compiler scope

The 0.1 spec defines an offline, source-preserving compiler for timestamps,
repo/ticket selection, word corrections, recurring sessions, and maps. It also
defines a safe data boundary for local note-scanning and agent adapters. The
compiler implementation is assigned separately; it is not delivered by this
spec update. See the implementation plan and conformance cases above.

## Historical roadmap

The older milestones below are preserved as history, not 0.1 requirements.

### Status

Currently, there is no code and I've just been writing IMUL on paper, which is **REALLY** useful because I know exactly what issues every part of my notes on Script2 and CT are about.

### Milestones

|        Milestone Name        | Is Complete | Description |
|:----------------------------:|:-----------:|:------------|
| Minimum Viable Product (MVP) |     No      | A MVP for IMUL is required to integrate with GitHub, read and write text and images to Markdown and Metadata-JSON, and parse Doxygen source code metadata in C++. |
|    Deep AI Integration       |     No      | IMUL is required to not only integrate with other programming languages, but also with other written languages. |

### MVP Phases

|       Phase Name       | Is Complete | Description |
|:----------------------:|:-----------:|:------------|
|  Markdown Integration  |     No      | We are required to be able to read and write text and images to Markdown into folders. |
|     UML Integration    |     No      | We are required to be able to read and write Metadata-JSON. |
|   Doxygen Integration  |     No      | We are required to be able to parse Doxygen comments from source code. |
|   GitHub Integration   |     No      | We are required to integrate with the GitHub Issue-tracking System and to edit Markdown files and to upload images to git repos. |

### Deep AI Integration

|       Phase Name       | Is Complete | Description |
|:----------------------:|:-----------:|:------------|
|   MXNet Integration    |      No     | IMUL has been integrated into an AI system using [MXNet](https://mxnet.apache.org/). |
|  CrabsNet Integration |      No     | IMUL has been used to create the CrabsNet, an advanced neutral network built with the [Interactive Gym Environment and Education Kit](https://github.com/AStarship/iGeek). |

## License

Copyright © [AStarship](https://astarship.net); all rights reserved.

This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed with this file, You can obtain one at <https://mozilla.org/MPL/2.0/>.

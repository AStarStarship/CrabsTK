# CrabsUpdate

This directory is the migration boundary between CrabsToolkit and the evolving
ASCIICrabs repository. ASCIICrabs remains read-only to CrabsToolkit agents.
Broken or not-yet-unified API pieces are implemented and tested here first.

Current C++23 foundations:

- `AType.h`: 16-bit ASCII type words with a 12-bit payload and four MOD bits.
- `ContiguousStack.h`: inline contiguous storage, stack-to-heap growth, pop-based
  checkpoints and rollback.
- `UndoStack.h`: contiguous undo records that restore mutations as records pop.
- `ORM.h`: validated schemas and PostgreSQL/SQLite DDL/DML generation.
- `JSX.h`: escaped HTML/JSX-style element rendering.
- `OAuth.h`: OAuth 2 authorization URLs and RFC 7636 S256 PKCE.
- `Wcb.h`: Write-Combining Buffer Cache — buffer writes without polluting
  the CPU cache; read directly from memory.

Build and run the portable tests:

    g++ -std=c++23 -Wall -Wextra -Wpedantic \
      CrabsUpdate/Tests/CrabsUpdateTests.cpp -o /tmp/crabs-update-tests
    /tmp/crabs-update-tests

The OAuth layer intentionally stops at protocol construction. An HTTP transport
adapter can be added later without coupling credentials or sockets to the core.

# Env Module

CrabsTK Env module: bounded, pointer-free `.env` variable table for
loading key=value credential files into iGeek and other embedded
C++ programs.

## Files

- `EnvVar.h` — declarations (TEnvVar, TEnvTable, TEnvLoad, TEnvGet, TEnvGetI, TEnvExport).
- `EnvVar.hxx` — implementations.
- `Tests/EnvTest.cpp` — standalone unit tests (build with the command in the file header).
- `Tests/_Config.h` — minimal type shim for standalone test builds.

## Seam

`CRABSTK_ENV_CORE` / `CRABSTK_ENV_VAR` = 61 in `_Seams.h`.

## API

```cpp
TEnvTable<64> table;
memset(&table, 0, sizeof(table));
EnvStatus st = TEnvLoad<64>(".env", &table);
if (st == EnvOK) {
  const CHA* host = TEnvGet<64>(table, "SLAVE_HOST");
  ISC port = 0;
  TEnvGetI<64>(table, "SLAVE_PORT", &port);
  ISN exported = TEnvExport<64>(table);
}
```

## Design

- `TEnvVar`: fixed 128-char key + 1024-char value + line number. No pointers, no heap.
- `TEnvTable<Capacity>`: array of `TEnvVar`, POD. `count` = loaded entries, `count_max` = Capacity.
- `TEnvLoad`: reads a file, skips comments/blanks/malformed lines. First occurrence of a duplicate key wins.
- `TEnvGet` / `TEnvFind`: linear search, O(n).
- `TEnvGetI`: `strtol` numeric parse with full validation.
- `TEnvExport`: `setenv()` for each entry; skips keys already in the process environment.

## Future

- OS-specific hiding: make `.env` values invisible to child processes and
  other agents (e.g. `madvise(MADV_DONTDUMP)`, `process_vm_readv`
  restrictions). Current approach is load-into-memory; the table lives in
  the calling process's address space.
- Multi-file loading (e.g. `~/.env` + local `.env`).
- `TEnvTable` as a member of `TRoom` / `TSession` for the iGeek agent
  runtime.

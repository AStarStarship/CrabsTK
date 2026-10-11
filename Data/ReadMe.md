This folder contains the CrabsTK Database API that is the backend for CrabsDB.

## RL-gym data primitives

The opt-in no-C++-stdlib data path is now implemented:

- [Record contract](Records.md): pointer-free TRecord/TRollout batches,
  canonical little-endian Core POD serialization, CRC32 integrity, and
  bounded append/sequential-read POSIX dataset I/O.
- [ASCII plot contract](AsciiPlot.md): TAsciiPlot keeps a bounded scalar window
  and emits connected console charts without GUI/OpenGL dependencies.

Use .h declarations, .hpp template definitions, and import _Package.hxx once
in the application's translation unit. The independent Data seam avoids
changing the shared root build selection while IMUL work is active. These
primitives are not yet hooked into iGeek's Gym/TPPO producers.

From the repository root:

    python _Seams/Data/Verify.py
    python _Seams/Data/Verify.py --mode ubsan
    python _Seams/Data/Verify.py --mode asan
    python -B -OO _Seams/Data/VerifyTest.py

The verifier creates a fresh directory under TMPDIR and reports its paths.
It runs 21 named seam groups, a split-TU consumer, a deterministic corpus of
10,000 mutated decoder inputs plus 10,000 plot pushes, an independent Python
reader of the real two-record dataset, and stdout/file-equality checks for
five synthetic metric charts. Outputs are test inputs/results, not RL
training measurements. -Werror is enabled; two known inherited Core-header
warnings (MSVC pragma and deprecated enum arithmetic) are suppressed.

The separate compiler-free verifier tests check valid/corrupted inputs at
Python optimization levels 0/1/2; all checks remain active under -O/-OO.
Record R07 also covers direct-byte positive/negative signaling-NaN transfers
in every floating field, verified with GCC x86 -O0 -mfpmath=387.

Dataset I/O is POSIX-only in v1; use one writer per file. CRC is not an
authentication mechanism. No fsync/power-loss or concurrent-writer guarantee
is claimed. New user/data paths are never silently truncated by the tests.

## License

Copyright AStarship <https://astarship.net>.

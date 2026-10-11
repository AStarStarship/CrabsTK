# RL rollout record contract (version 1)

This opt-in toolkit primitive fulfills AGENT_PLAN.md Gap A. It is not PPO,
GAE, a policy, a tensor library, or a TRoom repair. The existing Core File
write/read methods are placeholders; the standalone file adapter uses POSIX
file descriptors instead of pretending those methods persist data.

## Record semantics

TRecord<Capacity, ObsLength, ActionCount, StateWords> (also TRollout) is a
pointer-free, fixed-array Core POD. Only the first count rows are active.
Each row is one observed environment transition. A vectorized Gym contributes
one row per environment per batch step; a PPO horizon contributes H*B rows.
Rows may belong to different episodes: episode_ids, env_ids, and steps identify
which episode/environment and zero-based episode step each row belongs to.
The caller supplies run_id, sequence, seed, and those row identifiers; the
codec never infers an episode, seed, terminal, or missing transition.

Row fields, in wire order:

    episode_id IUD, env_id ISC, step IUD
    obs[ObsLength] FPC (observation BEFORE the recorded action)
    action ISC, reward FPC, terminal IUA
    action_mask[ActionCount] IUA (mask applicable to that action)
    value FPC, logp FPC, states[StateWords] IUC

The fixed state snapshot consists of Core IUC words, never live pointers.
StateWords may be zero. Snapshot timing is chosen by the producer and must be
consistent within a dataset; this codec does not manufacture internal state.
Gym currently supplies FPC terminals/masks: an adapter must validate exactly
0.0/1.0 and convert to IUA, never memcpy those float arrays into byte flags.
Float bits are preserved, including negative zero, infinities, and NaN payloads:
nonfinite learning signals are evidence too. Signed action values are preserved
even when a policy chose an illegal action. Binary terminal/mask flags are
validated. No padding, native pointers, host-width aliases, or C++ bool goes
on disk. The wire float representation is IEEE-754 binary32.

The codec reads/stores float object representations through references, not
floating-value returns/assignments. This preserves positive/negative signaling
NaN payloads even on x87, which can quiet them during ordinary value transfers.
RecordDetail::Float is only a convenience for callers needing a float value;
the wire decode path does not use it.

## Canonical wire format

All integers/float bit patterns are little-endian. A record is a 72-byte
header followed by count rows. There is no native-struct memcpy serialization.
Concatenate complete records to make a dataset; different shapes can coexist.

| Offset | Field | Core storage |
| --- | --- | --- |
| 0 | magic: eight ASCII bytes CRBROLL1 | CHA[8] |
| 8 | version: 1 | IUC |
| 12 | header_bytes: 72 | IUC |
| 16 | payload_bytes | IUC |
| 20 | count | IUC |
| 24 | obs_len | IUC |
| 28 | action_count | IUC |
| 32 | state_count | IUC |
| 36 | reserved: 0 | IUC |
| 40 | run_id | IUD |
| 48 | sequence | IUD |
| 56 | seed | IUD |
| 64 | CRC32/IEEE | IUC |
| 68 | reserved_tail: 0 | IUC |

Row bytes = 37 + 4*obs_len + action_count + 4*state_count.
CRC32 uses polynomial 0xedb88320, initial/final XOR 0xffffffff; cover the
whole header and payload except bytes 64..67. This detects accidental
corruption, not malicious tampering or authenticity. Header dimensions must
be nonzero except state_count; count is nonzero. All size arithmetic is
checked against a 16 MiB record limit before reading/iterating payloads.
Unknown versions, reserved bits, shape mismatches, invalid binary flags,
truncated input, or checksum failures return explicit status codes.

Encode/decode use caller-owned non-overlapping buffers. A failed encode or
decode leaves output bytes/record unchanged and sets the byte count to zero.
This precondition includes all input/output objects and byte-count storage:
written/consumed must not alias input bytes, header outputs, or record fields.
The same non-overlap requirement applies to HeaderRead and Inspect arguments.
Decode consumes one record and may leave following records in the input.
Unused in-memory rows and zero-state placeholders are never serialized.

## Dataset I/O

TWriter opens with append semantics, never truncation, and writes encoded
records. TReader reads sequentially and distinguishes clean EOF from a
truncated record. Both use bounded caller-owned wire scratch buffers; no
allocation, exceptions, C++ standard-library imports, or global state.
The codec is separate from the POSIX adapter. Call Close explicitly.

An existing dataset is not silently repaired or revalidated on open. Use one
writer per file; interrupted/failed writes can leave a partial tail. I/O
failures poison a writer until close/reopen. Validation/encode rejection
before any write does not poison it. Reader I/O, malformed-record, shape, or
capacity failures poison it until close/reopen, so later reads cannot pretend
they recovered at a record boundary. Reader scratch may contain partial bytes
on failure, but a typed output record remains unchanged. No fsync/power-loss
durability, locking, concurrent-writer atomicity, or Windows adapter is promised
by version 1.

## Integration and verification

Include Data/Record.hpp for the POD/templates and Data/Record.hxx once in
one translation unit for codec helpers. Include Data/Dataset.hpp and import
Data/Dataset.hxx once for the POSIX file adapter. Use an existing Crabs config
and SEAM >= CRABSTK_DATA_RECORD; Data/_Seams.h supplies the opt-in level.
Data/_Package.h rolls up declarations/templates; Data/_Package.hxx rolls up
implementations. The independent _Seams/Data/_Main.cpp entrypoint avoids
racing the active IMUL worker over shared root build/test files. It does not
claim that the default
legacy seam executes these new tests.

Run the Data seam from the repository root (output paths belong in TMPDIR):

    g++ -std=c++2b -Wall -Wextra -Werror -Wno-unknown-pragmas \
      -Wno-deprecated-enum-enum-conversion \
      -I_Seams/Data -I. -I.. _Seams/Data/_Main.cpp -o "$TMPDIR/data-seams"
    "$TMPDIR/data-seams" "$TMPDIR/data-seams.crabs"

The first regression is a canonical wire/field round trip. Add each later
acceptance case with a real RED-GREEN loop, including file append/read/EOF,
malformed/oversized/truncated data, caller-buffer boundaries, and sanitizers.

`python -B _Seams/Data/Verify.py` automates the Data builds/dataset/demo checks.
`python -B -OO _Seams/Data/VerifyTest.py` runs three compiler-free verifier
regressions, checking valid and corrupted fixtures at optimization levels 0/1/2.
Verification predicates explicitly raise on failure and stay active under
Python -O/-OO/PYTHONOPTIMIZE. R07 covers direct-byte signaling-NaN round trips
in observations, rewards, values and logp; GCC x86's -O0 -mfpmath=387 is the
regression backend used to expose and confirm the representation-transfer fix.

Copyright AStarship <https://astarship.net>.

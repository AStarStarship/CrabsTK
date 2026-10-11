// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_DATA_RECORD_DECL
#define CRABSTK_DATA_RECORD_DECL
#include <_Config.h>
#include "_Seams.h"
#if SEAM >= CRABSTK_DATA_RECORD
namespace _ {

enum RecordStatus : ISC {
  RecordOK = 0,
  RecordEnd = 1,
  RecordInvalid = -1,
  RecordCapacity = -2,
  RecordTruncated = -3,
  RecordVersion = -4,
  RecordChecksum = -5,
  RecordShape = -6,
  RecordIO = -7
};

enum : IUC { RecordHeaderBytes = 72, RecordMaxBytes = 16 * 1024 * 1024 };

/* Host-side decoded envelope; never memcpy this struct onto the wire.
@link Records.md */
struct TRecordHeader {
  IUC count, obs_len, action_count, state_count, payload_bytes, checksum;
  IUD run_id, sequence, seed;
};

/* Fixed-capacity, pointer-free Core POD. Each active row is one transition.
@pre Callers supply stable episode/env/step IDs and PRE-action observations.
@pre State words contain no live pointers; inactive rows are not serialized.
@link Records.md */
template<IUC Capacity, IUC ObsLength, IUC ActionCount, IUC StateWords = 0>
struct TRecord {
  static_assert(Capacity > 0 && Capacity <= 4096);
  static_assert(ObsLength > 0 && ObsLength <= 4096);
  static_assert(ActionCount > 0 && ActionCount <= 4096 && StateWords <= 4096);
  static_assert(RecordHeaderBytes + IUD(Capacity) *
                (37 + IUD(ObsLength) * 4 + ActionCount + IUD(StateWords) * 4)
                <= RecordMaxBytes);
  IUD run_id, sequence, seed;
  IUC count;
  IUD episode_ids[Capacity];
  ISC env_ids[Capacity];
  IUD steps[Capacity];
  FPC obs[Capacity * ObsLength];
  ISC actions[Capacity];
  FPC rewards[Capacity];
  IUA terminals[Capacity];
  IUA action_mask[Capacity * ActionCount];
  FPC values[Capacity], logp[Capacity];
  IUC states[Capacity * (StateWords ? StateWords : 1)];
};

template<IUC Capacity, IUC ObsLength, IUC ActionCount, IUC StateWords = 0>
using TRollout = TRecord<Capacity, ObsLength, ActionCount, StateWords>;

/* Validate the envelope/dimensions and compute checked wire size. */
RecordStatus TRecordBytes(const TRecordHeader& header, IUC* bytes);

/* Parse one complete header, without consuming or allocating payload data.
@return Version/shape/size failures explicitly; output unchanged on failure. */
RecordStatus TRecordHeaderRead(const IUA* input, IUC bytes,
                              TRecordHeader* header);

/* Validate one complete canonical record and its CRC. Trailing records are
allowed. Output header is unchanged on failure; consumed is zero on failure. */
RecordStatus TRecordInspect(const IUA* input, IUC bytes, TRecordHeader* header,
                           IUC* consumed);

/* Caller-owned, non-overlapping buffers; encode never dumps native padding.
@return RecordOK or an explicit status; written is zero on failure. */
template<IUC C, IUC O, IUC A, IUC S>
RecordStatus TRecordEncode(const TRecord<C, O, A, S>& source, IUA* output,
                          IUC capacity, IUC* written);

/* Decode into the same feature/action/state shape, allowing spare row
capacity. A rejected record leaves output unchanged and consumed zero. */
template<IUC C, IUC O, IUC A, IUC S>
RecordStatus TRecordDecode(const IUA* input, IUC bytes,
                          TRecord<C, O, A, S>& output, IUC* consumed);

namespace RecordDetail {
// Internal wire helpers; caller supplies valid ranges (Load/Store <=8 bytes).
IUD Load(const IUA* data, IUC bytes);
void Store(IUA* data, IUD value, IUC bytes);
// Reference transfers preserve signaling NaNs without FP value conversions.
IUC FloatBits(const FPC& value);
void FloatStore(FPC& value, IUC bits);
// Floating-value convenience only; the codec uses the reference transfers.
FPC Float(IUC bits);
IUC Checksum(const IUA* data, IUC bytes);
}  //< namespace RecordDetail

}  //< namespace _
#endif
#endif

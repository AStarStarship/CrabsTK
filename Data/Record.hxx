// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_DATA_RECORD_HXX
#define CRABSTK_DATA_RECORD_HXX
#include "Record.h"
#if SEAM >= CRABSTK_DATA_RECORD
namespace _ {
namespace RecordDetail {

static_assert(sizeof(IUA) == 1 && sizeof(IUC) == 4 && sizeof(ISC) == 4 &&
              sizeof(IUD) == 8 && sizeof(FPC) == 4);

IUD Load(const IUA* data, IUC bytes) {
  IUD result = 0;
  for (IUC i = 0; i < bytes; ++i) result |= IUD(data[i]) << (8 * i);
  return result;
}

void Store(IUA* data, IUD value, IUC bytes) {
  for (IUC i = 0; i < bytes; ++i) data[i] = IUA(value >> (8 * i));
}

IUC FloatBits(const FPC& value) {
  IUC bits = 0;
  const IUA* input = reinterpret_cast<const IUA*>(&value);
  IUA* output = reinterpret_cast<IUA*>(&bits);
  for (IUC i = 0; i < 4; ++i) output[i] = input[i];
  return bits;
}

void FloatStore(FPC& value, IUC bits) {
  const IUA* input = reinterpret_cast<const IUA*>(&bits);
  IUA* output = reinterpret_cast<IUA*>(&value);
  for (IUC i = 0; i < 4; ++i) output[i] = input[i];
}

FPC Float(IUC bits) {
  FPC value = 0;
  FloatStore(value, bits);
  return value;
}

IUC Checksum(const IUA* data, IUC bytes) {
  IUC crc = 0xffffffffU;
  for (IUC i = 0; i < bytes; ++i) {
    if (i >= 64 && i < 68) continue;
    crc ^= data[i];
    for (IUC bit = 0; bit < 8; ++bit)
      crc = (crc >> 1) ^ ((crc & 1) ? 0xedb88320U : 0);
  }
  return crc ^ 0xffffffffU;
}

}  //< namespace RecordDetail

RecordStatus TRecordBytes(const TRecordHeader& header, IUC* bytes) {
  if (!bytes) return RecordInvalid;
  *bytes = 0;
  if (!header.count || !header.obs_len || !header.action_count)
    return RecordInvalid;
  if (header.count > 4096 || header.obs_len > 4096 ||
      header.action_count > 4096 || header.state_count > 4096)
    return RecordCapacity;
  IUD row_bytes = 37 + IUD(header.obs_len) * 4 + header.action_count +
                  IUD(header.state_count) * 4;
  IUD total = RecordHeaderBytes + IUD(header.count) * row_bytes;
  if (total > RecordMaxBytes) return RecordCapacity;
  *bytes = IUC(total);
  return RecordOK;
}

RecordStatus TRecordHeaderRead(const IUA* input, IUC bytes,
                              TRecordHeader* header) {
  if (!input || !header) return RecordInvalid;
  if (bytes < RecordHeaderBytes) return RecordTruncated;
  const CHA* magic = "CRBROLL1";
  for (IUC i = 0; i < 8; ++i)
    if (input[i] != IUA(magic[i])) return RecordInvalid;
  using namespace RecordDetail;
  if (Load(input + 8, 4) != 1) return RecordVersion;
  if (Load(input + 12, 4) != RecordHeaderBytes || Load(input + 36, 4) ||
      Load(input + 68, 4))
    return RecordInvalid;
  TRecordHeader candidate = {};
  candidate.payload_bytes = IUC(Load(input + 16, 4));
  candidate.count = IUC(Load(input + 20, 4));
  candidate.obs_len = IUC(Load(input + 24, 4));
  candidate.action_count = IUC(Load(input + 28, 4));
  candidate.state_count = IUC(Load(input + 32, 4));
  candidate.run_id = Load(input + 40, 8);
  candidate.sequence = Load(input + 48, 8);
  candidate.seed = Load(input + 56, 8);
  candidate.checksum = IUC(Load(input + 64, 4));
  IUC total = 0;
  RecordStatus status = TRecordBytes(candidate, &total);
  if (status != RecordOK) return status;
  if (candidate.payload_bytes != total - RecordHeaderBytes)
    return RecordInvalid;
  *header = candidate;
  return RecordOK;
}

RecordStatus TRecordInspect(const IUA* input, IUC bytes, TRecordHeader* header,
                           IUC* consumed) {
  if (!consumed) return RecordInvalid;
  *consumed = 0;
  if (!header) return RecordInvalid;
  TRecordHeader candidate = {};
  RecordStatus status = TRecordHeaderRead(input, bytes, &candidate);
  if (status != RecordOK) return status;
  IUC total = candidate.payload_bytes + RecordHeaderBytes;
  if (bytes < total) return RecordTruncated;
  if (RecordDetail::Checksum(input, total) != candidate.checksum)
    return RecordChecksum;
  IUC row_bytes = candidate.payload_bytes / candidate.count;
  for (IUC i = 0; i < candidate.count; ++i) {
    IUC flags = RecordHeaderBytes + i * row_bytes + 28 +
                candidate.obs_len * 4;
    if (input[flags] > 1) return RecordInvalid;
    for (IUC j = 0; j < candidate.action_count; ++j)
      if (input[flags + 1 + j] > 1) return RecordInvalid;
  }
  *header = candidate;
  *consumed = total;
  return RecordOK;
}

}  //< namespace _
#endif
#endif

// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_DATA_RECORD_HPP
#define CRABSTK_DATA_RECORD_HPP
#include "Record.h"
#if SEAM >= CRABSTK_DATA_RECORD
namespace _ {

template<IUC C, IUC O, IUC A, IUC S>
RecordStatus TRecordEncode(const TRecord<C, O, A, S>& source, IUA* output,
                          IUC capacity, IUC* written) {
  if (!written) return RecordInvalid;
  *written = 0;
  if (!output || !source.count) return RecordInvalid;
  if (source.count > C) return RecordCapacity;
  for (IUC i = 0; i < source.count; ++i) {
    if (source.terminals[i] > 1) return RecordInvalid;
    for (IUC j = 0; j < A; ++j)
      if (source.action_mask[i * A + j] > 1) return RecordInvalid;
  }
  TRecordHeader header = {};
  header.count = source.count;
  header.obs_len = O;
  header.action_count = A;
  header.state_count = S;
  header.run_id = source.run_id;
  header.sequence = source.sequence;
  header.seed = source.seed;
  IUC bytes = 0;
  RecordStatus status = TRecordBytes(header, &bytes);
  if (status != RecordOK) return status;
  if (capacity < bytes) return RecordCapacity;
  header.payload_bytes = bytes - RecordHeaderBytes;
  using namespace RecordDetail;
  const CHA* magic = "CRBROLL1";
  for (IUC i = 0; i < RecordHeaderBytes; ++i) output[i] = 0;
  for (IUC i = 0; i < 8; ++i) output[i] = IUA(magic[i]);
  Store(output + 8, 1, 4);
  Store(output + 12, RecordHeaderBytes, 4);
  Store(output + 16, header.payload_bytes, 4);
  Store(output + 20, source.count, 4);
  Store(output + 24, O, 4);
  Store(output + 28, A, 4);
  Store(output + 32, S, 4);
  Store(output + 40, source.run_id, 8);
  Store(output + 48, source.sequence, 8);
  Store(output + 56, source.seed, 8);
  IUC cursor = RecordHeaderBytes;
  for (IUC i = 0; i < source.count; ++i) {
    Store(output + cursor, source.episode_ids[i], 8);
    cursor += 8;
    Store(output + cursor, IUC(source.env_ids[i]), 4);
    cursor += 4;
    Store(output + cursor, source.steps[i], 8);
    cursor += 8;
    for (IUC j = 0; j < O; ++j) {
      Store(output + cursor, FloatBits(source.obs[i * O + j]), 4);
      cursor += 4;
    }
    Store(output + cursor, IUC(source.actions[i]), 4);
    cursor += 4;
    Store(output + cursor, FloatBits(source.rewards[i]), 4);
    cursor += 4;
    output[cursor++] = source.terminals[i];
    for (IUC j = 0; j < A; ++j)
      output[cursor++] = source.action_mask[i * A + j];
    Store(output + cursor, FloatBits(source.values[i]), 4);
    cursor += 4;
    Store(output + cursor, FloatBits(source.logp[i]), 4);
    cursor += 4;
    for (IUC j = 0; j < S; ++j) {
      Store(output + cursor, source.states[i * S + j], 4);
      cursor += 4;
    }
  }
  Store(output + 64, Checksum(output, bytes), 4);
  *written = bytes;
  return RecordOK;
}

template<IUC C, IUC O, IUC A, IUC S>
RecordStatus TRecordDecode(const IUA* input, IUC bytes,
                          TRecord<C, O, A, S>& output, IUC* consumed) {
  if (!consumed) return RecordInvalid;
  *consumed = 0;
  TRecordHeader header = {};
  IUC total = 0;
  RecordStatus status = TRecordInspect(input, bytes, &header, &total);
  if (status != RecordOK) return status;
  if (header.count > C) return RecordCapacity;
  if (header.obs_len != O || header.action_count != A ||
      header.state_count != S)
    return RecordShape;
  using namespace RecordDetail;
  output.count = header.count;
  output.run_id = header.run_id;
  output.sequence = header.sequence;
  output.seed = header.seed;
  IUC cursor = RecordHeaderBytes;
  for (IUC i = 0; i < header.count; ++i) {
    output.episode_ids[i] = Load(input + cursor, 8);
    cursor += 8;
    output.env_ids[i] = ISC(IUC(Load(input + cursor, 4)));
    cursor += 4;
    output.steps[i] = Load(input + cursor, 8);
    cursor += 8;
    for (IUC j = 0; j < O; ++j) {
      FloatStore(output.obs[i * O + j], IUC(Load(input + cursor, 4)));
      cursor += 4;
    }
    output.actions[i] = ISC(IUC(Load(input + cursor, 4)));
    cursor += 4;
    FloatStore(output.rewards[i], IUC(Load(input + cursor, 4)));
    cursor += 4;
    output.terminals[i] = input[cursor++];
    for (IUC j = 0; j < A; ++j)
      output.action_mask[i * A + j] = input[cursor++];
    FloatStore(output.values[i], IUC(Load(input + cursor, 4)));
    cursor += 4;
    FloatStore(output.logp[i], IUC(Load(input + cursor, 4)));
    cursor += 4;
    for (IUC j = 0; j < S; ++j) {
      output.states[i * S + j] = IUC(Load(input + cursor, 4));
      cursor += 4;
    }
  }
  *consumed = total;
  return RecordOK;
}

}  //< namespace _
#endif
#endif

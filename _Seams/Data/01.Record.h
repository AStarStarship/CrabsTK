// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>

#if __has_include("../../Data/Record.hpp") && \
    __has_include("../../Data/Record.hxx")
#include "../../Data/Record.hpp"
#include "../../Data/Record.hxx"
#define CRABSTK_RECORD_PRESENT 1
#endif

namespace _ {
namespace DataTests {

inline const CHA* Record(const CHA* args) {
  (void)args;
#if !defined(CRABSTK_RECORD_PRESENT)
  return "FAIL R01: pointer-free rollout codec is not implemented";
#else
  using Rollout = TRollout<2, 2, 2, 2>;
  static_assert(__is_trivial(Rollout) && __is_standard_layout(Rollout));
  Rollout source = {}, decoded = {};
  source.run_id = 42;
  source.sequence = 0;
  source.seed = 99;
  source.count = 1;
  source.episode_ids[0] = 7;
  source.env_ids[0] = 0;
  source.steps[0] = 3;
  source.obs[0] = 1;
  source.obs[1] = -2;
  source.actions[0] = -1;
  source.rewards[0] = 0.25F;
  source.terminals[0] = 1;
  source.action_mask[0] = 1;
  source.action_mask[1] = 0;
  source.values[0] = 0.5F;
  source.logp[0] = -0.5F;
  source.states[0] = 0x12345678U;
  source.states[1] = 0xffffffffU;
  IUA wire[256] = {};
  IUC written = 0, consumed = 0;
  if (TRecordEncode(source, wire, sizeof(wire), &written) != RecordOK)
    return "FAIL R01: encode";
  if (written != 127 || wire[0] != 'C' || wire[8] != 1 ||
      wire[12] != 72 || wire[72] != 7)
    return "FAIL R01: canonical wire envelope";
  if (TRecordDecode(wire, written, decoded, &consumed) != RecordOK)
    return "FAIL R01: decode";
  if (consumed != written || decoded.run_id != 42 || decoded.seed != 99 ||
      decoded.count != 1 || decoded.episode_ids[0] != 7 ||
      decoded.steps[0] != 3 || decoded.obs[0] != 1 ||
      decoded.obs[1] != -2 || decoded.actions[0] != -1 ||
      decoded.rewards[0] != 0.25F || decoded.terminals[0] != 1 ||
      decoded.action_mask[0] != 1 || decoded.action_mask[1] != 0 ||
      decoded.values[0] != 0.5F || decoded.logp[0] != -0.5F ||
      decoded.states[0] != 0x12345678U ||
      decoded.states[1] != 0xffffffffU)
    return "FAIL R01: field round trip";

  // R02: invalid binary flags reject before mutating caller wire storage.
  source.terminals[0] = 2;
  for (IUC i = 0; i < sizeof(wire); ++i) wire[i] = 0xa5;
  written = 123;
  if (TRecordEncode(source, wire, sizeof(wire), &written) != RecordInvalid ||
      written != 0)
    return "FAIL R02: non-binary terminal accepted";
  for (IUC i = 0; i < sizeof(wire); ++i)
    if (wire[i] != 0xa5) return "FAIL R02: failed encode mutated bytes";
  source.terminals[0] = 1;
  source.action_mask[0] = 2;
  if (TRecordEncode(source, wire, sizeof(wire), &written) != RecordInvalid)
    return "FAIL R02: non-binary action mask accepted";
  source.action_mask[0] = 1;

  // R03: even a correctly checksummed wire record must have binary flags.
  if (TRecordEncode(source, wire, sizeof(wire), &written) != RecordOK)
    return "FAIL R03: setup";
  const IUC TerminalOffset = RecordHeaderBytes + 20 + 2 * 4 + 8;
  wire[TerminalOffset] = 2;
  RecordDetail::Store(wire + 64, RecordDetail::Checksum(wire, written), 4);
  decoded.run_id = 77;
  consumed = 99;
  if (TRecordDecode(wire, written, decoded, &consumed) != RecordInvalid ||
      consumed != 0 || decoded.run_id != 77)
    return "FAIL R03: non-binary wire terminal accepted or output mutated";
  wire[TerminalOffset] = 1;
  wire[TerminalOffset + 1] = 2;
  RecordDetail::Store(wire + 64, RecordDetail::Checksum(wire, written), 4);
  if (TRecordDecode(wire, written, decoded, &consumed) != RecordInvalid ||
      decoded.run_id != 77)
    return "FAIL R03: non-binary wire action mask accepted";
  wire[TerminalOffset + 1] = 1;
  RecordDetail::Store(wire + 64, RecordDetail::Checksum(wire, written), 4);

  // R05: every truncated prefix, damaged payload, bad version/reserved fields.
  for (IUC length = 0; length < written; ++length) {
    consumed = 999;
    if (TRecordDecode(wire, length, decoded, &consumed) != RecordTruncated ||
        consumed != 0 || decoded.run_id != 77)
      return "FAIL R05: truncated input mutated output or was accepted";
  }
  wire[RecordHeaderBytes + 20] ^= 1;
  if (TRecordDecode(wire, written, decoded, &consumed) != RecordChecksum ||
      decoded.run_id != 77)
    return "FAIL R05: payload corruption accepted";
  wire[RecordHeaderBytes + 20] ^= 1;
  wire[8] = 2;
  if (TRecordDecode(wire, written, decoded, &consumed) != RecordVersion)
    return "FAIL R05: unsupported version accepted";
  wire[8] = 1;
  wire[36] = 1;
  if (TRecordDecode(wire, written, decoded, &consumed) != RecordInvalid)
    return "FAIL R05: reserved bits accepted";
  wire[36] = 0;
  RecordDetail::Store(wire + 24, 0xffffffffU, 4);
  if (TRecordDecode(wire, written, decoded, &consumed) != RecordCapacity)
    return "FAIL R05: oversized dimensions accepted";
  RecordDetail::Store(wire + 24, 2, 4);

  // R06: capacity/shape/input bounds and inactive rows never serialized.
  TRollout<2, 3, 2, 2> wrong_shape = {};
  wrong_shape.run_id = 999;
  if (TRecordDecode(wire, written, wrong_shape, &consumed) != RecordShape ||
      wrong_shape.run_id != 999 || consumed)
    return "FAIL R06: wrong shape accepted or output mutated";
  IUA copy[256] = {};
  for (IUC i = 0; i < sizeof(wire); ++i) copy[i] = wire[i];
  if (TRecordEncode(source, wire, written - 1, &consumed) != RecordCapacity)
    return "FAIL R06: insufficient output capacity accepted";
  for (IUC i = 0; i < sizeof(wire); ++i)
    if (wire[i] != copy[i]) return "FAIL R06: capacity failure mutated bytes";
  source.terminals[1] = 0xff;
  source.obs[2] = 999;
  source.states[2] = 0xdeadbeefU;
  if (TRecordEncode(source, wire, sizeof(wire), &consumed) != RecordOK ||
      consumed != written)
    return "FAIL R06: spare rows influenced size/validation";
  for (IUC i = 0; i < written; ++i)
    if (wire[i] != copy[i]) return "FAIL R06: spare row data leaked to disk";
  source.count = 3;
  if (TRecordEncode(source, wire, sizeof(wire), &consumed) != RecordCapacity)
    return "FAIL R06: count exceeds POD capacity";
  source.count = 0;
  if (TRecordEncode(source, wire, sizeof(wire), &consumed) != RecordInvalid)
    return "FAIL R06: empty transition record accepted";
  source.count = 1;
  if (TRecordEncode(source, NILP, 0, &consumed) != RecordInvalid ||
      TRecordDecode(NILP, 0, decoded, &consumed) != RecordInvalid ||
      TRecordEncode(source, wire, sizeof(wire), NILP) != RecordInvalid)
    return "FAIL R06: null buffers/count pointers accepted";

  // R07: exact float bits survive (signed zero, infinities, NaN payloads).
  source.obs[0] = RecordDetail::Float(0x80000000U);
  source.obs[1] = RecordDetail::Float(0x7fc12345U);
  source.rewards[0] = RecordDetail::Float(0x7f800000U);
  source.values[0] = RecordDetail::Float(0xff800000U);
  if (TRecordEncode(source, wire, sizeof(wire), &written) != RecordOK ||
      TRecordDecode(wire, written, decoded, &consumed) != RecordOK)
    return "FAIL R07: nonfinite values discarded";
  if (RecordDetail::Load(wire + RecordHeaderBytes + 20, 4) != 0x80000000U ||
      RecordDetail::FloatBits(decoded.obs[0]) != 0x80000000U ||
      RecordDetail::FloatBits(decoded.obs[1]) != 0x7fc12345U ||
      RecordDetail::FloatBits(decoded.rewards[0]) != 0x7f800000U ||
      RecordDetail::FloatBits(decoded.values[0]) != 0xff800000U)
    return "FAIL R07: float bit patterns changed";

  // Install and observe signaling NaNs as bytes, never floating values.
  const IUC SignalingBits[] = {0x7f800001U, 0x7f812345U, 0xff800001U,
                              0xff812345U};
  const IUC FloatOffsets[] = {20, 24, 32, 39, 43};
  FPC* source_fields[] = {source.obs, source.obs + 1, source.rewards,
                          source.values, source.logp};
  const FPC* decoded_fields[] = {decoded.obs, decoded.obs + 1, decoded.rewards,
                                decoded.values, decoded.logp};
  for (IUC bits : SignalingBits) {
    const IUA* expected = reinterpret_cast<const IUA*>(&bits);
    for (IUC field = 0; field < 5; ++field) {
      IUA* target = reinterpret_cast<IUA*>(source_fields[field]);
      for (IUC byte = 0; byte < sizeof(bits); ++byte)
        target[byte] = expected[byte];
    }
    if (TRecordEncode(source, wire, sizeof(wire), &written) != RecordOK ||
        TRecordDecode(wire, written, decoded, &consumed) != RecordOK ||
        consumed != written)
      return "FAIL R07: signaling NaN codec round trip";
    for (IUC field = 0; field < 5; ++field) {
      if (RecordDetail::Load(wire + RecordHeaderBytes + FloatOffsets[field],
                             4) != bits)
        return "FAIL R07: signaling NaN wire bits changed";
      const IUA* original =
          reinterpret_cast<const IUA*>(source_fields[field]);
      const IUA* actual =
          reinterpret_cast<const IUA*>(decoded_fields[field]);
      for (IUC byte = 0; byte < sizeof(bits); ++byte) {
        if (original[byte] != expected[byte])
          return "FAIL R07: signaling NaN source bits changed";
        if (actual[byte] != expected[byte])
          return "FAIL R07: signaling NaN decoded bits changed";
      }
    }
  }

  // R11: no snapshot words, max IDs, signed actions, and exact shape bounds.
  TRollout<1, 1, 1> tiny = {}, tiny_decoded = {};
  tiny.count = 1;
  tiny.run_id = IUD(-1);
  tiny.episode_ids[0] = IUD(-1);
  tiny.actions[0] = ISC(-2147483647) - 1;
  tiny.action_mask[0] = 1;
  tiny_decoded.states[0] = 0xdeadbeefU;
  if (TRecordEncode(tiny, copy, sizeof(copy), &consumed) != RecordOK ||
      TRecordDecode(copy, consumed, tiny_decoded, &consumed) != RecordOK ||
      tiny_decoded.run_id != IUD(-1) ||
      tiny_decoded.episode_ids[0] != IUD(-1) ||
      tiny_decoded.actions[0] != ISC(-2147483647) - 1 ||
      tiny_decoded.states[0] != 0xdeadbeefU)
    return "FAIL R11: zero-state/minimal record round trip";
  TRecordHeader header = {};
  if (TRecordHeaderRead(copy, RecordHeaderBytes, &header) != RecordOK ||
      header.state_count != 0 || header.count != 1)
    return "FAIL R11: zero-state envelope";

  // R12: decoding consumes one framed record, not an entire dataset buffer.
  IUA concatenated[512] = {};
  for (IUC i = 0; i < written; ++i) {
    concatenated[i] = wire[i];
    concatenated[written + i] = wire[i];
  }
  if (TRecordDecode(concatenated, written * 2, decoded, &consumed) != RecordOK ||
      consumed != written)
    return "FAIL R12: concatenated record framing";
  decoded.obs[2] = 123;
  if (TRecordDecode(concatenated + consumed, written, decoded, &consumed) !=
      RecordOK || consumed != written || decoded.obs[2] != 123)
    return "FAIL R12: trailing record/spare output rows";
  return NILP;
#endif
}

}  //< namespace DataTests
}  //< namespace _

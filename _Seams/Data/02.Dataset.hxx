// Copyright AStarship <https://astarship.net>.
#pragma once
#include "_Config.h"
#include <fcntl.h>
#include <unistd.h>

#if __has_include("../../Data/Dataset.hpp") && \
    __has_include("../../Data/Dataset.hxx")
#include "../../Data/Dataset.hpp"
#include "../../Data/Dataset.hxx"
#define CRABSTK_DATASET_PRESENT 1
#endif

namespace _ {
namespace DataTests {

inline const CHA* Dataset(const CHA* path) {
#if !defined(CRABSTK_DATASET_PRESENT)
  (void)path;
  return "FAIL R04: append/read dataset is not implemented";
#else
  // R04: create an owned fresh fixture, append across reopen, read to EOF.
  // O_EXCL protects existing data: this test NEVER truncates/removes a file.
  if (!path || !path[0]) return "FAIL R04: supply a fresh fixture path";
  ISC fixture = open(path, O_WRONLY | O_CREAT | O_EXCL, 0600);
  if (fixture < 0) return "FAIL R04: fixture already exists or cannot create";
  if (close(fixture)) return "FAIL R04: fixture close";
  TRollout<2, 2, 2, 2> first = {}, second = {}, decoded = {};
  first.count = 2;
  first.run_id = 73;
  first.seed = 17;
  first.sequence = 88;
  for (IUC i = 0; i < 2; ++i) {
    first.episode_ids[i] = i ? 222 : 111;
    first.env_ids[i] = ISC(i);
    first.steps[i] = i + 5;
    first.obs[i * 2] = FPC(i + 1);
    first.obs[i * 2 + 1] = -FPC(i + 1);
    first.actions[i] = ISC(i);
    first.rewards[i] = FPC(i) + 0.25F;
    first.terminals[i] = IUA(i);
    first.action_mask[i * 2] = 1;
    first.action_mask[i * 2 + 1] = 1;
    first.values[i] = 0.5F;
    first.logp[i] = -0.5F;
    first.states[i * 2] = i + 7;
    first.states[i * 2 + 1] = 0xffffffffU;
  }
  second = first;
  second.count = 1;
  second.sequence = 89;
  second.episode_ids[0] = 333;
  IUA scratch[512] = {};
  TWriter writer;
  if (TWriterOpen(writer, path) != RecordOK ||
      TWriterAppend(writer, first, scratch, sizeof(scratch)) != RecordOK ||
      writer.records != 1 || TWriterClose(writer) != RecordOK)
    return "FAIL R04: first append";
  if (TWriterOpen(writer, path) != RecordOK ||
      TWriterAppend(writer, second, scratch, sizeof(scratch)) != RecordOK ||
      TWriterClose(writer) != RecordOK)
    return "FAIL R04: append on reopen";
  TReader reader;
  if (TReaderOpen(reader, path) != RecordOK ||
      TReaderRead(reader, decoded, scratch, sizeof(scratch)) != RecordOK)
    return "FAIL R04: first read";
  if (decoded.count != 2 || decoded.run_id != 73 || decoded.seed != 17 ||
      decoded.sequence != 88 || decoded.episode_ids[0] != 111 ||
      decoded.episode_ids[1] != 222 || decoded.env_ids[1] != 1 ||
      decoded.steps[1] != 6 || decoded.obs[3] != -2 ||
      decoded.terminals[1] != 1 || decoded.states[2] != 8)
    return "FAIL R04: first dataset record fields";
  if (TReaderRead(reader, decoded, scratch, sizeof(scratch)) != RecordOK ||
      decoded.count != 1 || decoded.sequence != 89 ||
      decoded.episode_ids[0] != 333 || reader.records != 2)
    return "FAIL R04: second dataset record fields";
  if (TReaderRead(reader, decoded, scratch, sizeof(scratch)) != RecordEnd ||
      TReaderRead(reader, decoded, scratch, sizeof(scratch)) != RecordEnd ||
      decoded.sequence != 89 || TReaderClose(reader) != RecordOK)
    return "FAIL R04: clean and stable EOF";

  // R08: real files with partial headers/payloads, CRC damage, huge counts.
  IUC encoded = 0;
  if (TRecordEncode(first, scratch, sizeof(scratch), &encoded) != RecordOK)
    return "FAIL R08: encode setup";
  CHA bad_path[512] = {};
  IUC path_length = 0;
  while (path[path_length] && path_length + 6 < sizeof(bad_path)) {
    bad_path[path_length] = path[path_length];
    ++path_length;
  }
  if (path[path_length]) return "FAIL R08: fixture path too long";
  bad_path[path_length] = '.';
  bad_path[path_length + 1] = 'b';
  bad_path[path_length + 2] = 'a';
  bad_path[path_length + 3] = 'd';
  const RecordStatus Expected[6] = {RecordTruncated, RecordTruncated,
    RecordTruncated, RecordTruncated, RecordChecksum, RecordCapacity};
  const IUC Lengths[6] = {1, RecordHeaderBytes - 1, RecordHeaderBytes,
    encoded - 1, encoded, RecordHeaderBytes};
  for (IUC i = 0; i < 6; ++i) {
    IUA damaged[512] = {};
    for (IUC j = 0; j < encoded; ++j) damaged[j] = scratch[j];
    if (i == 4) damaged[encoded - 1] ^= 1;
    if (i == 5) RecordDetail::Store(damaged + 20, 4097, 4);
    bad_path[path_length + 4] = CHA('0' + i);
    ISC bad_fd = open(bad_path, O_WRONLY | O_CREAT | O_EXCL, 0600);
    if (bad_fd < 0 || write(bad_fd, damaged, Lengths[i]) != ISW(Lengths[i]) ||
        close(bad_fd))
      return "FAIL R08: malformed fixture write";
    decoded.run_id = 999;
    if (TReaderOpen(reader, bad_path) != RecordOK ||
        TReaderRead(reader, decoded, damaged, sizeof(damaged)) != Expected[i] ||
        decoded.run_id != 999 || reader.records != 0 ||
        TReaderRead(reader, decoded, damaged, sizeof(damaged)) != Expected[i] ||
        TReaderClose(reader) != RecordOK)
      return "FAIL R08: malformed stream accepted or lost error state";
  }

  // R09: insufficient scratch poisons a reader; reopen restores alignment.
  IUC used = 999;
  if (TReaderOpen(reader, path) != RecordOK ||
      TReaderNext(reader, scratch, RecordHeaderBytes, &used) != RecordCapacity ||
      used || reader.records ||
      TReaderNext(reader, scratch, sizeof(scratch), &used) != RecordCapacity ||
      TReaderClose(reader) != RecordOK || TReaderOpen(reader, path) != RecordOK ||
      TReaderRead(reader, decoded, scratch, sizeof(scratch)) != RecordOK ||
      decoded.sequence != 88 || TReaderClose(reader) != RecordOK)
    return "FAIL R09: reader capacity/reopen handling";

  // R10: a real read-only descriptor makes write fail and poison the writer.
  writer.fd = open(path, O_RDONLY);
  writer.status = RecordOK;
  writer.records = 0;
  if (writer.fd < 0 ||
      TWriterAppend(writer, first, scratch, sizeof(scratch)) != RecordIO ||
      writer.status != RecordIO || writer.records ||
      TWriterAppend(writer, first, scratch, sizeof(scratch)) != RecordIO ||
      TWriterClose(writer) != RecordOK)
    return "FAIL R10: OS write failure not propagated/sticky";
  if (TWriterAppend(writer, first, scratch, sizeof(scratch)) != RecordIO ||
      TReaderRead(reader, decoded, scratch, sizeof(scratch)) != RecordIO ||
      TReaderOpen(reader, NILP) != RecordInvalid ||
      TWriterOpen(writer, "") != RecordInvalid)
    return "FAIL R10: closed/null streams accepted";
  return NILP;
#endif
}

}  //< namespace DataTests
}  //< namespace _

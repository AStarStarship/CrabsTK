// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_DATA_DATASET_HXX
#define CRABSTK_DATA_DATASET_HXX
#include "Dataset.h"
#if SEAM >= CRABSTK_DATA_RECORD
#if defined(_WIN32)
#error Dataset v1 file adapter is POSIX; Record codec is platform-independent.
#else
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
namespace _ {
namespace DatasetDetail {

RecordStatus Read(ISC fd, IUA* output, IUC bytes) {
  IUC received = 0;
  while (received < bytes) {
    ISW result = read(fd, output + received, bytes - received);
    if (result < 0) {
      if (errno == EINTR) continue;
      return RecordIO;
    }
    if (!result) return received ? RecordTruncated : RecordEnd;
    received += IUC(result);
  }
  return RecordOK;
}

}  //< namespace DatasetDetail

RecordStatus TWriterOpen(TWriter& writer, const CHA* path) {
  if (writer.fd >= 0 || !path || !path[0]) return RecordInvalid;
  writer.fd = open(path, O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC, 0600);
  writer.records = 0;
  writer.status = writer.fd < 0 ? RecordIO : RecordOK;
  return writer.status;
}

RecordStatus TWriterClose(TWriter& writer) {
  ISC fd = writer.fd;
  writer.fd = -1;
  writer.status = RecordOK;
  return fd >= 0 && close(fd) ? RecordIO : RecordOK;
}

RecordStatus TWriterAppend(TWriter& writer, const IUA* record, IUC bytes) {
  if (writer.fd < 0) return RecordIO;
  if (writer.status != RecordOK) return writer.status;
  if (writer.records == IUD(-1)) return RecordCapacity;
  TRecordHeader header = {};
  IUC consumed = 0;
  RecordStatus status = TRecordInspect(record, bytes, &header, &consumed);
  if (status != RecordOK) return status;
  if (consumed != bytes) return RecordInvalid;
  IUC sent = 0;
  while (sent < bytes) {
    ISW result = write(writer.fd, record + sent, bytes - sent);
    if (result < 0 && errno == EINTR) continue;
    if (result <= 0) {
      writer.status = RecordIO;
      return writer.status;
    }
    sent += IUC(result);
  }
  ++writer.records;
  return RecordOK;
}

RecordStatus TReaderOpen(TReader& reader, const CHA* path) {
  if (reader.fd >= 0 || !path || !path[0]) return RecordInvalid;
  reader.fd = open(path, O_RDONLY | O_CLOEXEC);
  reader.records = 0;
  reader.status = reader.fd < 0 ? RecordIO : RecordOK;
  return reader.status;
}

RecordStatus TReaderClose(TReader& reader) {
  ISC fd = reader.fd;
  reader.fd = -1;
  reader.status = RecordOK;
  return fd >= 0 && close(fd) ? RecordIO : RecordOK;
}

RecordStatus TReaderNext(TReader& reader, IUA* scratch, IUC capacity,
                        IUC* used) {
  if (!used) return RecordInvalid;
  *used = 0;
  if (reader.fd < 0) return RecordIO;
  if (reader.status != RecordOK) return reader.status;
  if (!scratch || capacity < RecordHeaderBytes || reader.records == IUD(-1)) {
    reader.status = scratch ? RecordCapacity : RecordInvalid;
    return reader.status;
  }
  RecordStatus status = DatasetDetail::Read(reader.fd, scratch,
                                           RecordHeaderBytes);
  if (status != RecordOK) {
    reader.status = status;
    return status;
  }
  TRecordHeader header = {};
  status = TRecordHeaderRead(scratch, RecordHeaderBytes, &header);
  IUC bytes = RecordHeaderBytes + header.payload_bytes;
  if (status == RecordOK && bytes > capacity) status = RecordCapacity;
  if (status == RecordOK) {
    status = DatasetDetail::Read(reader.fd, scratch + RecordHeaderBytes,
                                header.payload_bytes);
    if (status == RecordEnd) status = RecordTruncated;
  }
  if (status == RecordOK) status = TRecordInspect(scratch, bytes, &header, used);
  if (status != RecordOK) {
    reader.status = status;
    return status;
  }
  ++reader.records;
  return RecordOK;
}

}  //< namespace _
#endif
#endif
#endif

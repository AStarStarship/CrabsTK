// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_DATA_DATASET_DECL
#define CRABSTK_DATA_DATASET_DECL
#include "Record.h"
#if SEAM >= CRABSTK_DATA_RECORD
namespace _ {

/* Owned POSIX stream state, NOT a serializable record. Do not copy an open
stream. Close explicitly; default construction owns no descriptor. */
struct TWriter {
  ISC fd = -1;
  IUD records = 0;
  RecordStatus status = RecordOK;
};

struct TReader {
  ISC fd = -1;
  IUD records = 0;
  RecordStatus status = RecordOK;
};

/* Open/create with append semantics, mode 0600 for newly created files.
No truncation, existing-file repair, concurrent-write or fsync guarantee. */
RecordStatus TWriterOpen(TWriter& writer, const CHA* path);
RecordStatus TWriterClose(TWriter& writer);

/* Validate and append exactly one complete wire record. Short/interrupted
OS writes are handled. I/O failure poisons the writer until close/reopen. */
RecordStatus TWriterAppend(TWriter& writer, const IUA* record, IUC bytes);

template<IUC C, IUC O, IUC A, IUC S>
RecordStatus TWriterAppend(TWriter& writer, const TRecord<C, O, A, S>& record,
                          IUA* scratch, IUC capacity);

RecordStatus TReaderOpen(TReader& reader, const CHA* path);
RecordStatus TReaderClose(TReader& reader);

/* Read and validate one record into bounded scratch. Clean EOF returns
RecordEnd; a partial header/payload returns RecordTruncated. Scratch may
contain a partial record on failure, but used is always zero on failure.
Read errors/capacity/format failures poison the stream until close/reopen. */
RecordStatus TReaderNext(TReader& reader, IUA* scratch, IUC capacity, IUC* used);

template<IUC C, IUC O, IUC A, IUC S>
RecordStatus TReaderRead(TReader& reader, TRecord<C, O, A, S>& record,
                        IUA* scratch, IUC capacity);

}  //< namespace _
#endif
#endif

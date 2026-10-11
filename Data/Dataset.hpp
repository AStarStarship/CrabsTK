// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_DATA_DATASET_HPP
#define CRABSTK_DATA_DATASET_HPP
#include "Dataset.h"
#include "Record.hpp"
#if SEAM >= CRABSTK_DATA_RECORD
namespace _ {

template<IUC C, IUC O, IUC A, IUC S>
RecordStatus TWriterAppend(TWriter& writer, const TRecord<C, O, A, S>& record,
                          IUA* scratch, IUC capacity) {
  if (writer.fd < 0) return RecordIO;
  if (writer.status != RecordOK) return writer.status;
  IUC bytes = 0;
  RecordStatus status = TRecordEncode(record, scratch, capacity, &bytes);
  if (status != RecordOK) return status;
  return TWriterAppend(writer, scratch, bytes);
}

template<IUC C, IUC O, IUC A, IUC S>
RecordStatus TReaderRead(TReader& reader, TRecord<C, O, A, S>& record,
                        IUA* scratch, IUC capacity) {
  IUC bytes = 0;
  RecordStatus status = TReaderNext(reader, scratch, capacity, &bytes);
  if (status != RecordOK) return status;
  IUC consumed = 0;
  status = TRecordDecode(scratch, bytes, record, &consumed);
  if (status != RecordOK) reader.status = status;
  return status;
}

}  //< namespace _
#endif
#endif

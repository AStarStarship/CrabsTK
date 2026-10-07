// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#ifndef CRABSTK_DOCUMENT_DECL
#define CRABSTK_DOCUMENT_DECL
namespace _ {
class Document {
 public:
  Document();

  ISN Run(ISN arg_count, CHA** args);

  template<typename Printer>
  Printer& PrintTo(Printer& p) {
    return p << "Document";
  }
};
}  //< namespace _
#endif

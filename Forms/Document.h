// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#ifndef KABUKITOOLKIT_DOCUMENT
#define KABUKITOOLKIT_DOCUMENT
namespace _ {
class Document {
 public:
  Document();

  SIN Run(SIN arg_count, CHA** args);

  template<ypename Printer>
  Printer& PrintTo(Printer& p) {
    return p << "Document";
  }
};
}  //< namespace _
#endif

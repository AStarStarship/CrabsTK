// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#ifndef CRABSTK_DOCUMENTH
#define CRABSTK_DOCUMENTH
namespace CTK {
namespace Forms {

class Document {
 public:
  Document() {

  }

  ISC Run(ISC arg_count, CHA** args) {
    return 0;
  }

  template<typename Printer>
  Printer& PrintTo(Printer& p) {
    return p << "Document";
  }
};

}  //< namespace Forms
}  //< namespace CTK
#endif

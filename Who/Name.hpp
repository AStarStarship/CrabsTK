// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_WHO_NAME_DECL_DECL
#define CRABSTK_WHO_NAME_DECL_DECL
#include <_Config.h>
#if SEAM >= CRABSTK_WHO_NAME
namespace _ {
/* A name of an entity. */
class TName {
 public:
  /* Default constructor. */
  TName() {}

  /* Writes this object to the given text. */
  Printer& Print (Printer& o) {
    o << "\nName:";
  };

private:
  THandle name_;
};

} //< namespace _
#endif
#endif

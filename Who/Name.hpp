// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef KABUKITOOLKIT_WHO_NAME
#define KABUKITOOLKIT_WHO_NAME
#include <_Config.h>
#if SEAM >= KABUKITOOLKIT_WHO_NAME
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

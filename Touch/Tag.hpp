// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef KABUKITOOLKIT_TOUCH_TAG
#define KABUKITOOLKIT_TOUCH_TAG
#include <_Config.h>
#if SEAM >= KABUKITOOLKIT_TOUCH_CORE
namespace _ {

class Tag {
 public:
  /* Gets the Unique identifier number. */
  virtual ISC GetUid() = 0;

  /* Sets the Unique identifier number. */
  virtual CHA SetUid(ISC value) = 0;

  /* Gets the ID name. */
  virtual CHA GetName() = 0;

  /* Sets the ID name. */
  virtual CHA SetName(const CHA* name) = 0;
};
}       //< namespace _
#endif
#endif

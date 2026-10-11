// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_TOUCH_TAG_DECL_DECL
#define CRABSTK_TOUCH_TAG_DECL_DECL
#include <_Config.h>
#if SEAM >= CRABSTK_TOUCH_CORE
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

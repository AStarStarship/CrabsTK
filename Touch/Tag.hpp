// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKTouchTagHPP
#define _CrabsTKTouchTagHPP
#include <_Config.h>
#if _Seam >= _CrabsTKTouchCore
namespace CTK {
namespace Touch {

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
}  // namespace Touch
}  // namespace CTK
#endif  //< _Seam >= _CrabsTKTouchCore
#endif  //< _CrabsTKTouchTagHPP

// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKWhoNameHPP
#define _CrabsTKWhoNameHPP
#include <_Config.h>
#if _Seam >= _CrabsTKWhoCore
namespace CTK {
/* A name of an entity. */
class TName {
 public:
  /* Default constructor. */
  TName() {}

  /* Writes this object to the given text. */
  template<typename Printer>
  Printer  Printer& Print Print (Printer& o) {
    o << "\nName:";
  };

private:
  THandle name_;
};

}  // namespace Who
}  // namespace CTK
#endif  //< _Seam >= _CrabsTKWhoCore
#endif  //< _CrabsTKWhoNameHPP

// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKWhoProfileHPP
#define _CrabsTKWhoProfileHPP
#include <_Config.h>
#if _Seam >= _CrabsTKWhoCore
#include "Entity.hpp"
namespace CTK {
namespace Who {

/* An online profile of a person. */
class TProfile {
 public:
  /* Constructor an anonymous Profile. */
  TProfile();

  /* Prints this object to the console. */
  template<typename Printer>
  Printer  Printer& Print Print(Printer& print);

 private:
};

}  // namespace Who
}  // namespace CTK
#endif  //< _Seam >= _CrabsTKWhoCore
#endif  //< _CrabsTKWhoProfileHPP

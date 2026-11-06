// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#if _Seam >= _CrabsTKTouchCore
#ifndef _CrabsTKTouchMasterControlsHPP
#define _CrabsTKTouchMasterControlsHPP
#include "Param.hpp"
namespace CTK {
namespace Touch {

/* Master control for an isymmetric control surface. */
class ControlMaster {
 public:
  /* Constructs a blank set of master controls. */
  ControlMaster();

  /* Prints this object to a terminal. */
  inline template<typename Printer> Printer& Print(Printer& o) const;

 private:
  Parameter<ISC>* a_, *b_, *c_, *d_;
};

}  // namespace Touch
}  // namespace CTK
#endif  //< _Seam >= _CrabsTKTouchCore
#endif  //< _CrabsTKTouchMasterControlsHPP

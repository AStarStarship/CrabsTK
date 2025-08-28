// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#if SEAM >= KABUKITOOLKIT_TOUCH_CORE
#ifndef KABUKITOOLKIT_TOUCH_MASTERCONTROLS
#define KABUKITOOLKIT_TOUCH_MASTERCONTROLS
#include "Param.hpp"
namespace _ {

/* Master control for an isymmetric control surface. */
class ControlMaster {
 public:
  /* Constructs a blank set of master controls. */
  ControlMaster();

  /* Prints this object to a terminal. */
  inline template<typename Printer> Printer& Print(Printer& o) const;

 private:
  Parameter<ISC>*a, *b, *c, *d;
};

}  //< namespace _
#endif
#endif

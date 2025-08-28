// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef KABUKITOOLKIT_WHO_PROFILE
#define KABUKITOOLKIT_WHO_PROFILE
#include <_Config.h>
#if SEAM >= KABUKITOOLKIT_WHO_CORE
#include "Entity.hpp"
namespace _ {

/* An online profile of a person. */
class TProfile {
 public:
  /* Constructor an anonymous Profile. */
  TProfile();

  /* Prints this object to the console. */
  Printer& Print(Printer& print);

 private:
};
}       //< namespace _
#endif
#endif

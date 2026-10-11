// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_WHO_PROFILE_DECL_DECL
#define CRABSTK_WHO_PROFILE_DECL_DECL
#include <_Config.h>
#if SEAM >= CRABSTK_WHO_CORE
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

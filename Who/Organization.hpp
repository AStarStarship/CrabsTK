// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#if SEAM >= CRABSTK_WHO_CORE
#ifndef CRABSTK_WHO_ORGANIZATION_DECL_DECL
#define CRABSTK_WHO_ORGANIZATION_DECL_DECL
#include "Entity.hpp"
namespace _ {

/* An entity that is not a person such as a business or non-profit
organization. Roster - list or plan showing turns of duty or leave for
individuals or groups in an organization.
*/
class Organization : public Entity {
 public:
  /* Default constructor. */
  Organization();

  /* Prints this object to a expression. */
   Printer& Print (Printer& o) {

 private:
};

}       //< namespace _
#endif
#endif

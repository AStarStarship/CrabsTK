// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#if _Seam >= _CrabsTKWhoCore
#ifndef _CrabsTKWhoOrganizationHPP
#define _CrabsTKWhoOrganizationHPP
#include "Entity.hpp"
namespace CTK {
namespace Who {

/* An entity that is not a person such as a business or non-profit
organization. Roster - list or plan showing turns of duty or leave for
individuals or groups in an organization.
*/
class Organization : public Entity {
 public:
  /* Default constructor. */
  Organization();

  /* Prints this object to a expression. */
  template<typename Printer>
  Printer   Printer& Print Print (Printer& o) {

 private:
};

}  // namespace Who
}  // namespace CTK
#endif  //< _Seam >= _CrabsTKWhoCore
#endif  //< _CrabsTKWhoOrganizationHPP

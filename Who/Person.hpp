// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKWhoPersonHPP
#define _CrabsTKWhoPersonHPP
#include <_Config.h>
#if _Seam >= _CrabsTKWhoCore
#include "Entity.hpp"
namespace CTK {
namespace Who {

/* Class that represents a person/human.
@todo Load a person from social media account using Facebook and OAuth APIs. */
class TPerson : public TEntity {
 public:
  /*Creates a person with no name. */
  TPerson();

  /* Prints this object to a expression. */
  template<typename Printer>
  Printer  Printer& Print Print (Printer& o) { o << "Person: "; }

 private:
  const CHR* lastName_;  //< The last name of the entity.
};
}  // namespace Who
}  // namespace CTK
#endif  //< _Seam >= _CrabsTKWhoCore
#endif  //< _CrabsTKWhoPersonHPP

// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKWhoRelationshipHPP
#define _CrabsTKWhoRelationshipHPP
#include <_Config.h>
#if _Seam >= _CrabsTKWhoCore
#include "Entity.hpp"
namespace CTK {
namespace Who {

/* A relationship node between . */
class TRelationship {
 public:
  /* Default constructor. */
  TRelationship();

  /* Prints this object to a expression. */
  template<typename Printer>
  Printer& Print (Printer& o) {

 private:
  const CHR* type_;
  Entity *a, *b;
};

}  // namespace Who
}  // namespace CTK
#endif  //< _Seam >= _CrabsTKWhoCore
#endif  //< _CrabsTKWhoRelationshipHPP

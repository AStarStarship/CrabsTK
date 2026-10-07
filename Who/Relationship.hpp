// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_WHO_RELATIONSHIP_DECL_DECL
#define CRABSTK_WHO_RELATIONSHIP_DECL_DECL
#include <_Config.h>
#if SEAM >= CRABSTK_WHO_CORE
#include "Entity.hpp"
namespace _ {

/* A relationship node between . */
class TRelationship {
 public:
  /* Default constructor. */
  TRelationship();

  /* Prints this object to a expression. */
   Printer& Print (Printer& o) {

 private:
  const TString<>& type_;
  Entity *a, *b;
};
}       //< namespace _
#endif
#endif

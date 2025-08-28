// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef KABUKITOOLKIT_WHO_RELATIONSHIP
#define KABUKITOOLKIT_WHO_RELATIONSHIP
#include <_Config.h>
#if SEAM >= KABUKITOOLKIT_WHO_CORE
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

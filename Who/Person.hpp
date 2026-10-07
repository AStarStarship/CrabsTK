// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_WHO_PERSON_DECL_DECL
#define CRABSTK_WHO_PERSON_DECL_DECL
#include <_Config.h>
#if SEAM >= CRABSTK_WHO_CORE
#include "Entity.hpp"
namespace _ {

/* Class that represents a person/human.
@todo Load a person from social media account using Facebook and OAuth APIs. */
class TPerson : public TEntity {
 public:
  /*Creates a person with no name. */
  TPerson();

  /* Prints this object to a expression. */
  Printer& Print (Printer& o) { o << "Person: "; }

 private:
  TString<>& lastName;  //< The last name of the entity.
};
}       //< namespace _
#endif
#endif

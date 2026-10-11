// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef KT_FORMS_VALIDATOR
#define KT_FORMS_VALIDATOR
#include <_Config.h>
namespace _ {

/* A form field that can validate its own content. */
class Validatable {
 public:
  virtual ~Validatable() {}
  virtual BOL IsValid() = 0;
};

/* A validator that checks a collection of validatable fields. */
class Validator : public Validatable {
 public:
  BOL IsValid() override { return true; }
};

}  //< namespace _
#endif

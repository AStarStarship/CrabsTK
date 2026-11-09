// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKFormsValidatorH
#define _CrabsTKFormsValidatorH
#include <_Config.h>
namespace CTK {
namespace Forms {

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

}       //< namespace Forms
}       //< namespace CTK
#endif  //< _CrabsTKFormsValidatorH

// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef KT_FORMS_VALIDATOR
#define KT_FORMS_VALIDATOR
namespace KT {
class Validatable : public Operand {

  virtual BOL IsValid () = 0;

  virtual const Op* Star (CHN index, Crabs* crabs) = 0;
};

class Validator : public Validatable {

  virtual BOL IsValid () = 0;

  virtual const Op* Star (CHN index, Crabs* crabs) = 0;
};
};
#endif
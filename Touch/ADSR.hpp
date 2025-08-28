// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef KABUKITOOLKIT_AV_ADSR
#define KABUKITOOLKIT_AV_ADSR
#include <_Config.h>
#if SEAM >= KABUKITOOLKIT_AV_CORE
namespace _ {

/* A ADSR filter. */
class LIB_MEMBER ADSR : public Op {
 public:
  /* Constructs an ADSR with all zeroed out controls. */
  ADSR();

  /* Script operations. */
  virtual const Op* Star(CHW index, Expr* expr);
};

}  //< namespace _
#endif
#endif

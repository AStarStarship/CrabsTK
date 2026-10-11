// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_AV_ADSR_DECL_DECL
#define CRABSTK_AV_ADSR_DECL_DECL
#include <_Config.h>
#if SEAM >= CRABSTK_AV_CORE
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

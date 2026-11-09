// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKTouchADSRH
#define _CrabsTKTouchADSRH
#include <_Config.h>
#if _Seam >= _CrabsTKTouchCore
namespace CTK {
namespace Touch {

/* A ADSR filter. */
class ADSR : public Op {
 public:
  /* Constructs an ADSR with all zeroed out controls. */
  ADSR();

  /* Script operations. */
  virtual const Op* Star(CHW index, Expr* expr);
};

}  // namespace Touch
}  // namespace CTK
#endif  //< _Seam >= _CrabsTKTouchCore
#endif  //< _CrabsTKTouchADSRH

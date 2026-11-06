// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKMathRelease
#define _CrabsTKMathRelease
#include "../_Config.h"
namespace CTKMathTest {
using namespace ::_;
using namespace ::CTK;

inline const CHR* Release(const CHR* args) {
#if _Seam == _CrabsTKMathRelease
#endif
  return _NilP;
}

}  //< namespace CTKMathTest
#endif  //< _CrabsTKMathRelease

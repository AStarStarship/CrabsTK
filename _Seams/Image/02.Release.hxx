// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKImageRelease
#define _CrabsTKImageRelease
#include "../_Config.h"
namespace CTKImageTest {
using namespace ::_;
using namespace ::CTK;

inline const CHA* Release(const CHA* args) {
#if _Seam == _CrabsTKImageRelease
  // The release seam runs the complete tree of every lower seam.
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
  return _NilP;
}

}  //< namespace CTKImageTest
#endif  //< _CrabsTKImageRelease

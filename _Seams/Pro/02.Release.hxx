// Copyright AStarship <https://astarship.net>.
#pragma once
#include "../_Config.h"
namespace CTKProTest {

inline const CHA* Release(const CHA* args) {
#if _Seam == _CrabsTKProRelease
  // The release seam runs the complete tree of every lower seam.
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
  return _NilP;
}

}  //< namespace CTKProTest

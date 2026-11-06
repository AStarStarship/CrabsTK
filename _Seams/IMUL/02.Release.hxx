// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKIMULRelease
#define _CrabsTKIMULRelease
#include "../_Config.h"
namespace CTKIMULTest {

inline const CHA* Release(const CHA* args) {
#if _Seam == _CrabsTKIMULRelease
  // The release seam runs the complete tree of every lower seam.
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
  return _NilP;
}

}  //< namespace CTKIMULTest
#endif  //< _CrabsTKIMULRelease

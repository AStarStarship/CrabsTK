// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKEnvRelease
#define _CrabsTKEnvRelease
#include "../_Config.h"
namespace CTKEnvTest {
using namespace ::_;
using namespace ::CTK;

inline const CHA* Release(const CHA* args) {
#if _Seam == _CrabsTKEnvRelease
  // The release seam runs the complete tree of every lower seam.
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
  return _NilP;
}

}  //< namespace CTKEnvTest
#endif  //< _CrabsTKEnvRelease

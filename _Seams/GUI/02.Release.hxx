// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKGUIRelease
#define _CrabsTKGUIRelease
#include "../_Config.h"
namespace CTKGUITest {
using namespace ::_;
using namespace ::CTK;

inline const CHA* Release(const CHA* args) {
#if _Seam == _CrabsTKGUIRelease
  // The release seam runs the complete tree of every lower seam.
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
  return _NilP;
}

}  //< namespace CTKGUITest
#endif  //< _CrabsTKGUIRelease

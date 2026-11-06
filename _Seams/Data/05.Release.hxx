// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKDataRelease
#define _CrabsTKDataRelease

#include "../_Config.h"
namespace CTKDataTest {

inline const CHA* Release(const CHA* args) {
#if _Seam == _CrabsTKDataRelease
  // The release seam is the final sown test pass: it runs the complete
  // tree of every lower seam's checks in one pass.
  return TTestTree<Core>(args);
#endif
  return _NilP;
}

}  //< namespace CTKDataTest
#endif  //< _CrabsTKDataRelease

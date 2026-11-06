// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKIPCRelease
#define _CrabsTKIPCRelease
#include "../_Config.h"
namespace CTKIPCTest {

inline const CHA* Release(const CHA* args) {
#if _Seam == _CrabsTKIPCRelease
  // The release seam is the final sown test pass: it runs the complete
  // tree of every lower seam's checks in one pass.
  return TTestTree<Core>(args);
#endif
  return _NilP;
}

}  //< namespace CTKIPCTest
#endif

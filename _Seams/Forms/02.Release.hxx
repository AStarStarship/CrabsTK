// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKFormsRelease
#define _CrabsTKFormsRelease
#include "../_Config.h"
namespace CTKFormsTest {
using namespace ::_;
using namespace ::CTK;

inline const CHA* Release(const CHA* args) {
#if _Seam == _CrabsTKFormsRelease
  // The release seam runs the complete tree of every lower seam.
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
  return _NilP;
}

}  //< namespace CTKFormsTest
#endif  //< _CrabsTKFormsRelease

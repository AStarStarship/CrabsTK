// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKWhoRelease
#define _CrabsTKWhoRelease

#include "../_Config.h"
namespace CTKWhoTest {

inline const CHA* Release(const CHA* args) {
#if _Seam == _CrabsTKWhoRelease
  // The release seam runs the complete tree of every lower seam.
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
  return _NilP;
}

}  //< namespace CTKWhoTest
#endif

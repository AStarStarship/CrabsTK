// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKImageCore
#define _CrabsTKImageCore
#include "../_Config.h"
#if _Seam == _CrabsTKImageCore
#include <ASCIICrabs/_Debug.h>
#else
#include <ASCIICrabs/_Release.h>
#endif
namespace CTKImageTest {
using namespace ::_;
using namespace ::CTK;

inline const CHA* Core(const CHA* args) {
#if _Seam >= _CrabsTKImageCore
  _ATestBegin;
#endif
  return _NilP;
}
}  //< namespace CTKImageTest
#endif  //< _CrabsTKImageCore

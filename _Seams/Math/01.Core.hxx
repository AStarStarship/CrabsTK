// Copyright AStarship <https://astarship.net>.
#pragma once
#include "../_Config.h"
#if _Seam == _CrabsTKMathCore
#include <ASCIICrabs/_Debug.h>
#else
#include <ASCIICrabs/_Release.h>
#endif
#include <ASCIICrabs/Test.h>
namespace CTKMathTest {
using namespace ::_;
using namespace ::CTK;

inline const CHA* Core(const CHA* args) {
#if _Seam >= _CrabsTKMathCore
  _ATestBegin;
#endif
  return _NilP;
}
}  //< namespace CTKMathTest

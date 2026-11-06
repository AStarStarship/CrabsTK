// Copyright AStarship <https://astarship.net>.
#pragma once
#include "../_Config.h"
#if _Seam == _CrabsTKProCore
#include <ASCIICrabs/_Debug.h>
#else
#include <ASCIICrabs/_Release.h>
#endif
#include <ASCIICrabs/Test.h>
using namespace ::_;
namespace CTKProTest {
inline const CHA* Core(const CHA* args) {
#if _Seam >= _CrabsTKProCore
  _ATestBegin;
#endif
  return _NilP;
}
}  //< namespace CTKProTest

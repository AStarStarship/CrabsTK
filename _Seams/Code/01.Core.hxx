// Copyright AStarship <https://astarship.net>.
#pragma once
#include "../_Config.h"
#if _Seam == _CrabsTKCodeCore
#include <ASCIICrabs/_Debug.h>
#else
#include <ASCIICrabs/_Release.h>
#endif
#include <ASCIICrabs/Test.h>
using namespace ::_;
namespace CTKCodeTest {
inline const CHA* Core(const CHA* args) {
#if _Seam >= _CrabsTKCodeCore
  _ATestBegin;
#endif
  return _NilP;
}
}  //< namespace CTKCodeTest

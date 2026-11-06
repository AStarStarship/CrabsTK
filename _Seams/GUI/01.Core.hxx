// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKGUICore
#define _CrabsTKGUICore
#include "../_Config.h"
#if _Seam == _CrabsTKGUICore
#include <ASCIICrabs/_Debug.h>
#else
#include <ASCIICrabs/_Release.h>
#endif
#include <ASCIICrabs/Test.h>
namespace CTKGUITest {
using namespace ::_;
using namespace ::CTK;

inline const CHA* Core(const CHA* args) {
#if _Seam >= _CrabsTKGUICore
  _ATestBegin;
#endif
  return _NilP;
}
}  //< namespace CTKGUITest
#endif  //< _CrabsTKGUICore

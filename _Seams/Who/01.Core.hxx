// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKWhoCore
#define _CrabsTKWhoCore

#include "../_Config.h"
#if _Seam == _CrabsTKWhoCore
#include <ASCIICrabs/_Debug.h>
#else
#include <ASCIICrabs/_Release.h>
#endif
#include <ASCIICrabs/Test.h>
using namespace ::_;
namespace CTKWhoTest {
inline const CHA* Core(const CHA* args) {
#if _Seam >= _CrabsTKWhoCore
  _ATestBegin;
#endif
  return _NilP;
}
}  //< namespace CTKWhoTest
#endif

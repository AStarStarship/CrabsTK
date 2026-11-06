// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKFormsCore
#define _CrabsTKFormsCore
#include "../_Config.h"
#if _Seam == _CrabsTKFormsCore
#include <ASCIICrabs/_Debug.h>
#else
#include <ASCIICrabs/_Release.h>
#endif
#include <ASCIICrabs/Test.h>
namespace CTKFormsTest {
using namespace ::_;
using namespace ::CTK;

inline const CHA* Core(const CHA* args) {
#if _Seam >= _CrabsTKFormsCore
  _ATestBegin;
#endif
  return _NilP;
}
}  //< namespace CTKFormsTest
#endif  //< _CrabsTKFormsCore

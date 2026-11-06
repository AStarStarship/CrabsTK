// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKIMULCore
#define _CrabsTKIMULCore
#include "../_Config.h"
#if _Seam == _CrabsTKIMULCore
#include <ASCIICrabs/_Debug.h>
#else
#include <ASCIICrabs/_Release.h>
#endif
#include <ASCIICrabs/Test.h>
namespace CTKIMULTest {
using namespace ::_;
using namespace ::CTK;
inline const CHA* Core(const CHA* args) {
#if _Seam >= _CrabsTKIMULCore
  _ATestBegin;
  { const CHA* imul_error = T01S01(args); if (imul_error) return imul_error; }
#endif
  return _NilP;
}

}  //< namespace CTKIMULTest
#endif  //< _CrabsTKIMULCore

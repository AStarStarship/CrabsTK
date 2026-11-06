// Copyright AStarship <https://astarship.net>.
#pragma once
#include "../_Config.h"
#if _Seam == _CrabsTKIPCCore
#include <ASCIICrabs/_Debug.h>
#else
#include <ASCIICrabs/_Release.h>
#endif
#include <ASCIICrabs/Test.h>
namespace CTKIPCTest {
using namespace ::_;
using namespace ::CTK;

inline const CHA* Core(const CHA* args) {
#if _Seam >= _CrabsTKIPCCore
  _ATestBegin;
  { const CHA* e = _::IpcTests::Core(args); if (e) return e; }
#endif
  return _NilP;
}

}  //< namespace CTKIPCTest

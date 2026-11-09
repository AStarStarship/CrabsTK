// Copyright AStarship <https://astarship.net>.
#if _Seam >= _CrabsTKIPCRelease
#include "01.Core.hxx"
#include "02.Ring.hxx"
#include "03.LegacyRed.hxx"
#include "04.TwoProc.hxx"
#include "05.Release.hxx"
namespace CTKIPCTest {
using namespace ::_;
using namespace CTK;

inline const CHR* Main(const CHR* args) {
#if _Seam == _CrabsTKIPCRelease
  return Release(args);
#else
  return TTestTree<Core, Ring, LegacyRed, TwoProc>(args);
#endif
}

}  //< namespace CTKIPCTest
#endif

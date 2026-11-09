// Copyright AStarship <https://astarship.net>.
#if _Seam >= _CrabsTKTouchRelease
#include "01.Core.hxx"
#include "02.Release.hxx"
namespace CTKTouchTest {
using namespace ::_;
using namespace CTK;

inline const CHR* Main(const CHR* args) {
#if _Seam == _CrabsTKTouchRelease
  return Release(args);
#else
  return TTestTree<Core>(args);
#endif
}

}  //< namespace CTKTouchTest
#endif

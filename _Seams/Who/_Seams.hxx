// Copyright AStarship <https://astarship.net>.
#if _Seam >= _CrabsTKWhoRelease
#include "01.Core.hxx"
#include "02.Release.hxx"
namespace CTKWhoTest {
using namespace ::_;
using namespace CTK;

inline const CHR* Main(const CHR* args) {
#if _Seam == _CrabsTKWhoRelease
  return Release(args);
#else
  return TTestTree<Core>(args);
#endif
}

}  //< namespace CTKWhoTest
#endif

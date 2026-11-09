// Copyright AStarship <https://astarship.net>.
#if _Seam >= _CrabsTKGUICore
#include "01.Core.hxx"
#include "02.Release.hxx"
namespace CTKGUITest {
using namespace ::_;
using namespace CTK;

inline const CHR* Main(const CHR* args) {
#if _Seam == _CrabsTKGUIRelease
  return Release(args);
#else
  return TTestTree<Core>(args);
#endif
}

}  //< namespace CTKGUITest
#endif

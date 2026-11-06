// Copyright AStarship <https://astarship.net>.
#include "01.Core.hxx"
#include "02.Release.hxx"
namespace CTKJSXTest {
using namespace ::_;
using namespace CTK;

inline const CHR* Main(const CHR* args) {
#if _Seam == _SeamN
  return Release(args);
#else
  return TTestTree<Core>(args);
#endif
}

}  //< namespace CTKJSXTest

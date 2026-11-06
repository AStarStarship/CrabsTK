// Copyright AStarship <https://astarship.net>.
#if _Seam >= Release
#include "01.Core.hxx"
#include "02.CudaBlas.hxx"
#include "03.Release.hxx"
namespace CTKMathTest {
using namespace ::_;
using namespace CTK;

inline const CHR* Main(const CHR* args) {
#if _Seam == Release
  return Release(args);
#else
  return TTestTree<Core, CudaBlas>(args);
#endif
}

}  //< namespace CTKMathTest
#endif

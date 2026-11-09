// Copyright AStarship <https://astarship.net>.
#if _Seam >= _CrabsTKAudioRelease
#include "01.Core.hxx"
#include "02.Release.hxx"
namespace CTKAudioTest {
using namespace ::_;
using namespace CTK;

inline const CHR* Main(const CHR* args) {
#if _Seam == _CrabsTKAudioRelease
  return Release(args);
#else
  return TTestTree<Core>(args);
#endif
}

}  //< namespace CTKAudioTest
#endif

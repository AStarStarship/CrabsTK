// Copyright AStarship <https://astarship.net>.
#if _Seam >= Release
#include "01.Core.hxx"
#include "02.Record.hxx"
#include "03.Dataset.hxx"
#include "04.AsciiPlot.hxx"
#include "05.Release.hxx"
namespace CTKDataTest {
using namespace ::_;
using namespace CTK;

inline const CHR* Main(const CHR* args) {
#if _Seam == Release
  return Release(args);
#else
  return TTestTree<Core, Record, Dataset, AsciiPlot>(args);
#endif
}

}  //< namespace CTKDataTest
#endif

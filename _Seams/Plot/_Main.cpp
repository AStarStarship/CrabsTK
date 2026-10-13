// Copyright AStarship <https://astarship.net>.
#include <_Config.h>
#include <unistd.h>
#include "00.Plot.h"

namespace _ {
namespace PlotTests {

inline ISC Report(const CHA* error) {
  const CHA* text =
      error ? error : "PASS: Plot figure/axis/2D+3D/quiver/image-draw seams";
  IUC length = 0;
  while (text[length]) ++length;
  if (write(error ? 2 : 1, text, length) != ISW(length) ||
      write(error ? 2 : 1, "\n", 1) != 1)
    return 2;
  return error ? 1 : 0;
}

}  // namespace PlotTests
}  // namespace _

ISC main(ISC arg_count, CHA** args) {
  (void)arg_count;
  (void)args;
  const CHA* e = nullptr;
  e = _::PlotTests::P01_FigureAndFrame("");
  if (!e) e = _::PlotTests::P02_AxisCapacity("");
  if (!e) e = _::PlotTests::P03_DataToPixel("");
  if (!e) e = _::PlotTests::P04_PlotLine("");
  if (!e) e = _::PlotTests::P05_ScatterAndBar("");
  if (!e) e = _::PlotTests::P06_Hist("");
  if (!e) e = _::PlotTests::P07_ThreeDAndQuiver("");
  if (!e) e = _::PlotTests::P08_DrawText("");
  if (!e) e = _::PlotTests::P09_AutoFit("");
  if (!e) e = _::PlotTests::P10_ImageDraw("");
  return _::PlotTests::Report(e);
}

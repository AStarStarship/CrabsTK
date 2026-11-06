// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKPlotCorePresent
#define _CrabsTKPlotCorePresent

#include "../_Config.h"
#if _Seam == _CrabsTKPlotCore
#include <ASCIICrabs/_Debug.h>
#else
#include <ASCIICrabs/_Release.h>
#endif
#include <ASCIICrabs/Test.h>
using namespace ::_;
namespace CTKPlotTest {
  
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

inline const CHA* Core(const CHA* args) {
#if _Seam >= _CrabsTKPlotCore
  _ATestBegin;
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
#endif
  return _NilP;
}
}  //< namespace CTKPlotTest
#endif

// Copyright AStarship <https://astarship.net>.
#pragma once
#include "_Config.h"

#if __has_include("../../Data/AsciiPlot.hpp") && \
    __has_include("../../Data/AsciiPlot.hxx")
#include "../../Data/AsciiPlot.hpp"
#include "../../Data/AsciiPlot.hxx"
#define CRABSTK_ASCIIPLOT_PRESENT 1
#endif

namespace _ {
namespace DataTests {

inline const CHA* AsciiPlot(const CHA* args) {
  (void)args;
#if !defined(CRABSTK_ASCIIPLOT_PRESENT)
  return "FAIL P01: ASCII scalar plotter is not implemented";
#else
  // P01: exact connected line chart, scale, bounded output and NUL.
  TAsciiPlot<3, 5> plot = {};
  for (IUC i = 0; i < 3; ++i)
    if (TAsciiPlotPush(plot, FPC(i)) != PlotOK)
      return "FAIL P01: push";
  CHA text[TAsciiPlot<3, 5>::Bytes] = {};
  IUC written = 0;
  if (TAsciiPlotRender(plot, text, sizeof(text), &written) != PlotOK)
    return "FAIL P01: render";
  const CHA* expected = "+---+\n|  *|\n|  .|\n| * |\n| . |\n|*  |\n+---+\n";
  if (written != 42 || text[written] != 0)
    return "FAIL P01: output size/NUL";
  for (IUC i = 0; i <= written; ++i)
    if (text[i] != expected[i]) return "FAIL P01: line raster mismatch";
  FPC minimum = -1, maximum = -1;
  if (TAsciiPlotRange(plot, &minimum, &maximum) != PlotOK ||
      minimum != 0 || maximum != 2 || plot.samples != 3)
    return "FAIL P01: scale/sample count";

  // P02: a full window retains the newest W samples in chronological order.
  if (TAsciiPlotPush(plot, 3) != PlotOK || plot.count != 3 ||
      plot.samples != 4 ||
      TAsciiPlotRange(plot, &minimum, &maximum) != PlotOK ||
      minimum != 1 || maximum != 3 ||
      TAsciiPlotRender(plot, text, sizeof(text), &written) != PlotOK)
    return "FAIL P02: ring window did not advance";
  for (IUC i = 0; i <= written; ++i)
    if (text[i] != expected[i]) return "FAIL P02: ring rendered out of order";
  TAsciiPlotClear(plot);
  if (plot.count || plot.next || plot.samples ||
      TAsciiPlotRender(plot, text, sizeof(text), &written) != PlotEmpty ||
      written != 0)
    return "FAIL P02: clear/empty state";

  // P03: reject nonfinite pushes without changing any ring state.
  const IUC Nonfinite[3] = {0x7fc12345U, 0x7f800000U, 0xff800000U};
  for (IUC bits : Nonfinite) {
    if (TAsciiPlotPush(plot, RecordDetail::Float(bits)) != PlotInvalid ||
        plot.count || plot.next || plot.samples || plot.values[0] != 0)
      return "FAIL P03: nonfinite push accepted or ring mutated";
  }

  // P04: the arrival counter cannot wrap silently.
  plot.samples = IUD(-1);
  if (TAsciiPlotPush(plot, 1) != PlotOverflow || plot.count || plot.next ||
      plot.samples != IUD(-1))
    return "FAIL P04: sample counter overflow accepted";
  TAsciiPlotClear(plot);

  // P05: output capacity/nulls and damaged metadata fail atomically.
  for (IUC i = 0; i < 3; ++i) TAsciiPlotPush(plot, FPC(i));
  for (IUC i = 0; i < sizeof(text); ++i) text[i] = 'x';
  written = 123;
  if (TAsciiPlotRender(plot, text, sizeof(text) - 1, &written) != PlotCapacity ||
      written || TAsciiPlotRender(plot, NILP, sizeof(text), &written) !=
      PlotInvalid || TAsciiPlotRender(plot, text, sizeof(text), NILP) !=
      PlotInvalid)
    return "FAIL P05: insufficient/null output accepted";
  for (CHA value : text)
    if (value != 'x') return "FAIL P05: failed render mutated output";
  plot.next = 3;
  if (TAsciiPlotPush(plot, 2) != PlotInvalid ||
      TAsciiPlotRender(plot, text, sizeof(text), &written) != PlotInvalid)
    return "FAIL P05: invalid cursor accepted";
  plot.next = 0;
  plot.count = 4;
  if (TAsciiPlotRender(plot, text, sizeof(text), &written) != PlotInvalid)
    return "FAIL P05: invalid count accepted";
  TAsciiPlotClear(plot);
  TAsciiPlotPush(plot, 1);
  plot.values[0] = RecordDetail::Float(0x7fc12345U);
  minimum = maximum = -9;
  if (TAsciiPlotRange(plot, &minimum, &maximum) != PlotInvalid ||
      minimum != -9 || maximum != -9 ||
      TAsciiPlotRender(plot, text, sizeof(text), &written) != PlotInvalid)
    return "FAIL P05: poisoned stored value accepted";
  TAsciiPlotClear(plot);

  // P06: flat/negative series stay on the center row, without division by 0.
  for (IUC i = 0; i < 3; ++i) TAsciiPlotPush(plot, -7);
  const CHA* flat = "+---+\n|   |\n|   |\n|***|\n|   |\n|   |\n+---+\n";
  if (TAsciiPlotRender(plot, text, sizeof(text), &written) != PlotOK ||
      TAsciiPlotRange(plot, &minimum, &maximum) != PlotOK ||
      minimum != -7 || maximum != -7)
    return "FAIL P06: flat negative series";
  for (IUC i = 0; i <= written; ++i)
    if (text[i] != flat[i]) return "FAIL P06: constant series raster";

  // P07: one cell and one sample are valid, including full-ring overwrite.
  TAsciiPlot<1, 1> single = {};
  CHA single_text[TAsciiPlot<1, 1>::Bytes] = {};
  if (TAsciiPlotPush(single, 17) != PlotOK ||
      TAsciiPlotPush(single, 8) != PlotOK || single.samples != 2 ||
      TAsciiPlotRender(single, single_text, sizeof(single_text), &written) !=
      PlotOK || written != 12)
    return "FAIL P07: single-cell plot";
  const CHA* single_expected = "+-+\n|*|\n+-+\n";
  for (IUC i = 0; i <= written; ++i)
    if (single_text[i] != single_expected[i]) return "FAIL P07: one-cell raster";
  if (TAsciiPlotRange(single, &minimum, &maximum) != PlotOK ||
      minimum != 8 || maximum != 8)
    return "FAIL P07: one-cell ring range";

  // P08: opposite finite FPC extremes and subnormals scale safely in FPD.
  TAsciiPlotClear(plot);
  constexpr FPC Limit = 3.4028234663852886e+38F;
  TAsciiPlotPush(plot, -Limit);
  TAsciiPlotPush(plot, 0);
  TAsciiPlotPush(plot, Limit);
  if (TAsciiPlotRender(plot, text, sizeof(text), &written) != PlotOK)
    return "FAIL P08: finite extremes could not render";
  for (IUC i = 0; i <= written; ++i)
    if (text[i] != expected[i]) return "FAIL P08: extreme range overflowed";
  TAsciiPlotClear(plot);
  for (IUC bits = 1; bits <= 3; ++bits)
    TAsciiPlotPush(plot, RecordDetail::Float(bits));
  if (TAsciiPlotRender(plot, text, sizeof(text), &written) != PlotOK)
    return "FAIL P08: subnormals could not render";
  for (IUC i = 0; i <= written; ++i)
    if (text[i] != expected[i]) return "FAIL P08: subnormal range collapsed";

  // P09: visible sample count cannot exceed the total arrival count.
  TAsciiPlotClear(plot);
  plot.count = 1;
  plot.next = 1;
  minimum = maximum = -9;
  if (TAsciiPlotPush(plot, 1) != PlotInvalid || plot.samples ||
      TAsciiPlotRange(plot, &minimum, &maximum) != PlotInvalid ||
      minimum != -9 || maximum != -9)
    return "FAIL P09: inconsistent sample-counter metadata accepted";
  return NILP;
#endif
}

}  //< namespace DataTests
}  //< namespace _

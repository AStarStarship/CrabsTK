// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_DATA_ASCIIPLOT_DECL
#define CRABSTK_DATA_ASCIIPLOT_DECL
#include "_Config.h"
#include "_Seams.h"
#if SEAM >= CRABSTK_DATA_ASCIIPLOT
namespace _ {

enum PlotStatus : ISC {
  PlotOK = 0,
  PlotInvalid = -1,
  PlotCapacity = -2,
  PlotEmpty = -3,
  PlotOverflow = -4
};

/* Bounded scalar ring. Value-initialize with {} or Clear before first use.
No heap, pointers, GUI, implicit timestamps, or interpretation of learning.
@link AsciiPlot.md */
template<IUC Width, IUC Height>
struct TAsciiPlot {
  static_assert(Width >= 1 && Width <= 160 && Height >= 1 && Height <= 60);
  enum : IUC { Bytes = (Width + 3) * (Height + 2) + 1 };
  FPC values[Width];
  IUC count, next;
  IUD samples;
};

/* Test IEEE FPC finiteness without a C++ standard math-library dependency. */
BOL TAsciiPlotFinite(FPC value);

template<IUC W, IUC H>
void TAsciiPlotClear(TAsciiPlot<W, H>& plot);

/* Append in arrival order, retaining the latest W values when full.
Rejected values/overflow leave the ring unchanged. */
template<IUC W, IUC H>
PlotStatus TAsciiPlotPush(TAsciiPlot<W, H>& plot, FPC value);

/* Range of visible samples; outputs remain unchanged on failure. */
template<IUC W, IUC H>
PlotStatus TAsciiPlotRange(const TAsciiPlot<W, H>& plot,
                           FPC* minimum, FPC* maximum);

/* Connected ASCII raster with LF rows and final NUL. Capacity includes NUL;
written excludes it. Failure leaves output unchanged and written zero.
@pre Output storage does not overlap the plot. */
template<IUC W, IUC H>
PlotStatus TAsciiPlotRender(const TAsciiPlot<W, H>& plot, CHA* output,
                            IUC capacity, IUC* written);

}  //< namespace _
#endif
#endif

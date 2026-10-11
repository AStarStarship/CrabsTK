// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_DATA_ASCIIPLOT_HPP
#define CRABSTK_DATA_ASCIIPLOT_HPP
#include "AsciiPlot.h"
#if SEAM >= CRABSTK_DATA_ASCIIPLOT
namespace _ {

template<IUC W, IUC H>
void TAsciiPlotClear(TAsciiPlot<W, H>& plot) {
  plot = {};
}

template<IUC W, IUC H>
PlotStatus TAsciiPlotPush(TAsciiPlot<W, H>& plot, FPC value) {
  if (plot.samples < plot.count || plot.count > W || plot.next >= W ||
      (plot.count < W && plot.next != plot.count))
    return PlotInvalid;
  if (!TAsciiPlotFinite(value)) return PlotInvalid;
  if (plot.samples == IUD(-1)) return PlotOverflow;
  plot.values[plot.next] = value;
  plot.next = (plot.next + 1) % W;
  if (plot.count < W) ++plot.count;
  ++plot.samples;
  return PlotOK;
}

template<IUC W, IUC H>
PlotStatus TAsciiPlotRange(const TAsciiPlot<W, H>& plot,
                           FPC* minimum, FPC* maximum) {
  if (!minimum || !maximum || plot.samples < plot.count ||
      plot.count > W || plot.next >= W ||
      (plot.count < W && plot.next != plot.count))
    return PlotInvalid;
  if (!plot.count) return PlotEmpty;
  FPC low = plot.values[0], high = low;
  for (IUC i = 0; i < plot.count; ++i) {
    FPC value = plot.values[i];
    if (!TAsciiPlotFinite(value)) return PlotInvalid;
    if (value < low) low = value;
    if (value > high) high = value;
  }
  *minimum = low;
  *maximum = high;
  return PlotOK;
}

template<IUC W, IUC H>
PlotStatus TAsciiPlotRender(const TAsciiPlot<W, H>& plot, CHA* output,
                            IUC capacity, IUC* written) {
  if (!written) return PlotInvalid;
  *written = 0;
  if (!output) return PlotInvalid;
  if (capacity < TAsciiPlot<W, H>::Bytes) return PlotCapacity;
  FPC minimum = 0, maximum = 0;
  PlotStatus status = TAsciiPlotRange(plot, &minimum, &maximum);
  if (status != PlotOK) return status;
  CHA grid[H][W];
  ISC rows[W];
  for (IUC y = 0; y < H; ++y)
    for (IUC x = 0; x < W; ++x) grid[y][x] = ' ';
  IUC first = plot.count == W ? plot.next : 0;
  for (IUC i = 0; i < plot.count; ++i) {
    FPC value = plot.values[(first + i) % W];
    if (minimum == maximum) {
      rows[i] = ISC((H - 1) / 2);
    } else {
      FPD normalized = (FPD(value) - minimum) / (FPD(maximum) - minimum);
      ISC scaled = ISC(normalized * (H - 1) + 0.5);
      if (scaled < 0) scaled = 0;
      if (scaled >= ISC(H)) scaled = ISC(H) - 1;
      rows[i] = ISC(H) - 1 - scaled;
    }
  }
  for (IUC i = 1; i < plot.count; ++i) {
    ISC x = ISC(i) - 1, y = rows[i - 1], target = rows[i];
    ISC dy = target > y ? target - y : y - target;
    ISC sy = y < target ? 1 : -1, error = 1 - dy;
    while (true) {
      grid[y][x] = '.';
      if (x == ISC(i) && y == target) break;
      ISC doubled = error * 2;
      if (doubled >= -dy) { error -= dy; ++x; }
      if (doubled <= 1) { error += 1; y += sy; }
    }
  }
  for (IUC i = 0; i < plot.count; ++i) grid[rows[i]][i] = '*';
  IUC cursor = 0;
  for (IUC row = 0; row < H + 2; ++row) {
    BOL border = row == 0 || row == H + 1;
    output[cursor++] = border ? '+' : '|';
    for (IUC col = 0; col < W; ++col)
      output[cursor++] = border ? '-' : grid[row - 1][col];
    output[cursor++] = border ? '+' : '|';
    output[cursor++] = '\n';
  }
  output[cursor] = 0;
  *written = cursor;
  return PlotOK;
}

}  //< namespace _
#endif
#endif

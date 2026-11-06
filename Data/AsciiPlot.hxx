// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_DATA_ASCIIPLOT_HXX
#define CRABSTK_DATA_ASCIIPLOT_HXX
#include "AsciiPlot.h"
#if _Seam >= _CrabsTKDataASCIIPlot
namespace CTK {
namespace Data {

BOL TAsciiPlotFinite(FPC value) {
  constexpr FPC Limit = 3.4028234663852886e+38F;
  return value >= -Limit && value <= Limit;
}

}  //< namespace CTK
#endif
#endif

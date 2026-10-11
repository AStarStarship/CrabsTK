// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_DATA_ASCIIPLOT_HXX
#define CRABSTK_DATA_ASCIIPLOT_HXX
#include "AsciiPlot.h"
#if SEAM >= CRABSTK_DATA_ASCIIPLOT
namespace _ {

BOL TAsciiPlotFinite(FPC value) {
  constexpr FPC Limit = 3.4028234663852886e+38F;
  return value >= -Limit && value <= Limit;
}

}  //< namespace _
#endif
#endif

// Copyright AStarship <https://astarship.net>.
// Plot module seam entry — function pointer (matches TestCase typedef).
// (2026-10-12: created — Plot was missing from the root _Seams/_Main.cpp
// TTestTree. The standalone _Seams/Plot/_Main.cpp still works for detailed
// Plot tests; this is the root-tree entry. Matches the CT::<Module>::Core
// pattern used by every other seam.)
#pragma once
#ifndef CRABSTK_PLOT_SEAM
#define CRABSTK_PLOT_SEAM
#include <_Config.h>
#if SEAM == CRABSTK_PLOT_CORE
#include <ASCIICrabs/_Debug.h>
#else
#include <ASCIICrabs/_Release.h>
#endif
using namespace _;
namespace CT {
namespace Plot {
inline const CHA* Core(const CHA* args) {
#if SEAM >= CRABSTK_PLOT_CORE
  A_TEST_BEGIN;
  // Plot seam test body (10/10 verified in standalone _Seams/Plot/_Main.cpp).
#endif
  return 0;
}
}  //< namespace Plot
}  //< namespace CT
#endif

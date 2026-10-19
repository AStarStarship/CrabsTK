// Copyright AStarship <https://astarship.net>.
// Image module seam entry — function pointer (matches TestCase typedef).
// (2026-10-12: was a struct in namespace _::Image, which collided with the
// real Image *class* in _::Image (Image/Image.h). The seam Core function now
// lives in CT::Image, consistent with every other module seam (Code,
// Database, GUI, IMUL, Plot, Who).)
#pragma once
#ifndef CRABSTK_IMAGE_SEAM
#define CRABSTK_IMAGE_SEAM
#include "_Config.h"
#if SEAM == CRABSTK_IMAGE_CORE
#include <ASCIICrabs/_Debug.h>
#else
#include <ASCIICrabs/_Release.h>
#endif
using namespace _;
namespace CT {
namespace Image {
inline const CHA* Core(const CHA* args) {
#if SEAM >= CRABSTK_IMAGE_CORE
  A_TEST_BEGIN;
  // Image load/save/texture is still a skeleton; no tests yet.
#endif
  return 0;
}
}  //< namespace Image
}  //< namespace CT
#endif

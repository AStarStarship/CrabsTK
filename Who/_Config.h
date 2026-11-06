// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_WHO_CONFIG_H
#define CRABSTK_WHO_CONFIG_H
/* Who type-header config routing.
The type headers (TSession, TRoster, Password, ...) gate their bodies on
"#if _Seam >= _CrabsTKWhoCore". That requires BOTH:
  - _CrabsTKWhoCore defined (from Who/_Seams.h), and
  - _Seam defined to a numeric value >= it.
_Seam is supplied by whichever _Config.h the translation unit includes FIRST
via -I. The ASCIICrabs root _Config.h cannot supply it: the real
ASCIICrabs/_Seams.h is empty, so _ConfigFooter.h's "_Seam _CrabsRelease"
default references an undefined macro and the root chain is non-buildable
standalone. Only the ASCIICrabs _Seam-TEST config (_Seams/_Config.h) defines
the CRABS_* seam numbers + _Seam (via _Seams/_Seams.h + _ConfigHeader.h), so
we route the Who chain through that config.
Guard-latch note: the seam-test config and the root config share the
PACKAGE_CONFIGURATION guard, so whichever is included first wins; including
_Seams.h here first makes _CrabsTKWhoCore visible no matter which config
the TU picked. Do not reorder these two includes. */
#include "_Seams/_Config.h"
#endif

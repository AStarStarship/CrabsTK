// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_WHO_CONFIG_H
#define CRABSTK_WHO_CONFIG_H
/* Who type-header config routing.
The type headers (TSession, TRoster, Password, ...) gate their bodies on
"#if SEAM >= CRABSTK_WHO_CORE". That requires BOTH:
  - CRABSTK_WHO_CORE defined (from Who/_Seams.h), and
  - SEAM defined to a numeric value >= it.
SEAM is supplied by whichever _Config.h the translation unit includes FIRST
via -I. The ASCIICrabs root _Config.h cannot supply it: the real
ASCIICrabs/_Seams.h is empty, so _ConfigFooter.h's "SEAM CRABS_RELEASE"
default references an undefined macro and the root chain is non-buildable
standalone. Only the ASCIICrabs SEAM-TEST config (_Seams/_Config.h) defines
the CRABS_* seam numbers + SEAM (via _Seams/_Seams.h + _ConfigHeader.h), so
we route the Who chain through that config.
Guard-latch note: the seam-test config and the root config share the
PACKAGE_CONFIGURATION guard, so whichever is included first wins; including
_Seams.h here first makes CRABSTK_WHO_CORE visible no matter which config
the TU picked. Do not reorder these two includes. */
#include "_Seams.h"
#include "_Seams/_Config.h"
#endif

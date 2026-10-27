// Copyright AStarship <https://astarship.net>.
// Precompiled Header File
#pragma once
// The rollup TU (_Seams/_Main.hxx) gates every module with #if SEAM >=
// CRABSTK_*, so the seam table must be in place before any module
// 00.Core.hxx. pch.h is the first header of the TU (quoted include from
// _Seams/_Main.hxx resolves to this file in the same directory), so it
// pulls in the local seam table and defaults SEAM to the release seam.
// The root _Config.h (canonical types) comes next, from -I., on purpose:
// the root config deliberately does NOT pull the full ASCIICrabs machine
// (_Package.hxx); only modules that need it include it in their own
// _Seams/ tree. Per-seam debug builds override with -DSEAM=<n>.
#include "_Seams.h"

#ifndef SEAM
#define SEAM SEAM_N
#endif

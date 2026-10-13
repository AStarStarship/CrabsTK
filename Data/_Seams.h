// Copyright AStarship <https://astarship.net>.
// Data module seam include.
// (2026-10-12: removed the local redefinition of CRABSTK_DATA_RECORD /
// CRABSTK_DATA_ASCIIPLOT — they are now defined in the single source of
// truth at the root _Seams.h (54 / 56). This file just re-exports the
// root table so Data/*.h files can #include "_Seams.h" and find the IDs.)
#pragma once
#ifndef CRABSTK_DATA_SEAMS
#define CRABSTK_DATA_SEAMS
#include "../_Seams.h"
#endif

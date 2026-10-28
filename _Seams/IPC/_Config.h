// Copyright AStarship <https://astarship.net>.
// IPC seam test configuration.
//
// The canonical type layer (CHA/ISC/ISW/IUC/IUD/BOL) is what the IPC
// module's type-identity asserts need. The full ASCIICrabs _ConfigFooter.h
// pulls the Room/Slot/Clock enum layers, which require the seam macros
// (USING_STR, CRABS_ROOM_BYTES) normally supplied by _Seams/_Config.h —
// not a dependency of a transport module. So: Header (types) + the
// footer's type-only prefix. The footer's type typedefs (CHR/CHL, ISR/
// IUR/FPR, ISX/IUX/ISV/IUV, TMS, BYT) all precede the seam-macro usage at
// line 304, and every type the IPC module uses (CHA, ISC, ISW, IUC, IUD,
// BOL, IUA) is defined in the Header alone.
#pragma once
#ifndef CRABSTK_IPC_TEST_CONFIG
#define CRABSTK_IPC_TEST_CONFIG
#include "../../../ASCIICrabs/_ConfigHeader.h"
#endif

// Copyright AStarship <https://astarship.net>.
// Root configuration header — the canonical type seam for CrabsTK modules.
//
// 2026-10-20: this file was an empty stub, so each module carried its own
// shadow typedefs (IPC, Env/Tests) that defeated cross-project type
// identity. It now routes to the canonical ASCIICrabs type header — the
// single source of truth for CHA/CHA-family/ISC/ISW/IUC/IUD/BOL/etc. —
// when built against a real sibling ASCIICrabs checkout, and falls back to
// the minimal local type set otherwise (embedded/solo builds).
//
// The full ASCIICrabs config FOOTER (Room/Slot/Clock enums) is NOT pulled
// here: it requires the seam macros supplied by each project's _Seams/
// _Config.h and is a dependency of the abstract machine, not of CrabsTK
// transport/data modules. Modules that need the full chain include the
// seam config in their _Seams/ tree.

#pragma once

#ifndef CRABSTK_CONFIG_H
#define CRABSTK_CONFIG_H

#if !defined(CRABSTK_IPC_LEGACY) && \
    __has_include("../ASCIICrabs/_ConfigHeader.h")
#include "../ASCIICrabs/_ConfigHeader.h"
#else
// Minimal canonical-width type set for solo/embedded builds. Widths match
// ASCIICrabs; identity against the real config is only provable when the
// sibling checkout is present.
#include <cstdint>
#include <cstddef>
#ifndef CHA
typedef char CHA;
#endif
#ifndef ISA
typedef int8_t ISA;
#endif
#ifndef IUA
typedef uint8_t IUA;
#endif
#ifndef ISB
typedef int16_t ISB;
#endif
#ifndef IUB
typedef uint16_t IUB;
#endif
#ifndef ISC
typedef int32_t ISC;
#endif
#ifndef IUC
typedef uint32_t IUC;
#endif
#ifndef ISD
typedef int64_t ISD;
#endif
#ifndef IUD
typedef uint64_t IUD;
#endif
#ifndef ISN
typedef int ISN;
#endif
#ifndef ISW
typedef intptr_t ISW;
#endif
#ifndef IUN
typedef unsigned int IUN;
#endif
#ifndef IUW
typedef uintptr_t IUW;
#endif
#ifndef BOL
typedef bool BOL;
#endif
#ifndef FPC
typedef float FPC;
#endif
#ifndef FPD
typedef double FPD;
#endif
#endif // canonical header available

#endif // CRABSTK_CONFIG_H

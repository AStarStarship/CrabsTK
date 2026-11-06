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

#include <ASCIICrabs/_ConfigHeader.h>

#endif  //< CRABSTK_CONFIG_H

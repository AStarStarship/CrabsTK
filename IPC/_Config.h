// Copyright AStarship <https://astarship.net>.
// IPC configuration — canonical types come from the ASCIICrabs config chain.
//
// 2026-10-20: the previous file re-declared CHA/ISC/ISW/BOL/IUC locally.
// Those shadow typedefs defeated cross-project type identity. This header
// now pulls the canonical definitions through ../_Config.h (which routes to
// ASCIICrabs/_ConfigHeader.h + _ConfigFooter.h in a normal single-TU build)
// and proves identity with static_asserts, not width checks alone.
//
// The old duplicate typedefs are retained ONLY behind CRABSTK_IPC_LEGACY
// for the historical pipe_test.hxx reproduction binary. New code must not
// define that macro.

#pragma once
#ifndef CRABSTK_IPC_CONFIG_H
#define CRABSTK_IPC_CONFIG_H

#ifdef CRABSTK_IPC_LEGACY
// Legacy standalone reproduction build (see IPC/_Seams/02.LegacyRed.h).
// Self-contained: it must compile the NEW Pipe.h/Pipe.hxx against the OLD
// type spellings so the historical pipe_test.hxx RED binary still builds
// and still demonstrates the legacy fault.
#include <cstdint>
#include <cstddef>
typedef CHR CHA;
typedef ISC ISC;
typedef ISC ISC;
typedef intptr_t ISW;
typedef BOL BOL;
typedef uint32_t IUC;
typedef uint8_t IUA;
typedef int64_t IUD;
static_assert(sizeof(BOL) == 1, "BOL must be 1 byte");
static_assert(sizeof(ISW) == sizeof(void*), "ISW must be pointer-sized");
static_assert(sizeof(IUC) == 4, "IUC must be 32-bit");
#else

#include "../_Config.h"

// Compile-time TYPE-IDENTITY against the canonical ASCIICrabs configuration.
// Width checks were insufficient: the old IPC types had the right size and
// the wrong spelling (IUC ISW, CHR BOL, long long IUC).
// std::is_same against the canonical types is the real integration proof.
#include <type_traits>
static_assert(std::is_same<CHA, CHR>::value,
              "IPC CHA must be the canonical ASCIICrabs CHR");
static_assert(std::is_same<ISC, ISC>::value,
              "IPC ISC must be the canonical 32-bit signed index");
static_assert(std::is_same<ISW, intptr_t>::value,
              "IPC ISW must be the canonical pointer-sized signed word");
// ISC is the canonical 32-bit signed index (the old IPC re-declared it
// locally as `ISC` — the same type, a shadow copy that defeated identity
// checks; that shadow is gone now, so the identity assert is real).
static_assert(std::is_same<ISC, ISC>::value,
              "IPC ISC must be the canonical 32-bit signed index");
static_assert(std::is_same<BOL, BOL>::value,
              "IPC BOL must be the canonical BOL");
static_assert(std::is_same<IUC, uint32_t>::value,
              "IPC IUC must be the canonical 32-bit unsigned");
static_assert(std::is_same<IUD, uint64_t>::value,
              "IPC IUD must be the canonical 64-bit unsigned");
static_assert(std::is_same<IUA, uint8_t>::value,
              "IPC IUA must be the canonical 1-byte unsigned");
static_assert(sizeof(BOL) == 1 && sizeof(IUC) == 4 && sizeof(IUD) == 8 &&
              sizeof(ISW) == sizeof(void*),
              "IPC canonical widths must match ASCIICrabs");

#endif  //< CRABSTK_IPC_LEGACY

// Boolean-like constants kept for the legacy test harness.
#ifndef YES_0
#define YES_0 0
#endif
#ifndef NO_0
#define NO_0 1
#endif

#endif  //< CRABSTK_IPC_CONFIG_H

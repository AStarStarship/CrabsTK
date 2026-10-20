// Minimal _Config.h shim for standalone Env test builds.
// Provides the type definitions and macros that EnvVar.h expects,
// without pulling in the full ASCIICrabs config chain.
// Uses ASCIICrabs-compatible type widths to ensure cross-project consistency.

#include <cstdint>
#include <cstddef>

#pragma once
#ifndef CRABSTK_ENV_TESTS_CONFIG_H
#define CRABSTK_ENV_TESTS_CONFIG_H

// ============================================================
// Type definitions — match ASCIICrabs canonical widths
// ============================================================

// CHA: Character type (1 byte, signed char)
typedef char CHA;

// ISA: 1-byte signed integer
typedef signed char ISA;

// IUA: 1-byte unsigned integer
typedef unsigned char IUA;

// ISB: 2-byte signed integer
typedef short ISB;

// IUB: 2-byte unsigned integer
typedef unsigned short IUB;

// ISC: 4-byte signed integer
typedef int ISC;

// IUC: 4-byte unsigned integer (uint32_t)
// Matches ASCIICrabs: typedef uint32_t IUC; in _ConfigHeader.h
typedef uint32_t IUC;

// ISD: 8-byte signed integer (int64_t)
// Matches ASCIICrabs: typedef int64_t ISD; in _ConfigHeader.h
typedef int64_t ISD;

// IUD: 8-byte unsigned integer (uint64_t)
// Matches ASCIICrabs: typedef uint64_t IUD; in _ConfigHeader.h
typedef uint64_t IUD;

// ISN: Index/size type (4-byte int)
// Matches ASCIICrabs: typedef int ISN; in _ConfigHeader.h
typedef int ISN;

// IUN: Unsigned index type
typedef unsigned int IUN;

// ISW: Word-sized int / pointer-sized integer
// Matches ASCIICrabs: typedef intptr_t ISW; in _ConfigHeader.h
// Old: typedef long ISW; — may mismatch on some platforms
// FIX: Use intptr_t for guaranteed pointer-sized width
typedef intptr_t ISW;

// IUW: Unsigned pointer-sized integer
typedef uintptr_t IUW;

// FPC: 4-byte floating-point (float)
// Matches ASCIICrabs: typedef float FPC; in _ConfigHeader.h
typedef float FPC;

// FPD: 8-byte floating-point (double)
// Matches ASCIICrabs: typedef double FPD; in _ConfigHeader.h
typedef double FPD;

// BOL: Boolean type (1 byte)
// Matches ASCIICrabs: typedef bool BOL; in _ConfigHeader.h
typedef bool BOL;

// ============================================================
// Compile-time width verification static_assert
// ============================================================

// BOL must be 1 byte
static_assert(sizeof(BOL) == 1, "Env Tests BOL must be 1 byte width — matches ASCIICrabs bool");

// ISW must be pointer-sized
static_assert(sizeof(ISW) == sizeof(void*), "Env Tests ISW must be pointer-sized — matches ASCIICrabs intptr_t");

// IUC must be 32-bit
static_assert(sizeof(IUC) == 4, "Env Tests IUC must be 32-bit width — matches ASCIICrabs uint32_t");

// ============================================================
// Minimal constants
// ============================================================

#define YES_0 0
#define NO_0 1

// ============================================================
// End of file
// ============================================================

#endif // CRABSTK_ENV_TESTS_CONFIG_H
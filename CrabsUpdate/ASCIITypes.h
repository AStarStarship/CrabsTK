// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_TOOLKIT_UPDATE_ASCII_TYPES_H
#define CRABS_TOOLKIT_UPDATE_ASCII_TYPES_H

/*
ASCII Data Types — the core type system for CrabsUpdate.

These types are modeled after the C0 control codes and the ASCII Data
Specification. They are predictable by humans:
  A = 8-bit, B = 16-bit, C = 32-bit, D = 64-bit, E = 128-bit.
  I = Integer, F = Floating-point, CH = Character, S = Signed, U = Unsigned.
  W = Word-sized (ALU), N = native, P = Pointer.

These are the same typedefs as in ASCIICrabs' _ConfigHeader.h, but
defined here so the CrabsUpdate layer is self-contained and does not
depend on the upstream configuration.
*/

typedef char CHA;
typedef char16_t CHB;
typedef char32_t CHC;

typedef signed char ISA;
typedef unsigned char IUA;
typedef short ISB;
typedef unsigned short IUB;
typedef int ISC;
typedef unsigned int IUC;
typedef long long ISD;
typedef unsigned long long IUD;

typedef signed char ISG;   // Half-word signed.
typedef unsigned char IUG; // Half-word unsigned.
typedef float FPG;         // Half-word floating-point.

typedef long ISW;          // Word-sized signed.
typedef unsigned long IUW; // Word-sized unsigned.
typedef float FLW;         // Word-sized floating-point.

typedef long ISM;          // Medium signed.
typedef unsigned short IUM; // Medium unsigned.

typedef int ISN;           // Native signed (at least 16-bit).
typedef unsigned int IUN;  // Native unsigned (at least 16-bit).

typedef long ISR;          // At least 8-bit signed.
typedef unsigned int IUR;  // At least 8-bit unsigned.
typedef float FPR;         // At least 16-bit floating-point.

typedef ISC TMC;           // 32-bit Unix timestamp.
typedef ISD TMD;           // 64-bit Unix timestamp.

typedef ISW ERC;           // Error code.

typedef IUB FPB;           // 16-bit floating-point (half).
typedef float FPC;         // 32-bit floating-point.
typedef double FPD;        // 64-bit floating-point.
typedef double FPE;        // 128-bit floating-point (placeholder).

typedef void* PTR;
typedef const void* PTC;

typedef ISW DTW;           // Word-sized ASCII Data Type.
typedef IUA DTA;           // 8-bit ASCII Data Type.
typedef IUB DTB;           // 16-bit ASCII Data Type.
typedef ISC DTC;           // 32-bit ASCII Data Type.
typedef ISD DTD;           // 64-bit ASCII Data Type.

typedef bool BOL;

#endif

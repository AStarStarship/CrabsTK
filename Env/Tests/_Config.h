// Minimal _Config.h shim for standalone Env test builds.
// Provides the type definitions and macros that EnvVar.h expects,
// without pulling in the full ASCIICrabs config chain.
#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>

typedef char CHA;
typedef signed char ISA;
typedef unsigned char IUA;
typedef short ISB;
typedef unsigned short IUB;
typedef int ISC;
typedef unsigned int IUC;
typedef long long ISD;
typedef unsigned long long IUD;
typedef int ISN;
typedef unsigned int IUN;
typedef long ISW;
typedef unsigned long IUW;
typedef float FPC;
typedef double FPD;
typedef bool BOL;

inline BOL IsError(const void* p) { return p == nullptr; }
#define nilp nullptr
#define NILP nullptr

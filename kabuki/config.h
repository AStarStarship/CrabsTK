#pragma once

#ifndef INCLUDED_KABUKI_CONFIG
#define INCLUDED_KABUKI_CONFIG 1

#include <kabuki/f2/config.h>
#define SEAM_N SEAM_0_0_0_0__00

#if DLL
#define API Foo
#else
#define API
#endif

#endif  //< #ifndef INCLUDED_KABUKI_CONFIG

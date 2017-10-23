/***
* Copyright (C) Microsoft. All rights reserved.
* Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
*
* =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
*
* stdafx.h
*
* Pre-compiled headers
*
* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
****/

#pragma once

#include "unittestpp.h"

#include <random>

#define NOMINMAX
#include "json_tests.h"

#ifdef WIN32
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define NOMINMAX
#include <Windows.h>
#endif

#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <cstdint>
#include <chrono>
#include <iostream>
#include <iterator>
#include <fstream>
#include <cfloat>
#include <algorithm>
#include <locale>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <math.h>
#include <string>
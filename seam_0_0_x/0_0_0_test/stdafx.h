/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/f2/stdafx.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once

#include <cstdarg>
#include <cstdint>

typedef unsigned int uint;

#define API

#define SEAM_LAYER 0
#define SEAM_MAJOR 0
#define SEAM_MINOR 0

#define SEAM_0_0_0 1
#define SEAM_0_0_1 2
#define SEAM_0_0_2 3
#define SEAM_N 3

enum { kSeamCount = SEAM_N };

#define WORD_SIZE 64

#ifndef INCLUDED_SEAMS
#define INCLUDED_SEAMS

#if SEAM_LAYER == 0
#if SEAM_MAJOR == 0
#if SEAM_MINOR == 0
#define SEAM 1
#elif SEAM_MINOR == 1
#define SEAM 2
#elif SEAM_MINOR == 2
#define SEAM 3
#endif
#endif  //< #if SEAM_MAJOR == 0
#endif  //< #if SEAM_LAYER == 0

#if SEAM > 0 && SEAM <= SEAM_N
#define DEBUG 1
#endif

#define APP_EXIT_SUCCESS 0
#define APP_EXIT_FAILURE 1

#if COMPILER == VISUAL_CPP
#define getch() _getch();
#define FORMAT_SI8 "%I64i"
#define FORMAT_UI8 "%I64u"
#else
#define FORMAT_SI8 "%lld"
#define FORMAT_UI8 "%llu"
#endif

#endif  //< #if INCLUDED_SEAMS

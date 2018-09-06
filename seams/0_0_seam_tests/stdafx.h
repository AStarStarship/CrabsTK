/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/stdafx.h
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

#define DLL

#define SEAM_PAGE 0
#define SEAM_MAJOR 0
#define SEAM_MINOR 0

#ifndef INCLUDED_SEAMS
#define INCLUDED_SEAMS

#define SEAM_0_0_0 1
#define SEAM_0_0_1 2
#define SEAM_0_0_2 3
#define SEAM_0_0_3 4
#define SEAM_0_0_4 5
#define SEAM_0_0_5 6
#define SEAM_0_0_6 7
#define SEAM_0_0_7 8

#if SEAM_PAGE == 0
#if SEAM_MAJOR == 0
#if SEAM_MINOR == 0
#define SEAM 1
#elif SEAM_MINOR == 1
#define SEAM 2
#elif SEAM_MINOR == 2
#define SEAM 3
#elif SEAM_MINOR == 3
#define SEAM 4
#elif SEAM_MINOR == 4
#define SEAM 5
#elif SEAM_MINOR == 5
#define SEAM 6
#elif SEAM_MINOR == 6
#define SEAM 7
#elif SEAM_MINOR == 7
#define SEAM 8
#else
#define SEAM 0
#endif
#endif
#if SEAM_MAJOR == 1
#if SEAM_MINOR == 0
#define SEAM_0_1_0
#define SEAM 9
#endif
#endif  //< #if SEAM_MAJOR == 0
#endif  //< #if SEAM_PAGE == 0

#define SEAM_COUNT 9
typedef unsigned int uint;
#endif

#include <cstdarg>
#include <cstdint>

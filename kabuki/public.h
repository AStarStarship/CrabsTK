/* Kabuki Toolkit
    @version 0.x
    @file    ~/library/global.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-8 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (R). Licensed under the Apache License, Version
             2.0 (the "License"); you may not use this file except in
             compliance with the License. You may obtain a copy of the License
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
             implied. See the License for the specific language governing
             permissions and limitations under the License.
*/

#include <stdafx.h>

#ifndef HEADER_FOR_GLOBAL
#define HEADER_FOR_GLOBAL

#define USING_KABUKI_CRABS      1
#define USING_KABUKI_SERIAL     2
#define USING_KABUKI_DATA       3
#define USING_KABUKI_STORE      4
#define USING_KABUKI_MATH       5
#define USING_KABUKI_ID         6
#define USING_HEADER_FOR_KT_PRO        7
#define USING_KABUKI_HMI        8
#define USING_KABUKI_MIDI       9
#define USING_KABUKI_AL         10
#define USING_KABUKI_APP        11
#define USING_KABUKI_SLOTH      12
#define USING_KABUKI_FILE       13
#define USING_KABUKI_AIML       14
#define USING_ALL               LARGEST_INT  //< Largest signed integer 2^31-1.

#if MAJOR_SEAM >= 1
#include "crabs/global.h"
#elif MAJOR_SEAM >= 2
#include "kabuki/toolkit/hmi/global.h"
#include "kabuki/toolkit/midi/global.h"
#include "kabuki/toolkit/serial/global.h"
#elif MAJOR_SEAM >= 3
#include "kabuki/toolkit/pro/global.h"
#elif MAJOR_SEAM >= 4
#elif MAJOR_SEAM >= 5
#elif MAJOR_SEAM >= 6
#include "kabuki/toolkit/al/global.h"
#elif MAJOR_SEAM >= 7
#include "kabuki/toolkit/code/global.h"
#include "kabuki/toolkit/file/global.h"
#elif MAJOR_SEAM >= 8
#include "kabuki/toolkit/gl/global.h"
#include "kabuki/toolkit/gui/global.h"
#elif MAJOR_SEAM >= 9
#include "kabuki/math/global.h"
#include "kabuki/toolkit/pro/global.h"
#elif MAJOR_SEAM >= 10
#include "kabuki/toolkit/store/global.h"
#elif MAJOR_SEAM >= 18
#include "kabuki/toolkit/aiml/global.h"
#endif  //< #if MAJOR_SEAM >= 2
#endif  //< #ifndef HEADER_FOR_GLOBAL

/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/global.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#ifndef HEADER_FOR_GLOBAL
#define HEADER_FOR_GLOBAL

#ifndef LARGEST_INT
// Largest value possible to store in int type.
#define LARGEST_INT            2147483647   //< Use this for 32-bit CPUs
//#define LARGEST_INT          32767        //< Use this for 16-bit CPUs
#endif
#define USING_KABUKI_CRABS     1
#define USING_KABUKI_HMI       2
#define USING_KABUKI_ID        3
#define USING_KABUKI_MIDI      4
#define USING_KABUKI_DATA      5
#define USING_KABUKI_WEB       6
#define USING_KABUKI_ICAN3     7
#define USING_KABUKI_AL        8
#define USING_KABUKI_APP       9
#define USING_KABUKI_CODE      10
#define USING_KABUKI_FILE      11
#define USING_KABUKI_GL        12
#define USING_KABUKI_GUI       13
#define USING_KABUKI_MATH      14
#define USING_KABUKI_PRO       15
#define USING_KABUKI_STORE     16
#define USING_KABUKI_AIML      17
#define USING_ALL              LARGEST_INT  //< Largest signed integer 2^31-1.

#include "crabs/global.h"

#if MAJOR_SEAM >= 2
#include "kabuki/hmi/global.h"
#include "kabuki/midi/global.h"
#include "kabuki/serial/global.h"
#include "kabuki/web/global.h"
#elif MAJOR_SEAM >= 3
#include "kabuki/app/global.h"
#elif MAJOR_SEAM >= 4
#include "kabuki/id/global.h"
#include "kabuki/data/global.h"
#elif MAJOR_SEAM >= 5
#include "ican3/global.h"
#elif MAJOR_SEAM >= 6
#include "kabuki/al/global.h"
#elif MAJOR_SEAM >= 7
#include "kabuki/code/global.h"
#include "kabuki/file/global.h"
#elif MAJOR_SEAM >= 8
#include "kabuki/gl/global.h"
#include "kabuki/gui/global.h"
#elif MAJOR_SEAM >= 9
#include "kabuki/math/global.h"
#include "kabuki/pro/global.h"
#elif MAJOR_SEAM >= 10
#include "kabuki/store/global.h"
#elif MAJOR_SEAM >= 11
#include "kabuki/video/global.h"
#elif MAJOR_SEAM >= 18
#include "kabuki/aiml/global.h"
#endif
#endif  //< HEADER_FOR_GLOBAL

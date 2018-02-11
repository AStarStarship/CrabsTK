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

#define KABUKI_TESTING_MODULE  1

#ifndef LARGEST_INT
// Largest value possible to store in int type.
#define LARGEST_INT            2147483647   //< Use this for 32-bit CPUs
//#define LARGEST_INT          32767        //< Use this for 16-bit CPUs
#endif
#define USING_KABUKI_CRABS     1
#define USING_KABUKI_HMI       2
#define USING_KABUKI_ID        3
#define USING_KABUKI_DATA      4
#define USING_KABUKI_WEB       5
#define USING_KABUKI_ICAN3     6
#define USING_KABUKI_AL        7
#define USING_KABUKI_APP       8
#define USING_KABUKI_CODE      9
#define USING_KABUKI_FILE      10
#define USING_KABUKI_GL        11
#define USING_KABUKI_GUI       12
#define USING_KABUKI_MATH      13
#define USING_KABUKI_MIDI      14
#define USING_KABUKI_PRO       15
#define USING_KABUKI_STORE     16
#define USING_KABUKI_VIDEO     17
#define USING_KABUKI_AIML      18
#define USING_ALL              LARGEST_INT  //< Largest signed integer 2^31-1.

#if KABUKI_TESTING_MODULE >= USING_KABUKI_CRABS
#include "crabs/global.h"
#endif

#if KABUKI_TESTING_MODULE >= USING_KABUKI_HMI
#include "kabuki/hmi/global.h"
#endif

#if KABUKI_TESTING_MODULE >= USING_KABUKI_ID
#include "kabuki/id/global.h"
#endif

#if KABUKI_TESTING_MODULE >= USING_KABUKI_DATA
#include "kabuki/data/global.h"
#endif

#if KABUKI_TESTING_MODULE >= USING_KABUKI_WEB
#include "kabuki/web/global.h"
#endif

#if KABUKI_TESTING_MODULE >= USING_KABUKI_ICAN3
#include "ican3/global.h"
#endif

#if KABUKI_TESTING_MODULE >= USING_KABUKI_AL
#include "kabuki/al/global.h"
#endif

#if KABUKI_TESTING_MODULE >= USING_KABUKI_APP
#include "kabuki/app/global.h"
#endif

#if KABUKI_TESTING_MODULE >= USING_KABUKI_CODE
#include "kabuki/code/global.h"
#endif

#if KABUKI_TESTING_MODULE >= USING_KABUKI_FILE
#include "kabuki/file/global.h"
#endif

#if KABUKI_TESTING_MODULE >= USING_KABUKI_GL
#include "kabuki/gl/global.h"
#endif

#if KABUKI_TESTING_MODULE >= USING_KABUKI_GUI
#include "kabuki/gui/global.h"
#endif

#if KABUKI_TESTING_MODULE >= USING_KABUKI_MATH
#include "kabuki/math/global.h"
#endif

#if KABUKI_TESTING_MODULE >= USING_KABUKI_MIDI
#include "kabuki/midi/global.h"
#endif

#if KABUKI_TESTING_MODULE >= USING_KABUKI_PRO
#include "kabuki/pro/global.h"
#endif

#if KABUKI_TESTING_MODULE >= USING_KABUKI_STORE
#include "kabuki/store/global.h"
#endif

#if KABUKI_TESTING_MODULE >= USING_KABUKI_VIDEO
#include "kabuki/video/global.h"
#endif

#if KABUKI_TESTING_MODULE >= USING_KABUKI_AIML
#include "kabuki/aiml/global.h"
#endif

// Not sure if we actually want to undef these yet.

#undef USING_KABUKI_CRABS
#undef USING_KABUKI_ID
#undef USING_KABUKI_DATA
#undef USING_KABUKI_WEB
#undef USING_KABUKI_HMI
#undef USING_KABUKI_MIDI
#undef USING_KABUKI_PRO
#undef USING_KABUKI_STORE
#undef USING_KABUKI_APP
#undef USING_KABUKI_GL
#undef USING_KABUKI_GUI
#undef USING_KABUKI_AL
#undef USING_KABUKI_CODE
#undef USING_KABUKI_FILE
#undef USING_KABUKI_MATH
#undef USING_KABUKI_VIDEO
#undef USING_KABUKI_AIML
#undef USING_KABUKI_ICAN3

#endif  //< HEADER_FOR_GLOBAL

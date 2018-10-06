/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    kabuki-toolkit.git/kabuki/f2/utf.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>
#if SEAM >= SEAM_00_00_00__03
#ifndef INCLUDED_KABUKI_F2_UTF
#define INCLUDED_KABUKI_F2_UTF

#if USING_UTF8
#include "str1.h"
#endif
#if USING_UTF16
#include "str2.h"
#endif
#if USING_UTF32
#include "str4.h"
#endif

#if F2_TEXT
namespace _ {
#if F2_TEXT == UTF8
using Utf = Utf8;
using Center = Utf8Center;
using Right = Utf8Right;
using Line = Utf8Line;
using LineString = Utf8LineString;
#elif F2_TEXT == UTF16
using Utf = Utf16;
using Center = Utf16Center;
using Right = Utf16Right;
using Line = Utf16Line;
using LineString = Utf16LineString;
#elif F2_TEXT == UTF32
using Utf = Utf32;
using Center = Utf32Center;
using Right = Utf32Right;
using Line = Utf32Line;
using LineString = Utf32LineString;
#endif
}  // namespace _
#endif
#undef PRINT
#undef PRINTF
#endif  //< #if INCLUDED_KABUKI_F2_UTF
#endif  //< #if SEAM >= SEAM_00_00_00__03

/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit
@file    ~/kabuki/crabs/language.h
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
#include <stdafx.h>
#if SEAM >= SEAM_0_0_0
#ifndef INCLUDED_CRABS_LANGUAGE
#define INCLUDED_CRABS_LANGUAGE
// Dependencies:
#include "config.h"
// End dependencies.

namespace _ {

namespace langauge {

enum {
  kEnUppercaseBegin = 'A',
  kEnUppercaseEnd = 'Z',
  kEnLowercaseBegin = 'a',
  kEnLowercaseEnd = 'z',
};
}  // namespace langauge

/* A spoken language Unicode character range.

*/
struct Language {
  wchar_t uppercase_begin,  //< First uppercase Unicode char index.
      uppercase_end,        //< Last uppercase Unicode char index.
      lowercase_begin,      //< First lowercase Unicode char index.
      lowercase_end;        //< Last lowercase Unicode char index.
};

}  // namespace _
#endif  //< #if SEAM >= SEAM_0_0_0
#endif  //< INCLUDED_CRABS_LANGUAGE

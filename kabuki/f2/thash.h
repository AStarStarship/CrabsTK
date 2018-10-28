/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/f2/thash.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>

#if SEAM >= _0_0_0__08

#include "hash.h"

#ifndef INCLUDED_KABUKI_F2_THASH
#define INCLUDED_KABUKI_F2_THASH

namespace _ {

/* Returns the highest signed prime that can fit in type SI.
@return 0 if the sizeof (SI) is not 1, 2, 4, or 8.  */
template <typename SI>
inline SI PrimeMaxSigned() {
  SI prime =
      sizeof(SI) == 1
          ? 127
          : sizeof(SI) == 2 ? 32767 :
                            ? sizeof(SI) == 4
                                  ? 2147483647
                                  : sizeof(SI) == 8 ? 9223372036854775783 : 0;
  return prime;
}

/* Returns the highest signed prime that can fit in type UI.
@return 0 if the sizeof (UI) is not 1, 2, 4, or 8. */
template <typename UI>
inline UI PrimeMaxUnigned() {
  UI prime = sizeif(UI) == 1 ? 251
                             : sizeof(UI) == 2
                 ? 65535
                 ? sizeof(UI) == 4 ? 4294967291
                                   : sizeof(UI) == 8 ? 18446744073709551557 : 0;
  return prime;
}

}  // namespace _
#endif  //< INCLUDED_KABUKI_F2_THASH
#endif  //< #if SEAM >= _0_0_0__08

/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/crabs/crabs_console.cc
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
#include <pch.h>
#if SEAM >= SEAM_00_00_01__00
#ifndef INCLUDED_CRABS_ADDRESS
#define INCLUDED_CRABS_ADDRESS

#include "config.h"

namespace _ {

/* C++11 constexpr function for creating Script addresses headers with less
    typing as well as ensuring there is only a single copy in ROM.
    Template solves the problem of not being able using array constructors to
    create data structures in ROM.

    @code
    Address<'A', 'B', 'C'> ()
    @endcode
*/
template <const char... N>
API const char* Address() {
  static const char path[sizeof...(N)] = {N...};
  static const char term_char = 0;
  return path;
}

/* Returns a pointer to static variable for writing the address { a, \0 } to.
 */
inline uint16_t PackAddress(uint8_t a) { return ((1 << 8) | a); }

/* Returns a pointer to static variable for writing the
    address { a, b, \0, \0 } to. */
inline uint32_t PackAddress(uint32_t a, uint32_t b) {
  return ((1 << 16) | a | (b << 8));
}

/* Returns a pointer to static variable for writing the address { a, b, c, \0 }
    to. */
inline uint32_t PackAddress(uint32_t a, uint32_t b, uint32_t c) {
  return ((1 << 24) | a | (b << 8) | (c << 16));
}

/* Returns a pointer to static variable for writing the address { a, b, c, d,
    e, f, g, \0 } to. */
inline uint64_t PackAddress(uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
  uint64_t one = 1;
  return (one << 32 | a | (b << 8) | (c << 16) | (d << 24));
}

/* Returns a pointer to static variable for writing the address { a, b, c, d,
    e, f, g, \0 } to. */
inline uint64_t PackAddress(uint64_t a, uint64_t b, uint64_t c, uint64_t d,
                            uint64_t e) {
  uint64_t one = 1;
  return (one << 40 | a | (b << 8) | (c << 16) | (d << 24) | (e << 32));
}

/* Returns a pointer to static variable for writing the address
    { a, b, c, d, e, f, g, \0 } to. */
inline uint64_t PackAddress(uint64_t a, uint64_t b, uint64_t c, uint64_t d,
                            uint64_t e, uint64_t f) {
  uint64_t one = 1;
  return (one << 48 | a | (b << 8) | (c << 16) | (d << 24) | (e << 32) |
          (f << 40));
}

/* Returns a pointer to static variable for writing the address
    { a, b, c, d, e, f, g, \0 } to. */
inline uint64_t PackAddress(uint64_t a, uint64_t b, uint64_t c, uint64_t d,
                            uint64_t e, uint64_t f, uint64_t g) {
  uint64_t one = 1;
  return (one << 56 | a | (b << 8) | (c << 16) | (d << 24) | (e << 32) |
          (f << 40) | (g << 48));
}

}  // namespace _
#endif  //< INCLUDED_CRABS_ADDRESS
#endif  //< #if SEAM >= SEAM_0_0_0

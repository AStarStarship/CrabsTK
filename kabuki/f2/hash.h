/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/f2/hash.h
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

#if SEAM >= SEAM_0_0_0__04

#ifndef INCLUDED_KABUKI_F2_HASH
#define INCLUDED_KABUKI_F2_HASH
// Dependencies:
#include "config.h"
// End dependencies.

typedef uint16_t hash16_t;
typedef uint32_t hash32_t;
typedef uint64_t hash64_t;

namespace _ {

API inline hash16_t Hash16(hash16_t hash, char value);

API inline hash32_t Hash32(hash32_t hash, char16_t value);

API inline hash64_t Hash64(hash64_t hash, char32_t value);

/* Hashes the given char using the primeHash function. */
API hash16_t Hash16(const char* string, hash16_t hash = 65535);

/* Hashes the given char using the primeHash function. */
API hash32_t Hash32(const char* string, hash32_t hash = 4294967291);

/* Hashes the given char using the primeHash function. */
API hash64_t Hash64(const char* string, hash64_t hash = 18446744073709551557);

}  // namespace _
#endif  //< #if SEAM >= SEAM_0_0_0
#endif  //< INCLUDED_KABUKI_F2_HASH

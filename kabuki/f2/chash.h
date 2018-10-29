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

#if SEAM >= _0_0_0__08

#ifndef INCLUDED_KABUKI_F2_HASH
#define INCLUDED_KABUKI_F2_HASH
#include "config.h"

typedef uint16_t hash16_t;
typedef uint32_t hash32_t;
typedef uint64_t hash64_t;

namespace _ {

/* Basic prime multiple hash algorithm.
@return hash + value * hash.
@param  value The char value to hash.
@param  hash  The hash. */
API inline hash16_t Hash16(char value, hash16_t hash = 65535);

/* Basic prime multiple hash algorithm.
@return hash + value * hash.
@param  value The char value to hash.
@param  hash  The hash. */
API inline hash32_t Hash32(char16_t value, hash32_t hash = 4294967291);

/* Basic prime multiple hash algorithm.
@return hash + value * hash.
@param  value The char value to hash.
@param  hash  The hash. */
API inline hash64_t Hash64(char32_t value,
                           hash64_t hash = 18446744073709551557);

/* Hashes the given char using the primeHash function.
If you are experiencing collisions in your hash table, you may want to change
the see to a different prime number.
@return A prime multiple hash of the given string.
@param  string The string to hash.
@param  hash   A prime number seed hash. */
API hash16_t Hash16(const char* string, hash16_t hash = 65535);

/* Hashes the given char using the primeHash function.
If you are experiencing collisions in your hash table, you may want to change
the see to a different prime number.
@return A prime multiple hash of the given string.
@param  string The string to hash.
@param  hash   A prime number seed hash. */
API hash32_t Hash32(const char* string, hash32_t hash = 4294967291);

/* Hashes the given char using the primeHash function.
If you are experiencing collisions in your hash table, you may want to change
the see to a different prime number.
@return A prime multiple hash of the given string.
@param  string The string to hash.
@param  hash   A prime number seed hash. */
API hash64_t Hash64(const char* string, hash64_t hash = 18446744073709551557);

}  //< namespace _
#endif  //< #if SEAM >= _0_0_0__08
#endif  //< INCLUDED_KABUKI_F2_HASH

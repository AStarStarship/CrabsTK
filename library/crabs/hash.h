/** Kabuki Toolkit
    @version 0.x
    @file    ~/libraries/crabs/hash.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#pragma once
#include <stdafx.h>

#ifndef HEADER_FOR_CRABS_HASH
#define HEADER_FOR_CRABS_HASH

#include "config.h"

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3

namespace _ {

/** Hashes a single byte. */
inline uint16_t Hash16 (char c, uint16_t hash) {
    return (c * kLargest16BitPrime) + hash;
}

/** Hashes a single byte. */
inline hash32_t Hash32 (char c, hash32_t hash) {
    return (c * kLargest32BitPrime) + hash;
}

/** Hashes a single byte. */
inline hash64_t Hash64 (char c, hash64_t hash) {
    return (c * kLargest64BitPrime) + hash;
}

/** Hashes the given char using the primeHash function. */
KABUKI uint16_t Hash16 (const char* string, uint16_t hash = kLargest16BitPrime);

/** Hashes the given char using the primeHash function. */
KABUKI uint16_t Hash32 (const char* string, hash32_t hash = kLargest32BitPrime);

/** Hashes the given char using the primeHash function. */
KABUKI hash64_t Hash64 (const char* string, hash64_t hash = kLargest64BitPrime);

}       //< namespace _
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3
#endif  //< HEADER_FOR_CRABS_HASH

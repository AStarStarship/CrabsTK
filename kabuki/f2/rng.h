/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/f2/rng.h
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

#if SEAM >= SEAM_0_0_0__01
#ifndef INCLUDED_KABUKI_F2_RNGUNIFORM
#define INCLUDED_KABUKI_F2_RNGUNIFORM

namespace _ {

/* Gets a random generator device seed. */
API inline uint RandomizeSeed();

/* Gets a random 8-bit/1-byte unsigned integer. */
API inline uint8_t Random(uint8_t min, uint8_t max);

/* Gets a random 8-bit/1-byte signed integer. */
API inline uint8_t Random(int8_t min, int8_t max);

/* Gets a random 16-bit/2-byte unsigned integer. */
API inline uint16_t Random(uint16_t min, uint16_t max);

/* Gets a random 16-bit/2-byte signed integer. */
API inline uint16_t Random(int16_t min, int16_t max);

/* Gets a random 32-bit/4-byte unsigned integer. */
API inline uint32_t Random(uint32_t min, uint32_t max);

/* Gets a random 64-bit/8-byte unsigned integer. */
API inline uint64_t Random(uint64_t min, uint64_t max);

/* Gets a random 64-bit/8-byte signed integer. */
API inline uint64_t Random(int64_t min, int64_t max);

/* Gets a random 8-bit/1-byte unsigned integer. */
API inline uint8_t RandomUI1();

/* Gets a random 8-bit/1-byte signed integer. */
API inline int8_t RandomSI1();

/* Gets a random 16-bit/2-byte unsigned integer. */
API inline uint16_t RandomUI2();

/* Gets a random 16-bit/2-byte signed integer. */
API inline int16_t RandomSI2();

/* Gets a random 32-bit/4-byte unsigned integer. */
API inline uint32_t RandomUI4();

/* Gets a random 32-bit/4-byte signed integer. */
API inline int32_t RandomSI4();

/* Gets a random 64-bit/8-byte unsigned integer. */
API inline uint64_t RandomUI8();

/* Gets a random 64-bit/8-byte signed integer. */
API inline uint64_t RandomSI8();

}  // namespace _
#endif  //< INCLUDED_KABUKI_F2_RNGUNIFORM

#endif  //< #if SEAM >= SEAM_0_0_0__01

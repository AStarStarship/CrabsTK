/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/f2/crng.h
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

#ifndef INCLUDED_KABUKI_F2_CRNG
#define INCLUDED_KABUKI_F2_CRNG 1

namespace _ {

/* Gets a random generator device seed. */
API uint RandomSeed();

/* Re-seeds the static mersenne twister engine. */
API void RandomizeSeed();

/* Gets a random 16-bit/2-byte unsigned integer. */
API uint16_t RandomNumber(uint16_t min, uint16_t max);

/* Gets a random 8-bit/1-byte unsigned integer. */
API uint8_t RandomNumber(uint8_t min, uint8_t max);

/* Gets a random 8-bit/1-byte signed integer. */
API uint8_t RandomNumber(int8_t min, int8_t max);

/* Gets a random 16-bit/2-byte signed integer. */
API uint16_t RandomNumber(int16_t min, int16_t max);

/* Gets a random 32-bit/4-byte unsigned integer. */
API uint32_t RandomNumber(uint32_t min, uint32_t max);

/* Gets a random 64-bit/8-byte unsigned integer. */
API uint64_t RandomNumber(uint64_t min, uint64_t max);

/* Gets a random 64-bit/8-byte signed integer. */
API uint64_t RandomNumber(int64_t min, int64_t max);

/* Gets a random 8-bit/1-byte unsigned integer. */
API uint8_t RandomUI1();

/* Gets a random 8-bit/1-byte signed integer. */
API int8_t RandomSI1();

/* Gets a random 16-bit/2-byte unsigned integer. */
API uint16_t RandomUI2();

/* Gets a random 16-bit/2-byte signed integer. */
API int16_t RandomSI2();

/* Gets a random 32-bit/4-byte unsigned integer. */
API uint32_t RandomUI4();

/* Gets a random 32-bit/4-byte signed integer. */
API int32_t RandomSI4();

/* Gets a random 64-bit/8-byte unsigned integer. */
API uint64_t RandomUI8();

/* Gets a random 64-bit/8-byte signed integer. */
API int64_t RandomSI8();

/* Gets a random 8-bit/1-byte unsigned integer. */
API void RandomNumber(uint8_t& result);

/* Gets a random 8-bit/1-byte signed integer. */
API void RandomNumber(int8_t& result);

/* Gets a random 16-bit/2-byte unsigned integer. */
API void RandomNumber(uint16_t& result);

/* Gets a random 16-bit/2-byte signed integer. */
API void RandomNumber(int16_t& result);

/* Gets a random 32-bit/4-byte unsigned integer. */
API void RandomNumber(uint32_t& result);

/* Gets a random 32-bit/4-byte signed integer. */
API void RandomNumber(int32_t& result);

/* Gets a random 64-bit/8-byte unsigned integer. */
API void RandomNumber(uint64_t& result);

/* Gets a random 64-bit/8-byte signed integer. */
API void RandomNumber(int64_t& result);

}  // namespace _
#endif  //< INCLUDED_KABUKI_F2_CRNG

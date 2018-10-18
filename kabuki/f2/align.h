/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/f2/align.h
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

#if SEAM >= SEAM_0_0_0__01

#ifndef INCLUDED_KABUKI_F2_ALIGN
#define INCLUDED_KABUKI_F2_ALIGN 1

namespace _ {

enum {
  kCpuCacheLineSize = 64,
};

/* Compute the next highest power of 2. */
API inline uint32_t AlignPowerOf2(uint32_t value);

/* Compute the next highest power of 2. */
API inline int32_t AlignPowerOf2(int32_t value);

/* Aligns the given buffer pointer up to a cache line boundary (64 bytes). */
API inline uintptr_t* AlignUp(uintptr_t* buffer,
                              uintptr_t mask = sizeof(void*) - 1);

/* Aligns the given pointer up to a word boundary. */
API char* AlignUp(char* pointer, uintptr_t mask = sizeof(void*));

/* Aligns the given pointer up to a word boundary. */
API const char* AlignUp(const char* pointer, uintptr_t mask = sizeof(void*));

/* Aligns the given value up to an 8-byte boundary. */
API inline uint8_t AlignUp(uint8_t value, uint8_t mask = sizeof(void*) - 1);

/* Aligns the given value up to an 8-byte boundary. */
API inline int8_t AlignUp(int8_t value, int8_t mask = sizeof(void*) - 1);

/* Aligns the given value up to an 16-byte boundary. */
API uint16_t AlignUp(uint16_t value, uint16_t mask = sizeof(void*) - 1);

/* Aligns the given value up to an 16-byte boundary. */
API int16_t AlignUp(int16_t value, int16_t mask = sizeof(void*) - 1);

/* Aligns the given value up to an 32-byte boundary. */
API inline uint32_t AlignUp(uint32_t value, uint32_t mask = sizeof(void*) - 1);

/* Aligns the given value up to an 32-byte boundary. */
API inline int32_t AlignUp(int32_t value, int32_t mask = sizeof(void*) - 1);

/* Aligns the given value up to an 64-byte boundary. */
API inline uint64_t AlignUp(uint64_t value, uint64_t mask = sizeof(void*) - 1);

/* Aligns the given value up to an 64-byte boundary. */
API inline int64_t AlignUp(int64_t value, int64_t mask = sizeof(void*) - 1);

}  // namespace _
#endif  //< INCLUDED_KABUKI_F2_ALIGN
#endif  //< #if SEAM >= SEAM_0_0_0__01

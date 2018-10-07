/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/f2/f2_utils.cc
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>

#if SEAM >= SEAM_0_0_0__01
#include "align.h"
#include "socket.h"
#include "talign.h"

namespace _ {

uint32_t AlignPowerOf2(uint32_t value) {
  // @cite https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
  if (value < 0) {
    return 4;
  }
  //
  --value;
  value |= value >> 1;
  value |= value >> 2;
  value |= value >> 4;
  value |= value >> 8;
  value |= value >> 16;
  ++value;
  return value;
}

int32_t AlignPowerOf2(int32_t value) {
  if (value < 0) {
    return 4;
  }
  uint32_t v = (uint32_t)value;
  --v;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  ++v;
  return (int32_t)value;
}

uintptr_t* AlignUp(uintptr_t* buffer) {
  return AlignUp<uintptr_t>(buffer, kCpuCacheLineSize - 1);
}

char* AlignUp(char* pointer, uintptr_t mask) {
  return AlignUp<char>(pointer, mask);
}

const char* AlignUp(const char* pointer, uintptr_t mask) {
  return AlignUp<const char>(pointer, mask);
}

char* AlignDown(char* pointer, uintptr_t mask) {
  return AlignDown<char*>(pointer, mask);
}

const char* AlignDown(const char* pointer, uintptr_t mask) {
  return AlignDown<const char*>(pointer, mask);
}

uintptr_t* AlignUp(uintptr_t* pointer, uintptr_t boundary_bit_count) {
  return AlignUp<uintptr_t>(pointer, boundary_bit_count);
}

uint8_t AlignUp(uint8_t value, uint8_t mask) {
  return AlignUp<uint8_t>(value, mask);
}

int8_t AlignUp(int8_t value, int8_t mask) {
  return AlignUp<int8_t>(value, mask);
}

uint16_t AlignUp(uint16_t value, uint16_t mask) {
  return AlignUp<uint16_t>(value, mask);
}

int16_t AlignUp(int16_t value, int16_t mask) {
  return AlignUp<int16_t>(value, mask);
}

uint32_t AlignUp(uint32_t value, uint32_t mask) {
  return AlignUp<uint32_t>(value, mask);
}

int32_t AlignUp(int32_t value, int32_t mask) {
  return AlignUp<int32_t>(value, mask);
}

uint64_t AlignUp(uint64_t value, uint64_t mask) {
  return AlignUp<uint64_t>(value, mask);
}

int64_t AlignUp(int64_t value, int64_t mask) {
  return AlignUp<int64_t>(value, mask);
}

}  // namespace _
#undef PRINTF
#undef PRINT
#endif  //< #if SEAM >= SEAM_0_0_0__01

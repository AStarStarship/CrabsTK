/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/crabs_utils.cc
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <stdafx.h>

#if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2
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

uintptr_t* AlignUpCacheLine(uintptr_t* buffer) {
  return AlignUp<kCpuCacheLineSize, uintptr_t>(buffer);
}

char* AlignUpPointerWord(char* pointer) {
  return AlignUpPointerWord<char>(pointer);
}

const char* AlignUpPointerWord(const char* pointer) {
  return AlignUpPointerWord<char>(pointer);
}

char* AlignDownPointerWord(char* pointer) {
  return AlignDownPointer<char>(pointer);
}

const char* AlignDownPointerWord(const char* pointer) {
  return AlignDownPointer<char>(pointer);
}

uintptr_t* AlignUp(uintptr_t* pointer, uintptr_t boundary_bit_count) {
  return AlignUp<uintptr_t>(pointer, boundary_bit_count);
}

int8_t AlignUp(int8_t value) { return AlignUp<8, int8_t>(value); }

int16_t AlignUp(int16_t value) { return AlignUp<8, int16_t>(value); }

int32_t AlignUp(int32_t value) { return AlignUp<8, int32_t>(value); }

int64_t AlignUp(int64_t value) { return AlignUp<8, int64_t>(value); }

}  // namespace _
#undef PRINTF
#undef PRINT
#endif  //< SEAM_MAJOR == 0 && SEAM_MINOR >= 2

/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/f2/f2_hash.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>
#if SEAM >= SEAM_0_0_0__04
#include "thash.h"

namespace _ {

uint16_t Hash16(uint16_t hash, uint8_t value) {
  return Hash<uint16_t, uint8_t>(hash, value);
}

uint32_t Hash32(uint32_t hash, uint16_t value) {
  return Hash<uint32_t, uint16_t>(hash, value);
}

uint64_t Hash64(uint64_t hash, uint32_t value) {
  return Hash<uint64_t, uint32_t>(hash, value);
}

uint16_t Hash16(const char* string, uint16_t hash) {
  uint8_t c = *string;
  while (c) {
    hash = Hash16(c, hash);
    ++string;
    c = *string;
  }
  return hash;
}

uint16_t Hash32(const char* string, hash32_t hash) {
  uint8_t c = *string;
  while (c) {
    hash = Hash32(hash, c);
    ++string;
    c = *string;
  }
  return hash;
}

hash64_t Hash64(const char* string, hash64_t hash) {
  uint8_t c = *string;
  while (c) {
    hash = Hash64(hash, c);
    ++string;
    c = *string;
  }
  return hash;
}

uint16_t Hash16UI2(uint16_t value, uint16_t hash) {
  hash = ((value & 0xff) * Prime2Unsigned<uint16_t>()) + hash;
  hash = ((value >> 8) * Prime2Unsigned<uint16_t>()) + hash;
  return hash;
}

uint16_t Hash16UI4(uint32_t value, uint16_t hash) {
  hash = ((value & 0xff) * Prime2Unsigned<uint16_t>()) + hash;
  hash = (((value >> 8) & 0xff) * Prime2Unsigned<uint16_t>()) + hash;
  hash = (((value >> 16) & 0xff) * Prime2Unsigned<uint16_t>()) + hash;
  hash = (((value >> 24) & 0xff) * Prime2Unsigned<uint16_t>()) + hash;
  return hash;
}

uint16_t Hash16UI8(uint64_t value, uint16_t hash) {
  hash = ((value & 0xff) * Prime2Unsigned<uint16_t>()) + hash;
  hash = (((value >> 8) & 0xff) * Prime2Unsigned<uint16_t>()) + hash;
  hash = (((value >> 16) & 0xff) * Prime2Unsigned<uint16_t>()) + hash;
  hash = (((value >> 24) & 0xff) * Prime2Unsigned<uint16_t>()) + hash;
  hash = (((value >> 32) & 0xff) * Prime2Unsigned<uint16_t>()) + hash;
  hash = (((value >> 40) & 0xff) * Prime2Unsigned<uint16_t>()) + hash;
  hash = (((value >> 48) & 0xff) * Prime2Unsigned<uint16_t>()) + hash;
  hash = (((value >> 56) & 0xff) * Prime2Unsigned<uint16_t>()) + hash;
  return hash;
}

}  // namespace _
#endif  //> #if SEAM >= SEAM_0_0_0__04

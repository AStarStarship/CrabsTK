/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/crabs_text.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
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
// Dependencies:
#include "hex.h"
#if CRABS_UTF
// End dependencies.

namespace _ {

byte HexNibbleToLowerCase(byte b) {
  b = b & 0xf;
  if (b > 9) return b + ('a' - 10);
  return b + '0';
}

byte HexNibbleToUpperCase(byte b) {
  b = b & 0xf;
  if (b > 9) return b + ('A' - 10);
  return b + '0';
}

uint16_t HexByteToLowerCase(byte b) {
  uint16_t value = HexNibbleToLowerCase(b & 0xf);
  value = value << 8;
  value |= HexNibbleToLowerCase(b >> 4);
  return value;
}

uint16_t HexByteToUpperCase(byte b) {
  uint16_t value = HexNibbleToUpperCase(b & 0xf);
  value = value << 8;
  uint16_t second_nibble = HexNibbleToUpperCase(b >> 4);
  value |= second_nibble;
  return value;
}

int HexToByte(byte c) {
  if (c < '0') {
    return -1;
  }
  if (c >= 'a') {
    if (c > 'f') return -1;
    return c - ('a' - 10);
  }
  if (c >= 'A') {
    if (c > 'F') return -1;
    return c - ('A' - 10);
  }
  if (c > '9') return -1;
  return c - '0';
}

int HexToByte(uint16_t h) {
  int lowerValue = HexToByte((byte)(h >> 8));

  if (lowerValue < 0) return -1;

  int upper_value = HexToByte((byte)h);
  if (upper_value < 0) return -1;

  return lowerValue | (upper_value << 4);
}

}  // namespace _
#undef PRINTF
#undef PRINT
#endif  //< CRABS_UTF
#endif  //< #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2

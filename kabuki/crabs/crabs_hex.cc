/* Kabuki Toolkit
    @version 0.x
    @file    ~/kabuki-toolkit/kabuki/crabs/crabs_text.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough@gmail.com>;
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

#include <stdafx.h>

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 2
// Dependencies:
#include "printer.h"
#if USING_PRINTER
#include "text.h"
#include "type.h"
// End dependencies.
#if MAJOR_SEAM == 1 && MINOR_SEAM == 2
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PUTCHAR(c) putchar(c);
#define PRINT_HEADING \
  std::cout << '\n';  \
  for (int i = 80; i > 0; --i) std::cout << '-';
#else
#define PRINTF(x, ...)
#define PUTCHAR(c)
#define PRINT_HEADING
#endif

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
  PRINTF("\n    First nibble:%c", value)
  value = value << 8;
  uint16_t second_nibble = HexNibbleToUpperCase(b >> 4);
  PRINTF(" second nibble:%c", second_nibble)
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

char* PrintHex(char* cursor, char* buffer_end, uint8_t value) {
  enum { kHexStringLengthSizeMax = sizeof(uint8_t) * 2 + 3 };

  ASSERT(cursor);
  if (cursor + kHexStringLengthSizeMax >= buffer_end) {
    return nullptr;
  }

  *cursor++ = '0';
  *cursor++ = 'x';
  for (int num_bits_shift = sizeof(uint8_t) * 8 - 4; num_bits_shift >= 0;
       num_bits_shift -= 4) {
    *cursor++ = HexNibbleToUpperCase((byte)(value >> num_bits_shift));
  }
  *cursor = 0;
  return cursor;
}

char* PrintHex(char* cursor, char* buffer_end, uint16_t value) {
  enum { kHexStringLengthSizeMax = sizeof(uint16_t) * 2 + 3 };

  ASSERT(cursor);
  if (cursor + kHexStringLengthSizeMax >= buffer_end) {
    return nullptr;
  }

  *cursor++ = '0';
  *cursor++ = 'x';
  for (int num_bits_shift = sizeof(uint16_t) * 8 - 4; num_bits_shift >= 0;
       num_bits_shift -= 4) {
    *cursor++ = HexNibbleToUpperCase((byte)(value >> num_bits_shift));
  }
  *cursor = 0;
  return cursor;
}

char* PrintHex(char* cursor, char* buffer_end, uint32_t value) {
  enum { kHexStringLengthSizeMax = sizeof(uint32_t) * 2 + 3 };

  ASSERT(cursor);
  if (cursor + kHexStringLengthSizeMax >= buffer_end) {
    return nullptr;
  }

  *cursor++ = '0';
  *cursor++ = 'x';
  for (int num_bits_shift = sizeof(uint32_t) * 8 - 4; num_bits_shift >= 0;
       num_bits_shift -= 4) {
    *cursor++ = HexNibbleToUpperCase((byte)(value >> num_bits_shift));
  }
  *cursor = 0;
  return cursor;
}

char* PrintHex(char* cursor, char* end, uint64_t value) {
  enum { kHexStringLengthSizeMax = sizeof(uint64_t) * 2 + 3 };

  ASSERT(cursor);
  if ((end - cursor) < kHexStringLengthSizeMax) {
    return nullptr;
  }

  *cursor++ = '0';
  *cursor++ = 'x';
  for (int num_bits_shift = sizeof(uint64_t) * 8 - 4; num_bits_shift >= 0;
       num_bits_shift -= 4) {
    *cursor++ = HexNibbleToUpperCase((byte)(value >> num_bits_shift));
  }
  *cursor = 0;
  return cursor;
}

char* PrintHex(char* buffer, char* end, char c) {
  enum { kHexStringLengthSizeMax = sizeof(void*) * 2 + 3 };

  ASSERT(buffer);
  if (buffer >= end) {
    return nullptr;
  }
  ASSERT((end - buffer > 2))

  *buffer++ = HexNibbleToUpperCase(c);
  *buffer = 0;
  return buffer;
}

}       //< namespace _
#undef PRINTF
#undef PUTCHAR
#endif  //< USING_PRINTER
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3

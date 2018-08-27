/* Kabuki Toolkit @version 0.x
@file    $kabuki-toolkit/kabuki/crabs/decimal_32.h
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
#include <stdafx.h>

#ifndef INCLUDED_CRABS_BINARY_32
#define INCLUDED_CRABS_BINARY_32

#include "config.h"

#if WORD_SIZE == 32
#if SEAM == SEAM_0_0_0
#include "decimal.h"

namespace _ {
inline void PrintBinaryTable(uint32_t value) {
  enum { kSize = sizeof(uint32_t) * 8 };

  Print("\n    ");
  for (int i = kSize; i > 0; --i) {
    char c = (char)('0' + (value >> (kSize - 1)));
    Print(c);
    value = value << 1;
  }

  Print(
      "\n    bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "\n    33222222222211111111110000000000"
      "\n    10987654321098765432109876543210"
      "\n    ||  |  |   |  |  |   |  |  |   |"
      "\n    |1  0  0   0  0  0   0  0  0   0"
      "\n    |0  9  8   7  6  5   4  3  2   1");
}
}  // namespace _

#define PRINT(item) Print(item)
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT_PRINTED                                                   \
  ;                                                                     \
  sprintf_s(buffer, 24, "%u", value);                                   \
  *end = 0;                                                             \
  Printf("\n    Printed \"%s\" leaving value:\"%s\":%u", begin, buffer, \
         (uint)strlen(buffer))
#define PRINT_BINARY \
  Print("\n    ");   \
  PrintBinary(value);
#define PRINT_BINARY_TABLE PrintBinaryTable(value);
#define PRINT_HEADER                   \
  for (int i = 0; i < 10; ++i) {       \
    *(cursor + i) = 'x';               \
  }                                    \
  *(cursor + 21) = 0;                  \
  Char* begin = cursor;                \
  Char buffer[256];                    \
  sprintf_s(buffer, 256, "%u", value); \
  Printf("Expecting %s:%u", buffer, (uint)strlen(buffer));
#define PRINT_HEADING \
  Print('\n');        \
  for (int i = 80; i > 0; --i) Print('-')
#else
#define PRINT(item)
#define PRINTF(x, ...)
#define PRINT_PRINTED ;
#define PRINT_HEADER
#define PRINT_HEADING
#endif

/* Writes the give value to the given buffer as an ASCII string.
@return Nil upon buffer overflow and pointer to the nil-term char upon success.
@param  cursor The beginning address of the buffer.
@param  end    The end address of the buffer.
@param  value  The value to write. */
template <typename Char = char>
Char* Print(Char* cursor, Char* end, uint32_t value) {
  PRINT_HEADING;

  static const Char kMsbShift[] = {
      4, 7, 11, 14, 17, 21, 24, 27, 30,
  };

  if (!cursor) {
    return nullptr;
  }
  if (cursor >= end) {
    return nullptr;
  }

  uint16_t* text16;
  Char digit;
  uint32_t scalar;
  uint16_t digits1and2, digits3and4, digits5and6, digits7and8;
  uint32_t comparator;

  PRINT_HEADER;

  if (value < 10) {
    PRINT("\n    Range:[0, 9] length:1 ");
    if (cursor + 1 >= end) {
      return nullptr;
    }
    *cursor++ = '0' + (Char)value;
    PRINT_PRINTED;
    *cursor = 0;
    return cursor;
  }
  if (value < 100) {
    PRINT("\n    Range:[10, 99] length:2 ");
    if (cursor + 2 >= end) {
      return nullptr;
    }
    *reinterpret_cast<uint16_t*>(cursor) = kDigits00To99[value];
    PRINT_PRINTED;
    *(cursor + 2) = 0;
    return cursor + 2;
  }
  if (value >> 14) {
    if (value >> 27) {
      if (value >> 30) {
        PRINT("\n    Range:[1073741824, 4294967295] length:10");
      Print10:
        if (cursor + 10 >= end) {
          return nullptr;
        }
        comparator = 100000000;
        digits1and2 = (uint16_t)(value / comparator);
        PRINTF("\n    digits1and2:%u", digits1and2);
        value -= digits1and2 * comparator;
        *reinterpret_cast<uint16_t*>(cursor) = kDigits00To99[digits1and2];
        PRINT_PRINTED;
        cursor += 2;
        goto Print8;
      } else {
        comparator = 1000000000;
        if (value >= comparator) {
          PRINT("\n    Range:[100000000, 1073741823] length:10");
          goto Print10;
        }
        PRINT("\n    Range:[134217727, 999999999] length:9");
        if (cursor + 9 >= end) {
          return nullptr;
        }
        comparator = 100000000;
        digit = (Char)(value / comparator);
        *cursor++ = digit + '0';
        PRINT_PRINTED;
        value -= comparator * digit;
        goto Print8;
      }
    } else if (value >> 24) {
      comparator = 100000000;
      if (value >= comparator) {
        PRINT("\n    Range:[100000000, 134217728] length:9");
        if (cursor + 9 >= end) {
          return nullptr;
        }
        *cursor++ = '1';
        PRINT_PRINTED;
        value -= comparator;
      }
      PRINT("\n    Range:[16777216, 9999999] length:8");
      if (cursor + 8 >= end) {
        return nullptr;
      }
    Print8:
      PRINTF("\n    Print8:");
      scalar = 10000;
      digits5and6 = (uint16_t)(value / scalar);
      digits1and2 = value - scalar * digits5and6;
      digits7and8 = digits5and6 / 100;
      digits3and4 = digits1and2 / 100;
      digits5and6 -= 100 * digits7and8;
      digits1and2 -= 100 * digits3and4;
      *reinterpret_cast<uint16_t*>(cursor + 6) = kDigits00To99[digits1and2];
      PRINT_PRINTED;
      *reinterpret_cast<uint16_t*>(cursor + 4) = kDigits00To99[digits3and4];
      PRINT_PRINTED;
      *reinterpret_cast<uint16_t*>(cursor + 2) = kDigits00To99[digits5and6];
      PRINT_PRINTED;
      *reinterpret_cast<uint16_t*>(cursor) = kDigits00To99[digits7and8];
      PRINT_PRINTED;
      *(cursor + 8) = 0;
      return cursor + 8;
    } else if (value >> 20) {
      comparator = 10000000;
      if (value >= comparator) {
        PRINT("\n    Range:[10000000, 16777215] length:8");
        if (cursor + 8 >= end) {
          return nullptr;
        }
        *cursor++ = '1';
        PRINT_PRINTED;
        value -= comparator;
      } else {
        PRINT("\n    Range:[1048576, 9999999] length:7");
        if (cursor + 7 >= end) {
          return nullptr;
        }
      }
      scalar = 10000;
      digits5and6 = (uint16_t)(value / scalar);
      digits1and2 = value - scalar * digits5and6;
      digits7and8 = digits5and6 / 100;
      digits3and4 = digits1and2 / 100;
      digits5and6 -= 100 * digits7and8;
      digits1and2 -= 100 * digits3and4;
      *reinterpret_cast<uint16_t*>(cursor + 5) = kDigits00To99[digits1and2];
      PRINT_PRINTED;
      *reinterpret_cast<uint16_t*>(cursor + 3) = kDigits00To99[digits3and4];
      PRINT_PRINTED;
      *reinterpret_cast<uint16_t*>(cursor + 1) = kDigits00To99[digits5and6];
      PRINT_PRINTED;
      *cursor = (Char)digits7and8 + '0';
      *(cursor + 7) = 0;
      return cursor + 7;
    } else if (value >> 17) {
      comparator = 1000000;
      if (value >= comparator) {
        PRINT("\n    Range:[100000, 1048575] length:7");
        if (cursor + 7 >= end) {
          return nullptr;
        }
        *cursor++ = '1';
        PRINT_PRINTED;
        value -= comparator;
      } else {
        PRINT("\n    Range:[131072, 999999] length:6");
        if (cursor + 6 >= end) {
          return nullptr;
        }
      }
    Print6:
      scalar = 10000;
      digits5and6 = (uint16_t)(value / scalar);
      digits1and2 = value - scalar * digits5and6;
      digits7and8 = digits5and6 / 100;
      digits3and4 = digits1and2 / 100;
      digits5and6 -= 100 * digits7and8;
      digits1and2 -= 100 * digits3and4;
      text16 = reinterpret_cast<uint16_t*>(cursor + 6);
      *reinterpret_cast<uint16_t*>(cursor + 4) = kDigits00To99[digits1and2];
      PRINT_PRINTED;
      *reinterpret_cast<uint16_t*>(cursor + 2) = kDigits00To99[digits3and4];
      PRINT_PRINTED;
      *reinterpret_cast<uint16_t*>(cursor) = kDigits00To99[digits5and6];
      PRINT_PRINTED;
      *(cursor + 6) = 0;
      return cursor + 6;
    } else {  // (value >> 14)
      if (value >= 100000) {
        PRINT("\n    Range:[65536, 131071] length:6");
        goto Print6;
      }
      PRINT("\n    Range:[10000, 65535] length:5");
      if (cursor + 5 >= end) {
        return nullptr;
      }
      digits5and6 = 10000;
      digit = (uint8_t)(value / digits5and6);
      value -= digits5and6 * digit;
      *cursor = digit + '0';
      PRINT_PRINTED;
      digits1and2 = (uint16_t)value;
      digits5and6 = 100;
      digits3and4 = digits1and2 / digits5and6;
      digits1and2 -= digits3and4 * digits5and6;
      *reinterpret_cast<uint16_t*>(cursor + 1) = kDigits00To99[digits3and4];
      PRINT_PRINTED;
      PRINTF("\n    digits1and2:%u", digits1and2);
      *reinterpret_cast<uint16_t*>(cursor + 3) = kDigits00To99[digits1and2];
      PRINT_PRINTED;
      *(cursor + 5) = 0;
      return cursor + 5;
    }
  }
  digits1and2 = (uint16_t)value;
  if (value >> 10) {
    digits5and6 = 10000;
    if (digits1and2 >= digits5and6) {
      if (cursor + 5 >= end) {
        return nullptr;
      }
      PRINT("\n    Range:[10000, 16383] length:5");
      *cursor++ = '1';
      PRINT_PRINTED;
      digits1and2 -= digits5and6;

    } else {
      PRINT("\n    Range:[1024, 9999] length:4");
      if (cursor + 4 >= end) {
        return nullptr;
      }
    }
    digits5and6 = 100;
    digits3and4 = digits1and2 / digits5and6;
    digits1and2 -= digits3and4 * digits5and6;
    *reinterpret_cast<uint16_t*>(cursor) = kDigits00To99[digits3and4];
    PRINT_PRINTED;
    *reinterpret_cast<uint16_t*>(cursor + 2) = kDigits00To99[digits1and2];
    PRINT_PRINTED;
    *(cursor + 4) = 0;
    return cursor + 4;
  } else {
    if (cursor + 4 >= end) {
      return nullptr;
    }
    digits3and4 = 1000;
    if (digits1and2 >= digits3and4) {
      PRINT("\n    Range:[1000, 1023] length:4");
      digits1and2 -= digits3and4;
      text16 = reinterpret_cast<uint16_t*>(cursor + 2);
      *text16-- = kDigits00To99[digits1and2];
      PRINT_PRINTED;
      *text16 = (((uint16_t)'1') | (((uint16_t)'0') << 8));
      PRINT_PRINTED;
      *(cursor + 4) = 0;
      return cursor + 4;
    }
    PRINT("\n    Range:[100, 999] length:3");
    digits1and2 = (uint16_t)value;
    digits3and4 = 100;
    digit = (Char)(digits1and2 / digits3and4);
    digits1and2 -= digit * digits3and4;
    *cursor = digit + '0';
    PRINT_PRINTED;
    *reinterpret_cast<uint16_t*>(cursor + 1) = kDigits00To99[digits1and2];
    PRINT_PRINTED;
    *(cursor + 3) = 0;
    return cursor + 3;
  }
}
#endif  //< #if WORD_SIZE == 32
#endif  //< INCLUDED_CRABS_BINARY_32

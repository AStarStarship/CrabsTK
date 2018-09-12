/** Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/source/crabs/print_itos.cc
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

#if SEAM >= SEAM_0_0_1
#include "itos_algorithms.h"

#include "../../kabuki/f2/tbinary.h"

#include "debug_itos_header.h"

namespace _ {

char* Print(uint32_t value, char* text, char* text_end) {
  // Lookup table for powers of 10.
  static const uint32_t k10ToThe[]{1,         10,         100,         1000,
                                   10000,     100000,     1000000,     10000000,
                                   100000000, 1000000000, ~(uint32_t)0};

  /** Lookup table of ASCII char pairs for 00, 01, ..., 99.
      To convert this algorithm to big-endian, flip the digit pair bytes. */
  static const uint16_t kDigits00To99[100] = {
      0x3030, 0x3130, 0x3230, 0x3330, 0x3430, 0x3530, 0x3630, 0x3730, 0x3830,
      0x3930, 0x3031, 0x3131, 0x3231, 0x3331, 0x3431, 0x3531, 0x3631, 0x3731,
      0x3831, 0x3931, 0x3032, 0x3132, 0x3232, 0x3332, 0x3432, 0x3532, 0x3632,
      0x3732, 0x3832, 0x3932, 0x3033, 0x3133, 0x3233, 0x3333, 0x3433, 0x3533,
      0x3633, 0x3733, 0x3833, 0x3933, 0x3034, 0x3134, 0x3234, 0x3334, 0x3434,
      0x3534, 0x3634, 0x3734, 0x3834, 0x3934, 0x3035, 0x3135, 0x3235, 0x3335,
      0x3435, 0x3535, 0x3635, 0x3735, 0x3835, 0x3935, 0x3036, 0x3136, 0x3236,
      0x3336, 0x3436, 0x3536, 0x3636, 0x3736, 0x3836, 0x3936, 0x3037, 0x3137,
      0x3237, 0x3337, 0x3437, 0x3537, 0x3637, 0x3737, 0x3837, 0x3937, 0x3038,
      0x3138, 0x3238, 0x3338, 0x3438, 0x3538, 0x3638, 0x3738, 0x3838, 0x3938,
      0x3039, 0x3139, 0x3239, 0x3339, 0x3439, 0x3539, 0x3639, 0x3739, 0x3839,
      0x3939,
  };

  static const char kMsbShift[] = {
      4, 7, 11, 14, 17, 21, 24, 27, 30,
  };

  if (!text) {
    return nullptr;
  }
  if (text >= text_end) {
    return nullptr;
  }

  const uint32_t* power_of_ten;
  uint16_t* text16;
  char index,  //< Temp multi-purpose variable.
      length;
  uint16_t digits;  //<
  uint32_t comparator, offset;

#if DEBUG
  // Write a bunches of xxxxxx to the buffer for debug purposes.
  for (int i = 0; i <= 21; ++i) {
    *(text + i) = 'x';
  }
  *(text + 21) = 0;
  char* begin = text;
  char buffer[256];
  uint32_t smallest_value, largest_value;
  double range, lower_probability, upper_probability;

  static const char kRangeShift[] = {
      4, 7, 10, 14, 17, 20, 24, 27, 30,
  };
#endif

  if (value < 10) {
    PRINTF("\n    Range:[0, 9] length:1 ");
    if (text + 1 >= text_end) {
      return nullptr;
    }
    *text++ = '0' + (char)value;
    PRINT_PRINTED;
    return text;
  }
  if (value < 100) {
    PRINTF("\n    Range:[10, 99] length:2 ");
    if (text + 2 >= text_end) {
      return nullptr;
    }
    *reinterpret_cast<uint16_t*>(text) = kDigits00To99[value];
    PRINT_PRINTED;
    return text + 2;
  }
  digits = value % 100;
  if (value >> 17) {
    if (value >> 24) {
      if (value >> 30) {
        power_of_ten = &k10ToThe[10];
        length = 10;
      } else if (value >> 27) {
        power_of_ten = &k10ToThe[9];
        length = 9;
      } else {  // if (value >> 24) {
        power_of_ten = &k10ToThe[8];
        length = 8;
      }
    } else if (value >> 20) {
      power_of_ten = &k10ToThe[7];
      length = 7;
    } else {  // if (value >> 17) {
      power_of_ten = &k10ToThe[6];
      length = 6;
    }
  } else if (value >> 14) {
    power_of_ten = &k10ToThe[5];
    length = 5;
  } else if (value >> 10) {
    power_of_ten = &k10ToThe[4];
    comparator = *power_of_ten;
#if DEBUG
    length = 4;
    smallest_value = 1 << (kRangeShift[length - 2]);
    largest_value = (~(uint32_t)0) >> (32 - (kRangeShift[length - 1]));

    PRINTF("\n    B. Range:[%u, %u]:%u comparator:%u", smallest_value,
           largest_value, largest_value - smallest_value, comparator);

    range = (double)(largest_value - smallest_value);
    lower_probability = 100.0 * ((double)(comparator - smallest_value)) / range;
    upper_probability = 100.0 * ((double)(largest_value - comparator)) / range;
#endif
    if (value >= comparator) {
      PRINTF("\n    B. Range:[%u, %u] length:%u probablity:%f%%", comparator,
             (1 << (kRangeShift[length - 1])) - 1, (uint)length,
             lower_probability);
      *text++ = '1';
      PRINT_PRINTED;
      value -= comparator;
    } else {
      PRINTF("\n    B. Range:[%u, %u] length:%u probablity:%f%%",
             1 << (kRangeShift[length - 2]), comparator, (uint)length,
             lower_probability)
    }
    text16 = reinterpret_cast<uint16_t*>(text + 2);
    *text16-- = kDigits00To99[digits];
    *text16 = kDigits00To99[value / 100];
    PRINT_PRINTED;
    return text + 4;
  } else {
    offset = 1000;
    if (value >= offset) {
      PRINTF("\n    Range:[1000, 1024] length:4");
      if (text + 4 >= text_end) {
        return nullptr;
      }
      text16 = reinterpret_cast<uint16_t*>(text + 2);
      *text16-- = kDigits00To99[digits];
      *text16 = (((uint16_t)'1') | (((uint16_t)'0') << 8));
      PRINT_PRINTED;
      return text + 4;
    }
    PRINTF("\n    Range:[100, 999] length:3");
    if (text + 3 >= text_end) {
      return nullptr;
    }
    *reinterpret_cast<uint16_t*>(text + 1) = kDigits00To99[digits];
    value /= 100;
    *text = (char)value + '0';
    PRINT_PRINTED;
    return text + 3;
  }

  comparator = *power_of_ten--;

#if DEBUG
  smallest_value = 1 << (kRangeShift[length - 2]);
  largest_value = (~(uint32_t)0) >> (32 - (kRangeShift[length - 1]));

  Printf("\n    Range:[%u, %u]:%u comparator:%u", smallest_value, largest_value,
         largest_value - smallest_value, comparator);

  range = (double)(largest_value - smallest_value);
  lower_probability = 100.0 * ((double)(comparator - smallest_value)) / range;
  upper_probability = 100.0 * ((double)(largest_value - comparator)) / range;
#endif

  if (value >= comparator) {
    if (text >= text_end) {
      return nullptr;
    }
    text_end = text + length + 1;
    text16 = reinterpret_cast<uint16_t*>(text_end - 2);
    PRINTF("\n    Range:[%u, %u] length:%u probablity:%f%%", comparator,
           (1 << (kRangeShift[length - 1])) - 1, (uint)length,
           lower_probability)
    value -= comparator;
    *text++ = '1';
    *text16-- = kDigits00To99[digits];
    PRINT_PRINTED;
    offset = *power_of_ten;
    if (value < offset) {
      value /= 100;
      PRINTF("\n    A.) Found a 0");
      if (length & 1) {
        PRINTF(" and printed a 0");
        length -= 3;
        *text = '0';
        PRINT_PRINTED
        goto PrintLoop;
      }
      length -= 2;
      goto PrintLoop;
    }
  } else {
    PRINTF("\n    Range:[%u, %u] length:%u probablity:%f%%",
           1 << (kRangeShift[length - 2]), comparator, (uint)length,
           lower_probability);
    if (text + length >= text_end) {
      return nullptr;
    }
    text_end = text + length;
    text16 = reinterpret_cast<uint16_t*>(text_end - 2);
    *text16-- = kDigits00To99[digits];
    PRINT_PRINTED;
  }
  value /= 100;
  length -= 2;
  if (length & 1) {
    PRINTF("\n\n    Converting MSD:%u with length:%u", value, (uint)length);
    index = kMsbShift[length - 2];
    PRINTF("\n    Shifting >> %u bits to equal %u", (uint)index,
           value >> index);
    PRINT_BINARY_TABLE;
    index = (value >> index) + 2;
    power_of_ten -= 2;
    offset = *power_of_ten--;
    comparator = index * offset;
    PRINTF("\n    value:%u comparator:%u offset:%u\n    Searching:", value,
           comparator, offset);
    while (comparator < value) {
      comparator += offset;
      ++index;
      PRINTF("\n    [%c]:%u", index + '0', comparator);
    }
    while (comparator > value) {
      comparator -= offset;
      --index;
      PRINTF("\n    [%c]:%u", index + '0', comparator);
    }
    value -= comparator;
    *text++ = index + '0';
    --length;
    PRINT_PRINTED;
    offset = *power_of_ten--;
    if (value < offset) {
      PRINTF("\n    B.) Found a 0");
      if (length & 1) {
        PRINTF(" and printed a 0");
        *text = '0';
        PRINT_PRINTED;
        goto PrintLoop;
      }
      goto PrintLoop;
    }
  }
PrintLoop:
  PRINTF("\n\n    Entering modulo 100 loop with value:%u and length:%u", value,
         (uint)length);
  length -= 2;
  while (length > 0) {
    digits = kDigits00To99[value % 100];
    *text16-- = digits;
    value /= 100;
    length -= 2;
    PRINTF("\n    [%u]:%c%c value:%u", (uint)length, (char)digits,
           (char)(digits >> 8), value);
    PRINT_PRINTED;
  }
  digits = kDigits00To99[value];
  *text16 = digits;
  PRINTF("\n    [%u]:%c%c value:%u", (uint)length, (char)digits,
         (char)(digits >> 8), value);
  PRINT_PRINTED;
  return text_end;
}

}  // namespace _

#include "debug_itos_footer.h"
#endif  //< #if SEAM >= SEAM_0_0_1

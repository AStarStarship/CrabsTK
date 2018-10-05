/** Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/benchmarks/0_0_benchmark_f2/print_mlsd.cc
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

#if SEAM >= SEAM_00_00_00__01
#include "itos_algorithms.h"

#include "debug_itos_header.h"

namespace _ {

char* PrintMsld(uint32_t value, char* text, char* text_end) {
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
      4, 7, 10, 14, 17, 20, 24, 27, 30,
  };

  if (!text || text >= text_end) {
    return nullptr;
  }

  const uint32_t* power_of_ten;
  uint16_t* text16;
  char index,  //< Temp multi-purpose variable.
      length;
  uint16_t digits;  //<
  uint32_t comparator, offset;

#if SEAM == SEAM_0_0_0
  // Write a bunches of xxxxxx to the buffer for debug purposes.
  for (int i = 0; i <= 21; ++i) {
    *(text + i) = 'x';
  }
  *(text + 21) = 0;
  char* begin = text;
  char buffer[256];
#endif
  if (value < 10) {
    PRINTF("\n    Range:[0, 9] length:1 ");
    if (text + 1 >= text_end) return nullptr;
    *text++ = '0' + (char)value;
    PRINT_PRINTED;
    return text;
  }
  if (value < 100) {
    PRINTF("\n    Range:[10, 99] length:2 ");
    if (text + 2 >= text_end) return nullptr;
    *reinterpret_cast<uint16_t*>(text) = kDigits00To99[value];
    PRINT_PRINTED;
    return text + 2;
  }
  if (!(value >> 10)) {
    offset = 1000;
    if (value >= offset) {
      PRINTF("\n    Range:[1000, 1023] length:4 ");
      if (text + 4 >= text_end) return nullptr;
      *text++ = '1';
      *text++ = '0';
      value -= offset;
      *reinterpret_cast<uint16_t*>(text) = kDigits00To99[value];
      PRINT_PRINTED;
      return text + 2;
    }
    PRINTF("\n    Range:[100, 999] length:3 ");
    if (text + 3 >= text_end) return nullptr;
    text_end = text + 3;
  Print3:
    PRINTF("\n    Print3:%u", value);
    PRINT_BINARY_TABLE;
    index = 2 + (char)(value >> 7);
    offset = 100;
    comparator = index * offset;
    PRINTF("\n    [%c] comparator:%u", index + '0', comparator);
    while (comparator < value) {
      comparator += offset;
      ++index;
      PRINTF("\n    [%c] comparator:%u", index + '0', comparator);
    }
    while (comparator > value) {
      comparator -= offset;
      --index;
      PRINTF("\n    [%c] comparator:%u", index + '0', comparator);
    }
    PRINTF("\n    index:%u", index + '0');
    *text++ = index + '0';
    *reinterpret_cast<uint16_t*>(text) = kDigits00To99[value - comparator];
    PRINT_PRINTED;
    return text_end;
  }
  digits = value % 100;
  if (value >> 17) {
    if (value >> 24) {
      if (value >> 30) {
        power_of_ten = &k10ToThe[10];
        length = 10;
        index = 2 + (char)(value >> 30);
      } else if (value >> 27) {
        power_of_ten = &k10ToThe[9];
        length = 9;
        index = 2 + (char)(value >> 27);
      } else {  // if (value >> 24) {
        power_of_ten = &k10ToThe[8];
        length = 8;
        index = 2 + (char)(value >> 24);
      }
    } else if (value >> 20) {
      power_of_ten = &k10ToThe[7];
      length = 7;
      index = 2 + (char)(value >> 21);
    } else {  // if (value >> 17) {
      if (text + 6 >= text_end) return nullptr;
      text16 = reinterpret_cast<uint16_t*>(text + 4);
      *text16-- = kDigits00To99[value % 100];
      value /= 100;
      *text16-- = kDigits00To99[value % 100];
      *text16-- = kDigits00To99[value / 100];
      return text + 6;
    }
  } else if (value >> 14) {
    comparator = k10ToThe[5];
    if (value >= comparator) {
      if (text + 6 >= text_end) return nullptr;
      text_end = text + 7;
      PRINTF("\n    Range:[100000, 131071] length:6");
      value -= comparator;
      *text++ = '1';
      PRINT_PRINTED;
      comparator = k10ToThe[4];
      if (value < comparator) {
        *text++ = '0';
        PRINT_PRINTED;
        if (value < 1000) goto PrintZeros4;
      }
    } else {
      if (text + 5 >= text_end) return nullptr;
      text_end = text + 7;
      PRINTF("\n    Range:[16384, 100000] length:5");
    }
    *reinterpret_cast<uint16_t*>(text + 3) = kDigits00To99[digits];
    PRINT_PRINTED;
    value /= 100;
    text_end = text + 5;
    goto Print3;
  } else {  // (value >> 10) > 0
    power_of_ten = &k10ToThe[4];
    comparator = *power_of_ten;
    if (value >= comparator) {
      if (text + 5 >= text_end) return nullptr;
      text_end = text + 6;
      PRINTF("\n    Range:[10000, 16383] length:5");
      value -= comparator;
      *text++ = '1';
      PRINT_PRINTED;
      if (value < 1000) {
      // 1 Zero  Example: 99999-90000 = 9999
      // 2 Zeros Example: 90999-90000 = 0999
      // 3 Zeros Example: 90099-90000 = 0099
      // 4 Zeros Example: 90009-90000 = 0009
      PrintZeros4:
        PRINTF("\n    PrintZeros4:");
        *text++ = '0';
        PRINT_PRINTED;
        if (value < 100) {
          *text++ = '0';
          *reinterpret_cast<uint16_t*>(text) = kDigits00To99[value];
          PRINT_PRINTED;
          return text + 2;
        }
        goto Print3;
      }
    } else {
      if (text + 4 >= text_end) return nullptr;
      text_end = text + 5;
      PRINTF("\n    Range:[1024, 9999] length:4");
    }
    text_end = text + 4;
  Print4:
    text16 = reinterpret_cast<uint16_t*>(text + 2);
    PRINTF("\n    Print4:%u", value);
    *text16-- = kDigits00To99[value % 100];
    *text16 = kDigits00To99[value / 100];
    PRINT_PRINTED;
    return text_end;
  }
  comparator = *power_of_ten--;
  offset = *power_of_ten--;
  if (value >= comparator) {
    if (text >= text_end) return nullptr;
    text_end = text + length + 1;
    PRINTF("\n    Range:[%u, %u] length:%u offset:%u", comparator,
           (1 << (kMsbShift[length - 1])) - 1, (uint)length, offset);
    value -= comparator;
    *text++ = '1';
    PRINT_PRINTED;
    if (value < offset) {
      PRINTF("\n\n    Converting first MSD:0");
      *text++ = '0';
      --length;
      offset = *power_of_ten--;
      text16 = reinterpret_cast<uint16_t*>(text_end - 2);
      PRINT_PRINTED;
      ++power_of_ten;
      goto NextMsd;
    }
  } else {
    PRINTF("\n    Range:[%u, %u] length:%u comparator:%u offset:%u",
           1 << (kMsbShift[length - 2]), comparator, (uint)length, comparator,
           offset);
    if (text + length >= text_end) return nullptr;
    text_end = text + length;
  }
  value /= 100;
  text16 = reinterpret_cast<uint16_t*>(text_end - 2);
  offset = *(power_of_ten - 1);
  power_of_ten -= 2;
  comparator = index * offset;
  *text16-- = kDigits00To99[digits];
  length -= 2;
  PRINT_PRINTED;
  PRINTF("\n\n    Converting first MSD");
BeginConversion:
  digits = value % 100;
  PRINTF("\n    [%c] comparator:%u offset:%u", index + '0', comparator, offset);
  while (comparator < value) {
    comparator += offset;
    ++index;
    PRINTF("\n    [%c] comparator:%u", index + '0', comparator);
  }
  while (comparator > value) {
    comparator -= offset;
    --index;
    PRINTF("\n    [%c] comparator:%u", index + '0', comparator);
  }
  value -= comparator;
  PRINTF(". value is now:%u", value);
  *text++ = index + '0';
  --length;
  PRINT_PRINTED;

NextMsd:
  value /= 100;
  *text16-- = kDigits00To99[digits];
  length -= 2;
  PRINT_PRINTED;
  power_of_ten -= 2;
  offset = *power_of_ten--;
  PRINTF("\n    After value /= 100 offset is now %u and length is %u", offset,
         (uint)length);
  if (value < offset) {
    PRINTF("\n    B.) Found 0 with value %u and offset %u", value, offset);
    *text++ = '0';
    --length;
    PRINT_PRINTED;
    offset = *power_of_ten--;
    if (value < offset) goto PrintZeros;
    PRINTF("\n    Did not find another zero.");
  } else {
    PRINT_PRINTED;
    PRINTF("\n\n    Converting second MSD");
    index = kMsbShift[length - 2];
    PRINT_BINARY_TABLE;
    PRINTF("\n    Shifting %u >> %u bits to equal %u", value, (uint)index,
           (uint)(value >> index));
    index = 2 + (char)(value >> index);
    comparator = index * offset;
    PRINTF("\n    [%c] comparator:%u offset:%u", index + '0', comparator,
           offset);
    while (comparator < value) {
      comparator += offset;
      ++index;
      PRINTF("\n    [%c] comparator:%u", index + '0', comparator);
    }
    while (comparator > value) {
      comparator -= offset;
      --index;
      PRINTF("\n    [%c] comparator:%u", index + '0', comparator);
    }
    *text++ = index + '0';
    --length;
    value -= comparator;
    PRINT_PRINTED;
    offset = *power_of_ten--;
    if (value < offset) {
    PrintZeros:
      PRINTF("\n    PrintZeros:%u offset %u", value, offset);
      *text++ = '0';
      --length;
      PRINT_PRINTED;

      if (value == 0) {
        while (length-- > 0) {
          *text++ = '0';
        }
        return text_end;
      }
      if (length - 1 == 0) {
        *text = (char)value + '0';
        PRINT_PRINTED;
        return text_end;
      }
      if (length - 2 == 0) {
        *text16 = kDigits00To99[value];
        PRINT_PRINTED;
        return text_end;
      }
      if (length - 3 == 0) {
        if (value < 100) {
          *text = '0';
          *text16 = kDigits00To99[value];
          PRINT_PRINTED;
          return text_end;
        }
        goto Print3;
      }
      *text16-- = kDigits00To99[value % 100];
      *text16 = kDigits00To99[value / 100];
      PRINT_PRINTED;
      return text_end;
    }
  }
  if (length - 4 > 0) {
    PRINTF("\n\n    BeginConversion:%u", value);
    index = kMsbShift[length - 2];
    PRINT_BINARY_TABLE;
    PRINTF("\n    Shifting %u >> %u bits to equal %u", value, (uint)index,
           (uint)(value >> index));
    index = 2 + (char)(value >> index);
    comparator = index * offset;
    goto BeginConversion;
  }
  PRINTF("\n\n    Converting final %u decimals...", (uint)length);
  if (length - 4 == 0) goto Print4;
  if (length - 3 == 0) goto Print3;
  if (length - 1 == 0) {
    *text = (char)value + '0';
    return text_end;
  }
  // else (length == 2)
  // Print2:
  PRINTF("\n    Print2:");
  *text16 = kDigits00To99[value];
  PRINT_PRINTED;
  return text_end;
}

}  // namespace _

#include "debug_itos_footer.h"
#endif  //< #if SEAM >= SEAM_00_00_00__01

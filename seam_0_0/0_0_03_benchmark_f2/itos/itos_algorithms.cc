/** Kabuki Toolkit @version 0.x
@file     ~/tests/script_itos_competition.ccCopyright(c) 2014
@author  Florian Loitsch, Milo Yip (miloyip@gmail.com) and Cale McCollough
<cale.mccollough@gmail.com>
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

#include "debug_itos_header.h"

using namespace std;

#define PRINT_PAUSE(message) \
  // cout << '\n' << message << "               "; system ("PAUSE");

char* PrintNil(char* text, char* text_end, uint32_t value) {
  if (!text) {
    return nullptr;
  }
  if (text > text_end) {
    return nullptr;
  }
  return text;
}

char* PrintSprintf(char* text, char* text_end, uint32_t value) {
  char buffer[24];
  if (!text) {
    return nullptr;
  }
  if (text > text_end) {
    return nullptr;
  }
  sprintf_s(buffer, 24, "%u", value);
  return text;
}

char* PrintMod10(char* text, char* text_end, uint32_t value) {
  static const char kDigits00To99[201] = {
      "00010203040506070809"
      "10111213141516171819"
      "20212223242526272829"
      "30313233343536373839"
      "40414243444546474849"
      "50515253545556575859"
      "60616263646566676869"
      "70717273747576777879"
      "80818283848586878889"
      "90919293949596979899"};

  // Lookup table for powers of 10.
  static const uint32_t k10ToThe[10]{1,         10,        100,     1000,
                                     10000,     100000,    1000000, 10000000,
                                     100000000, 1000000000};

  if (value == 0) {
    *text = '0';
    return text + 1;
  }

  int size;
  if (value >= k10ToThe[4]) {
    if (value >= k10ToThe[7]) {
      if (value >= k10ToThe[9])
        size = 10;
      else if (value >= k10ToThe[8])
        size = 9;
      else
        size = 8;
    } else {
      // cout << "\n    value < k10ToThe[7]";
      if (value >= k10ToThe[6])
        size = 7;
      else if (value >= k10ToThe[5])
        size = 6;
      else
        size = 5;
    }
  } else {
    if (value >= 100) {
      if (value >= 1000)
        size = 4;
      else
        size = 3;
    } else {
      if (value >= 10)
        size = 2;
      else
        size = 1;
    }
  }

  if (text + size > text_end) {
    return nullptr;
  }
  text_end = text + size - 1;
  text = text_end;
  while (--size > 0) {
    char lsd = value % 10;
    value /= 10;
    *text-- = '0' + lsd;
  }
  return text_end + 1;
}

char* PrintMod100(uint32_t value, char* text, char* text_end) {
  static const char kDigits00To99[201] = {
      "00010203040506070809"
      "10111213141516171819"
      "20212223242526272829"
      "30313233343536373839"
      "40414243444546474849"
      "50515253545556575859"
      "60616263646566676869"
      "70717273747576777879"
      "80818283848586878889"
      "90919293949596979899"};

  // Lookup table for powers of 10.
  static const uint32_t k10ToThe[20]{1,         10,        100,     1000,
                                     10000,     100000,    1000000, 10000000,
                                     100000000, 1000000000};

  if (value == 0) {
    *text = '0';
    return text + 1;
  }

  int size;
  if (value >= k10ToThe[4]) {
    if (value >= k10ToThe[7]) {
      if (value >= k10ToThe[9])
        size = 10;
      else if (value >= k10ToThe[8])
        size = 9;
      else
        size = 8;
    } else {
      if (value >= k10ToThe[6])
        size = 7;
      else if (value >= k10ToThe[5])
        size = 6;
      else
        size = 5;
    }
  } else {
    if (value >= 100) {
      if (value >= 1000)
        size = 4;
      else
        size = 3;
    } else {
      if (value >= 10)
        size = 2;
      else
        size = 1;
    }
  }

  if (text + size > text_end) {
    return nullptr;
  }
  text_end = &text[size - 1];
  while (value >= 100) {
    int digits = value % 100;
    value /= 100;
    *(short*)(text_end - 1) = *(short*)(kDigits00To99 + 2 * digits);
    text_end -= 2;
  }
  while (value > 0) {
    *text_end-- = '0' + (value % 10);
    value /= 10;
  }
  return text + size;
}

/*
char* PrintMod100B (uint32_t value, char* text, char* text_end) {
    


    // Lookup table of ASCII char pairs for 00, 01, ..., 99.
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
        0x3939, };

    // Lookup table for powers of 10.
    static const uint32_t k10ToThe[20]{
        1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000,
        1000000000 };

    if (value == 0) {
        *text = '0';
        return text + 1;
    }

    int size;
    if (value >= k10ToThe[4]) {
        if (value >= k10ToThe[7]) {
            if (value >= k10ToThe[9])
                size = 10;
            else if (value >= k10ToThe[8])
                size = 9;
            else
                size = 8;
        }
        else {
            if (value >= k10ToThe[6])
                size = 7;
            else if (value >= k10ToThe[5])
                size = 6;
            else
                size = 5;
        }
    }
    else {
        if (value >= 100) {
            if (value >= 1000)
                size = 4;
            else
                size = 3;
        }
        else {
            if (value >= 10)
                size = 2;
            else
                size = 1;
        }
    }

    text += size;
    if (text > text_end) {
        return nullptr;
    }
    uint16_t* text16 = reinterpret_cast<uint16_t*> (text - 2);
    uint16_t digits;

    while (value >= 100) {
        digits = value % 100;
        value /= 100;
        *text16-- = kDigits00To99[digits];
    }
    if (value == 0) {
        return text + 1;
    }
    if (value == 0) {
        return text + 1;
    }
    *text16-- = kDigits00To99[digits];
    return text + size;
}
*/

void TestPrintMod100() {
  enum { kSize = 24 };

  printf("\n\nTesting random numbers...\n\n");

  // Setup C++1x random number generator.
  std::random_device rd;
  std::mt19937_64 eng(rd());
  std::uniform_int_distribution<uint32_t> distr;

  uint32_t value;
  char expecting[kSize], text[kSize];
  char* result;

  for (int i = 0; i < 0x00ffffff; ++i) {
    value = distr(eng);
    sprintf_s(expecting, 24, "%u", value);
    // cout << '\n' << i + 1 << ".) Expecting " << expecting
    //     << " with length "
    //     << strlen (expecting);
    result = PrintMod100(value, text, text + kSize);
    if (!result) {
      PRINT_PAUSE("An error occurred :-(")
      break;
    }
    if (strcmp(expecting, text)) {
      cout << "\n\nERROR: Expecting \"" << expecting
           << "\":" << strlen(expecting) << " and found \"" << text
           << "\":" << strlen(text);
      PRINT_PAUSE("\n");
    }
  }

  // cout << "\n Done testing ItoS :)\n\n";
}

#include "debug_itos_footer.h"
#endif  //< #if SEAM >= SEAM_0_0_1

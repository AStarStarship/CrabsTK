/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    \kabuki\f2\01\socket.h
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
#include <pch.h>

//#include <cmath>
//#include <random>
#include "../trng.h"

#include <kabuki/f2/tsocket.h>
#include <kabuki/f2/tstr.h>
#include <kabuki/f2/ttest.h>

#include "seam_header.inl"

namespace _ {

static const char* _0_0_0__01_ASCII_Strings_and_Socket(char* seam_log,
                                                       char* seam_end,
                                                       const char* args) {
#if SEAM >= SEAM_0_0_0__01
  DTEST_BEGIN;
  PRINTF("\n\nTesting Text...");
  enum {
    kCompareStringsCount = 5,
    kSize = 2048,
  };

  static const char* test_strings[kCompareStringsCount][2] = {
      {"?", ""},
      {"?", "?"},
      {"? ", "?"},
      {"Apples", "Apples"},
      {"Apples", "Apples"},
  };

  static const char* kCompareStrings[] = {
      "Testing",
      "Texting",
      "Testing@",
      "Texting@",
  };

  static const char kTestingString[] = "Testing one, two, three.";

  static const char* kStringsRightAligned[] = {
      "    Testing one, two, three.", "Test...", ".", "..", "...", "T...",
  };

  static const char kStringNumbers[] = "1234567890\0";

  static const char* kStringsCentered[] = {" 1234567890\0", " 1234567890 \0",
                                           "  1234567890 \0", "123...\0"};

  const char* end;
  char buffer[kSize + 1], buffer_b[kSize + 1];

  Utf8 utf(buffer, kSize);

  for (int i = 0; i < kCompareStringsCount; ++i) {
    end = Print(buffer, buffer + kSize, test_strings[i][0]);
    Test(end);

    end = StringEquals<>(buffer, test_strings[i][0]);
    Test(end);
  }

  static const char kTesting123[] = "Testing 1, 2, 3\0";

  PRINT_HEADING("    Testing Utf...");
  PRINTF("    Expecting \"%s\"...", kTesting123);

  utf.Set(buffer) << "Testing " << 1 << ", " << 2 << ", " << 3;

  AVOW(kTesting123, buffer);

  PRINT_HEADING("    Running HexTest...");
  for (int i = 0; i < 16; ++i) {
    int value = HexToByte(HexNibbleToLowerCase(i));
    Test(i, value);
    PRINTF("\n    %i.) %i", i, value);
    value = HexToByte(HexNibbleToUpperCase(i));
    PRINTF(" value is now:%i", value);
    Test(i, value);
  }

  for (int i = 0; i < 256; ++i) {
    uint16_t c = HexByteToLowerCase(i);
    PRINTF("\n    %i.) expecting: %x        HexByteToLowerCase:%c%c", i, i,
           (char)c, (char)(c >> 8));
    int value = HexToByte(c);
    PRINTF("        HexToByte:%i", value);
    Test(i, value);
    value = HexToByte(HexByteToUpperCase(i));
    PRINTF("     value is now:%i", value);
    Test(i, value);
  }

  PRINT_HEADING("    Testing string utils...");

  Test(!StringEquals<>(kCompareStrings[0], kCompareStrings[1]));
  Test(!StringEquals<>(kCompareStrings[0], kCompareStrings[3]));
  Test(StringEquals<>(kCompareStrings[0], kCompareStrings[0]));
  Test(!StringEquals<>(kCompareStrings[2], kCompareStrings[3]));
  Test(StringEquals<>(kCompareStrings[2], kCompareStrings[2]));

  Test(9, StringLength<>("123456789"));

  Test(StringFind(kTestingString, "one"));
  Test(StringFind(kTestingString, "three."));

  PRINTF("\n\n    Testing PrintRight...");

  Test(PrintRight<>(buffer, buffer + kSize, kTestingString, 28));
  PRINTF("\n    Wrote:\"%s\":%i", buffer, StringLength<>(buffer));
  AVOW(kStringsRightAligned[0], buffer);

  Test(PrintRight(buffer, buffer + kSize, kTestingString, 7));
  AVOW(kStringsRightAligned[1], buffer);

  Test(PrintRight(buffer, buffer + kSize, kTestingString, 1));
  AVOW(kStringsRightAligned[2], buffer);

  Test(PrintRight(buffer, buffer + kSize, kTestingString, 2));
  AVOW(kStringsRightAligned[3], buffer);

  Test(PrintRight(buffer, buffer + kSize, kTestingString, 3));
  AVOW(kStringsRightAligned[4], buffer);

  Test(PrintRight(buffer, buffer + kSize, kTestingString, 4));
  AVOW(kStringsRightAligned[5], buffer);

  PRINTF("\n\n    Testing PrintCentered...");

  for (int i = 0; i < 4; ++i) {
    PRINTF("\n    %i.)\"%s\"", i, kStringsCentered[i]);
  }

  utf.Set(buffer) << Center(kStringNumbers, 10);
  AVOW(kStringNumbers, buffer);

  utf.Set(buffer) << Center(kStringNumbers, 11);
  AVOW(kStringsCentered[0], buffer);

  utf.Set(buffer) << Center(kStringNumbers, 12);
  AVOW(kStringsCentered[1], buffer);

  utf.Set(buffer) << Center(kStringNumbers, 13);
  AVOW(kStringsCentered[2], buffer);

  utf.Set(buffer) << Center(kStringNumbers, 6);
  AVOW(kStringsCentered[3], buffer);
  int i = 0;  //< Shared looping variable.

  PRINT_HEADING("\nTesting Console ().Print ()... ");

  // Console().Print() << "Testing " << 1 << ", 2, " << -3;

  PRINT_HEADING("\nTesting Core Text Functions!");

  PRINT_HEADING("Testing PrintMemory (void*, int size)...");

  for (int i = 1; i <= kSize; ++i) buffer_b[i - 1] = '0' + i % 10;
  buffer_b[kSize] = 0;
  Test(PrintMemory<>(buffer, buffer + kSize, buffer_b, buffer_b + 160));
  PRINTF("\n    Printed:\n%s", buffer);

  PRINT_HEADING("\n\nTest MemoryCopy and MemoryCompare...\n\n");
  enum {
    kTestCharsCount = 1024,
    kTestCharsOffsetCount = 16,
  };
  char test_chars[kTestCharsCount];
  char test_chars_result[kTestCharsCount + kTestCharsOffsetCount];

  PRINTF("\ntest_chars[0] = %p , test_chars_result[n] = %p  ", test_chars,
         test_chars_result);

  for (int i = 0; i < kTestCharsOffsetCount; ++i) {
    for (int j = 0; j < kTestCharsCount; ++j) test_chars[j] = (char)(j % 256);
    char* result = SocketCopy(test_chars_result + i, kTestCharsCount,
                              test_chars, kTestCharsCount);
    Test(result);
    Test(!SocketCompare(test_chars + i, kTestCharsCount, test_chars_result,
                        kTestCharsCount));
  }

  PRINTF("\n\nDone testing MemoryCopy!\n");

  DTEST_END;
#endif
  return nullptr;
}
}  // namespace _
#include "seam_footer.inl"

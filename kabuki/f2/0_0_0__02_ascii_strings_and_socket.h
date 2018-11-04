/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    \kabuki\f2\0_0_0__02_ascii_strings_and_socket.h
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

#include "crng.h"
#include "tsocket.h"
#include "tstring.h"
#include "ttest.h"

#include "cconsole.h"

#if SEAM == _0_0_0__02
#include "test_debug.inl"
#else
#include "test_release.inl"
#endif

namespace _ {

static const char* _0_0_0__02_ASCII_Strings_and_Socket(char* seam_log,
                                                       char* seam_end,
                                                       const char* args) {
#if SEAM >= _0_0_0__02
  TEST_BEGIN;

  PRINT_HEADING("Testing ASCII Strings");
  enum {
    kCompareStringsCount = 5,
    kSize = 512,
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

  static const char* kStringsCentered[] = {" 1234567890", " 1234567890 ",
                                           "  1234567890 ", "123..."};

  const char* end;
  char buffer[kSize + 2], buffer_b[kSize + 1];

  enum { kCheckChar = '!' };
  char* check_char = buffer + kSize + 1;

  SocketFill(buffer, kSize);
  Print<>(buffer, kSize, "Testing 1, 2, 3");
  PRINT_SOCKET(buffer, kSize);

  *check_char = kCheckChar;

  Utf8 utf(buffer, kSize);

  for (int i = 0; i < kCompareStringsCount; ++i) {
    end = Print(buffer, buffer + kSize, test_strings[i][0]);
    PRINT_SOCKET(buffer, kSize);
    Test(end);

    end = StringEquals<>(buffer, test_strings[i][0]);
    Test(end);
  }

  static const char kTesting123[] = "Testing 1, 2, 3\0";

  PRINT_HEADING("    Running HexTest");
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
    AVOW(i, value);
    value = HexToByte(HexByteToUpperCase(i));
    PRINTF("     value is now:%i", value);
    Test(i, value);
  }

  PRINT_HEADING("    Testing Universal Text Formatter");
  PRINTF("\n\n    Expecting \"%s\"", kTesting123);

  utf.Set(buffer) << "Testing " << 1 << ", " << 2 << ", " << 3;
  PRINT_SOCKET(buffer, kSize);
  AVOW(kTesting123, buffer);

  ASSERT(!StringEquals<>(kCompareStrings[0], kCompareStrings[1]));
  ASSERT(!StringEquals<>(kCompareStrings[0], kCompareStrings[3]));
  ASSERT(StringEquals<>(kCompareStrings[0], kCompareStrings[0]));
  ASSERT(!StringEquals<>(kCompareStrings[2], kCompareStrings[3]));
  ASSERT(StringEquals<>(kCompareStrings[2], kCompareStrings[2]));

  AVOW(9, StringLength<>("123456789"));

  ASSERT(StringFind(kTestingString, "one"));
  ASSERT(StringFind(kTestingString, "three."));

  PRINTF("\n\n    Testing PrintRight");

  ASSERT(PrintRight<>(buffer, buffer + kSize, kTestingString, 28));
  PRINT_SOCKET(buffer, kSize);
  PRINTF("\n    Wrote:\"%s\":%i", buffer, StringLength<>(buffer));
  AVOW(kStringsRightAligned[0], buffer);

  ASSERT(PrintRight(buffer, buffer + kSize, kTestingString, 7));
  PRINT_SOCKET(buffer, kSize);
  AVOW(kStringsRightAligned[1], buffer);

  ASSERT(PrintRight(buffer, buffer + kSize, kTestingString, 1));
  PRINT_SOCKET(buffer, kSize);
  AVOW(kStringsRightAligned[2], buffer);

  ASSERT(PrintRight(buffer, buffer + kSize, kTestingString, 2));
  PRINT_SOCKET(buffer, kSize);
  AVOW(kStringsRightAligned[3], buffer);

  ASSERT(PrintRight(buffer, buffer + kSize, kTestingString, 3));
  PRINT_SOCKET(buffer, kSize);
  AVOW(kStringsRightAligned[4], buffer);

  ASSERT(PrintRight(buffer, buffer + kSize, kTestingString, 4));
  PRINT_SOCKET(buffer, kSize);
  AVOW(kStringsRightAligned[5], buffer);

  PRINTF("\n\n    Testing PrintCentered");

  for (int i = 0; i < 4; ++i) {
    PRINTF("\n    %i.)\"%s\"", i, kStringsCentered[i]);
  }

  utf.Set(buffer) << Center(kStringNumbers, 10);
  PRINT_SOCKET(buffer, kSize);
  AVOW(kStringNumbers, buffer);

  utf.Set(buffer) << Center(kStringNumbers, 11);
  PRINT_SOCKET(buffer, kSize);
  AVOW(kStringsCentered[0], buffer);

  utf.Set(buffer) << Center(kStringNumbers, 12);
  PRINT_SOCKET(buffer, kSize);
  AVOW(kStringsCentered[1], buffer);

  utf.Set(buffer) << Center(kStringNumbers, 13);
  PRINT_SOCKET(buffer, kSize);
  AVOW(kStringsCentered[2], buffer);

  utf.Set(buffer) << Center(kStringNumbers, 6);
  PRINT_SOCKET(buffer, kSize);
  AVOW(kStringsCentered[3], buffer);
  int i = 0;  //< Shared looping variable.

  // PRINT_HEADING("\nTesting Console ().Print () ");
  // Console().Print() << "Testing " << 1 << ", 2, " << -3;

  PRINT_HEADING("Testing Socket");

  PRINT_HEADING("Testing PrintSocket (void*, int size)");

  for (int i = 1; i <= kSize; ++i) buffer_b[i - 1] = '0' + i % 10;
  buffer_b[kSize] = 0;
  ASSERT(TPrintSocket<>(buffer, buffer + kSize, buffer_b, buffer_b + 160));
  PRINT_SOCKET(buffer, kSize);
  PRINTF("\n    Printed:\n%s", buffer);

  PRINT_HEADING("\n\nTest SocketCopy and MemoryCompare\n\n");

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
    ASSERT(result);
    ASSERT(!SocketCompare(test_chars + i, kTestCharsCount, test_chars_result,
                          kTestCharsCount));
  }

  PRINTF("\n\nDone testing SocketCopy!\n");

  TEST_END;
#endif
  return nullptr;
}
}  // namespace _
#include "test_footer.inl"

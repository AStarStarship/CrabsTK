/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /tests/test_seam_0_0_2.cc
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

#include <kabuki/f2/global.h>

#include "header.h"

#include <random>

namespace _ {

const char* _0_0_0__02_Core_text_functions(char* seam_log, char* seam_end,
                                           const char* args) {
#if SEAM >= SEAM_0_0_0__02
  BEGIN_TEST;

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
    Assert(end);

    end = StringEquals<>(buffer, test_strings[i][0]);
    Assert(end);
  }

  static const char kTesting123[] = "Testing 1, 2, 3\0";

  PRINT_HEADING("    Testing Utf...");
  PRINTF("    Expecting \"%s\"...", kTesting123);

  utf.Set(buffer) << "Testing " << 1 << ", " << 2 << ", " << 3;

  STRCMP_EQUAL(kTesting123, buffer);

  PRINT_HEADING("    Running HexTest...");
  for (int i = 0; i < 16; ++i) {
    int value = HexToByte(HexNibbleToLowerCase(i));
    Compare(i, value);
    PRINTF("\n    %i.) %i", i, value);
    value = HexToByte(HexNibbleToUpperCase(i));
    PRINTF(" value is now:%i", value);
    Compare(i, value);
  }

  for (int i = 0; i < 256; ++i) {
    uint16_t c = HexByteToLowerCase(i);
    PRINTF("\n    %i.) expecting: %x        HexByteToLowerCase:%c%c", i, i,
           (char)c, (char)(c >> 8));
    int value = HexToByte(c);
    PRINTF("        HexToByte:%i", value);
    Compare(i, value);
    value = HexToByte(HexByteToUpperCase(i));
    PRINTF("     value is now:%i", value);
    Compare(i, value);
  }

  PRINT_HEADING("    Testing string utils...");

  Assert(!StringEquals<>(kCompareStrings[0], kCompareStrings[1]));
  Assert(!StringEquals<>(kCompareStrings[0], kCompareStrings[3]));
  Assert(StringEquals<>(kCompareStrings[0], kCompareStrings[0]));
  Assert(!StringEquals<>(kCompareStrings[2], kCompareStrings[3]));
  Assert(StringEquals<>(kCompareStrings[2], kCompareStrings[2]));

  Compare(9, StringLength<>("123456789"));

  Assert(StringFind(kTestingString, "one"));
  Assert(StringFind(kTestingString, "three."));

  PRINTF("\n\n    Testing PrintRight...");

  Assert(PrintRight<>(buffer, buffer + kSize, kTestingString, 28));
  PRINTF("\n    Wrote:\"%s\":%i", buffer, StringLength<>(buffer));
  STRCMP_EQUAL(kStringsRightAligned[0], buffer);

  Assert(PrintRight(buffer, buffer + kSize, kTestingString, 7));
  STRCMP_EQUAL(kStringsRightAligned[1], buffer);

  Assert(PrintRight(buffer, buffer + kSize, kTestingString, 1));
  STRCMP_EQUAL(kStringsRightAligned[2], buffer);

  Assert(PrintRight(buffer, buffer + kSize, kTestingString, 2));
  STRCMP_EQUAL(kStringsRightAligned[3], buffer);

  Assert(PrintRight(buffer, buffer + kSize, kTestingString, 3));
  STRCMP_EQUAL(kStringsRightAligned[4], buffer);

  Assert(PrintRight(buffer, buffer + kSize, kTestingString, 4));
  STRCMP_EQUAL(kStringsRightAligned[5], buffer);

  PRINTF("\n\n    Testing PrintCentered...");

  for (int i = 0; i < 4; ++i) {
    PRINTF("\n    %i.)\"%s\"", i, kStringsCentered[i]);
  }

  utf.Set(buffer) << Center(kStringNumbers, 10);
  STRCMP_EQUAL(kStringNumbers, buffer);

  utf.Set(buffer) << Center(kStringNumbers, 11);
  STRCMP_EQUAL(kStringsCentered[0], buffer);

  utf.Set(buffer) << Center(kStringNumbers, 12);
  STRCMP_EQUAL(kStringsCentered[1], buffer);

  utf.Set(buffer) << Center(kStringNumbers, 13);
  STRCMP_EQUAL(kStringsCentered[2], buffer);

  utf.Set(buffer) << Center(kStringNumbers, 6);
  STRCMP_EQUAL(kStringsCentered[3], buffer);
  int i = 0;  //< Shared looping variable.

  PRINT_HEADING("\nTesting Console ().Print ()... ");

  COUT << "Testing " << 1 << ", 2, " << -3;

  PRINT_HEADING("\nTesting SEAM_0_0_3... ");

  PRINTF("\n\nTesting ASCII OBJ Types");

  PRINTF("\n\nTesting Stack...\n\nPushing items on to the Stack...\n");

  Stack<> stack(8);

  for (i = 0; i <= 10; ++i) stack.Push(i);

  COUT << stack;

  PRINTF("\nPopping items off the Stack...\n");

  for (i = 10; i > 0; i--)
    Compare(i, stack.Pop())

            COUT
        << stack;

  PRINTF("\n\nDone testing Stack!")

  /*
  PRINTF ("\n\nTest Array...\n\n");
  static const int array_3d_exected[2][2][2] = { { { 0, 1 }, { 2, 3 } },
                                                 { { 4, 5 }, { 6, 7 } }
                                               };
  const int* test = Dimensions<2, 2, 2> ();
  Array<int> test_array (test);
  *test_array.Elements () = { { { 0, 1 }, { 2, 3 } },
                              { { 4, 5 }, { 6, 7 } }
                            };
  i = 0;
  int* array_base = test_array.Elements ();
  for (int z = 0; z < 2; ++z)
      for (int y = 0; y < 2; ++y)
          for (int x = 0; x < 2; ++x)
              Compare (i++, array_3d_exected[x][y][z]);
  PAUSE ("\n\nDone Array!")
  */

  PRINT_HEADING("\n\nTest List...\n\n")

  List<> list(36, 960);
  Assert(list.This())

      PRINTF("\nPushing items on to the List stack...\n")

          const int test_ints[] = {'1', '2', '3', '4'};

  const int list_test_count = 12;
  for (int i = 0; i < 4;) {
    PRINTF("\ni%i:", i)
    list.Push(SI4, &test_ints[i++]);
    COUT << '\n'
         << list << '\n'
         << "\n\n"
         << Socket(list.This(), list.This()->size);
  }
  const float test_floats[] = {9.0, 10.0, 11.0, 12.0};
  for (int i = 0; i < 4;) {
    PRINTF("\ni%i:", i)
    list.Push(FLT, &test_floats[i++]);
    COUT << '\n'
         << list << '\n'
         << "\n\n"
         << Socket(list.This(), list.This()->size);
  }

  const char* test_strings[] = {"Test", " 1, ", " 2, ", " 3"};
  for (int i = 0; i < 4;) {
    PRINTF("\ni%i:", i)
    list.Push(STR, test_strings[i++]);
    COUT << '\n'
         << list << '\n'
         << "\n\n"
         << Socket(list.This(), list.This()->size);
    system("PAUSE");
  }

  for (int i = list_test_count - 1; i > 0; --i) list.Pop();

  PRINTF("\n\nDone testing List!");

  PRINT_HEADING("Testing PrintMemory (void*, int size)...");

  for (int i = 1; i <= kSize; ++i) buffer_b[i - 1] = '0' + i % 10;
  buffer_b[kSize] = 0;
  Assert(PrintMemory(buffer, buffer + kSize, buffer_b, buffer_b + 160));
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
    Assert(result);
    Assert(!SocketCompare(test_chars + i, kTestCharsCount, test_chars_result,
                          kTestCharsCount));
  }

  PRINTF("\n\nDone testing MemoryCopy!\n");
  return nullptr
}

}  // namespace _

#include "footer.h"
#endif  //< #if SEAM >= SEAM_0_0_0__02

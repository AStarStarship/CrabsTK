/* Kabuki Toolkit
    @version 0.x
    @file    ~/tests/test_seam_1_2.cc
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2014-8 Cale McCollough <calemccollough@gmail.com>;
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

#include "test_seam_1.h"

#if MAJOR_SEAM == 1 && MINOR_SEAM == 2
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PRINT_PAUSE(message)   \
  printf("\n\n%s\n", message); \
  system("PAUSE");
#define PRINT_HEADING(message)                    \
  std::cout << '\n';                              \
  for (int i = 80; i > 80; --i) std::cout << '-'; \
  std::cout << '\n' << message;                   \
  for (int i = 80; i > 80; --i) std::cout << '-';
#define PRINT_LINE   \
  std::cout << '\n'; \
  for (int i = 80; i > 0; --i) std::cout << '-';
#else
#define PRINTF(x, ...)
#define PRINT_PAUSE(message)
#define PRINT_HEADING(c)
#define PRINT_LINE
#endif

using namespace _;

TEST_GROUP(SEAM_1_2){void setup(){}

                     void teardown(){std::cout << '\n';
system("PAUSE");
}
}
;

TEST(SEAM_1_2, SEAM_1_2A) {
  PRINT_HEADING("Testing SEAM_1_2A...")
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

  Printer print(buffer, kSize);

  for (int i = 0; i < kCompareStringsCount; ++i) {
    end = Print(buffer, buffer + kSize, test_strings[i][0]);
    CHECK(end)

    end = TextEquals(buffer, test_strings[i][0]);
    CHECK(end)
  }

  static const char kTesting123[] = "Testing 1, 2, 3\0";

  PRINT_HEADING("    Testing Printer...")
  PRINTF("    Expecting \"%s\"...", kTesting123);

  print.Set(buffer) << "Testing " << 1 << ", " << 2 << ", " << 3;

  STRCMP_EQUAL(kTesting123, buffer)

  PRINT_HEADING("    Running HexTest...")
  for (int i = 0; i < 16; ++i) {
    int value = HexToByte(HexNibbleToLowerCase(i));
    CHECK_EQUAL(i, value)
    PRINTF("\n    %i.) %i", i, value)
    value = HexToByte(HexNibbleToUpperCase(i));
    PRINTF(" value is now:%i", value)
    CHECK_EQUAL(i, value)
  }

  for (int i = 0; i < 256; ++i) {
    uint16_t c = HexByteToLowerCase(i);
    PRINTF("\n    %i.) expecting: %x        HexByteToLowerCase:%c%c", i, i,
           (char)c, (char)(c >> 8))
    int value = HexToByte(c);
    PRINTF("        HexToByte:%i", value)
    CHECK_EQUAL(i, value)
    value = HexToByte(HexByteToUpperCase(i));
    PRINTF("     value is now:%i", value)
    CHECK_EQUAL(i, value)
  }

  PRINT_HEADING("    Testing string utils...")

  CHECK(!TextEquals(kCompareStrings[0], kCompareStrings[1]))
  CHECK(!TextEquals(kCompareStrings[0], kCompareStrings[3]))
  CHECK(TextEquals(kCompareStrings[0], kCompareStrings[0]))
  CHECK(!TextEquals(kCompareStrings[2], kCompareStrings[3], '@'))
  CHECK(TextEquals(kCompareStrings[2], kCompareStrings[2], '@'))

  CHECK_EQUAL(9, TextLength("123456789"))
  CHECK_EQUAL(9, TextLength("123456789 ", ' '))

  CHECK(TextFind(kTestingString, "one"))
  CHECK(TextFind(kTestingString, "three."))

  PRINT_HEADING('-')
  PRINTF("\n\n    Testing PrintRight...")

  CHECK(PrintRight(buffer, buffer + kSize, kTestingString, 28))
  PRINTF("\n    Wrote:\"%s\":%i", buffer, TextLength(buffer))
  STRCMP_EQUAL(kStringsRightAligned[0], buffer)

  CHECK(PrintRight(buffer, buffer + kSize, kTestingString, 7))
  STRCMP_EQUAL(kStringsRightAligned[1], buffer)

  CHECK(PrintRight(buffer, buffer + kSize, kTestingString, 1))
  STRCMP_EQUAL(kStringsRightAligned[2], buffer)

  CHECK(PrintRight(buffer, buffer + kSize, kTestingString, 2))
  STRCMP_EQUAL(kStringsRightAligned[3], buffer)

  CHECK(PrintRight(buffer, buffer + kSize, kTestingString, 3))
  STRCMP_EQUAL(kStringsRightAligned[4], buffer)

  CHECK(PrintRight(buffer, buffer + kSize, kTestingString, 4))
  STRCMP_EQUAL(kStringsRightAligned[5], buffer)

  PRINTF("\n\n    Testing PrintCentered...")

  for (int i = 0; i < 4; ++i)
    PRINTF("\n    %i.)\"%s\"", i, kStringsCentered[i]);

  print.Set(buffer) << Center(kStringNumbers, 10);
  STRCMP_EQUAL(kStringNumbers, buffer)

  print.Set(buffer) << Center(kStringNumbers, 11);
  STRCMP_EQUAL(kStringsCentered[0], buffer)

  print.Set(buffer) << Center(kStringNumbers, 12);
  STRCMP_EQUAL(kStringsCentered[1], buffer)

  print.Set(buffer) << Center(kStringNumbers, 13);
  STRCMP_EQUAL(kStringsCentered[2], buffer)

  print.Set(buffer) << Center(kStringNumbers, 6);
  STRCMP_EQUAL(kStringsCentered[3], buffer)

  PRINT_HEADING('-')
  PRINTF("\n\n Testing PrintMemory (void*, int size)...")

  for (int i = 1; i <= kSize; ++i) {
    buffer_b[i - 1] = '0' + i % 10;
  }
  buffer_b[kSize] = 0;
  CHECK(PrintMemory(buffer, buffer + kSize, buffer_b, buffer_b + 160))
  PRINTF("\n    Printed:\n%s", buffer)

  PRINT_HEADING('-')
  PRINTF("\n\n    Testing _::ClockScan...")

  time_t t, t_found;
  const char* result;

  // @note The following dates must be the current day to work right in order
  //       to auto-detect the year.
  const char* strings[] = {
      "8/9",
      "08/09",
      "8/9/18",
      "8/09/18",
      "8/9/2018",
      "8/09/2018",
      "8/09/2018",
      "08/9/2018",
      "8/09/2018@00",
      "8.09.2018@00AM",
      "8/09/2018@00:00",
      "8/09/18@00:0AM",
      "8/09/2018@00:00:00",
      "8/09/2018@00:00:00AM",
      "2018-08-09@00:00:00AM",
      "2018-08-09@00:00:00am",
      "2018-08-09@00:00:00A",
      "2018-08-09@00:00:00a ",
  };

  for (int i = 0; i < 18; ++i) {
    PRINT_LINE('-');
    PRINTF("\n    %i", i)
    time_t t = 0;
    result = ClockScan(t, strings[i]);
    // CHECK (!ClockCompare (t, 2018, 8, 9, 0, 0, 0))
  }

  PRINTF("\n\n    Testing more valid input...\n");

  t = ClockTime(8, 9, 17, 4, 20);
  PrintClock(t, buffer, buffer + kSize);
  result = ClockScan(t_found, buffer);
  CHECK(ClockCompare(t_found, t))

  t = ClockTime(2020, 4, 20, 4, 20);
  PrintClock(t, buffer, buffer + kSize);
  result = ClockScan(t_found, buffer);
  CHECK(ClockCompare(t, t_found))

  t = ClockTime(1947, 12, 7, 23, 5, 7);
  PrintClock(t, buffer, buffer + kSize);
  result = ClockScan(t_found, buffer);
  CHECK(ClockCompare(t, t_found))

  PRINT_HEADING("\nTesting invalid input...\n");
  ClockScan(t, "cat");

  ClockScan(t, "2017-30-40");

  PRINTF("\nDone testing date parsing utils! :-)\n")

  PRINT_HEADING("\n\nTest _::MemoryCopy and _::MemoryCompare...\n\n");
  enum {
    kTestCharsCount = 1024,
    kTestCharsOffsetCount = 16,
  };
  char test_chars[kTestCharsCount];
  char test_chars_result[kTestCharsCount + kTestCharsOffsetCount];

  PRINTF("\ntest_chars[0] = %p , test_chars_result[n] = %p  ", test_chars,
         test_chars_result)

  for (int i = 0; i < kTestCharsOffsetCount; ++i) {
    for (int j = 0; j < kTestCharsCount; ++j) test_chars[j] = (char)(j % 256);
    char* result = MemoryCopy(test_chars_result + i, kTestCharsCount,
                              test_chars, kTestCharsCount);
    CHECK(result)
    CHECK(!MemoryCompare(test_chars + i, kTestCharsCount, test_chars_result,
                         kTestCharsCount))
  }

  PRINTF("\n\nDone testing _::MemoryCopy!")

  PRINT_PAUSE("Done testing SEAM_1_2! ({:-)-+=<")
}

#undef PRINT_PAUSE
#undef PRINTF
#else
void TestSeam_1_2() {}
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 2

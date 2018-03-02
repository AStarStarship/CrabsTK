/** Kabuki Toolkit
    @version 0.x
    @file    ~/tests/test_seam_1_2.cc
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
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
#include "tests_global.h"

#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 3

using namespace _;

TEST_GROUP (SEAM_1_3_TESTS) {
    void setup () {

    }

    void teardown () {
        std::cout << "\n";
        system ("PAUSE");
    }
};

TEST (SEAM_1_3_TESTS, TestPrinter) {
    puts ("\n\n Testing Text...\n\n");

    enum {
        kNumCompareStrings = 5,
        kSize = 10,
    };

    static const char* test_strings[kNumCompareStrings][2] = {
        { "?"      , ""        },
        { "?"      , "?"       },
        { "? "     , "?"       },
        { "Apples" , "Apples"  },
        { "Apples" , "Apples"  },
    };

    const char* end;

    char buffer_a[kSize],
         buffer_b[kSize];

    Printer printer (buffer_a, kSize);

    for (int i = 0; i < kNumCompareStrings; ++i) {
        end = Print (test_strings[i][0], buffer_a, buffer_a + kSize);
        CHECK (end != nullptr)
        end = Print (test_strings[i][0], buffer_b, buffer_b + kSize);
        CHECK (end != nullptr)

        end = TextEquals (buffer_a, buffer_b);
        CHECK (end != nullptr)
        end = TextEquals (buffer_a, buffer_a + kSize, buffer_b);
        CHECK (end != nullptr)
    }

    puts ("\n\n Testing Printer..."
          "\n\n Expecting \"Testing 1, 2, 3\"...");

    printer << "\n Testing " << 1 << ", " << 2 << ", " << 3;

    printf ("\n\n Wrote:\"%s\"", printer.GetCursor ());

    system ("PAUSE");

    enum {
        kSize = 16,
        kSlotSize = 2048,
        kSlotSizeWords = kSlotSize >> kWordSizeShift
    };

    static const char* compare_strings[] = {
        "Testing",
        "Texting",
        "Testing@",
        "Texting@",
    };
    static const char string[] = "Testing one, two, three.";

    static const char* right_string[] = {
        "    Testing one, two, three.",
        "Test...",
        ".",
        "..",
        "...",
        "T...",
    };

    uintptr_t slot_buffer[kSlotSizeWords];

    char buffer[kFloat64DigitsMax];

    std::cout << "\n|  - Running HexTest...";
    for (int i = 0; i < 16; ++i) {
        int value = TextHexToByte (TextNibbleToLowerCaseHex (i));
        CHECK_EQUAL (i, value)
            value = TextHexToByte (TextNibbleToUpperCaseHex (i));
        CHECK_EQUAL (i, value)
    }

    for (int i = 0; i < 256; ++i) {
        int value = TextHexToByte (TextByteToLowerCaseHex (i));
        CHECK_EQUAL (i, value)
            value = TextHexToByte (TextByteToUpperCaseHex (i));
        CHECK_EQUAL (i, value)
    }

    std::cout << "\n| - Testing string utils...\n";

    CHECK (!TextEquals (compare_strings[0], compare_strings[1]))
    CHECK (!TextEquals (compare_strings[0], compare_strings[3]))
    CHECK (TextEquals  (compare_strings[0], compare_strings[0]))
    CHECK (!TextEquals (compare_strings[2], compare_strings[3], '@'))
    CHECK (TextEquals  (compare_strings[2], compare_strings[2], '@'))

    CHECK_EQUAL (9, TextLength ("123456789"))
    CHECK_EQUAL (9, TextLength ("123456789 ", ' '))
        
    CHECK (TextFind (string, "one"))
    CHECK (TextFind (string, "three."))
    
    PrintRight (string, 28, buffer, buffer + kFloat64DigitsMax);
    STRCMP_EQUAL (right_string[0], buffer)
    
    PrintRight (string, 7, buffer, buffer + kFloat64DigitsMax);
    STRCMP_EQUAL (right_string[1], buffer)
    
    PrintRight (string, 1, buffer, buffer + kFloat64DigitsMax);
    STRCMP_EQUAL (right_string[2], buffer)
    
    PrintRight (string, 2, buffer, buffer + kFloat64DigitsMax);
    STRCMP_EQUAL (right_string[3], buffer)
    
    PrintRight (string, 3, buffer, buffer + kFloat64DigitsMax);
    STRCMP_EQUAL (right_string[4], buffer)
    
    PrintRight (string, 4, buffer, buffer + kFloat64DigitsMax);
    STRCMP_EQUAL (right_string[5], buffer)

    puts ("\n Testing Text::Memory (void*, int size)...");

    PrintMemory (printer.GetCursor (), printer.GetEnd ());
}

#endif      //< MAJOR_SEAM >= 1 && MINOR_SEAM >= 3

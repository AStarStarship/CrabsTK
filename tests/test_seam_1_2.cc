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

#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 2

#include "tests_global.h"


#if MAJOR_SEAM == 1 && MINOR_SEAM == 2
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PRINT_PAUSE(message)\
    printf ("\n%s               ", message); system ("PAUSE");
#else
#define PRINTF(x, ...)
#define PRINT_PAUSE(message)
#endif

using namespace _;

void TestSeam1_2 () {

    printf ("\n    Testing SEAM_1_2... ");

    PRINTF ("\n\n Testing Text...");

    enum {
        kNumCompareStrings = 5,
        kSize = 1024,
    };

    static const char* test_strings[kNumCompareStrings][2] = {
        { "?"      , ""        },
        { "?"      , "?"       },
        { "? "     , "?"       },
        { "Apples" , "Apples"  },
        { "Apples" , "Apples"  },
    };

    static const char* kCompareStrings[] = {
        "Testing",
        "Texting",
        "Testing@",
        "Texting@",
    };

    static const char kTestingString[] = "Testing one, two, three.";

    static const char* kStringsRightAligned[] = {
        "    Testing one, two, three.",
        "Test...",
        ".",
        "..",
        "...",
        "T...",
    };

    static const char kStringNumbers[] = "1234567890\0";

    static const char* kStringsCentered[] = {
        " 1234567890\0",
        " 1234567890 \0",
        "  1234567890 \0",
        "123...\0"
    };

    const char* end;
    char        buffer[kSize],
                buffer_b[kSize];

    Printer printer (buffer, kSize);

    for (int i = 0; i < kNumCompareStrings; ++i) {
        end = Print (test_strings[i][0], buffer, buffer + kSize);
        CHECK (end)
        end = Print (test_strings[i][0], buffer_b, buffer_b + kSize);
        CHECK (end)

        end = TextEquals (buffer, buffer_b);
        CHECK (end)
        end = TextEquals (buffer, buffer + kSize, buffer_b);
        CHECK (end)
    }

    static const char kTesting123[] = "Testing 1, 2, 3\0";

    PRINTF ("\n\n Testing Printer..."
            "\n\n Expecting \"%s\"...", kTesting123);

    printer << "Testing " << 1 << ", " << 2 << ", " << 3 << printer;

    STRCMP_EQUAL (kTesting123, buffer)

    PRINTF ("\n    Running HexTest...");
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
    
    PRINTF ("\n\n    Testing string utils...\n");

    CHECK (!TextEquals (kCompareStrings[0], kCompareStrings[1]))
    CHECK (!TextEquals (kCompareStrings[0], kCompareStrings[3]))
    CHECK (TextEquals  (kCompareStrings[0], kCompareStrings[0]))
    CHECK (!TextEquals (kCompareStrings[2], kCompareStrings[3], '@'))
    CHECK (TextEquals  (kCompareStrings[2], kCompareStrings[2], '@'))

    CHECK_EQUAL (9, TextLength ("123456789"))
    CHECK_EQUAL (9, TextLength ("123456789 ", ' '))
        
    CHECK (TextFind (kTestingString, "one"))
    CHECK (TextFind (kTestingString, "three."))

    PRINTF ("\n\n    Testing PrintRight...")

    CHECK (PrintRight (kTestingString, 28, buffer, buffer + kSize))
    PRINTF ("\n    Wrote:\"%s\":%i", buffer, TextLength (buffer))
    STRCMP_EQUAL (kStringsRightAligned[0], buffer)
    
    CHECK (PrintRight (kTestingString, 7, buffer, buffer + kSize))
    STRCMP_EQUAL (kStringsRightAligned[1], buffer)
    
    CHECK (PrintRight (kTestingString, 1, buffer, buffer + kSize))
    STRCMP_EQUAL (kStringsRightAligned[2], buffer)
    
    CHECK (PrintRight (kTestingString, 2, buffer, buffer + kSize))
    STRCMP_EQUAL (kStringsRightAligned[3], buffer)
    
    CHECK (PrintRight (kTestingString, 3, buffer, buffer + kSize))
    STRCMP_EQUAL (kStringsRightAligned[4], buffer)
    
    CHECK (PrintRight (kTestingString, 4, buffer, buffer + kSize))
    STRCMP_EQUAL (kStringsRightAligned[5], buffer)

    PRINTF ("\n\n    Testing PrintCentered...")

    for (int i = 0; i < 4; ++i) {
        PRINTF ("\n    %i.)\"%s\"", i, kStringsCentered[i]);
    }

    CHECK (PrintCentered (kStringNumbers, 10, buffer, buffer + kSize))
    STRCMP_EQUAL (kStringNumbers, buffer)

    CHECK (PrintCentered (kStringNumbers, 11, buffer, buffer + kSize))
    STRCMP_EQUAL (kStringsCentered[0], buffer)

    CHECK (PrintCentered (kStringNumbers, 12, buffer, buffer + kSize))
    STRCMP_EQUAL (kStringsCentered[1], buffer)

    CHECK (PrintCentered (kStringNumbers, 13, buffer, buffer + kSize))
    STRCMP_EQUAL (kStringsCentered[2], buffer)

    CHECK (PrintCentered (kStringNumbers, 6, buffer, buffer + kSize))
    STRCMP_EQUAL (kStringsCentered[3], buffer)

    PRINTF ("\n\n Testing Text::Memory (void*, int size)...");

    for (int i = 1; i <= kSize; ++i) {
        buffer_b[i - 1] = '0' + i % 10;
    }
    buffer_b[kSize - 1] = 0;
    CHECK (PrintMemory (buffer_b, buffer_b + 160, buffer, buffer + kSize))
    PRINTF ("\n   Printed:\n%s", buffer)
}

#undef PRINT_PAUSE
#undef PRINTF
#else
void TestSeam1_2 () {}
#endif      //< #if MAJOR_SEAM >= 1 && MINOR_SEAM >= 2

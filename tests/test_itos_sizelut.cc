/** Kabuki Starship
    @version 0.x
    @file    ~/source/kabuki/tests/script_itos_tests.cc
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
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
#include "test_itos_script.h"
#include "test_itos_sizelut.h"

using namespace std;

#define PRINT_PAUSE(message)\
    //cout << '\n' << message << "               "; system ("PAUSE");

void TestItoSSizeLut () {
    static const uint64_t k10ToThe[20] = {
        1,//< 10^0
        10,//< 10^1
        100,//< 10^2
        1000,//< 10^3
        10000,//< 10^4
        100000,//< 10^5
        1000000,//< 10^6
        10000000,//< 10^7
        100000000,//< 10^8
        1000000000,//< 10^9
        10000000000,//< 10^10
        100000000000,//< 10^11
        1000000000000,//< 10^12
        10000000000000,//< 10^13
        100000000000000,//< 10^14
        1000000000000000,//< 10^15
        10000000000000000,//< 10^16
        100000000000000000,//< 10^17
        1000000000000000000,//< 10^18
        10000000000000000000,//< 10^19
    };

    static const uint64_t test_value[] = {
        k10ToThe[0] - 1, k10ToThe[0], k10ToThe[0] + 1,
        k10ToThe[1] - 1, k10ToThe[1], k10ToThe[1] + 1,
        k10ToThe[1] + k10ToThe[1] / 2,
        k10ToThe[2] - 1, k10ToThe[2], k10ToThe[2] + 1,
        k10ToThe[2] + k10ToThe[2] / 2,
        k10ToThe[3] - 1, k10ToThe[3], k10ToThe[3] + 1,
        k10ToThe[3] + k10ToThe[3] / 2,
        k10ToThe[4] - 1, k10ToThe[4], k10ToThe[4] + 1,
        k10ToThe[4] + k10ToThe[4] / 2,
        k10ToThe[5] - 1, k10ToThe[5], k10ToThe[5] + 1,
        k10ToThe[5] + k10ToThe[5] / 2,
        k10ToThe[6] - 1, k10ToThe[6], k10ToThe[6] + 1,
        k10ToThe[6] + k10ToThe[6] / 2,
        k10ToThe[7] - 1, k10ToThe[7], k10ToThe[7] + 1,
        k10ToThe[7] + k10ToThe[7] / 2,
        k10ToThe[8] - 1, k10ToThe[8], k10ToThe[8] + 1,
        k10ToThe[8] + k10ToThe[8] / 2,
        k10ToThe[9] - 1, k10ToThe[9], k10ToThe[9] + 1,
        k10ToThe[9] + k10ToThe[9] / 2,
        k10ToThe[10] - 1, k10ToThe[10], k10ToThe[10] + 1,
        k10ToThe[10] + k10ToThe[10] / 2,
        k10ToThe[11] - 1, k10ToThe[11], k10ToThe[11] + 1,
        k10ToThe[11] + k10ToThe[11] / 2,
        k10ToThe[12] - 1, k10ToThe[12], k10ToThe[12] + 1,
        k10ToThe[12] + k10ToThe[12] / 2,
        k10ToThe[13] - 1, k10ToThe[13], k10ToThe[13] + 1,
        k10ToThe[13] + k10ToThe[13] / 2,
        k10ToThe[14] - 1, k10ToThe[14], k10ToThe[14] + 1,
        k10ToThe[14] + k10ToThe[14] / 2,
        k10ToThe[15] - 1, k10ToThe[15], k10ToThe[15] + 1,
        k10ToThe[15] + k10ToThe[15] / 2,
        k10ToThe[16] - 1, k10ToThe[16], k10ToThe[16] + 1,
        k10ToThe[16] + k10ToThe[16] / 2,
        k10ToThe[17] - 1, k10ToThe[17], k10ToThe[17] + 1,
        k10ToThe[17] + k10ToThe[17] / 2,
        k10ToThe[18] - 1, k10ToThe[18], k10ToThe[18] + 1,
        k10ToThe[18] + k10ToThe[18] / 2,
        k10ToThe[19] - 1, k10ToThe[19], k10ToThe[19] + 1,
        k10ToThe[19] + k10ToThe[19] / 2
    };

    static const uint64_t problem_childs[] = {
        10277856742680,
        16733192600430,
        386547074382,
    };
     enum { kSize = 24 };
    char     text[kSize],
             expecting[kSize];
             //found[kSize];
    char*    result;
    uint64_t value;

    //cout << "\n\nTesting Size-LUT ItoS Algorithm...\n\n";

    //cout << "\n\nTesting Problem Childs...\n\n";
    for (int i = 0; i < 2; ++i) {
        value = problem_childs[i];
        for (unsigned j = 0; j < 2; ++j) {

            // Write a bunches of xxxxxx to the buffer for debug purposes.
            result = text;
            for (int k = 24; k > 0; --k) {
                *result++ = 'x';
            }
            *(text + 23) = 0;

            sprintf_s (expecting, 24, "%I64u", value);
            //cout << '\n' << i + 1 << ".) Expecting " << expecting 
            //     << " with length " << strlen (expecting);
            result = PrintSizeLut (value, text + j, text + kSize);
            if (!result) {
                PRINT_PAUSE ("An error occurred :-(")
                    break;
            }
            if (strcmp (expecting, text + j)) {
                //cout << "\n\nERROR: Expecting \"" << expecting << "\":"
                //    << strlen (expecting) << " and found \""
                //    << text + j << "\":" << strlen (text + j);
                PRINT_PAUSE ("\n");
            }
        }
    }

    //cout << "\n\nTesting edge conditions...\n\n";
    for (int i = 0; i < 79; ++i) {
        value = test_value[i];
        for (unsigned j = 0; j < 2; ++j) {

            // Write a bunches of xxxxxx to the buffer for debug purposes.
            result = text;
            for (int k = 24; k > 0; --k) {
                *result++ = 'x';
            }
            *(text + 23) = 0;

            sprintf_s (expecting, 24, "%I64u", value);
            //cout << '\n' << i + 1 << ".) Expecting " << expecting 
            //     << " with length " << strlen (expecting);
            result = PrintSizeLut (value, text + j, text + kSize);
            if (!result) {
                PRINT_PAUSE ("An error occurred :-(")
                    break;
            }
            if (strcmp (expecting, text + j)) {
                //cout << "\n\nERROR: Expecting \"" << expecting << "\":"
                //     << strlen (expecting) << " and found \"" 
                //     << text + j << "\":" << strlen (text + j);
                PRINT_PAUSE ("\n");
            }
        }
    }

    //cout << "\n\nTesting random numbers...\n\n";

    for (int i = 0; i < 0x00ffffff; ++i) {
        for (unsigned j = 0; j < 2; ++j) {
            // Write a bunches of xxxxxx to the buffer for debug purposes.
            result = text + j;
            for (int k = kSize; k > 0; --k) {
                *result++ = 'x';
            }
            *result = 0;
            value = GetRandomUInt64 ();
            sprintf_s (expecting, 24, "%I64u", value);
            //cout << '\n' << i + 1 << ".) Expecting " << expecting
            //     << " with length "
            //     << strlen (expecting);
            result = PrintSizeLut (value, text + j, text + j + kSize);
            if (!result) {
                PRINT_PAUSE ("An error occurred :-(")
                    break;
            }
            if (strcmp (expecting, text + j)) {
                //cout << "\n\nERROR: Expecting \"" << expecting << "\":"
                //     << strlen (expecting) << " and found \"" << text + j << "\":"
                //     << strlen (text + j);
                PRINT_PAUSE ("\n");
            }
        }
    }

    //cout << "\n Done testing ItoS :)\n\n";
}

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
    "90919293949596979899"
};

// Lookup table for powers of 10.
static const uint64_t k10ToThe[20]{
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000,
    1000000000, 10000000000, 100000000000, 1000000000000, 10000000000000,
    100000000000000, 1000000000000000, 10000000000000000,
    100000000000000000, 1000000000000000000, 10000000000000000000, };

char* PrintSizeLutFast (uint64_t value, char* text, char* text_end);

char* PrintSizeLut (uint64_t value, char* text, char* text_end) {
    if (!text) {
        return nullptr;
    }
    if (text > text_end) {
        return nullptr;
    }
    text = PrintSizeLutFast (value, text, text_end);
    *text = 0;
    return text;
}
char* PrintSizeLutFast (uint64_t value, char* text, char* text_end) {

    if (value == 0) {
        *text = '0';
        return text + 1;
    }

    int size;
    if (value >= k10ToThe[10]) {
        //cout << "\n    value >= k10ToThe[10]";
        if (value >= k10ToThe[14]) {
            //cout << "\n    value >= k10ToThe[15]";
            if (value >= k10ToThe[17]) {
                if (value >= k10ToThe[19])
                    size = 20;
                else if (value >= k10ToThe[18])
                    size = 19;
                else
                    size = 18;
            }
            else {
                //cout << "\n    value < k10ToThe[17]";
                if (value >= k10ToThe[16])
                    size = 17;
                else if (value >= k10ToThe[15])
                    size = 16;
                else
                    size = 15;
            }
        }
        else {
            //cout << "\n    value < k10ToThe[15]";
            if (value >= k10ToThe[12]) {
                if (value >= k10ToThe[13])
                    size = 14;
                else
                    size = 13;
            }
            else {
                //cout << "\n    value < k10ToThe[13]";
                if (value >= k10ToThe[11])
                    size = 12;
                else
                    size = 11;
            }
        }
    }
    else if (value >= k10ToThe[4]) {
        //cout << "\n    value >= k10ToThe[4]";
        if (value >= k10ToThe[7]) {
            //cout << "\n    value >= k10ToThe[7]";
            if (value >= k10ToThe[9])
                size = 10;
            else if (value >= k10ToThe[8])
                size = 9;
            else
                size = 8;
        }
        else {
            //cout << "\n    value < k10ToThe[7]";
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


    if (text + size > text_end) {
        return nullptr;
    }
    text_end = &text[size - 1];
    //cout << "\n    size:" << size;
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
char* PrintSizeLut (uint32_t val, char* text, char* text_end) {
    if (!text) {
        return nullptr;
    }
    if (val == 0)
    {
        text = 0;
        return text;
    }

    int size;
    if (val >= 10000)
    {
        if (val >= 10000000)
        {
            if (val >= 1000000000)
                size = 10;
            else if (val >= 100000000)
                size = 9;
            else
                size = 8;
        }
        else
        {
            if (val >= 1000000)
                size = 7;
            else if (val >= 100000)
                size = 6;
            else
                size = 5;
        }
    }
    else {
        if (val >= 100)
         {
            if (val >= 1000)
                size = 4;
            else
                size = 3;
        }
        else {
            if (val >= 10)
                size = 2;
            else
                size = 1;
        }
    }

    if (text + size > text_end) {
        return nullptr;
    }
    char* c = &text[size - 1];
    while (val >= 100)
    {
        int pos = val % 100;
        val /= 100;
        *(short*)(c - 1) = *(short*)(kDigits00To99 + 2 * pos);
        c -= 2;
    }
    while (val>0)
    {
        *c-- = '0' + (val % 10);
        val /= 10;
    }
    return text;
}*/

#undef PRINT_PAUSE

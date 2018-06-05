/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/crabs_text.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough@gmail.com>;
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
// Dependencies:
#include "printer.h"
#if USING_PRINTER
#include "type.h"
// End dependencies.
#if MAJOR_SEAM == 1 && MINOR_SEAM == 2
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PUTCHAR(c) putchar(c);
#define PRINT_HEADING\
    std::cout << '\n';\
    for (int i = 80; i > 0; --i) std::cout << '-';
#else
#define PRINTF(x, ...)
#define PUTCHAR(c)
#define PRINT_HEADING
#endif

namespace _ {/** 
void PrintBinary (uint32_t value) {
    enum { kSize = sizeof (uint32_t) * 8 };
    
    std::cout << "\n    ";
    for (int i = kSize; i > 0; --i) {
        char c = (char)('0' + (value >> (kSize - 1)));
        std::cout << c;
        value = value << 1;
    }
}
Don't think I need this anymore. It was for chopping off the MSD but it
    was so slow it makes me shutter.

void PrintBinaryTable (uint32_t value) {
    enum { kSize = sizeof (uint32_t) * 8 };

    std::cout << "\n    ";
    for (int i = kSize; i > 0; --i) {
        char c = (char)('0' + (value >> (kSize - 1)));
        std::cout << c;
        value = value << 1;
    }
    std::cout << "\n    bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
                 "\n    33222222222211111111110000000000"
                 "\n    10987654321098765432109876543210"
                 "\n    ||  |  |   |  |  |   |  |  |   |"
                 "\n    |1  0  0   0  0  0   0  0  0   0"
                 "\n    |0  9  8   7  6  5   4  3  2   1";
}*/

char* PrintBinary (uint64_t value, char* buffer, char* buffer_end,
                   char delimiter) {
    if (!buffer) {
        return nullptr;
    }
    if (buffer + sizeof (uint64_t) * 8 >= buffer_end) {
        return nullptr;
    }

    for (int i = 0; i < 64; ++i) {
        *buffer++ = (char)('0' + (value >> 63));
        value = value << 1;
    }
    *buffer = delimiter;
    return buffer;
}

}       //< namespace _
#undef PRINTF
#undef PUTCHAR
#undef PRINT_HEADING
#endif  //< USING_PRINTER
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3

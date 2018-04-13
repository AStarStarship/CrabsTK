/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/crabs/crabs_text.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
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

#include "printer.h"

#if USING_PRINTER

#include "text.h"
#include "type.h"

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

namespace _ {


char* PrintHex (uint8_t value, char* buffer, char* buffer_end,
                char delimiter) {
    enum { kHexStringLengthSizeMax = sizeof (uint8_t) * 2 + 3 };

    assert (buffer);
    if (buffer >= buffer_end) {
        return nullptr;
    }
    assert (buffer_end - buffer > kHexStringLengthSizeMax);

    *buffer++ = '0';
    *buffer++ = 'x';
    for (int num_bits_shift = 0; num_bits_shift < sizeof (uint8_t) * 8;
         num_bits_shift += 8) {
        char c = (char)(value >> num_bits_shift);
        c = TextNibbleToUpperCaseHex (c);
        *buffer++ = c;
    }
    *buffer = delimiter;
    return buffer;
}

char* PrintHex (uint16_t value, char* buffer, char* buffer_end,
                char delimiter) {
    enum { kHexStringLengthSizeMax = sizeof (uint16_t) * 2 + 3 };

    assert (buffer);
    if (buffer >= buffer_end) {
        return nullptr;
    }
    assert (buffer_end - buffer > kHexStringLengthSizeMax);

    *buffer++ = '0';
    *buffer++ = 'x';
    for (int num_bits_shift = 0; num_bits_shift < sizeof (uint16_t) * 8;
         num_bits_shift += 8) {
        char c = (char)(value >> num_bits_shift);
        c = TextNibbleToUpperCaseHex (c);
        *buffer++ = c;
    }
    *buffer = delimiter;
    return buffer;
}

char* PrintHex (uint32_t value, char* buffer, char* buffer_end,
                char delimiter) {
    enum { kHexStringLengthSizeMax = sizeof (uint32_t) * 2 + 3 };

    assert (buffer);
    if (buffer >= buffer_end) {
        return nullptr;
    }
    assert (buffer_end - buffer >= kHexStringLengthSizeMax);

    *buffer++ = '0';
    *buffer++ = 'x';
    for (int num_bits_shift = 0; num_bits_shift < sizeof (uint32_t) * 8;
         num_bits_shift += 8) {
        char c = (char)(value >> num_bits_shift);
        c = TextNibbleToUpperCaseHex (c);
        *buffer++ = c;
    }
    *buffer = delimiter;
    return buffer;
}

char* PrintHex (uint64_t value, char* buffer, char* buffer_end,
                char delimiter) {
    enum { kHexStringLengthSizeMax = sizeof (uint64_t) * 2 + 3 };

    assert (buffer);
    if (buffer >= buffer_end) {
        return nullptr;
    }
    assert (buffer_end - buffer >= kHexStringLengthSizeMax);

    *buffer++ = '0';
    *buffer++ = 'x';
    for (int num_bits_shift = 0; num_bits_shift < sizeof (uint64_t) * 8;
         num_bits_shift += 8) {
        char c = (char)(value >> num_bits_shift);
        c = TextNibbleToUpperCaseHex (c);
        *buffer++ = c;
    }
    *buffer = delimiter;
    return buffer;
}

char* PrintHex (char c, char* buffer, char* buffer_end, char delimiter) {
    enum { kHexStringLengthSizeMax = sizeof (void*) * 2 + 3 };

    assert (buffer);
    if (buffer >= buffer_end) {
        return nullptr;
    }
    assert (buffer_end - buffer > 2);

    *buffer++ = TextNibbleToUpperCaseHex (c);
    *buffer = delimiter;
    return buffer;
}

}       //< namespace _
#undef PRINTF
#undef PUTCHAR
#endif  //< USING_PRINTER
#endif  //< #if MAJOR_SEAM >= 1 && MINOR_SEAM >= 2

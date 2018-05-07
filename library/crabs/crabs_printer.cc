/** Kabuki Toolkit
    @version 0.x
    @file    ~/libraries/crabs/crabs_text.cc
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

char* Print (const char* string, char* buffer, char* buffer_end, 
             char delimiter) {
    if (!string) {
        return nullptr;
    }
    if (!buffer) {
        return nullptr;
    }
    if (buffer >= buffer_end) {
        return nullptr;
    }
    char c = *string++;
    while (c) {
        *buffer++ = c;
        if (buffer >= buffer_end) {
            return nullptr;
        }
        c = *string++;
    }
    *buffer = delimiter;
    return buffer;
}

char* Print (const char* string, const char* string_end, char* buffer,
             char* buffer_end, char delimiter) {
    if (buffer) {
        return nullptr;
    }
    if (buffer >= buffer_end) {
        return nullptr;
    }
    if (!string) {
        return nullptr;
    }
    if (string >= string_end) {
        return nullptr;
    }
    char* cursor = buffer;
    char c = *string;
    while (c) {
        if (!c) {
            return buffer;
        }
        *buffer = c;
        if (++buffer >= buffer_end) {
            *buffer = delimiter;
            return nullptr;
        }
        if (++string >= string_end) {
            *buffer = delimiter;
            return nullptr;
        }
        c = *string;
    }
    *buffer = delimiter;
    return buffer;
}


char* Print (float value, char* buffer, char* buffer_end, char delimiter) {
    // @todo Replace with GrisuX algorithm that uses the Script ItoS Algorithm.
    intptr_t buffer_size = buffer_end - buffer,
             result = sprintf_s (buffer, buffer_size, "%f", value);
    if (result < 0) {
        *buffer = delimiter;
        return nullptr;
    }
    buffer += result;
    *buffer = delimiter;
    return buffer;
}

char* Print (double value, char* buffer, char* buffer_end, char delimiter) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = buffer_end - buffer,
             result = sprintf_s (buffer, buffer_size, "%f", value);
    if (result < 0) {
        *buffer = delimiter;
        return nullptr;
    }
    buffer += result;
    *buffer = delimiter;
    return buffer;
}

Printer::Printer (char* cursor, size_t buffer_size) :
    cursor (cursor),
    end    (cursor + buffer_size) {
    assert (cursor);
    assert (buffer_size);
}

Printer::Printer (char* cursor, char* end) :
    cursor (cursor),
    end    (end) {
    assert (cursor);
    assert (cursor < end);
}

Printer& Printer::Set (char* begin) {
    assert (begin);
    assert (begin < end);

    cursor = begin;
    return *this;
}

Dump::Dump (char* begin) :
    begin (begin) {
    // Nothing to do here! ({:-)-+=<
}

}       //< namespace _
#undef PRINTF
#undef PUTCHAR
#endif  //< USING_PRINTER
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 2

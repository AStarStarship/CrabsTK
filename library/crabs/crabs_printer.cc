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

char* TextStart () {
    static char buffer[kBufferSizeDefault];
    return reinterpret_cast<char*> (buffer);
}

char* TextEnd () {
    return reinterpret_cast<char*> (TextStart ()[kBufferSizeDefault - 1]);
}

char* Print (char* buffer, char* buffer_end, const char* string, 
             char delimiter) {
    assert (string);
    assert (buffer);

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

char* Print (char* cursor, char* end, const char* string, 
             const char* string_end, char delimiter) {
    assert (string);
    assert (string_end);
    assert (cursor);
    assert (end);

    if (cursor >= end) {
        return nullptr;
    }
    if (string >= string_end) {
        return nullptr;
    }

    char c = *string;
    while (c) {
        if (!c) {
            return cursor;
        }
        *cursor = c;
        if (++cursor >= end) {
            *cursor = delimiter;
            return nullptr;
        }
        if (++string >= string_end) {
            *cursor = delimiter;
            return nullptr;
        }
        c = *string;
    }
    *cursor = delimiter;
    return cursor;
}

char* Print (char* cursor, char* end, char character, char delimiter) {
    assert (cursor);
    assert (end);

    if (cursor + 1 >= end)
        return nullptr;
    *cursor++ = character;
    *cursor = delimiter;
    return cursor;
}

char* Print (char* cursor, char* end, float value, char delimiter) {
    // @todo Replace with GrisuX algorithm that uses the Script ItoS Algorithm.
    intptr_t buffer_size = end - cursor,
             result = sprintf_s (cursor, buffer_size, "%f", value);
    if (result < 0) {
        *cursor = delimiter;
        return nullptr;
    }
    cursor += result;
    *cursor = delimiter;
    return cursor;
}

char* Print (char* cursor, char* end, double value, char delimiter) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = end - cursor,
             result = sprintf_s (cursor, buffer_size, "%f", value);
    if (result < 0) {
        *cursor = delimiter;
        return nullptr;
    }
    cursor += result;
    *cursor = delimiter;
    return cursor;
}

Printer::Printer (char* cursor, size_t buffer_size) :
    cursor (cursor),
    end    (cursor + buffer_size - 1) {
    assert (cursor);
    assert (buffer_size);
}

Printer::Printer (char* cursor, char* end) :
    cursor (cursor),
    end    (end) {
    assert (cursor);
    assert (cursor < end);
}

Printer::Printer (const Printer& other) :
    cursor (other.cursor),
    end    (other.end) {
    // Nothing to do here! ({:-)-+=<
}

Printer& Printer::Set (char* begin) {
    assert (begin);
    assert (begin < end);

    cursor = begin;
    return *this;
}

Etx::Etx (char* begin) :
    begin (begin) {
    // Nothing to do here! ({:-)-+=<
}

}       //< namespace _
#undef PRINTF
#undef PUTCHAR
#endif  //< USING_PRINTER
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 2

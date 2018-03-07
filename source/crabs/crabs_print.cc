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

#include "print.h"

#if USING_TEXT_SCRIPT

#include "text.h"
#include "type.h"

void PrintBinary (uint32_t value) {
    enum { kSize = sizeof (uint32_t) * 8 };
    
    std::cout << "\n    ";
    for (int i = kSize; i > 0; --i) {
        char c = (char)('0' + (value >> (kSize - 1)));
        std::cout << c;
        value = value << 1;
    }
}
/** Don't think I need this anymore. It was for chopping off the MSD but it
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

#if MAJOR_SEAM == 1 && MINOR_SEAM == 2
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PUTCHAR(c) putchar(c);
#define PRINT_LINE\
    std::cout << '\n';\
    for (int i = 80; i > 0; --i) std::cout << '-';
#else
#define PRINTF(x, ...)
#define PUTCHAR(c)
#define PRINT_LINE
#endif

namespace _ {

char* Print (const char* string, char* buffer, char* buffer_end) {
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
    *buffer = c;
    return buffer;
}

char* Print (const char* text, const char* text_end, char* buffer,
             char* buffer_end) {
    if (buffer) {
        return nullptr;
    }
    if (buffer >= buffer_end) {
        return nullptr;
    }
    if (!text) {
        return nullptr;
    }
    if (text >= text_end) {
        return nullptr;
    }
    char* cursor = buffer;
    char c = *text;
    while (c) {
        if (!c) {
            return buffer;
        }
        *buffer = c;
        if (++buffer >= buffer_end) {
            *buffer = 0;
            return nullptr;
        }
        if (++text >= text_end) {
            *buffer = 0;
            return nullptr;
        }
        c = *text;
    }
    *buffer = c;
    return buffer;
}

char* PrintRight (const char* token, int num_columns, char* buffer, 
                  char* buffer_end) {
    char* start = buffer;
    if (!token) {
        return nullptr;
    }
    if (!buffer) {
        return nullptr;
    }
    if (buffer >= buffer_end) {
        return nullptr;
    }
    if (buffer + num_columns > buffer_end) {
        // Can't fit it in the buffer so this is an error.
        return nullptr;
    }
    
    //PRINTF ("\n\nPrinting \"%s\" aligned right %i columns", token, num_columns)

    intptr_t    length    = 0;     //< Length of the token.
    const char* token_end = token; //< Address of the last char.
    char*       cursor;            //< Print cursor pointer.
    char        c;                 //< Temp variable.
    
    // Find string length.
    while (*(++token_end));
    length = token_end - token;

    //PRINTF ("\n    Found length:%i", (int)length)

    if (token == token_end) {
        return buffer;
    }
    
    *(buffer + length) = 0;
    // If the length is less than the num_columns we need to print ".", "..", 
    // "..." or nothing and chop off some of the token.
    if (length > num_columns) {
        //PRINTF ("\n Wrote with dots:\"")
        intptr_t num_dots = length - num_columns;
        if (num_dots > 3) {
            num_dots = 3;
        }
        buffer_end = buffer + num_columns;
        cursor = buffer_end - num_dots;
        while (buffer < cursor) {
            c = *token++;
            *buffer++ = c;
            //PUTCHAR (c)
        }
        while (buffer < buffer_end) {
            *buffer++ = '.';
            //PUTCHAR ('.')
        }
        //PUTCHAR ('\"')
        *buffer = 0;
        return buffer;
    }
    //PRINTF ("\n Wrote:\"")
    cursor = buffer + num_columns;
    buffer_end = cursor - length;
    while (cursor >= buffer_end) {
        c = *token_end--;
        *cursor-- = c;
        //PUTCHAR (c)
    }
    //PUTCHAR ('\"')
    buffer_end = cursor - num_columns;
    //#if MAJOR_SEAM == 1 && MINOR_SEAM == 1 
    //printf ("\ncursor:0x%p text:0x%x delta:%i", cursor, text, text, cursor);
    //#endif
    while (cursor >= buffer) {
        *cursor-- = ' ';
    }
    buffer_end = buffer + num_columns;
    return buffer_end;
} 

char* PrintCentered (const char* string, int num_columns, char* text,
                     char* text_end) {
    if (!text) {
        return text;
    }
    if (!string) {
        return nullptr;
    }
    if (text >= text_end) {
        return nullptr;
    }
    if (num_columns < 2) {
        return nullptr;
    }
    // We need to leave at least one space to the left and right of
    int length = TextLength (string),
        offset;
    PRINTF ("\n\n    Printing \"%s\":%i num_columns:%i", string, length,
            num_columns)

    if (num_columns < length) {
        offset = length - num_columns;
        if (offset > 3) {
            offset = 3;
        }
        num_columns -= offset;
        while (num_columns-- > 0) {
            *text++ = *string++;
        }
        while (offset-- > 0) {
            *text++ = '.';
        }
        *text = 0;
        return text;
    }
    offset = (num_columns - length) >> 1; //< >> 1 to /2
    length = num_columns - length - offset;
    PRINTF ("\n    length:%i offset:%i", length, offset)
    
    while (length-- > 0) {
        *text++ = ' ';
    }
    char c = *string++;
    while (c) {
        *text++ = c;
        c = *string++;
    }
    while (offset-- > 0) {
        *text++ = ' ';
    }
    *text = c;
    PRINTF ("\n    Printed:\"%s\"", string);
    return text;
}

char* PrintLine (char c, int num_columns, char* text, char* text_end) {
    if (!text) {
        return nullptr;
    }
    if (text + num_columns > text_end) {
        return nullptr;
    }
    while (num_columns-- > 0) {
        *text++ = c;
    }
    *text = 0;
    return text;
}

char* PrintLine (const char* string, int num_columns, char* text,
                 char* text_end) {
    if (!text) {
        return nullptr;
    }
    if (text + num_columns > text_end) {
        return nullptr;
    }
    const char* cursor = string;
    while (num_columns-- > 0) {
        char c = *string++;
        if (!c) {
            cursor = string;
        }
        *text++ = c;
    }
    *text = 0;
    return text;
}

/*
Slot& PrintLineBreak (const char* message, int top_bottom_margin,
                      char c, int num_columns, Slot& slot) {
    PrintLines (top_bottom_margin, slot);
    slot << "\n " << message;
    return PrintLine (message, top_bottom_margin, c, "\n", num_columns, slot);
}*/

char* PrintHex (char c, char* text, char* text_end) {
    enum { kHexStringLengthSizeMax = sizeof (void*) * 2 + 3 };

    if (!text) {
        return nullptr;
    }
    if (text >= text_end) {
        return nullptr;
    }

    if (text_end - text < 2) {
        return nullptr;
    }
    *text++ = TextNibbleToUpperCaseHex (c);
    *text = 0;
    return text;
}

char* PrintHex (uintptr_t value, char* buffer, char* buffer_end) {
    enum { kHexStringLengthSizeMax = sizeof (void*) * 2 + 3 };

    if (!buffer) {
        return nullptr;
    }
    if (buffer > buffer_end) {
        return nullptr;
    }

    if (buffer_end - buffer < kHexStringLengthSizeMax) {
        return nullptr;
    }
    *buffer++ = '0';
    *buffer++ = 'x';
    for (int num_bits_shift = 0; num_bits_shift < sizeof (void*) * 8;
         num_bits_shift += 8) {
        char c = (char)(value >> num_bits_shift);
        c = TextNibbleToUpperCaseHex (c);
        *buffer++ = c;
    }
    *buffer = 0;
    return buffer;
}

Printer::Printer (char* cursor, size_t buffer_size) :
    cursor (cursor),
    end    (cursor + buffer_size) {
    assert (cursor != nullptr);
}

Printer::Printer (char* cursor, char* end) :
    cursor (cursor),
    end    (end) {
    assert (cursor != nullptr);
    assert (end != nullptr);

}


char* PrintBinary (uint64_t value, char* text, char* text_end) {
    if (!text) {
        return nullptr;
    }
    if (text + sizeof (uint64_t) * 8 >= text_end) {
        return nullptr;
    }

    for (int i = 0; i < 64; ++i) {
        *text++ = (char)('0' + (value >> 63));
        value = value << 1;
    }
    *text = 0;
    return text;
}


char* Print (float value, char* buffer, char* buffer_end) {
    // @todo Replace with GrisuX algorithm that uses the Script ItoS Algorithm.
    intptr_t buffer_size = buffer_end - buffer,
             result = sprintf_s (buffer, buffer_size, "%f", value);
    if (result < 0) {
        *buffer = 0;
        return nullptr;
    }
    buffer += result;
    *buffer = 0;
    return buffer;
}

char* Print (double value, char* buffer, char* buffer_end) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = buffer_end - buffer,
             result = sprintf_s (buffer, buffer_size, "%f", value);
    if (result < 0) {
        *buffer = 0;
        return nullptr;
    }
    buffer += result;
    *buffer = 0;
    return buffer;
}

char* PrintMemory (const void* token, const void* token_end, char* buffer,
                   char* buffer_end) {
    if (!token) {
        return nullptr;
    }
    if (!buffer) {
        return buffer;
    }
    if (buffer >= buffer_end) {
        return nullptr;
    }
    char* buffer_begin = buffer;
    const char* address_ptr     = reinterpret_cast<const char*> (token),
              * address_end_ptr = reinterpret_cast<const char*> (token_end);
    size_t      size            = address_end_ptr - address_ptr,
                num_columns     = size / 64;
    
    //PRINTF ("\n    Printing Buffer with length:%i", TextLength (token))

    if (size % 64 != 0) {
        ++num_columns;
    }
    size += 81 * (num_columns + 2);
    if (buffer + size >= buffer_end) {
        PRINTF ("\n    ERROR: buffer isn't big enough!")
        return nullptr;
    }
    *buffer++ = '\n';
    *buffer++ = '|';

    //  columns
    *buffer++ = '0';
    buffer = PrintRight (8, 7, buffer, buffer_end);
    for (int i = 16; i <= 64; i += 8) {
        buffer = PrintRight (i, 8, buffer, buffer_end);
    }
    *buffer++ = '|';
    *buffer++ = '\n';
    *buffer++ = '|';
    for (int j = 8; j > 0; --j) {
        for (int k = 7; k > 0; --k) {
            *buffer++ = '-';
        }
        *buffer++ = '+';
    }
    *buffer++ = '|';
    *buffer++ = ' ';
        
    buffer = PrintHex (address_ptr, buffer, buffer_end);
    char c;
    while (address_ptr < address_end_ptr) {
        *buffer++ = '\n';
        *buffer++ = '|';
        for (int i = 0; i < 64; ++i) {
            c = *address_ptr++;
            if (address_ptr > address_end_ptr)
                c = 'x';
            if (c < ' ') {
                c = ' ';
            }
            *buffer++ = c;
        }
        *buffer++ = '|';
        *buffer++ = ' ';
        buffer = PrintHex (address_ptr, buffer, buffer_end);
        //PRINT_LINE
        //PRINTF ("\n%s", buffer_begin)
        //PRINT_LINE
    }
    *buffer++ = '\n';
    *buffer++ = '|';
    for (int j = 8; j > 0; --j) {
        for (int k = 7; k > 0; --k) {
            *buffer++ = '-';
        }
        *buffer++ = '+';
    }
    *buffer++ = '|';
    *buffer++ = ' ';
    return PrintHex (address_ptr + size, buffer, buffer_end);
}

}       //< namespace _
#undef PRINTF
#undef PUTCHAR
#endif  //< USING_TEXT_SCRIPT
#endif  //< #if MAJOR_SEAM >= 1 && MINOR_SEAM >= 2

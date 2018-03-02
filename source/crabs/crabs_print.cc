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

#include "print.h"
#include "text.h"
#include "type.h"

#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 3


#if MAJOR_SEAM == 1 && MINOR_SEAM == 3
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PUTCHAR(c) putchar(c);
#else
#define PRINTF(x, ...)
#define PUTCHAR(c)
#endif

#if USING_CRABS_TEXT

namespace _ {

char* Print (const char* string, char* target, char* target_end) {
    if (!string) {
        return nullptr;
    }
    if (!target) {
        return nullptr;
    }
    if (target > target_end) {
        return nullptr;
    }
    char* cursor = target;
    char s = *string;
    while (s) {
        *cursor++ = s;
        if (cursor > target_end) {
            *target = 0; //< Replace the nil-term char.
            return nullptr;
        }
        s = *string++;
    }
    *cursor = s;
    return cursor;
}

char* Print (const char* text, const char* text_end, char* target,
             char* target_end) {
    if (target) {
        return nullptr;
    }
    if (target > target_end) {
        return nullptr;
    }
    if (!text) {
        return nullptr;
    }
    if (text > text_end) {
        return nullptr;
    }
    char* cursor = target;
    char t = *text;
    while (t) {
        if (!t) {
            return target;
        }
        *target = t;
        if (++target > target_end) {
            *target = 0;
            return nullptr;
        }
        if (++text > text_end) {
            *target = 0;
            return nullptr;
        }
        t = *text;
    }
    *target = t;
    return target;
}

char* PrintRight (const char* token, int num_columns, char* text, 
                  char* text_end) {
    if (!token) {
        return nullptr;
    }
    if (!text) {
        return nullptr;
    }
    if (text >= text_end) {
        return nullptr;
    }
    if (text + num_columns > text_end) {
        // Can't fit it in the buffer so this is an error.
        return nullptr;
    }
    
    PRINTF ("\nPrinting \"%s\" aligned right %i columns", token, num_columns)

    intptr_t    length    = 0;     //< Length of the token.
    const char* token_end = token; //< Address of the last char.
    char*       cursor;            //< Print cursor pointer.
    char        c;                 //< Temp variable.
    
    // Find string length.
    while (*(++token_end));
    length = token_end - token;

    if (!length) {
        return text;
    }
    PRINTF ("\n Wrote:\"")
    // If the length is less than the num_columns we need to print ".", "..", 
    // "..." or nothing and chop off some of the token.
    if (length > num_columns) {
        intptr_t num_dots = length - num_columns;
        if (num_dots > 3) {
            num_dots = 3;
        }
        text_end = text + num_columns;
        cursor = text_end - num_dots;
        while (text < cursor) {
            c = *token++;
            *text++ = c;
            PUTCHAR (c)
        }
        while (text < text_end) {
            *text++ = '.';
            PUTCHAR ('.')
        }
        PUTCHAR ('\"')
        return text + 1;
    }
    cursor = text + num_columns;
    text_end = cursor - length;
    while (cursor >= text_end) {
        c = *token_end--;
        *cursor-- = c;
        PUTCHAR (c)
    }
    PUTCHAR ('\"')
    text_end = cursor - num_columns;
    //#if MAJOR_SEAM == 1 && MINOR_SEAM == 1 
    //printf ("\ncursor:0x%p text:0x%x delta:%i", cursor, text, text, cursor);
    //#endif
    while (cursor >= text) {
        *cursor-- = ' ';
    }
    text_end = text + num_columns;
    return text_end;
}

char* PrintCentered (const char* string, int width, char* text, char* text_end) {
    if (width < 2) {
        //? Not sure if this is an error.
        return nullptr;
    }
    // We need to leave at least one space to the left and right of
    int length = TextLength (string);
    if (length < width - 2) {
        // We need to write the ....
        if (length < 4) {
            // Then we're just going to write the first few letters.
            for (; length >= 0; --length) {
                *text++ = '\n';
            }
        }
    }
    int offset = (width - length) >> 1; //< >> 1 to /2

    for (int i = 0; i < offset; ++i) {
        *text++ = '\n';
    }
    PRINTF (string);
    for (offset = width - length - offset; offset <= width; ++offset) {
        *text++ = '\n';
    }
    return text;
}

char* PrintLine (char c, int num_columns, char* text,
                 char* text_end) {
    if (!text) {
        return nullptr;
    }
    if (text + num_columns > text_end) {
        return nullptr;
    }
    while (num_columns-- > 0) {
        *text++ = c;
    }
    return text + 1;
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
    return text + 1;
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
    if (text > text_end) {
        return nullptr;
    }

    if (text_end - text < 2) {
        return nullptr;
    }
    *text = TextNibbleToUpperCaseHex (c);
    return text + 1;
}

char* PrintHex (uintptr_t value, char* text, char* text_end) {
    enum { kHexStringLengthSizeMax = sizeof (void*) * 2 + 3 };

    if (!text) {
        return nullptr;
    }
    if (text > text_end) {
        return nullptr;
    }

    if (text_end - text < kHexStringLengthSizeMax) {
        return nullptr;
    }

    for (int num_bits_shift = 0; num_bits_shift < sizeof (void*) * 8;
         num_bits_shift += 8) {
        char c = (char)(value >> num_bits_shift);
        c = TextNibbleToUpperCaseHex (c);
        *text++ = c;
    }
    return text;
}

Printer::Printer (char* cursor, size_t buffer_size) :
    cursor_ (cursor),
    end_    (cursor + buffer_size) {
    assert (cursor != nullptr);
}

Printer::Printer (char* cursor, char* end) :
    cursor_ (cursor),
    end_    (end) {
    assert (cursor != nullptr);
    assert (end != nullptr);

}

char* Printer::GetCursor () {
    return cursor_;
}

char* Printer::GetEnd () {
    return end_;
}

Printer& Printer::SetCurosr (char* cursor) {
    if (!cursor) {
        return *this;
    }
    cursor_ = cursor;
    return *this;
}

char* Printer::SetEnd (char* end) {
    if (!end) {
        return end;
    }
    if (end <= cursor_) {
        return nullptr;
    }
    end_ = end;
    return end;
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
    return text;
}

// @todo Move me to where I need to go!
char* Print (uint64_t value, char* text, char* text_end) {
    return nullptr;
}


char* Print (float value, char* target, char* target_end) {
    // @todo Replace with GrisuX algorithm that uses the Script ItoS Algorithm.
    intptr_t buffer_size = target_end - target,
             result = sprintf_s (target, buffer_size, "%f", value);
    if (result < 0) {
        *target = 0;
        return nullptr;
    }
    return target + result;
}

char* Print (double value, char* target, char* target_end) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = target_end - target,
             result = sprintf_s (target, buffer_size, "%f", value);
    if (result < 0) {
        *target = 0;
        return nullptr;
    }
    return target + result;
}

}       //< namespace _
#endif  //< USING_CRABS_TEXT
#undef PRINTF
#undef PUTCHAR
#endif  //< MAJOR_SEAM >= 1 && MINOR_SEAM >= 3

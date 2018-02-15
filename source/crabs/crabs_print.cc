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

#if CRABS_SEAM >= 1 || CRABS_SEAM == 0


#if CRABS_SEAM == 1
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PUTCHAR(c) putchar(c);
#else
#define PRINTF(x, ...)
#define PUTCHAR(c)
#endif

#if USING_CRABS_TEXT

namespace _ {

char* Print (const char* string, char* target, char* target_end, char delimiter) {
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
    while (s > delimiter) {
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

Slot& Print (const char* text, Slot& slot, char delimiter) {
    if (!text) {
        return slot;
    }
    char* cursor = slot.stop + 1,
        * end = slot.end;
    char c = *text;
    ++text;
    while (c > delimiter) {
        if (cursor > end) {
            cursor = slot.begin;
            end    = slot.start;
            if (cursor == end) {
                return slot;
            }
        }
        *cursor++ = c;
        c = *text++;
    }
    *cursor = 0;
    return slot;
}

char* Print (const char* text, const char* text_end, char* target,
             char* target_end, char delimiter) {
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
    while (t > delimiter) {
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

Slot& Print (const char* text, const char* text_end, Slot& slot, char delimiter) {
    if (!text) {
        return slot;
    }
    if (text > text_end) {
        return slot;
    }
    char* cursor = slot.stop + 1,
        * end = slot.end;
    char c = *text;
    ++text;
    while (c > delimiter) {
        if (cursor > end) {
            cursor = slot.begin;
            end    = slot.start;
            if (cursor == end) {
                return slot;
            }
        }
        *cursor++ = c;
        c = *text++;
    }
    *cursor = 0;
    return slot;
}

Slot& Print (int64_t value, Slot& slot, char delimiter) {
    enum {
        kSizeMax = 22,
    };

    char * begin,
         * start = slot.start,
         * cursor = slot.stop,
         * end;
    size_t size;
    //int    num_chars;

    // First we need to check if the ring buffer is wrapped around.
    if (start > cursor) {
        // 1a. Check if there is enough room in the upper chunk first.
        end = slot.end;
        size = end - cursor;
        if (size < kSizeMax) {
            // Add the lower chunk size to the size
            begin = slot.begin;
            end = slot.end;
            size += (start - begin);
            if (size < kSizeMax) {
                return slot;
            }
            cursor = Print (value, cursor, end + kSizeMax, delimiter);
            // Check if it was an Overflow Wraparound and shift the bytes into
            // the lower chunk if it was.
            cursor = SlotOverflowShift (begin, end, cursor);
        } else {
            end = slot.end;
            cursor = Print (value, cursor, end + kSizeMax, delimiter);
        }
    }
    else {
        // 2b. Check if there enough room in the buffer.
        size = start - cursor;
        if (size < kSizeMax) {
            return slot;
        }
        // 3b. There is no buffer overflow for this branch
        //     so just write the string like any old string.
        end = slot.end;
        cursor = Print (value, cursor, end + kSizeMax, delimiter);
        if (!cursor) {
            return slot;
        }
    }
    // 4. Update the ring buffer stop.
    slot.stop = cursor;
    return slot;
}

Slot& Print (uint64_t value, Slot& slot, char delimiter) {
    enum {
        kSizeMax = 21,
    };

    char * begin,
         * start = slot.start,
         * cursor = slot.stop,
         * end;
    size_t size;
    //int    num_chars;

    // First we need to check if the ring buffer is wrapped around.
    if (start > cursor) {
        // 1a. Check if there is enough room in the upper chunk first.
        end = slot.end;
        size = end - cursor;
        if (size < kSizeMax) {
            // Add the lower chunk size to the size
            begin = slot.begin;
            end = slot.end;
            size += (start - begin);
            if (size < kSizeMax) {
                // @todo Do I need to clear the buffer here in case of attack?
                return slot;
            }
            cursor = Print (value, cursor, end + kSizeMax, delimiter);
            // Check if it was an Overflow Wraparound and shift the bytes into
            // the lower chunk if it was.
            cursor = SlotOverflowShift (begin, end, cursor);
        }
        else {
            cursor = Print (value, cursor, end + kSizeMax, delimiter);
        }
    }
    else {
        // 2b. Check if there enough room in the buffer.
        size = start - cursor;
        if (size < kSizeMax) {
            return slot;
        }
        // 3b. There is no buffer overflow for this branch
        //     so just write the string like any old string.
        end = slot.end;
        cursor = Print (value, cursor, end + kSizeMax, delimiter);
        if (!cursor) {
            return slot;
        }
    }
    // 4. Update the ring buffer stop.
    slot.stop = cursor;
    return slot;
}

char* Print (float value, char* target, char* target_end, char delimiter) {
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
/*
inline size_t SlotSpaceFast (Slot& slot, char* start, char*stop) {
    if (start > stop) {
        return size + (start - slot.begin);
    }
    return stop - start;
}

inline char* SlotLastAddress (Slot& slot, char* start, char*stop, char* end, int size) {
    size_t space;
    if (stop < start) {
        if (stop - start < size) {
            return nullptr;
        }
    }
    space = end - stop;
    if (space < size) {
        if (space + (start - slot.begin) < size) {
            return nullptr;
        }
        return nullptr;
    }
    return stop + size;
}*/

Slot& Print (float value, Slot& slot, char delimiter) {
    char * begin,
         * start = slot.start,
         * cursor = slot.stop,
         * end;
    size_t size;
    //int    num_chars;



    // First we need to check if the ring buffer is wrapped around.
    if (start > cursor) {
        // 1a. Check if there is enough room in the upper chunk first.
        end = slot.end;
        size = end - cursor;
        if (size < kkFloat32DigitsMax) {
            // Add the lower chunk size to the size
            begin = slot.begin;
            end = slot.end;
            size += (start - begin);
            if (size < kkFloat32DigitsMax) {
                // @todo Do I need to clear the buffer here in case of attack?
                return slot;
            }
            cursor = Print (value, cursor, end + kkFloat32DigitsMax, delimiter);
            // Check if it was an Overflow Wraparound and shift the bytes into
            // the lower chunk if it was.
            cursor = SlotOverflowShift (begin, end, cursor);
        }
        else {
            end = slot.end;
            cursor = Print (value, cursor, end + kkFloat32DigitsMax, delimiter);
        }
    }
    else {
        // 2b. Check if there enough room in the buffer.
        size = start - cursor;
        if (size < kkFloat32DigitsMax) {
            return slot;
        }
        // 3b. There is no buffer overflow for this branch
        //     so just write the string like any old string
        end = slot.end;
        cursor = Print (value, cursor, end + kkFloat32DigitsMax, delimiter);
        if (!cursor) {
            return slot;
        }
    }
    // 4. Update the ring buffer stop.
    slot.stop = cursor;
    return slot;
}

char* Print (double value, char* target, char* target_end, char delimiter) {
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

Slot& Print (double value, Slot& slot, char delimiter) {
    char * begin,
         * start  = slot.start,
         * cursor = slot.stop,
         * end;
    size_t size;
    //int    num_chars;

    // First we need to check if the ring buffer is wrapped around.
    if (start > cursor) {
        // Check if there is enough room in the upper chunk first.
        end = slot.end;
        size = end - cursor;
        if (size < kFloat64DigitsMax) {
            // Add the lower chunk size to the size
            begin = slot.begin;
            end = slot.end;
            size += (start - begin);
            if (size < kFloat64DigitsMax) {
                // @todo Do I need to clear the buffer here in case of attack?
                return slot;
            }
            cursor = Print (value, cursor, end + kFloat64DigitsMax, delimiter);
            // Check if it was an Overflow Wraparound and shift the bytes into
            // the lower chunk if it was.
            cursor = SlotOverflowShift (begin, end, cursor);
        }
        else {
            end = slot.end;
            cursor = Print (value, cursor, end + kFloat64DigitsMax, delimiter);
        }
    }
    else {
        // 2b. Check if there enough room in the buffer.
        size = start - cursor;
        if (size < kFloat64DigitsMax) {
            return slot;
        }
        // 3b. There is no buffer overflow for this branch
        //     so just write the string like any old string.
        end = slot.end;
        cursor = Print (value, cursor, end + kFloat64DigitsMax);
    }
    // 4. Update the ring buffer stop.
    slot.stop = cursor;
    return slot;
}

char* PrintRight (const char* token, int num_columns, char* text, 
                  char* text_end, char delimiter) {
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
    while (*(++token_end) > delimiter);
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
        *text = delimiter;
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
    //#if CRABS_SEAM == 1 
    //printf ("\ncursor:0x%p text:0x%x delta:%i", cursor, text, text, cursor);
    //#endif
    while (cursor >= text) {
        *cursor-- = ' ';
    }
    text_end = text + num_columns;
    *text_end = delimiter;
    return text_end;
}

Slot& PrintRight (const char* token, int num_columns, Slot& slot,
                  char delimiter) {
    char    * start  = slot.start,
            * cursor = slot.stop,
            * end    = slot.end;
    uintptr_t size;
    //char      c;
    if (cursor < start) {
        size = start - cursor;
        slot.stop = PrintRight (token, num_columns, cursor, start - 1);
        return slot;
    }
    size = start - cursor;
    if (size < num_columns) {
        size += cursor - slot.begin;
        if (size < num_columns) {
            return slot;
        }
    }
    for (; num_columns > 0; --num_columns) {
        *cursor++ = *token++;
        if (cursor > end) {
            cursor = slot.begin;
        }
    }
    slot.stop = cursor;
    return slot;
}

char* PrintCentered (const char* string, int width, char* text, char* text_end,
                     char delimiter) {
    if (width < 2) {
        //? Not sure if this is an error.
        return nullptr;
    }
    // We need to leave at least one space to the left and right of
    int length = TextLength (string, delimiter);
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

Slot& PrintCentered (const char* string, int width, Slot& slot,
                     char delimiter) {
    return slot;
}

/*
Slot& SlotWrite (Slot& slot, const char* text,
                   char delimiter) {
    if (!text) {
        return slot;
    }
    char* cursor = target;
    char s = *text;
    while (s != delimiter) {
        if (!s) {
            *target = 0; //< Replace the nil-term char.
            return slot;
        }
        *cursor = s;
        if (++cursor > target_end) {
            *target = 0; //< Replace the nil-term char.
            return slot;
        }
        ++text;
        s = *text;
    }
    *cursor = s;
    return slot;
}

Slot& SlotWrite (Slot& slot, const char* text,
                   const char* text_end) {
    if (!text) {
        return slot;
    }
    if (text > text_end) {
        return slot;
    }
    char* cursor = target;
    char t = *text;
    while (t) {
        *cursor = t;
        if (++cursor > target_end) {
            *target = 0;
            return slot;
        }
        if (++text > text_end) {
            *target = 0;
            return slot;
        }
        t = *text;
    }
    *cursor = t;
    return slot;
}

Slot& SlotWrite (Slot& slot, const char* text,
                   const char* text_end, char delimiter) {
    if (target > target_end) {
        return slot;
    }
    if (!text) {
        return slot;
    }
    if (text > text_end) {
        return slot;
    }
    char* cursor = target;
    char t = *text;
    while (t != delimiter) {
        if (!t) {
            //std::cout << "\n There was a !t error in SlotWrite.";
            return target;
        }
        *target = t;
        if (++target > target_end) {
            *target = 0;
            return slot;
        }
        if (++text > text_end) {
            *target = 0;
            return slot;
        }
        t = *text;
    }
    *target = t;
    return target;
}
*/
/*
Slot& PrintLine (const char* text, const char* header, int length, Slot& slot) {
    #if CRABS_SEAM == 1
    std::cout << header << "Writing Line with length " << length;
    #endif

    char* begin,
        * start  = slot.start,
        * cursor = slot.stop,
        * end;
    intptr_t size;

    if (cursor < start) {
        size = cursor - start;
        if (size < length + 1) {
            return slot;
        }
        TextLine (cursor, stop, header);
    } else {

    }

    SlotWrite (slot, cursor, end, header);

    if (!cursor) {
        #if CRABS_SEAM == 1
        std::cout << "\n Error writing header!";
        #endif
        return slot;
    }
    #if CRABS_SEAM == 1
    std::cout << "... wrote " << TextLength (cursor) << " chars.";
    #endif
    
    stop = cursor + length;
    if ((cursor + length) > end) {
        stop = end;
    }
    #if CRABS_SEAM == 1
    std::cout << "\n new_stop_length:" << stop - cursor;
    #endif

    while (cursor < stop) {
        *cursor++ = text;
    }
    *cursor = 0;
    #if CRABS_SEAM == 1
    std::cout << "\n - Wrote " << SlotLength (cursor) << " chars: " 
              << cursor;
    #endif
    slot.stop = cursor;
    return slot;
}*/

char* PrintLine (char c, int num_columns, char* text,
                 char* text_end, char delimiter) {
    if (!text) {
        return nullptr;
    }
    if (text + num_columns > text_end) {
        return nullptr;
    }
    while (num_columns-- > 0) {
        *text++ = c;
    }
    *text = delimiter;
    return text + 1;
}

Slot& PrintLine (char c, int num_columns, Slot& slot,
                 char delimiter) {
    char* start = slot.start,
        *cursor = slot.stop,
        *end = slot.end;
    intptr_t size;
    if (num_columns < 1) {
        return slot;
    }
    if (start > cursor) {
        size = start - cursor;
        if (size < num_columns) {
            size += cursor - slot.begin;
            if (size < num_columns) {
                return slot;
            }
        }
    }
    else if (cursor - start < num_columns) {
        return slot;
    }

    while (num_columns > 0) {
        if (cursor > end) {
            cursor = slot.begin;
        }
        *cursor++ = c;
    }
    *cursor = delimiter;
    slot.stop = cursor;
    return slot;
}

char* PrintLine (const char* string, int num_columns, char* text,
                 char* text_end, char delimiter) {
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
    *text = delimiter;
    return text + 1;
}

Slot& PrintLine (const char* string, int num_columns, Slot& slot,
                 char delimiter) {
    char* start  = slot.start,
        * cursor = slot.stop,
        * end    = slot.end;
    intptr_t size;
    const char* read = string;
    if (num_columns < 1) {
        return slot;
    }
    if (!string) {
        return slot;
    }
    if (start > cursor) {
        size = start - cursor;
        if (size < num_columns) {
            size += cursor - slot.begin;
            if (size < num_columns) {
                return slot;
            }
        }
    }
    else if (cursor - start < num_columns) {
        return slot;
    }

    while (num_columns > 0) {
        char c = *read++;
        if (!c) {
            read = string;
        }
        if (cursor > end) {
            cursor = slot.begin;
        }
        *cursor++ = c;
    }
    *cursor = delimiter;
    slot.stop = cursor;
    return slot;
}

/*
Slot& PrintLineBreak (const char* message, int top_bottom_margin,
                      char c, int num_columns, Slot& slot, char delimiter) {
    PrintLines (top_bottom_margin, slot);
    slot << "\n " << message;
    return PrintLine (message, top_bottom_margin, c, "\n", num_columns, slot);
}*/

Slot& PrintLines (int num_rows, Slot& slot, char delimiter) {
    char* cursor = slot.stop + 1,
        *end = slot.end,
        *stop = cursor + num_rows + 1;
    uintptr_t upper_size = end - cursor + 1;
    if (upper_size < num_rows) {
        while (cursor < stop) {

        }
    }
    while (num_rows > 0) {
        *cursor++ = '\n';
    }
    *cursor = delimiter;
    slot.stop = cursor + 1;
    return slot;
}

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

Slot& PrintHex (char c, Slot& slot, char delimiter) {
    uint16_t chars = TextByteToUpperCaseHex (c);
    return slot << (char)chars << ((char)(chars >> 8)) << ' ';
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

Slot& PrintHex (uintptr_t value, Slot& slot) {
    enum { kHexStringLengthSizeMax = sizeof (void*) * 2 + 3 };

    // @todo Replace this with faster algorithm.
    char    * start = slot.start,
            * cursor = slot.stop + 1,
            * end    = slot.end;
            //* value_ptr;              //< Pointer to a byte in value.
    char      c;                        //< Temp variable.
    uintptr_t size;                     // Current size we care about.

    if (cursor < start) {
        size = start - cursor;
        if (size < kHexStringLengthSizeMax) {
            return slot;
        }
    }
    else {
        size = end - cursor;
        if (size < kHexStringLengthSizeMax) {
            size += (slot.start - slot.begin);
            if (size < kHexStringLengthSizeMax) {
                return slot;
            }
        }
    }

    for (int num_bits_shift = 0; num_bits_shift < sizeof (void*) * 8; 
         num_bits_shift += 8) {
        c = (char)(value >> num_bits_shift);
        c = TextNibbleToUpperCaseHex (c);
        if (++cursor > end) {
            cursor = slot.begin;
        }
    }
    slot.stop = cursor;
    return slot;
}

Slot& PrintMemory (const void* address, const void* address_end, Slot& slot,
                   char delimiter) {
    slot << "\n " << 0;
    //  columns
    char* start = slot.start,
        *cursor = slot.stop,
        *end = slot.end;
    const char* address_ptr     = reinterpret_cast<const char*> (address),
              * address_end_ptr = reinterpret_cast<const char*> (address_end);

    for (int i = 8; i <= 66; i += 8) {
        PrintRight (i, 8, slot);
    }
    slot << "\n|";
    for (int i = 0; i < 65; ++i) {
        *cursor++ = '_';
        if (cursor > end) {
            cursor = slot.begin;
        }
    }

    const char* chars = reinterpret_cast<const char*> (address);
    char temp;
    while (chars < cursor) {
        *cursor++ = '\n';
        if (cursor > end) {
            cursor = slot.begin;
        }
        *cursor++ = '|';
        if (cursor > end) {
            cursor = slot.begin;
        }
        for (int i = 0; i < 64; ++i) {
            temp = *chars++;
            if (chars >= cursor)
                temp = 'x';
            *cursor++ = temp;
            if (cursor > end) {
                cursor = slot.begin;
            }
        }
        *cursor++ = '|';
        if (cursor > end) {
            cursor = slot.begin;
        }
        *cursor++ = ' ';
        if (cursor > end) {
            cursor = slot.begin;
        }
        PrintHex (chars + (address_end_ptr - address_ptr), slot);
    }
    *cursor++ = '\n';
    if (cursor > end) {
        cursor = slot.begin;
    }
    *cursor++ = '|';
    if (cursor > end) {
        cursor = slot.begin;
    }
    for (int i = 0; i < 64; ++i) {
        slot << '_';
    }
    *cursor++ = '|';
    if (cursor > end) {
        cursor = slot.begin;
    }
    *cursor++ = ' ';
    if (cursor > end) {
        cursor = slot.begin;
    }
    return PrintHex (chars + (address_end_ptr - address_ptr), slot);
}

Slot& PrintError (const char* message, const char* end_string, Slot& slot) {
    return slot << "\nError: " << message << end_string;
}

Slot& PrintBsq (const uint_t* params, Slot& slot) {
    if (!params) {
        return slot << "nil";
    }
    if (params < (const uint_t*)256) {
        return slot << reinterpret_cast<uintptr_t> (params) << '\n';
    }
    uint_t num_params = *params++,
        i,
        type,
        value = 0;

    slot << "Param<";
    if (num_params > _::kParamsMax) {
        return slot << "\nInvalid num_params: " << num_params;
    }
    slot << num_params << ": ";
    for (i = 1; i < num_params; ++i) {
        value = *params++;
        type = value & 0x1f;    //< Mask off type.
        value = value >> 5;     //< Shift over array type.
        slot << TypeString (value) << ", ";
        if (type >= STR) {
            if (value) {
                return slot << "\nError: arrays may only be created from POD types.";
            }
            // Print out the max length of the string.
            ++i;
            value = *params++;
            slot << value;
        }
        else if (value > 31) {
            if (value > 127) {      //< It's a multi-dimensional array.
                slot << "Multi-dimensional Array:" << value << ", ";
            }
            // Then it's an array.
            ++i;
            switch (value) {        //< Print out the Array type.
                case 0: {
                    break;
                }
                case 1: {
                    value = *params++;
                    slot << "UI1:" << value << ", ";
                    break;
                }
                case 2: {
                    value = *params++;
                    slot << "UI2:" << value << ", ";
                    break;
                }
                case 3: {
                    value = *params++;
                    slot << "UI4:" << value << ", ";
                    break;
                }
                case 4: {
                    value = *params++;
                    slot << "UI8:" << value << ", ";
                    break;
                }
                case 5: {
                    value = *params++;
                    if (value == 0) {
                        slot << "UI1:[0]";
                        break;
                    }
                    slot << "UI1:[" << value << ": ";
                    for (uint_t i = value; i != 0; --i) {
                        value = *params++;
                        slot << value << ", ";
                    }
                    value = *params++;
                    slot << value << "]";
                    break;
                }
                case 6: {
                    value = *params++;
                    if (value == 0) {
                        slot << "UI2:[0]";
                        break;
                    }
                    slot << "UI2:[" << value << ": ";
                    for (uint_t i = value; i != 0; --i) {
                        value = *params++;
                        slot << value << ", ";
                    }
                    value = *params++;
                    slot << value << "]";
                    break;
                }
                case 7: {
                    value = *params++;
                    if (value == 0) {
                        slot << "UI4:[0]";
                        break;
                    }
                    slot << "UI4:[" << value << ": ";
                    for (uint_t i = value; i != 0; --i) {
                        value = *params++;
                        slot << value << ", ";
                    }
                    value = *params++;
                    slot << value << "]";
                    break;
                }
            }
        }
    }
    // Do the last set without a comma.
    value = *params++;
    slot << TypeString (value) << ", ";
    if (value == STR) {
        ++i;
        value = *params++;
        slot << value;
    }
    else if (value > 31) {
        // Then it's an array.
        type = value & 0x1f;    //< Mask off type.
        value = value >> 5;     //< Shift over array type.
        ++i;
        switch (value) {
            case 0:
            {
                break;
            }
            case 1:
            {
                value = *params++;
                slot << "UI1:" << value << ", ";
                break;
            }
            case 2:
            {
                value = *params++;
                slot << "UI2:" << value << ", ";
                break;
            }
            case 3:
            {
                value = *params++;
                slot << "UI4:" << value << ", ";
                break;
            }
            case 4:
            {
                value = *params++;
                slot << "UI5:" << value << ", ";
                break;
            }
            case 5:
            {
                value = *params++;
                if (value == 0) {
                    slot << "UI1:[0]";
                    break;
                }
                slot << "UI1:[" << value << ": ";
                for (uint_t i = value; i != 0; --i) {
                    value = *params++;
                    slot << value << ", ";
                }
                value = *params++;
                slot << value << "]";
                break;
            }
            case 6:
            {
                value = *params++;
                if (value == 0) {
                    slot << "UI2:[0]";
                    break;
                }
                slot << "UI2:[" << value << ": ";
                for (uint_t i = value; i != 0; --i) {
                    value = *params++;
                    slot << value << ", ";
                }
                value = *params++;
                slot << value << "]";
                break;
            }
            case 7:
            {
                value = *params++;
                if (value == 0) {
                    slot << "UI4:[0]";
                    break;
                }
                slot << "UI4:[" << value << ": ";
                for (uint_t i = value; i != 0; --i) {
                    value = *params++;
                    slot << value << ", ";
                }
                value = *params++;
                slot << value << "]";
                break;
            }
        }
    }
    return slot << '>';
}

Slot& PrintOp (const Op* op, Slot& slot) {
    slot << "\n Op:\n" << op->name << "\nparams_in:";

    PrintBsq (op->in, slot);
    slot << "\nparams_out:";
    PrintBsq (op->out, slot);
    slot << "\npop:" << op->pop << " close:" << op->close
         << " default_op:" << op->default_op
         << "\nignore_chars:" << op->ignore_chars
         << "\nallowed_chars:" << op->allowed_chars
         << "\n description :\"" << op->description;
    return slot;
}

}       //< namespace _
#endif  //< USING_CRABS_TEXT
#undef PRINTF
#undef PUTCHAR
#endif  //< CRABS_SEAM >= 1 || CRABS_SEAM == 0

/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/hex.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#pragma once
#include <stdafx.h>

#ifndef HEADER_FOR_CRABS_HEX
#define HEADER_FOR_CRABS_HEX

#include "printer.h"

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 2

namespace _ {

/** Utility class for printing hex with operator<<. */
template<typename T = uintptr_t>
class Hex {
    public:

    T value;    //< Value to convert to hex.

    /** Constructor saves value for use with operator overloads. */
    Hex (T value) :
        value (value) {
    }

    /** Constructor saves value for use with operator overloads. */
    Hex (void* pointer) :
        value (reinterpret_cast<T> (pointer)) {
    }

    Printer& Print (Printer& printer) {
        enum { kSize = sizeof (T) * 2 + 2 };
        char* l_cursor = printer.cursor;
        char* temp = l_cursor;
        intptr_t space = printer.end - l_cursor;
        if (space <= kSize) return print;
        *l_cursor++ = '0';
        *l_cursor++ = 'x';
        for (int num_bits_shift = sizeof (T) * 8 - 4; num_bits_shift >= 0;
             num_bits_shift -= 4) {
            *l_cursor++ = HexNibbleToUpperCase ((byte)(value >> num_bits_shift));
        }
        *l_cursor = 0;
        printer.cursor = l_cursor;
        return print;
    }
};

/** Prints the given byte in Hex.
    This function prints the hex in big endian.
    @param  buffer     Beginning of the buffer.
    @param  buffer_end End of the buffer.
    @return            Null upon failure or a pointer to the byte after the 
                       last byte written. */
KABUKI char* PrintHex (char c, char* text, char* text_end, char delimiter = 0);

/** Print's out the given word to the text buffer.
    @param  buffer     Beginning of the buffer.
    @param  buffer_end End of the buffer.
    @return            Null upon failure or a pointer to the byte after the last 
                       byte written. */
KABUKI char* PrintHex (uint8_t value, char* buffer, char* buffer_end,
                       char delimiter = 0);

/** Print's out the given word to the text buffer.
    @param  buffer     Beginning of the buffer.
    @param  buffer_end End of the buffer.
    @return            Null upon failure or a pointer to the byte after the last 
                       byte written. */
inline char* PrintHex (int8_t value, char* buffer, char* buffer_end,
                       char delimiter = 0) {
    return PrintHex ((uint8_t)value, buffer, buffer_end, delimiter);
}

/** Print's out the given word to the text buffer.
    @param  buffer     Beginning of the buffer.
    @param  buffer_end End of the buffer.
    @return            Null upon failure or a pointer to the byte after the last 
                       byte written. */
KABUKI char* PrintHex (uint16_t value, char* buffer, char* buffer_end,
                       char delimiter = 0);

/** Print's out the given word to the text buffer.
    @param  buffer     Beginning of the buffer.
    @param  buffer_end End of the buffer.
    @return            Null upon failure or a pointer to the byte after the last 
                       byte written. */
inline char* PrintHex (int16_t value, char* buffer, char* buffer_end,
                       char delimiter = 0) {
    return PrintHex ((uint16_t)value, buffer, buffer_end, delimiter);
}

/** Print's out the given word to the text buffer.
    @param  buffer     Beginning of the buffer.
    @param  buffer_end End of the buffer.
    @return            Null upon failure or a pointer to the byte after the last 
                       byte written. */
KABUKI char* PrintHex (uint32_t value, char* buffer, char* buffer_end,
                       char delimiter = 0);

/** Print's out the given word to the text buffer.
    @param  buffer     Beginning of the buffer.
    @param  buffer_end End of the buffer.
    @return            Null upon failure or a pointer to the byte after the last 
                       byte written. */
inline char* PrintHex (int32_t value, char* buffer, char* buffer_end,
                       char delimiter = 0) {
    return PrintHex ((uint32_t)value, buffer, buffer_end, delimiter);
}

/** Print's out the given word to the text buffer.
    @param  buffer     Beginning of the buffer.
    @param  buffer_end End of the buffer.
    @return            Null upon failure or a pointer to the byte after the last 
                       byte written. */
KABUKI char* PrintHex (uint64_t value, char* buffer, char* buffer_end,
                       char delimiter = 0);

/** Print's out the given word to the text buffer.
    @param  buffer     Beginning of the buffer.
    @param  buffer_end End of the buffer.
    @return            Null upon failure or a pointer to the byte after the last 
                       byte written. */
inline char* PrintHex (int64_t value, char* buffer, char* buffer_end,
                       char delimiter = 0) {
    return PrintHex ((uint64_t)value, buffer, buffer_end, delimiter);
}
    
/** Print's out the given word to the text buffer.
    @param  text     Beginning of the buffer.
    @param  text_end End of the buffer.
    @return          Null upon failure or a pointer to the byte after the last 
                     byte written. */
inline char* PrintHex (const void* ptr, char* text, char* text_end,
                       char delimiter = 0) {
    return PrintHex ((uintptr_t)ptr, text, text_end, delimiter);
}

}       //< namespace _

template<typename T>
inline _::Printer& operator<< (_::Printer& printer, _::Hex<T> value) {
    return value.Print (printer);
}

#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 2
#endif  //< HEADER_FOR_CRABS_HEX

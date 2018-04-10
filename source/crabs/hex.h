/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/crabs/hex.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
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

#pragma once
#include <stdafx.h>

#ifndef HEADER_FOR_CRABS_HEX
#define HEADER_FOR_CRABS_HEX

#include "print.h"
#include "slot.h"

#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 3

namespace _ {

/** Utility class for printing hex with operator<<. */
template<typename T>
class Hex {
    public:

    T value;    //< Value to convert to hex.

    /** Constructor saves value for use with operator overloads. */
    Hex (T value) :
        value (value) {
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
inline _::Printer& operator<< (_::Printer& print, _::Hex<T> hex) {
    print.cursor = _::PrintHex (hex, print.cursor, print.end, 0);
    return print;
}

template<typename T>
inline _::Slot& operator<< (_::Slot& slot, _::Hex<T> hex) {
    return _::PrintHex (hex.value, slot);
}

#endif  //< MAJOR_SEAM >= 1 && MINOR_SEAM >= 3
#endif  //< HEADER_FOR_CRABS_HEX

/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/hex.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-8 Cale McCollough <calemccollough@gmail.com>;
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

/** Converts a single byte a one-byte hex representation. */
KABUKI byte HexNibbleToLowerCase (byte b);

/** Converts a single byte a one-byte hex representation. */
KABUKI byte HexNibbleToUpperCase (byte b);

/** Converts a single byte a two-byte hex representation. */
KABUKI uint16_t HexByteToLowerCase (byte b);

/** Converts a single byte a two-byte hex representation. */
KABUKI uint16_t HexByteToUpperCase (byte b);

/** Converts a single hex byte a byte.
@return Returns -1 if c is not a hex byte.
*/
KABUKI int HexToByte (byte hex_byte);

/** Converts a single byte into a two-byte hex representation.
@return Returns -1 if c is not a hex byte.
*/
KABUKI int HexToByte (uint16_t hex);

/** Converts a single byte a one-byte hex representation. */
KABUKI byte HexNibbleToLowerCase (byte b);

/** Converts a single byte a one-byte hex representation. */
KABUKI byte HexNibbleToUpperCase (byte b);

/** Converts a single byte a two-byte hex representation. */
KABUKI uint16_t HexByteToLowerCase (byte b);

/** Converts a single byte a two-byte hex representation. */
KABUKI uint16_t HexByteToUpperCase (byte b);

/** Converts a single hex byte a byte.
    @return Returns -1 if c is not a hex byte.
*/
KABUKI int HexToByte (byte c);

/** Converts a single byte into a two-byte hex representation.
    @return Returns -1 if c is not a hex byte.
*/
KABUKI int HexToByte (uint16_t h);

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
        if (space <= kSize) return printer;
        *l_cursor++ = '0';
        *l_cursor++ = 'x';
        for (int num_bits_shift = sizeof (T) * 8 - 4; num_bits_shift >= 0;
             num_bits_shift -= 4) {
            *l_cursor++ = HexNibbleToUpperCase ((byte)(value >> num_bits_shift));
        }
        *l_cursor = 0;
        printer.cursor = l_cursor;
        return printer;
    }
};

/** Prints the given byte in Hex.
    This function prints the hex in big endian.
    @param  cursor The beginning of the buffer.
    @param  end    The end of the buffer.
    @return        Null upon failure or a pointer to the byte after the 
                   last byte written. */
KABUKI char* PrintHex (char* text, char* text_end, char c);

/** Print's out the given word to the buffer.
    @param  cursor The beginning of the buffer.
    @param  end    The end of the buffer.
    @return        Null upon failure or a pointer to the byte after the last 
                   byte written. */
KABUKI char* PrintHex (char* cursor, char* end, uint8_t value);

/** Print's out the given word to the buffer.
    @param  cursor The beginning of the buffer.
    @param  end    The end of the buffer.
    @return        Null upon failure or a pointer to the byte after the last 
                   byte written. */
inline char* PrintHex (char* cursor, char* end, int8_t value) {
    return PrintHex (cursor, end, (uint8_t)value);
}

/** Print's out the given word to the buffer.
    @param  cursor The beginning of the buffer.
    @param  end    The end of the buffer.
    @return        Null upon failure or a pointer to the byte after the last 
                   byte written. */
KABUKI char* PrintHex (char* cursor, char* end, uint16_t value);

/** Print's out the given word to the buffer.
    @param  cursor The beginning of the buffer.
    @param  end    The end of the buffer.
    @return        Null upon failure or a pointer to the byte after the last 
                   byte written. */
inline char* PrintHex (char* cursor, char* end, int16_t value) {
    return PrintHex (cursor, end, (uint16_t)value);
}

/** Print's out the given word to the buffer.
    @param  cursor The beginning of the buffer.
    @param  end    The end of the buffer.
    @return        Null upon failure or a pointer to the byte after the last 
                   byte written. */
KABUKI char* PrintHex (char* cursor, char* end, uint32_t value);

/** Print's out the given word to the buffer.
    @param  cursor The beginning of the buffer.
    @param  end    The end of the buffer.
    @return        Null upon failure or a pointer to the byte after the last 
                   byte written. */
inline char* PrintHex (char* cursor, char* end, int32_t value) {
    return PrintHex (cursor, end, (uint32_t)value);
}

/** Print's out the given word to the buffer.
    @param  cursor The beginning of the buffer.
    @param  end    The end of the buffer.
    @return        Nil upon failure or a pointer to the byte after the last 
                   byte written. */
KABUKI char* PrintHex (char* cursor, char* end, uint64_t value);

/** Print's out the given word to the buffer.
    @param  cursor The beginning of the buffer.
    @param  end    The end of the buffer.
    @return        Null upon failure or a pointer to the byte after the last 
                   byte written. */
inline char* PrintHex (char* cursor, char* end, int64_t value) {
    return PrintHex (cursor, end, (uint64_t)value);
}
    
/** Print's out the given word to the buffer.
    @param  text     Beginning of the buffer.
    @param  text_end End of the buffer.
    @return          Null upon failure or a pointer to the byte after the last 
                     byte written. */
inline char* PrintHex (char* text, char* text_end, const void* ptr) {
    return PrintHex (text, text_end, (uintptr_t)ptr);
}

}       //< namespace _

template<typename T>
inline _::Printer& operator<< (_::Printer& printer, _::Hex<T> value) {
    return value.Print (printer);
}

#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3
#endif  //< HEADER_FOR_CRABS_HEX

/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/text.h
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

#pragma once
#include <stdafx.h>

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 2

#include "text.h"
#include "script_itoa.h"

#if USING_PRINTER

#ifndef HEADER_FOR_CRABS_PRINT
#define HEADER_FOR_CRABS_PRINT

namespace _ {

/** String (STR) and Address (ADR) Text (TKN) writing utilities
    @ingroup Print

    Print specifically refers to writing a string to sockets.

    @see ~/kabuki-toolkit/source/crabs/text.h
*/

KABUKI char* TextStart ();

KABUKI char* TextEnd ();

/** Utility class for printing to strings.
    This class only stores the end of buffer pointer and a pointer to the write
    cursor. It is up the user to store start of buffer pointer and if they would
    like to replace the cursor with the beginning of buffer pointer when they
    are done printing.
*/
struct KABUKI Printer {

    char* cursor,  //< Write cursor pointer.
        * end;     //< End of buffer pointer.

    /** Initializes the Printer from the given buffer pointers.
        @param begin The beginning of the buffer.
        @param end   The end of the buffer. */
    Printer (char* begin = TextStart (), size_t size = kBufferSizeDefault);

    /** Initializes the Printer from the given buffer pointers.
        @param begin The beginning of the buffer.
        @param end   The end of the buffer. */
    Printer (char* begin, char* end);

    /** Clones the other print. */
    Printer (const Printer& other);

    Printer& Set (char* begin);
};

/** Copies a char from the source to the text.
    @param text      Beginning address of the buffer.
    @param text_end  The end address of the buffer.
    @param string    The potentially unsafe string to write. */
KABUKI char* Print (char* cursor, char* end, const char* string);

/** Copies a char from the source to the text.
    @param text     Beginning address of the buffer.
    @param text_end The end address of the buffer.
    @param value    The potentially unsafe string to write. */
KABUKI char* Print (char* cursor, char* end, const char* string, 
                    const char* string_end, char delimiter = 0);

/** Writes the give char to the given buffer.
    @param begin     Beginning address of the buffer.
    @param end       The end address of the buffer.
    @param character The value to write. */
KABUKI char* Print (char* cursor, char* end, char character,
                    char delimiter = 0);

/** Writes the give value to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
inline char* Print (char* cursor, char* end, int32_t value,
                    char delimiter = 0) {
    if (value >= 0) {
        return Print (cursor, end, (uint32_t)value, delimiter);
    }
    *cursor++ = '-';
    return Print (cursor, end, ~(uint32_t)value + 1, delimiter);
}

/** Writes the give value to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
inline char* Print (char* cursor, char* end, int64_t value, char delimiter = 0)
{
    if (value >= 0) {
        return Print (cursor, end, (uint64_t)value, delimiter);
    }
    *cursor++ = '-';
    return Print (cursor, end, ~(uint64_t)value + 1, delimiter);
}

/** Writes the give value to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* Print (char* cursor, char* end, float value,
                    char delimiter = 0);

/** Writes the give value to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* Print (char* target, char* target_end, double value,
                    char delimiter = 0);

/** Utility class for printing numbers. */
template<typename T>
struct Number {
    enum {
        kSize = sizeof (T) == 8 ? 24 :
        sizeof (T) == 4 ? 16 : 8    //< Size of the text buffer in bytes.
    };
    T value;            //< Value to print.
    char string[kSize]; //< String buffer.

    /** Constructor copies value and prints it to the string buffer. */
    Number (T value) :
        value (value) {
        Print (value, string, string + kSize - 1);
    }
};

/** Utility class for printing. */
struct Etx {

    char* begin;    //< Begin of the Printer buffer.

    Etx (char* begin = TextStart ());
};

}   //< namespace _

/** Writes a nil-terminated UTF-8 or ASCII string to the
    print. */
inline _::Printer& operator<< (_::Printer& print, const char* string) {
    print.cursor = _::Print (print.cursor, print.end, string);
    return print;
}

/** Writes the given value to the print.
    @param  printer The print.
    @param  value The value to write to the print. 
    @return The print. */
inline _::Printer& operator<< (_::Printer& print, char value) {
    print.cursor = _::Print (print.cursor, print.end, value, 0);
    return print;
}

/** Writes the given value to the print.
    @param  printer The print.
    @param  value The value to write to the print. 
    @return The print. */
inline _::Printer& operator<< (_::Printer& print, int32_t value) {
    print.cursor = _::Print (print.cursor, print.end, value, 0);
    return print;
}

/** Writes the given value to the print.
    @param  printer The print.
    @param  value The value to write to the print. 
    @return The print. */
inline _::Printer& operator<< (_::Printer& print, uint32_t value) {
    print.cursor = _::Print (print.cursor, print.end, value, 0);
    return print;
}

/** Writes the given value to the print.
    @param  printer The print.
    @param  value The value to write to the print. 
    @return The print. */
inline _::Printer& operator<< (_::Printer& print, int64_t value) {
    print.cursor = _::Print (print.cursor, print.end, value, 0);
    return print;
}

/** Writes the given value to the print.
    @param  printer The print.
    @param  value The value to write to the print. 
    @return The print. */
inline _::Printer& operator<< (_::Printer& print, uint64_t value) {
    print.cursor = _::Print (print.cursor, print.end, value, 0);
    return print;
}

/** Writes the given value to the print.
    @param  printer The print.
    @param  value The value to write to the print. 
    @return The print. */
inline _::Printer& operator<< (_::Printer& print, float value) {
    print.cursor = _::Print (print.cursor, print.end, value, 0);
    return print;
}

/** Writes the given value to the print.
    @param  printer The print.
    @param  value The value to write to the print. 
    @return The print. */
inline _::Printer& operator<< (_::Printer& print, double value) {
    print.cursor = _::Print (print.cursor, print.end, value, 0);
    return print;
}

/**  Prints out the parameters to the debug console. */
inline _::Printer& operator<< (_::Printer& print, _::Etx etx) {
    char* begin = _::TextStart ();
    print.cursor = begin;
    std::cerr << begin;
    return print;
}

#endif  //< HEADER_FOR_CRABS_PRINT
#endif  //< USING_PRINTER
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 2

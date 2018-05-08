/** Kabuki Toolkit
    @version 0.x
    @file    ~/libraries/crabs/text.h
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

/** Utility class for printing to strings.
    This class only stores the end of buffer pointer and a pointer to the write
    cursor. It is up the user to store start of buffer pointer and if they would
    like to replace the cursor with the beginning of buffer pointer when they
    are done printing.
*/
struct Printer {

    char* cursor,  //< Write cursor pointer.
        * end;     //< End of buffer pointer.

    /** Initializes the Printer from the given buffer pointers.
        @param begin The beginning of the buffer.
        @param end   The end of the buffer. */
    Printer (char* begin = BufferDefault (), size_t size = kBufferSizeDefault);

    /** Initializes the Printer from the given buffer pointers.
        @param begin The beginning of the buffer.
        @param end   The end of the buffer. */
    Printer (char* begin, char* end);

    Printer& Set (char* begin);
};

/** Copies a char from the source to the text.
    @param text      Beginning address of the buffer.
    @param text_end  The end address of the buffer.
    @param string    The potentially unsafe string to write. */
KABUKI char* Print (const char* string, char* text, char* text_end,
                    char delimiter = 0);

/** Copies a char from the source to the text.
    @param text     Beginning address of the buffer.
    @param text_end The end address of the buffer.
    @param value    The potentially unsafe string to write. */
KABUKI char* Print (const char* string, const char* string_end, char* text,
                    char* text_end, char delimiter = 0);

/** Writes the give value to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
inline char* Print (int32_t value, char* text, char* text_end,
                    char delimiter = 0) {
    if (value >= 0) {
        return Print ((uint32_t)value, text, text_end);
    }
    else {
        *text++ = '-';
        return Print (~(uint32_t)value + 1, text, text_end);
    }
}

/** Writes the give value to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
inline char* Print (int64_t value, char* text, char* text_end,
                    char delimiter = 0) {
    if (value >= 0) {
        return Print ((uint64_t)value, text, text_end);
    }
    else {
        *text++ = '-';
        return Print (~(uint64_t)value + 1, text, text_end);
    }
}

/** Writes the give value to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* Print (float value, char* text, char* text_end,
                    char delimiter = 0);

/** Writes the give value to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* Print (double value, char* target, char* target_end,
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
struct Dump {

    char* begin;    //< Begin of the Printer buffer.

    Dump (char* begin = BufferDefault ());
};

}   //< namespace _

/** Writes a nil-terminated UTF-8 or ASCII string to the
    printer. */
inline _::Printer& operator<< (_::Printer& printer, const char* string) {
    printer.cursor = _::Print (string, printer.cursor, printer.end);
    return printer;
}

/** Writes a nil-terminated UTF-8 or ASCII string to the
    printer. */
inline _::Printer& operator<< (_::Printer& printer, _::Printer& printer_b) {
    printer.cursor = 0;
    return printer;
}

/** Writes the given value to the printer.
    @param  printer The printer.
    @param  value The value to write to the printer. 
    @return The printer. */
inline _::Printer& operator<< (_::Printer& printer, int32_t value) {
    printer.cursor = _::Print (value, printer.cursor, printer.end);
    return printer;
}

/** Writes the given value to the printer.
    @param  printer The printer.
    @param  value The value to write to the printer. 
    @return The printer. */
inline _::Printer& operator<< (_::Printer& printer, uint32_t value) {
    printer.cursor = _::Print (value, printer.cursor, printer.end);
    return printer;
}

/** Writes the given value to the printer.
    @param  printer The printer.
    @param  value The value to write to the printer. 
    @return The printer. */
inline _::Printer& operator<< (_::Printer& printer, int64_t value) {
    printer.cursor = _::Print (value, printer.cursor, printer.end);
    return printer;
}

/** Writes the given value to the printer.
    @param  printer The printer.
    @param  value The value to write to the printer. 
    @return The printer. */
inline _::Printer& operator<< (_::Printer& printer, uint64_t value) {
    printer.cursor = _::Print (value, printer.cursor, printer.end);
    return printer;
}

/** Writes the given value to the printer.
    @param  printer The printer.
    @param  value The value to write to the printer. 
    @return The printer. */
inline _::Printer& operator<< (_::Printer& printer, float value) {
    printer.cursor = _::Print (value, printer.cursor, printer.end);
    return printer;
}

/** Writes the given value to the printer.
    @param  printer The printer.
    @param  value The value to write to the printer. 
    @return The printer. */
inline _::Printer& operator<< (_::Printer& printer, double value) {
    printer.cursor = _::Print (value, printer.cursor, printer.end);
    return printer;
}

/**  Prints out the parameters to the debug console. */
inline _::Printer& operator<< (_::Printer& printer, _::Dump dump) {
    char* begin = dump.begin;
    printer.cursor = begin;
    std::cerr << begin;
    return printer;
}

#endif  //< HEADER_FOR_CRABS_PRINT
#endif  //< USING_PRINTER
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 2

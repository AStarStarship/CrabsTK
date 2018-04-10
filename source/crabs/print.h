/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/crabs/text.h
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

#pragma once
#include <stdafx.h>

#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 2

#include "text.h"
#include "script_itos.h"

#if USING_TEXT_SCRIPT

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
    /** Initializes the Printer from the given buffer pointers.
        @param begin The beginning of the buffer.
        @param end   The end of the buffer. */
    Printer (char* begin, size_t size);

    /** Initializes the Printer from the given buffer pointers.
        @param begin The beginning of the buffer. 
        @param end   The end of the buffer. */
    Printer (char* begin, char* end);

    char* cursor,  //< Write cursor pointer.
        * end;     //< End of buffer pointer.
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
    } else {
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
    } else {
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

/** Prints the given char centered with a horizontal page bar to the left
    and right of the row.
    @param input The value to print.
    @param num_columns */
KABUKI char* PrintCentered (const char* string, int num_columns, char* text,
                            char* text_end, char delimiter = 0);

/** Prints the given value justified center to this string.
    @param value The value to print.
    @param num_columns The number of columns per row. */
inline char* PrintCentered (int32_t value, int num_columns, char* text, 
                            char* text_end, char delimiter = 0) {
    char buffer[8];
    Print ((int64_t)value, buffer, buffer + 8);
    return PrintCentered (buffer, num_columns, text, text_end);
}

/** Prints the given value justified center to this string.
    @param value The value to print.
    @param num_columns The number of columns per row. */
inline char* PrintCentered (uint32_t value, int num_columns, char* text, 
                            char* text_end, char delimiter = 0) {
    char buffer[16];
    Print ((uint32_t)value, buffer, buffer + 16);
    return PrintCentered (buffer, num_columns, text, text_end);
}

/** Prints the given value justified center to this string.
    @param value The value to print.
    @param num_columns The number of columns per row. */
inline char* PrintCentered (int64_t value, int num_columns, char* text, 
                            char* text_end, char delimiter = 0) {
    char buffer[16];
    Print (value, buffer, buffer + 16);
    return PrintCentered (buffer, num_columns, text, text_end);
}

/** Prints the given value justified center to this string.
    @param value The value to print.
    @param num_columns The number of columns per row. */
inline char* PrintCentered (uint64_t value, int num_columns, char* text, 
                            char* text_end, char delimiter = 0) {
    char buffer[24];
    Print (value, buffer, buffer + 24);
    return PrintCentered (buffer, num_columns, text, text_end);
}

/** Prints the given value justified center to this string.
    @param value The value to print.
    @param num_columns The number of columns per row. */
inline char* PrintCentered (float value, int num_columns, char* text, 
                            char* text_end, char delimiter = 0) {
    char buffer[kkFloat32DigitsMax];
    Print (value, buffer, buffer + kkFloat32DigitsMax);
    return PrintCentered (buffer, num_columns, text, text_end);
}

/** Prints the given value justified center to this string.
    @param value The value to print.
    @param num_columns The number of columns per row. */
inline char* PrintCentered (double value, int num_columns, char* text, 
                            char* text_end, char delimiter = 0) {
    char buffer[kFloat64DigitsMax];
    Print (value, buffer, buffer + kFloat64DigitsMax);
    return PrintCentered (buffer, num_columns, text, text_end);
}

KABUKI char* PrintBinary (uint64_t value, char* text, char* text_end, 
                          char delimiter = 0);

inline Printer PrinterInit (char* buffer, char* buffer_end) {
    return { buffer, buffer_end };
}

inline Printer PrinterInit (size_t buffer_size, char* buffer) {
    if (buffer == nullptr) {
        buffer = new char[buffer_size];
    }
    return { buffer, buffer + buffer_size - 1 };
}

}       //< namespace _

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

/**  Prints out the parameters to the debug console.
inline _::Printer& operator<< (_::Printer& printer, const uint_t* bsq) {
    printer.cursor = _::Print (bsq, printer.cursor, printer.end);
} */

#endif  //< HEADER_FOR_CRABS_PRINT
#endif  //< USING_TEXT_SCRIPT
#endif  //< #if MAJOR_SEAM >= 1 && MINOR_SEAM >= 2

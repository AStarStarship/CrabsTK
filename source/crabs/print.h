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

#ifndef HEADER_FOR_CRABS_PRINT
#define HEADER_FOR_CRABS_PRINT

#include "print_itos.h"

#if MAJOR_SEAM == 1 && MINOR_SEAM >= 3
#if USING_CRABS_TEXT

namespace _ {

/** String (STR) and Address (ADR) Text (TKN) writing utilities
    @ingroup Print

    Print specifically refers to writing a string to sockets.

    @see ~/kabuki-toolkit/source/crabs/text.h
*/

/** Copies a char from the source to the text.
    @param text      Beginning address of the buffer.
    @param text_end  The end address of the buffer.
    @param string    The potentially unsafe string to write. */
KABUKI char* Print (const char* string, char* text, char* text_end);

/** Copies a char from the source to the text.
    @param text     Beginning address of the buffer.
    @param text_end The end address of the buffer.
    @param value    The potentially unsafe string to write. */
KABUKI char* Print (const char* string, const char* string_end, char* text,
                    char* text_end);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* Print (int64_t value, char* text, char* text_end) {
    return Print ((uint64_t)value, text, text_end);
}

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* Print (float value, char* text, char* text_end);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* Print (double value, char* target, char* target_end);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
inline char* Print (int64_t value, char* text, char* text_end,
                    char delimiter) {
    if (value >= 0) {
        return Print ((uint64_t)value, text, text_end);
    }
    *text++ = '-';
    value = ~value + 1;
    return Print ((uint64_t)value, text, text_end);
}

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
inline char* Print (uint64_t value, char* text, char* text_end,
                    char delimiter) {
    text = Print (value, text, text_end);
    if (!text) {
        return nullptr;
    }
    *text = delimiter;
    return text;
}

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
inline char* Print (int32_t value, char* text, char* text_end,
                    char delimiter) {
    text = Print ((int64_t)value, text, text_end);
    if (!text) {
        return nullptr;
    }
    *text = delimiter;
    return text;
}

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
inline char* Print (uint32_t value, char* text, char* text_end,
                    char delimiter) {
    text = Print ((uint64_t)value, text, text_end);
    if (!text) {
        return nullptr;
    }
    *text = delimiter;
    return text;
}

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
inline char* Print (int16_t value, char* text, char* text_end,
                    char delimiter) {
    text = Print ((int64_t)value, text, text_end);
    if (!text) {
        return nullptr;
    }
    *text = delimiter;
    return text + 1;
}

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
inline char* Print (uint16_t value, char* text, char* text_end,
                    char delimiter) {
    text = Print ((uint64_t)value, text, text_end);
    if (!text) {
        return nullptr;
    }
    *text = delimiter;
    return text;
}

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
inline char* Print (int8_t value, char* text, char* text_end,
                    char delimiter) {
    text = Print ((int64_t)value, text, text_end);
    if (!text) {
        return nullptr;
    }
    *text = delimiter;
    return text;
}

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
inline char* Print (uint8_t value, char* text, char* text_end,
                    char delimiter) {
    text = Print ((uint64_t)value, text, text_end);
    if (!text) {
        return nullptr;
    }
    *text = delimiter;
    return text;
}

/** Prints the given string justified right to this string.
    @param input The string to print.
    @param num_columns The number of columns per row. */
KABUKI char* PrintRight (const char* string, int num_columns, char* text,
                         char* text_end);

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight (int8_t value, int num_columns, char* text,
                         char* text_end) {
    char buffer[8];
    Print ((uint64_t)value, buffer, buffer + 8);
    return PrintRight (buffer, num_columns, text, text_end);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight (uint8_t value, int num_columns, char* text,
                         char* text_end) {
    char buffer[8];
    Print ((uint64_t)value, buffer, buffer + 8);
    return PrintRight (buffer, num_columns, text, text_end);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight (int16_t value, int num_columns, char* text,
                         char* text_end) {
    char buffer[8];
    Print ((uint64_t)value, buffer, buffer + 8);
    return PrintRight (buffer, num_columns, text, text_end);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight (uint16_t value, int num_columns, char* text, 
                         char* text_end) {
    char buffer[8];
    Print ((uint64_t)value, buffer, buffer + 8);
    return PrintRight (buffer, num_columns, text, text_end);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight (int32_t value, int num_columns, char* text, 
                         char* text_end) {
    char buffer[16];
    Print ((uint64_t)value, buffer, buffer + 16);
    return PrintRight (buffer, num_columns, text, text_end);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight (uint32_t value, int num_columns, char* text, 
                         char* text_end) {
    char buffer[16];
    Print ((uint64_t)value, buffer, buffer + 16);
    return PrintRight (buffer, num_columns, text, text_end);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight (int64_t value, int num_columns, char* text, 
                         char* text_end) {
    char buffer[24];
    Print (value, buffer, buffer + 24);
    return PrintRight (buffer, num_columns, text, text_end);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight (uint64_t value, int num_columns, char* text, 
                         char* text_end) {
    char buffer[24];
    Print (value, buffer, buffer + 24);
    return PrintRight (buffer, num_columns, text, text_end);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight (float value, int num_columns, char* text, 
                         char* text_end) {
    char buffer[kkFloat32DigitsMax];
    Print (value, buffer, buffer + kkFloat32DigitsMax);
    return PrintRight (buffer, num_columns, text, text_end);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight (double value, int num_columns, char* text, 
                         char* text_end) {
    char buffer[kFloat64DigitsMax];
    Print (value, buffer, buffer + kFloat64DigitsMax);
    return PrintRight (buffer, num_columns, text, text_end);
}

/** Prints the given char centered with a horizontal page bar to the left
    and right of the row.
    @param input The char to print.
    @param num_columns */
KABUKI char* PrintCentered (const char* string, int num_columns, char* text,
                            char* text_end);

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintCentered (int8_t value, int num_columns, char* text,
                            char* text_end) {
    char buffer[8];
    Print ((int64_t)value, buffer, buffer + 8);
    return PrintCentered (buffer, num_columns, text, text_end);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintCentered (uint8_t value, int num_columns, char* text, char* text_end,
                         char delimiter) {
    char buffer[8];
    Print ((uint64_t)value, buffer, buffer + 8);
    return PrintCentered (buffer, num_columns, text, text_end);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintCentered (int16_t value, int num_columns, char* text,
                            char* text_end) {
    char buffer[8];
    Print ((int64_t)value, buffer, buffer + 8);
    return PrintCentered (buffer, num_columns, text, text_end);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintCentered (uint16_t value, int num_columns, char* text, 
                         char* text_end) {
    char buffer[8];
    Print ((uint64_t)value, buffer, buffer + 8);
    return PrintCentered (buffer, num_columns, text, text_end);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintCentered (int32_t value, int num_columns, char* text, 
                         char* text_end) {
    char buffer[8];
    Print ((int64_t)value, buffer, buffer + 8);
    return PrintCentered (buffer, num_columns, text, text_end);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintCentered (uint32_t value, int num_columns, char* text, 
                         char* text_end) {
    char buffer[16];
    Print ((uint64_t)value, buffer, buffer + 16);
    return PrintCentered (buffer, num_columns, text, text_end);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintCentered (int64_t value, int num_columns, char* text, 
                         char* text_end) {
    char buffer[16];
    Print (value, buffer, buffer + 16);
    return PrintCentered (buffer, num_columns, text, text_end);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintCentered (uint64_t value, int num_columns, char* text, 
                         char* text_end) {
    char buffer[24];
    Print (value, buffer, buffer + 24);
    return PrintCentered (buffer, num_columns, text, text_end);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintCentered (float value, int num_columns, char* text, 
                         char* text_end) {
    char buffer[kkFloat32DigitsMax];
    Print (value, buffer, buffer + kkFloat32DigitsMax);
    return PrintCentered (buffer, num_columns, text, text_end);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintCentered (double value, int num_columns, char* text, 
                         char* text_end) {
    char buffer[kFloat64DigitsMax];
    Print (value, buffer, buffer + kFloat64DigitsMax);
    return PrintCentered (buffer, num_columns, text, text_end);
}

/** Prints a string line of the char repeating. */
KABUKI char* PrintLine (char c, int num_columns, char* text,
                        char* text_end);

/** Prints a string line of the char repeating with an underscore. */
KABUKI char* PrintLine (const char* string, int num_columns, char* text,
                        char* text_end);

/** Prints the given byte in Hex.
    This function prints the hex in big endian.
    @param  text     Beginning of the buffer.
    @param  text_end End of the buffer.
    @return          Null upon failure or a pointer to the byte after the last 
                     byte written. */
KABUKI char* PrintHex (char c, char* text, char* text_end);

/** Print's out the given word to the text buffer.
    @param  text     Beginning of the buffer.
    @param  text_end End of the buffer.
    @return          Null upon failure or a pointer to the byte after the last 
                     byte written. */
KABUKI char* PrintHex (uintptr_t value, char* text, char* text_end);

KABUKI char* PrintBinary (uint64_t value, char* text, char* text_end);

struct Printer {
    char* cursor,  //< Write cursor pointer.
        * end;     //< End of buffer pointer.
};

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

/** Op << writes a nil-terminated UTF-8 or ASCII string to the
    printer. */
inline _::Printer& operator<< (_::Printer& printer, const char* string) {
    char* cursor = _::Print (string, printer.cursor, printer.end);
    printer.cursor = cursor;
    return printer;
}

/** Op << writes the given value to the printer.
    @param  printer The printer.
    @param  value The value to write to the printer. 
    @return The printer. */
inline _::Printer& operator<< (_::Printer& printer, int8_t value) {
    char* cursor = _::Print ((int64_t)value, printer.cursor,
                             printer.end);
    printer.cursor = cursor;
    return printer;
}

/** Op << writes the given value to the printer.
    @param  printer The printer.
    @param  value The value to write to the printer. 
    @return The printer. */
inline _::Printer& operator<< (_::Printer& printer, uint8_t value) {
    char* cursor = _::Print ((uint64_t)value, printer.cursor,
                                    printer.end);
    printer.cursor = cursor;
    return printer;
}

/** Op << writes the given value to the printer.
    @param  printer The printer.
    @param  value The value to write to the printer. 
    @return The printer. */
inline _::Printer& operator<< (_::Printer& printer, int16_t value) {
    char* cursor = _::Print ((int64_t)value, printer.cursor,
                             printer.end);
    printer.cursor = cursor;
    return printer;
}

/** Op << writes the given value to the printer.
    @param  printer The printer.
    @param  value The value to write to the printer. 
    @return The printer. */
inline _::Printer& operator<< (_::Printer& printer, uint16_t value) {
    char* cursor = _::Print ((uint64_t)value, printer.cursor, printer.end);
    printer.cursor = cursor;
    return printer;
}

/** Op << writes the given value to the printer.
    @param  printer The printer.
    @param  value The value to write to the printer. 
    @return The printer. */
inline _::Printer& operator<< (_::Printer& printer, int32_t value) {
    char* cursor = _::Print ((int64_t)value, printer.cursor,
                             printer.end);
    printer.cursor = cursor;
    return printer;
}

/** Op << writes the given value to the printer.
    @param  printer The printer.
    @param  value The value to write to the printer. 
    @return The printer. */
inline _::Printer& operator<< (_::Printer& printer, uint32_t value) {
    char* cursor = _::Print ((int64_t)value, printer.cursor,
                             printer.end);
    printer.cursor = cursor;
    return printer;
}

/** Op << writes the given value to the printer.
    @param  printer The printer.
    @param  value The value to write to the printer. 
    @return The printer. */
inline _::Printer& operator<< (_::Printer& printer, int64_t value) {
    char* cursor = _::Print (value, printer.cursor, printer.end);
    printer.cursor = cursor;
    return printer;
}

/** Op << writes the given value to the printer.
    @param  printer The printer.
    @param  value The value to write to the printer. 
    @return The printer. */
inline _::Printer& operator<< (_::Printer& printer, uint64_t value) {
    char* cursor = _::Print (value, printer.cursor, printer.end);
    printer.cursor = cursor;
    return printer;
}

/** Op << writes the given value to the printer.
    @param  printer The printer.
    @param  value The value to write to the printer. 
    @return The printer. */
inline _::Printer& operator<< (_::Printer& printer, float value) {
    char* cursor = _::Print (value, printer.cursor, printer.end);
    printer.cursor = cursor;
    return printer;
}

/** Op << writes the given value to the printer.
    @param  printer The printer.
    @param  value The value to write to the printer. 
    @return The printer. */
inline _::Printer& operator<< (_::Printer& printer, double value) {
    char* cursor = _::Print (value, printer.cursor, printer.end);
    printer.cursor = cursor;
    return printer;
}

/**  Prints out the parameters to the debug console.
inline _::Printer& operator<< (_::Printer& printer, const uint_t* bsq) {
    char* cursor = _::Print (bsq, printer.cursor, printer.end);
    printer.cursor = cursor;
} */

#endif  //< USING_CRABS_TEXT
#endif  //< MAJOR_SEAM == 1 && MINOR_SEAM >= 3
#endif  //< HEADER_FOR_CRABS_PRINT

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

#include "print.h"

#if USING_TEXT_SCRIPT

#ifndef HEADER_FOR_CRABS_RIGHT
#define HEADER_FOR_CRABS_RIGHT

namespace _ {

template<typename T = const char*>
struct Right {
    
    T           value;      //< 
    const char* string;     //< Pointer to string to print.
    bool        is_string;  //< Pointer for if it's a string or not.

    Right (T value) :
        value (value),
        is_string (false) {
        // Nothing to do here. :-)
    }

    Right (const char* string) :
        string (string),
        is_string (true) {
        // Nothing to do here. :-)
    }
};

/** Prints the given string justified right to this string.
    @param input The string to print.
    @param num_columns The number of columns per row. */
KABUKI char* PrintRight (const char* string, int num_columns, char* text,
                         char* text_end, char delimiter = 0);

/** Prints the given value justified right to this string.
    @param value The value to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight (int32_t value, int num_columns, char* text, 
                         char* text_end, char delimiter = 0) {
    char buffer[16];
    Print ((uint32_t)value, buffer, buffer + 16);
    return PrintRight (buffer, num_columns, text, text_end);
}

/** Prints the given value justified right to this string.
    @param value The value to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight (uint32_t value, int num_columns, char* text, 
                         char* text_end, char delimiter = 0) {
    char buffer[16];
    Print ((uint32_t)value, buffer, buffer + 16);
    return PrintRight (buffer, num_columns, text, text_end);
}

/** Prints the given value justified right to this string.
    @param value The value to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight (int64_t value, int num_columns, char* text, 
                         char* text_end, char delimiter = 0) {
    char buffer[24];
    Print (value, buffer, buffer + 24);
    return PrintRight (buffer, num_columns, text, text_end);
}

/** Prints the given value justified right to this string.
    @param value The value to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight (uint64_t value, int num_columns, char* text, 
                         char* text_end, char delimiter = 0) {
    char buffer[24];
    Print (value, buffer, buffer + 24);
    return PrintRight (buffer, num_columns, text, text_end);
}

/** Prints the given value justified right to this string.
    @param value The value to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight (float value, int num_columns, char* text, 
                         char* text_end, char delimiter = 0) {
    char buffer[kkFloat32DigitsMax];
    Print (value, buffer, buffer + kkFloat32DigitsMax);
    return PrintRight (buffer, num_columns, text, text_end);
}

/** Prints the given value justified right to this string.
    @param value The value to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight (double value, int num_columns, char* text, 
                         char* text_end, char delimiter = 0) {
    char buffer[kFloat64DigitsMax];
    Print (value, buffer, buffer + kFloat64DigitsMax);
    return PrintRight (buffer, num_columns, text, text_end);
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

#endif  //< HEADER_FOR_CRABS_RIGHT
#endif  //< USING_TEXT_SCRIPT
#endif  //< #if MAJOR_SEAM >= 1 && MINOR_SEAM >= 2

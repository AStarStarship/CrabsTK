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

#include "printer.h"

#if USING_PRINTER

#ifndef HEADER_FOR_CRABS_RIGHT
#define HEADER_FOR_CRABS_RIGHT

namespace _ {

template<typename T = const char*>
struct Right {
    
    T value; //< Value to print.

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
template<typename T>
inline _::Printer& operator<< (_::Printer& printer, _::Right<T> right) {
    printer.cursor = _::PrintRight (right.value, printer.cursor, printer.end);
    return printer;
}

#endif  //< HEADER_FOR_CRABS_RIGHT
#endif  //< USING_PRINTER
#endif  //< #if MAJOR_SEAM >= 1 && MINOR_SEAM >= 2

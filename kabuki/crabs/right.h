/* Kabuki Toolkit
    @version 0.x
    @file    ~/kabuki-toolkit/kabuki/crabs/text.h
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
#ifndef HEADER_FOR_CRABS_RIGHT
#define HEADER_FOR_CRABS_RIGHT
#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 2
// Dependencies:
#include "hex.h"
#include "printer.h"
// End dependencies.
#if USING_PRINTER

namespace _ {

/* Utility class for printing strings justified right. */
template <typename TextWord4 = const char*>
struct Right {
  TextWord4 value;   //< Value to print.
  int column_count;  //< Column count.

  Right(TextWord4 value, int column_count)
      : value(value), column_count(column_count) {
    // Nothing to do here. :-)
  }
};

/* Prints the given string justified right to this string.
    @param input The string to print.
    @param num_columns The number of columns per row. */
KABUKI char* PrintRight(char* text, char* text_end, const char* string,
                        int num_columns);

/* Prints the given value justified right to this string.
    @param value The value to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight(char* text, char* text_end, int32_t value,
                        int num_columns) {
  char buffer[16];
  Print(buffer, buffer + 15, (uint32_t)value);
  return PrintRight(text, text_end, buffer, num_columns);
}

/* Prints the given value justified right to this string.
    @param value The value to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight(char* text, char* text_end, uint32_t value,
                        int num_columns) {
  char buffer[16];
  Print(buffer, buffer + 15, (uint32_t)value);
  return PrintRight(text, text_end, buffer, num_columns);
}

/* Prints the given value justified right to this string.
    @param value The value to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight(char* text, char* text_end, int64_t value,
                        int num_columns) {
  char buffer[24];
  Print(buffer, buffer + 23, value);
  return PrintRight(text, text_end, buffer, num_columns);
}

/* Prints the given value justified right to this string.
    @param value The value to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight(char* text, char* text_end, uint64_t value,
                        int num_columns) {
  char buffer[24];
  Print(buffer, buffer + 23, value);
  return PrintRight(text, text_end, buffer, num_columns);
}

/* Prints the given value justified right to this string.
    @param value The value to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight(char* text, char* text_end, float value,
                        int num_columns) {
  char buffer[kkFloat32DigitsMax];
  Print(buffer, buffer + kkFloat32DigitsMax, value);
  return PrintRight(text, text_end, buffer, num_columns);
}

template <typename TextWord4>
char* PrintRight(char* begin, char* end, Hex<TextWord4> value,
                 int num_columns) {
  char* result = PrintHex(begin, end, value.value);
  if (!result) return printer;
  printer.cursor = result;
  return result;
}

/* Prints the given value justified right to this string.
    @param value The value to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight(char* text, char* text_end, double value,
                        int num_columns) {
  char buffer[kFloat64DigitsMax];
  Print(buffer, buffer + kFloat64DigitsMax, value);
  return PrintRight(text, text_end, buffer, num_columns);
}

}       //< namespace _

/* Writes a nil-terminated UTF-8 or ASCII string to the
    printer. */
template <typename TextWord4>
inline _::Printer& operator<<(_::Printer& printer, _::Right<TextWord4> right) {
  char* result = _::PrintRight(printer.cursor, printer.end, right.value,
                               right.column_count);
  if (!result) return printer;
  printer.cursor = result;
  return printer;
}

#endif  //< HEADER_FOR_CRABS_RIGHT
#endif  //< USING_PRINTER
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 2

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

#include "slot.h"

#if CRABS_SEAM >= 1
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
    @param string    The potentially unsafe string to write.
    @param delimiter A NON-ZERO delimiter to separate the string text into
                     texts. */
KABUKI char* Print (const char* string, char* text, char* text_end,
                    char delimiter = 0);

/** Copies a char from the source to the text.
    @param begin     Beginning address of the buffer.
    @param end       The end address of the buffer.
    @param string    The potentially unsafe string to write.
    @param delimiter A NON-ZERO delimiter to separate the string text into
                     texts. */
KABUKI Slot& Print (const char* string, Slot& slot, char delimiter = 0);

/** Copies a char from the source to the text.
    @param text     Beginning address of the buffer.
    @param text_end The end address of the buffer.
    @param value    The potentially unsafe string to write.
    @param delimiter A NON-ZERO delimiter to separate the string text into
                     texts. */
KABUKI char* Print (const char* string, const char* string_end, char* text,
                    char* text_end, char delimiter = 0);

/** Copies a char from the source to the text.
    @param begin     Beginning address of the buffer.
    @param end       The end address of the buffer.
    @param value     The potentially unsafe string to write.
    @param delimiter The last char to print . */
KABUKI Slot& Print (const char* text, const char* text_end, Slot& slot,
                    char delimiter = 0);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* Print (uint64_t value, char* text, char* text_end);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
inline char* Print (int64_t value, char* text, char* text_end) {
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
inline char* Print (int64_t value, char* text, char* text_end,
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
    @param value The value to write to the slot.
    @param slot  The slot tow rite to.
    @return The slot. */
KABUKI Slot& Print (int64_t value, Slot& slot, char delimiter = 0);

/** Writes the give value  to the given buffer as an ASCII string.
    @param value The value to write to the slot.
    @param slot  The slot tow rite to.
    @return The slot. */
KABUKI Slot& Print (uint64_t value, Slot& slot, char delimiter = 0);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
inline char* Print (int32_t value, char* text, char* text_end,
                    char delimiter = 0) {
    return Print ((int64_t)value, text, text_end, delimiter);
}

/** Writes the give value  to the given buffer as an ASCII string.
    @param value The value to write to the slot.
    @param slot  The slot tow rite to.
    @return The slot. */
inline Slot& Print (int32_t value, Slot& slot, char delimiter = 0) {
    return Print ((int64_t)value, slot, delimiter);
}

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
inline char* Print (uint32_t value, char* text, char* text_end,
                    char delimiter = 0) {
    return Print ((uint64_t)value, text, text_end, delimiter);
}

/** Writes the give value  to the given buffer as an ASCII string.
    @param value The value to write to the slot.
    @param slot  The slot tow rite to.
    @return The slot. */
inline Slot& Print (uint32_t value, Slot& slot, char delimiter = 0) {
    return Print ((uint64_t)value, slot, delimiter);
}

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
inline char* Print (int16_t value, char* text, char* text_end,
                    char delimiter = 0) {
    return Print ((int64_t)value, text, text_end, delimiter);
}

/** Writes the give value  to the given buffer as an ASCII string.
    @param value The value to write to the slot.
    @param slot  The slot tow rite to.
    @return The slot. */
inline Slot& Print (int16_t value, Slot& slot, char delimiter = 0) {
    return Print ((int64_t)value, slot, delimiter);
}

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
inline char* Print (uint16_t value, char* text, char* text_end,
                    char delimiter = 0) {
    return Print ((uint64_t)value, text, text_end, delimiter);
}

/** Writes the give value  to the given buffer as an ASCII string.
    @param value The value to write to the slot.
    @param slot  The slot tow rite to.
    @return The slot. */
inline Slot& Print (uint16_t value, Slot& slot, char delimiter = 0) {
    return Print ((uint64_t)value, slot, delimiter);
}

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
inline char* Print (int8_t value, char* text, char* text_end,
                    char delimiter = 0) {
    return Print ((int64_t)value, text, text_end, delimiter);
}

/** Writes the give value  to the given buffer as an ASCII string.
    @param value The value to write to the slot.
    @param slot  The slot tow rite to.
    @return The slot. */
inline Slot& Print (int8_t value, Slot& slot, char delimiter = 0) {
    return Print ((int64_t)value, slot, delimiter);
}

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
inline char* Print (uint8_t value, char* text, char* text_end,
                    char delimiter = 0) {
    return Print ((uint64_t)value, text, text_end, delimiter);
}

/** Writes the give value  to the given buffer as an ASCII string.
    @param value The value to write to the slot.
    @param slot  The slot tow rite to.
    @return The slot. */
inline Slot& Print (uint8_t value, Slot& slot, char delimiter = 0) {
    return Print ((uint64_t)value, slot, delimiter);
}

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* Print (float value, char* text, char* text_end,
                    char delimiter = 0);

/** Writes the give value  to the given buffer as an ASCII string.
    @param value The value to write to the slot.
    @param slot  The slot tow rite to.
    @return The slot. */
KABUKI Slot& Print (float value, Slot& slot, char delimiter = 0);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* Print (double value, char* text, char* text_end,
                    char delimiter = 0);

/** Writes the give value  to the given buffer as an ASCII string.
    @param value The value to write to the slot.
    @param slot  The slot tow rite to.
    @return The slot. */
KABUKI Slot& Print (double value, Slot& slot, char delimiter = 0);

/** Prints the given string justified right to this string.
    @param input The string to print.
    @param num_columns The number of columns per row. */
KABUKI char* PrintRight (const char* string, int num_columns, char* text,
                         char* text_end, char delimiter = 0);

/** Prints the given string justified right to this string.
    @param input The string to print.
    @param num_columns The number of columns per row. */
KABUKI Slot& PrintRight (const char* string, int num_columns, Slot& slot,
                         char delimiter = 0);

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight (int8_t value, int num_columns, char* text,
                         char* text_end, char delimiter = 0) {
    char buffer[8];
    Print (value, buffer, buffer + 8);
    return PrintRight (buffer, num_columns, text, text_end, delimiter);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintRight (int8_t value, int num_columns, Slot& slot,
                         char delimiter = 0) {
    char buffer[8];
    Print (value, buffer, buffer + 8);
    return PrintRight (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight (uint8_t value, int num_columns, char* text, char* text_end,
                         char delimiter = 0) {
    char buffer[8];
    Print (value, buffer, buffer + 8);
    return PrintRight (buffer, num_columns, text, text_end, delimiter);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintRight (uint8_t value, int num_columns, Slot& slot,
                         char delimiter = 0) {
    char buffer[8];
    Print (value, buffer, buffer + 8);
    return PrintRight (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight (int16_t value, int num_columns, char* text, char* text_end,
                         char delimiter = 0) {
    char buffer[8];
    Print (value, buffer, buffer + 8);
    return PrintRight (buffer, num_columns, text, text_end, delimiter);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintRight (int16_t value, int num_columns, Slot& slot,
                         char delimiter = 0) {
    char buffer[8];
    Print (value, buffer, buffer + 8);
    return PrintRight (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight (uint16_t value, int num_columns, char* text, 
                         char* text_end, char delimiter = 0) {
    char buffer[8];
    Print (value, buffer, buffer + 8);
    return PrintRight (buffer, num_columns, text, text_end, delimiter);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row.
    @return Returns the slot. */
inline Slot& PrintRight (uint16_t value, int num_columns, Slot& slot,
                  char delimiter = 0) {
    char buffer[8];
    Print (value, buffer, buffer + 8);
    return PrintRight (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight (int32_t value, int num_columns, char* text, 
                         char* text_end, char delimiter = 0) {
    char buffer[16];
    Print (value, buffer, buffer + 16);
    return PrintRight (buffer, num_columns, text, text_end, delimiter);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintRight (int32_t value, int num_columns, Slot& slot,
                  char delimiter = 0) {
    char buffer[16];
    Print (value, buffer, buffer + 16);
    return PrintRight (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight (uint32_t value, int num_columns, char* text, 
                         char* text_end, char delimiter = 0) {
    char buffer[16];
    Print (value, buffer, buffer + 16);
    return PrintRight (buffer, num_columns, text, text_end, delimiter);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintRight (uint32_t value, int num_columns, Slot& slot,
                  char delimiter = 0) {
    char buffer[16];
    Print (value, buffer, buffer + 16);
    return PrintRight (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight (int64_t value, int num_columns, char* text, 
                         char* text_end, char delimiter = 0) {
    char buffer[24];
    Print (value, buffer, buffer + 24);
    return PrintRight (buffer, num_columns, text, text_end, delimiter);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintRight (int64_t value, int num_columns, Slot& slot,
                  char delimiter = 0) {
    char buffer[24];
    Print (value, buffer, buffer + 24);
    return PrintRight (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight (uint64_t value, int num_columns, char* text, 
                         char* text_end, char delimiter = 0) {
    char buffer[24];
    Print (value, buffer, buffer + 24);
    return PrintRight (buffer, num_columns, text, text_end, delimiter);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintRight (uint64_t value, int num_columns, Slot& slot,
                  char delimiter = 0) {
    char buffer[24];
    Print (value, buffer, buffer + 24);
    return PrintRight (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight (float value, int num_columns, char* text, 
                         char* text_end, char delimiter = 0) {
    char buffer[kkFloat32DigitsMax];
    Print (value, buffer, buffer + kkFloat32DigitsMax);
    return PrintRight (buffer, num_columns, text, text_end, delimiter);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintRight (float value, int num_columns, Slot& slot,
                  char delimiter = 0) {
    char buffer[kkFloat32DigitsMax];
    Print (value, buffer, buffer + kkFloat32DigitsMax);
    return PrintRight (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight (double value, int num_columns, char* text, 
                         char* text_end, char delimiter = 0) {
    char buffer[kFloat64DigitsMax];
    Print (value, buffer, buffer + kFloat64DigitsMax);
    return PrintRight (buffer, num_columns, text, text_end, delimiter);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintRight (double value, int num_columns, Slot& slot,
                        char delimiter = 0) {
    char buffer[kFloat64DigitsMax];
    Print (value, buffer, buffer + kFloat64DigitsMax);
    return PrintRight (buffer, num_columns, slot, delimiter);
}

/** Prints the given char centered with a horizontal page bar to the left
    and right of the row.
    @param input The char to print.
    @param num_columns */
KABUKI char* PrintCentered (const char* string, int num_columns, char* text,
                            char* text_end, char delimiter = 0);

/** Prints the given char centered with a horizontal page bar to the left
    and right of the row.
    @param input The char to print.
    @param num_columns */
KABUKI Slot& PrintCentered (const char* text, int num_columns, Slot& slot,
                            char delimiter = 0);

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintCentered (int8_t value, int num_columns, char* text,
                            char* text_end, char delimiter = 0) {
    char buffer[8];
    Print (value, buffer, buffer + 8);
    return PrintCentered (buffer, num_columns, text, text_end, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintCentered (int8_t value, int num_columns, Slot& slot,
                            char delimiter = 0) {
    char buffer[8];
    Print (value, buffer, buffer + 8);
    return PrintCentered (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintCentered (uint8_t value, int num_columns, char* text, char* text_end,
                         char delimiter = 0) {
    char buffer[8];
    Print (value, buffer, buffer + 8);
    return PrintCentered (buffer, num_columns, text, text_end, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintCentered (uint8_t value, int num_columns, Slot& slot,
                            char delimiter = 0) {
    char buffer[8];
    Print (value, buffer, buffer + 8);
    return PrintCentered (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintCentered (int16_t value, int num_columns, char* text,
                            char* text_end, char delimiter = 0) {
    char buffer[8];
    Print (value, buffer, buffer + 8);
    return PrintCentered (buffer, num_columns, text, text_end, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintCentered (int16_t value, int num_columns, Slot& slot,
                         char delimiter = 0) {
    char buffer[8];
    Print (value, buffer, buffer + 8);
    return PrintCentered (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintCentered (uint16_t value, int num_columns, char* text, 
                         char* text_end, char delimiter = 0) {
    char buffer[8];
    Print (value, buffer, buffer + 8);
    return PrintCentered (buffer, num_columns, text, text_end, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row.
    @return Returns the slot. */
inline Slot& PrintCentered (uint16_t value, int num_columns, Slot& slot,
                  char delimiter = 0) {
    char buffer[8];
    Print (value, buffer, buffer + 8);
    return PrintCentered (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintCentered (int32_t value, int num_columns, char* text, 
                         char* text_end, char delimiter = 0) {
    char buffer[8];
    Print (value, buffer, buffer + 8);
    return PrintCentered (buffer, num_columns, text, text_end, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintCentered (int32_t value, int num_columns, Slot& slot,
                  char delimiter = 0) {
    char buffer[16];
    Print (value, buffer, buffer + 16);
    return PrintCentered (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintCentered (uint32_t value, int num_columns, char* text, 
                         char* text_end, char delimiter = 0) {
    char buffer[16];
    Print (value, buffer, buffer + 16);
    return PrintCentered (buffer, num_columns, text, text_end, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintCentered (uint32_t value, int num_columns, Slot& slot,
                  char delimiter = 0) {
    char buffer[16];
    Print (value, buffer, buffer + 16);
    return PrintCentered (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintCentered (int64_t value, int num_columns, char* text, 
                         char* text_end, char delimiter = 0) {
    char buffer[16];
    Print (value, buffer, buffer + 16);
    return PrintCentered (buffer, num_columns, text, text_end, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintCentered (int64_t value, int num_columns, Slot& slot,
                  char delimiter = 0) {
    char buffer[24];
    Print (value, buffer, buffer + 24);
    return PrintCentered (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintCentered (uint64_t value, int num_columns, char* text, 
                         char* text_end, char delimiter = 0) {
    char buffer[24];
    Print (value, buffer, buffer + 24);
    return PrintCentered (buffer, num_columns, text, text_end, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintCentered (uint64_t value, int num_columns, Slot& slot,
                  char delimiter = 0) {
    char buffer[24];
    Print (value, buffer, buffer + 24);
    return PrintCentered (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintCentered (float value, int num_columns, char* text, 
                         char* text_end, char delimiter = 0) {
    char buffer[kkFloat32DigitsMax];
    Print (value, buffer, buffer + kkFloat32DigitsMax);
    return PrintCentered (buffer, num_columns, text, text_end, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintCentered (float value, int num_columns, Slot& slot,
                  char delimiter = 0) {
    char buffer[kkFloat32DigitsMax];
    Print (value, buffer, buffer + kkFloat32DigitsMax);
    return PrintCentered (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintCentered (double value, int num_columns, char* text, 
                         char* text_end, char delimiter = 0) {
    char buffer[kFloat64DigitsMax];
    Print (value, buffer, buffer + kFloat64DigitsMax);
    return PrintCentered (buffer, num_columns, text, text_end, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintCentered (double value, int num_columns, Slot& slot,
                        char delimiter = 0) {
    char buffer[kFloat64DigitsMax];
    Print (value, buffer, buffer + kFloat64DigitsMax);
    return PrintCentered (buffer, num_columns, slot, delimiter);
}

/** The primary out slot.
    Upon system start Write will not be defined and will get defined using
KABUKI Slot& PrintLine ();*/

/** Prints a string line of the char repeating. */
KABUKI char* PrintLine (char c, int num_columns, char* text,
                        char* text_end, char delimiter = 0);

/** Prints a string line of the char repeating with an underscore. */
KABUKI Slot& PrintLine (char c, int num_columns, Slot& slot,
                        char delimiter = 0);

/** Prints a string line of the char repeating with an underscore. */
KABUKI char* PrintLine (const char* string, int num_columns, char* text,
                        char* text_end, char delimiter = 0);

/** Prints a line of the given string repeating for the given num_columns. */
KABUKI Slot& PrintLine (const char* string, int num_columns, Slot& slot,
                        char delimiter = 0);

/** Prints a line to the slot of the given string repeating for the given num_columns. */
KABUKI Slot& PrintLines (int numRows, Slot& slot, char delimiter = 0);

/** Prints the given byte in Hex.
    This function prints the hex in big endian.
    @param  text     Beginning of the buffer.
    @param  text_end End of the buffer.
    @return          Null upon failure or a pointer to the byte after the last 
                     byte written. */
KABUKI char* PrintHex (char c, char* text, char* text_end);

/** Prints the given byte in Hex.
    This function prints the hex in big endian.
    @param  delimiter 
    @return The slot. */
KABUKI Slot& PrintHex (char c, Slot& slot, char delimiter = 0);

/** Print's out the given word to the text buffer.
    @param  text     Beginning of the buffer.
    @param  text_end End of the buffer.
    @return          Null upon failure or a pointer to the byte after the last 
                     byte written. */
KABUKI char* PrintHex (uintptr_t value, char* text, char* text_end);

/** Print's out the given pointer to the text. */
KABUKI Slot& PrintHex (uintptr_t value, Slot& slot);

/** Print's out the given pointer to the text. */
inline Slot& PrintHex (const void* pointer, Slot& slot) {
    return PrintHex (reinterpret_cast<uintptr_t> (pointer), slot);
}

/** Prints out the contents of the address to the debug stream. */
KABUKI Slot& PrintMemory (const void* address, const void* end, Slot& slot,
                          char delimiter = 0);

/** Prints out the contents of the address to the debug stream. */
inline Slot& PrintMemory (const void* address, int size, Slot& slot) {
    return PrintMemory (address, reinterpret_cast<const char*> (address) + size,
                        slot);
}

/**  Prints out the parameters to the slot. */
KABUKI Slot& PrintBsq (const uint_t* bsq, Slot& slot);

KABUKI Slot& PrintOp (const Op* op, Slot& slot);

}       //< namespace _
        
/** Op << writes a nil-terminated UTF-8 or ASCII string to the slot. * /
inline _::Slot& operator<< (_::Slot& text_a, _::Slot& text_b) {
    return text_a.Write (text_b);
}

/** Op << writes a nil-terminated UTF-8 or ASCII string to the
    slot.
    @param  slot  The slot.
    @param  other The slot to write into the slot.
    @param  value The value to write to the slot. 
    @return The slot. */
inline _::Slot& operator<< (_::Slot& slot, _::Slot& other) {
    return SlotWrite (slot, other);
}

/** Op << writes a nil-terminated UTF-8 or ASCII string to the
    slot. */
inline _::Slot& operator<< (_::Slot& slot, const char* string) {
    return Print (string, slot);
}

/** Op << writes the given value to the slot.
    @param  slot The slot.
    @param  value The value to write to the slot. 
    @return The slot. */
inline _::Slot& operator<< (_::Slot& slot, int8_t value) {
    return Print (value, slot);
}

/** Op << writes the given value to the slot.
    @param  slot The slot.
    @param  value The value to write to the slot. 
    @return The slot. */
inline _::Slot& operator<< (_::Slot& slot, uint8_t value) {
    return Print (value, slot);
}

/** Op << writes the given value to the slot.
    @param  slot The slot.
    @param  value The value to write to the slot. 
    @return The slot. */
inline _::Slot& operator<< (_::Slot& slot, int16_t value) {
    return Print (value, slot);
}

/** Op << writes the given value to the slot.
    @param  slot The slot.
    @param  value The value to write to the slot. 
    @return The slot. */
inline _::Slot& operator<< (_::Slot& slot, uint16_t value) {
    return Print (value, slot);
}

/** Op << writes the given value to the slot.
    @param  slot The slot.
    @param  value The value to write to the slot. 
    @return The slot. */
inline _::Slot& operator<< (_::Slot& slot, int32_t value) {
    return Print (value, slot);
}

/** Op << writes the given value to the slot.
    @param  slot The slot.
    @param  value The value to write to the slot. 
    @return The slot. */
inline _::Slot& operator<< (_::Slot& slot, uint32_t value) {
    return Print (value, slot);
}

/** Op << writes the given value to the slot.
    @param  slot The slot.
    @param  value The value to write to the slot. 
    @return The slot. */
inline _::Slot& operator<< (_::Slot& slot, int64_t value) {
    return Print (value, slot);
}

/** Op << writes the given value to the slot.
    @param  slot The slot.
    @param  value The value to write to the slot. 
    @return The slot. */
inline _::Slot& operator<< (_::Slot& slot, uint64_t value) {
    return Print (value, slot);
}

/** Op << writes the given value to the slot.
    @param  slot The slot.
    @param  value The value to write to the slot. 
    @return The slot. */
inline _::Slot& operator<< (_::Slot& slot, float value) {
    return Print (value, slot);
}

/** Op << writes the given value to the slot.
    @param  slot The slot.
    @param  value The value to write to the slot. 
    @return The slot. */
inline _::Slot& operator<< (_::Slot& slot, double value) {
    return Print (value, slot);
}

/**  Prints out the parameters to the debug console. */
inline _::Slot& operator<< (_::Slot& slot, const uint_t* bsq) {
    return PrintBsq (bsq, slot);
}

#endif  //< USING_CRABS_TEXT
#endif  //< CRABS_SEAM >= 1
#endif  //< HEADER_FOR_CRABS_PRINT

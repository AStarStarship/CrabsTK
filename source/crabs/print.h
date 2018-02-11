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
KABUKI char* Print (int8_t value, char* text, char* text_end,
                    char delimiter = 0);

/** Writes the give value  to the given buffer as an ASCII string.
    @param value The value to write to the slot.
    @param slot  The slot tow rite to.
    @return The slot. */
KABUKI Slot& Print (int8_t value, Slot& slot, char delimiter = 0);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* Print (uint8_t value, char* text, char* text_end,
                    char delimiter = 0);

/** Writes the give value  to the given buffer as an ASCII string.
    @param value The value to write to the slot.
    @param slot  The slot tow rite to.
    @return The slot. */
KABUKI Slot& Print (uint8_t value, Slot& slot, char delimiter = 0);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* Print (int16_t value, char* text, char* text_end,
                    char delimiter = 0);

/** Writes the give value  to the given buffer as an ASCII string.
    @param value The value to write to the slot.
    @param slot  The slot tow rite to.
    @return The slot. */
KABUKI Slot& Print (int16_t value, Slot& slot, char delimiter = 0);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* Print (uint16_t value, char* text, char* text_end,
                    char delimiter = 0);

/** Writes the give value  to the given buffer as an ASCII string.
    @param value The value to write to the slot.
    @param slot  The slot tow rite to.
    @return The slot. */
KABUKI Slot& Print (uint16_t value, Slot& slot, char delimiter = 0);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* Print (int32_t value, char* text, char* text_end,
                    char delimiter = 0);

/** Writes the give value  to the given buffer as an ASCII string.
    @param value The value to write to the slot.
    @param slot  The slot tow rite to.
    @return The slot. */
KABUKI Slot& Print (int32_t value, Slot& slot, char delimiter = 0);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* Print (uint32_t value, char* text, char* text_end,
                    char delimiter = 0);

/** Writes the give value  to the given buffer as an ASCII string.
    @param value The value to write to the slot.
    @param slot  The slot tow rite to.
    @return The slot. */
KABUKI Slot& Print (uint32_t value, Slot& slot, char delimiter = 0);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* Print (int64_t value, char* text, char* text_end,
                    char delimiter = 0);

/** Writes the give value  to the given buffer as an ASCII string.
    @param value The value to write to the slot.
    @param slot  The slot tow rite to.
    @return The slot. */
KABUKI Slot& Print (int64_t value, Slot& slot, char delimiter = 0);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* Print (uint64_t value, char* text, char* text_end,
                    char delimiter = 0);

/** Writes the give value  to the given buffer as an ASCII string.
    @param value The value to write to the slot.
    @param slot  The slot tow rite to.
    @return The slot. */
KABUKI Slot& Print (uint64_t value, Slot& slot, char delimiter = 0);

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
inline char* PrintRight (int8_t value, int num_columns, char* text, char* text_end,
                         char delimiter = 0) {
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
Slot& PrintRight (uint16_t value, int num_columns, Slot& slot,
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
Slot& PrintRight (int32_t value, int num_columns, Slot& slot,
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
Slot& PrintRight (uint32_t value, int num_columns, Slot& slot,
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
Slot& PrintRight (int64_t value, int num_columns, Slot& slot,
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
Slot& PrintRight (uint64_t value, int num_columns, Slot& slot,
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
    char buffer[kStringFloatSizeMax];
    Print (value, buffer, buffer + kStringFloatSizeMax);
    return PrintRight (buffer, num_columns, text, text_end, delimiter);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
Slot& PrintRight (float value, int num_columns, Slot& slot,
                  char delimiter = 0) {
    char buffer[kStringFloatSizeMax];
    Print (value, buffer, buffer + kStringFloatSizeMax);
    return PrintRight (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintRight (double value, int num_columns, char* text, 
                         char* text_end, char delimiter = 0) {
    char buffer[kStringDoubleSizeMax];
    Print (value, buffer, buffer + kStringDoubleSizeMax);
    return PrintRight (buffer, num_columns, text, text_end, delimiter);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintRight (double value, int num_columns, Slot& slot,
                        char delimiter = 0) {
    char buffer[kStringDoubleSizeMax];
    Print (value, buffer, buffer + kStringDoubleSizeMax);
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
inline char* PrintCentered (int8_t value, int num_columns, char* text, char* text_end,
                         char delimiter = 0) {
    char buffer[8];
    sprintf_s (buffer, 8, "%i", value);
    return PrintCentered (buffer, num_columns, text, text_end, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintCentered (int8_t value, int num_columns, Slot& slot,
                         char delimiter = 0) {
    char buffer[8];
    sprintf_s (buffer, 8, "%i", value);
    return PrintCentered (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintCentered (uint8_t value, int num_columns, char* text, char* text_end,
                         char delimiter = 0) {
    char buffer[8];
    sprintf_s (buffer, 8, "%u", value);
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
inline char* PrintCentered (int16_t value, int num_columns, char* text, char* text_end,
                         char delimiter = 0) {
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
Slot& PrintCentered (uint16_t value, int num_columns, Slot& slot,
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
Slot& PrintCentered (int32_t value, int num_columns, Slot& slot,
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
Slot& PrintCentered (uint32_t value, int num_columns, Slot& slot,
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
Slot& PrintCentered (int64_t value, int num_columns, Slot& slot,
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
Slot& PrintCentered (uint64_t value, int num_columns, Slot& slot,
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
    char buffer[kStringFloatSizeMax];
    Print (value, buffer, buffer + kStringFloatSizeMax);
    return PrintCentered (buffer, num_columns, text, text_end, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintCentered (float value, int num_columns, Slot& slot,
                  char delimiter = 0) {
    char buffer[kStringFloatSizeMax];
    Print (value, buffer, buffer + kStringFloatSizeMax);
    return PrintCentered (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline char* PrintCentered (double value, int num_columns, char* text, 
                         char* text_end, char delimiter = 0) {
    char buffer[kStringDoubleSizeMax];
    Print (value, buffer, buffer + kStringDoubleSizeMax);
    return PrintCentered (buffer, num_columns, text, text_end, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintCentered (double value, int num_columns, Slot& slot,
                        char delimiter = 0) {
    char buffer[kStringDoubleSizeMax];
    Print (value, buffer, buffer + kStringDoubleSizeMax);
    return PrintCentered (buffer, num_columns, slot, delimiter);
}

/** The primary out slot.
    Upon system start Write will not be defined and will get defined using
KABUKI Slot& PrintLine ();*/

/** Prints a string line of the char repeating with an underscore
    i.e. s_s_... */
KABUKI Slot& PrintLine (const char* text, int num_columns, Slot& slot,
                        char delimiter = 0);

/** Prints a vertical tab with the given number of rows. */
KABUKI Slot& PrintLines (int numRows, Slot& slot, char delimiter = 0);

/** Prints the given byte in Hex.
    This function prints the hex in big endian.
    @param  delimiter 
    @return The slot. */
KABUKI Slot& PrintHex (byte c, Slot& slot, char delimiter = 0);

/** Print's out the given pointer to the text. */
KABUKI Slot& PrintHex (uintptr_t value, Slot& slot, char delimiter = 0);

/** Print's out the given pointer to the text. */
inline Slot& PrintHex (const void* pointer, Slot& slot, char delimiter = 0);

/** Prints out the contents of the address to the debug stream. */
KABUKI Slot& PrintMemory (const void* address, const void* end, Slot& slot,
                          char delimiter = 0);

/** Prints out the contents of the address to the debug stream. */
KABUKI Slot& PrintMemory (const void* address, int size, Slot& slot);

}   //< namespace _
#endif  //< USING_CRABS_TEXT
#endif  //< CRABS_SEAM >= 1
#endif  //< HEADER_FOR_CRABS_PRINT

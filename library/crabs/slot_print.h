/** Kabuki Toolkit
    @version 0.x
    @file    ~/libraries/crabs/slot.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 3

#ifndef HEADER_FOR_CRABS_SLOT_PRINT
#define HEADER_FOR_CRABS_SLOT_PRINT

#include "slot.h"

namespace _ {

/** Shifts the overflow buffer to the begging of the ring buffer. */
inline char* SlotOverflowShift (char* begin, char* end, char* cursor) {
    size_t size;
    if (cursor <= end) {
        return cursor;
    }
    size = end - begin;
    while (++end > cursor) {
        *(end - size) = *end;
    }
    return cursor - size;
}

/** Copies a char from the source to the text.
    @param begin     Beginning address of the buffer.
    @param end       The end address of the buffer.
    @param string    The potentially unsafe string to write.
    @param delimiter A NON-ZERO delimiter to separate the string text into
                     texts. */
KABUKI Slot& Out (const char* string, Slot& slot, char delimiter = 0);

/** Copies a char from the source to the text.
    @param begin     Beginning address of the buffer.
    @param end       The end address of the buffer.
    @param value     The potentially unsafe string to write.
    @param delimiter The last char to print . */
KABUKI Slot& Out (const char* text, const char* text_end, Slot& slot,
                    char delimiter = 0);

/** Writes the give value to the given buffer as an ASCII string.
    @param  value The value to write to the slot.
    @param  slot  The slot tow rite to.
    @return The slot. */
KABUKI Slot& Out (int32_t value, Slot& slot, char delimiter = 0);

/** Writes the give value to the given buffer as an ASCII string.
    @param  value The value to write to the slot.
    @param  slot  The slot to write to.
    @return The slot. */
KABUKI Slot& Out (uint32_t value, Slot& slot, char delimiter = 0);

/** Writes the give value to the given buffer as an ASCII string.
    @param  value The value to write to the slot.
    @param  slot  The slot tow rite to.
    @return The slot. */
KABUKI Slot& Out (int64_t value, Slot& slot, char delimiter = 0);

/** Writes the give value to the given buffer as an ASCII string.
    @param  value The value to write to the slot.
    @param  slot  The slot to write to.
    @return The slot. */
KABUKI Slot& Out (uint64_t value, Slot& slot, char delimiter = 0);

/** Writes the give value to the given buffer as an ASCII string.
    @param  value The value to write to the slot.
    @param  slot  The slot tow rite to.
    @return The slot. */
KABUKI Slot& Out (float value, Slot& slot, char delimiter = 0);

/** Writes the give value to the given buffer as an ASCII string.
    @param  value The value to write to the slot.
    @param  slot  The slot tow rite to.
    @return The slot. */
KABUKI Slot& Out (double value, Slot& slot, char delimiter = 0);

/** Prints the given string justified right to this string.
    @param input The string to print.
    @param num_columns The number of columns per row. */
KABUKI Slot& PrintRight (const char* string, int num_columns, Slot& slot,
                         char delimiter = 0);

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintRight (int32_t value, int num_columns, Slot& slot,
                         char delimiter = 0) {
    char buffer[16];
    Out (value, buffer, buffer + 16);
    return PrintRight (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintRight (uint32_t value, int num_columns, Slot& slot,
                         char delimiter = 0) {
    char buffer[16];
    Out (value, buffer, buffer + 16);
    return PrintRight (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintRight (int64_t value, int num_columns, Slot& slot,
                         char delimiter = 0) {
    char buffer[24];
    Out (value, buffer, buffer + 24);
    return PrintRight (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintRight (uint64_t value, int num_columns, Slot& slot,
                         char delimiter = 0) {
    char buffer[24];
    Out (value, buffer, buffer + 24);
    return PrintRight (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintRight (float value, int num_columns, Slot& slot,
                         char delimiter = 0) {
    char buffer[kkFloat32DigitsMax];
    Out (value, buffer, buffer + kkFloat32DigitsMax);
    return PrintRight (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified right to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintRight (double value, int num_columns, Slot& slot,
                         char delimiter = 0) {
    char buffer[kFloat64DigitsMax];
    Out (value, buffer, buffer + kFloat64DigitsMax);
    return PrintRight (buffer, num_columns, slot, delimiter);
}

/** Prints the given char centered with a horizontal page bar to the left
    and right of the row.
    @param input The char to print.
    @param num_columns */
KABUKI Slot& PrintCentered (const char* text, int num_columns, Slot& slot,
                            char delimiter = 0);

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintCentered (int32_t value, int num_columns, Slot& slot,
                            char delimiter = 0) {
    char buffer[16];
    Out (value, buffer, buffer + 16);
    return PrintCentered (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintCentered (uint32_t value, int num_columns, Slot& slot,
                            char delimiter = 0) {
    char buffer[16];
    Out (value, buffer, buffer + 16);
    return PrintCentered (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintCentered (int64_t value, int num_columns, Slot& slot,
                            char delimiter = 0) {
    char buffer[24];
    Out (value, buffer, buffer + 24);
    return PrintCentered (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintCentered (uint64_t value, int num_columns, Slot& slot,
                            char delimiter = 0) {
    char buffer[24];
    Out (value, buffer, buffer + 24);
    return PrintCentered (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintCentered (float value, int num_columns, Slot& slot,
                            char delimiter = 0) {
    char buffer[kkFloat32DigitsMax];
    Out (value, buffer, buffer + kkFloat32DigitsMax);
    return PrintCentered (buffer, num_columns, slot, delimiter);
}

/** Prints the given value justified center to this string.
    @param value The char to print.
    @param num_columns The number of columns per row. */
inline Slot& PrintCentered (double value, int num_columns, Slot& slot,
                            char delimiter = 0) {
    char buffer[kFloat64DigitsMax];
    Out (value, buffer, buffer + kFloat64DigitsMax);
    return PrintCentered (buffer, num_columns, slot, delimiter);
}

/** Prints a line of the given string repeating for the given num_columns. */
KABUKI Slot& PrintLine (const char* string, int num_columns, Slot& slot,
                        char delimiter = 0);

/** Prints a line to the slot of the given string repeating for the given num_columns. */
KABUKI Slot& PrintLines (int numRows, Slot& slot, char delimiter = 0);

/** The primary out slot.
    Upon system start Write will not be defined and will get defined using
KABUKI Slot& PrintLine ();*/

/** Prints a string line of the char repeating with an underscore. */
KABUKI Slot& PrintLine (char c, int num_columns, Slot& slot,
                        char delimiter = 0);

/** Prints the given byte in Hex.
    This function prints the hex in big endian.
    @param  delimiter 
    @return The slot. */
KABUKI Slot& PrintHex (char c, Slot& slot, char delimiter = 0);

/** Print's out the given pointer to the text. */
KABUKI Slot& PrintHex (uintptr_t value, Slot& slot);

/** Prints out the contents of the address to the debug stream. */
KABUKI Slot& PrintMemory (const void* address, const void* end, Slot& slot,
                          char delimiter = 0);

/** Prints out the contents of the address to the debug stream. */
inline Slot& PrintMemory (const void* address, intptr_t size, Slot& slot) {
    return PrintMemory (address, reinterpret_cast<const char*> (address) + size,
                        slot);
}

KABUKI Slot& Out (const Op* op, Slot& slot);

/** Prints the slot to the console. */
KABUKI void Out (Slot& slot);

/** Prints a string line of the char repeating. */
KABUKI Slot& PrintLine (char c, int num_columns, Slot& slot,
                        char delimiter = 0);

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
    //return SlotWrite (slot, other);
    return slot;
}

/** Op << writes a nil-terminated UTF-8 or ASCII string to the
    slot. */
inline _::Slot& operator<< (_::Slot& slot, const char* string) {
    return Out (string, slot);
}

/** Op << writes the given value to the slot.
    @param  slot The slot.
    @param  value The value to write to the slot. 
    @return The slot. */
inline _::Slot& operator<< (_::Slot& slot, int32_t value) {
    return Out (value, slot);
}

/** Op << writes the given value to the slot.
    @param  slot The slot.
    @param  value The value to write to the slot. 
    @return The slot. */
inline _::Slot& operator<< (_::Slot& slot, uint32_t value) {
    return Out (value, slot);
}

/** Op << writes the given value to the slot.
    @param  slot The slot.
    @param  value The value to write to the slot. 
    @return The slot. */
inline _::Slot& operator<< (_::Slot& slot, int64_t value) {
    return Out (value, slot);
}

/** Op << writes the given value to the slot.
    @param  slot The slot.
    @param  value The value to write to the slot. 
    @return The slot. */
inline _::Slot& operator<< (_::Slot& slot, uint64_t value) {
    return Out (value, slot);
}

/** Op << writes the given value to the slot.
    @param  slot The slot.
    @param  value The value to write to the slot. 
    @return The slot. */
inline _::Slot& operator<< (_::Slot& slot, float value) {
    return Out (value, slot);
}

/** Op << writes the given value to the slot.
    @param  slot The slot.
    @param  value The value to write to the slot. 
    @return The slot. */
inline _::Slot& operator<< (_::Slot& slot, double value) {
    return Out (value, slot);
}

/** Op << writes the given value to the slot.
    @param  slot The slot.
    @param  value The value to write to the slot.
    @return The slot. */
inline _::Slot& operator<< (_::Slot& slot, _::Socket memory) {
    return _::PrintMemory (memory.begin, memory.end, slot);
}

template<typename T>
inline _::Slot& operator<< (_::Slot& slot, _::Hex<T> hex) {
    return _::PrintHex (hex.value, slot);
}

inline _::Slot& operator<< (_::Slot& slot, _::Line line) {
    return _::PrintLine (line.token, line.column_count, slot);
}

#endif  //< HEADER_FOR_CRABS_SLOT_PRINT
#endif  //< #if MAJOR_SEAM >= 1 && MINOR_SEAM >= 3

/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/crabs/slot.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#ifndef HEADER_FOR_CRABS_SLOT
#define HEADER_FOR_CRABS_SLOT

#include "op.h"
#include "print.h"

#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 3

namespace _ {

/** A Slot in a Door in a Chinese Room to pass messages through.
    A Slot is Ring Buffer Socket similar to a TCP port. The operation of the 
    Slot is similar to the Text class except that it introduces two more 
    pointers for the (buffer) begin and (data) start of the ring buffer and
    you may write packed data.

*/
struct Slot {
    char* begin, //< Beginning of the ring buffer.
        * start, //< Start of the data in the ring buffer.
        * stop,  //< Stop of the data in the ring buffer.
        * end;   //< End of the ring buffer.
};

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

inline void SlotClear (Slot& slot) {
    slot.start = slot.end = slot.begin;
}

/** Initializes the ring buffer with the given buffer begin and size.
    @param slot  The slot to initialize.
    @param begin Pointer to the beginning of the ring buffer.
    @param size  The size of the ring buffer in bytes. */
KABUKI Slot& SlotInit (Slot& slot, uintptr_t* buffer, uintptr_t size);

/** Initializes the given buffer into . */
KABUKI Slot& SlotInit (uintptr_t* buffer, uintptr_t size);

/** Initializes the ring buffer with the given buffer begin and size.
    @param slot  The slot to initialize.
    @param begin Pointer to the beginning of the ring buffer.
    @param size  The size of the ring buffer in bytes.
KABUKI void SlotInit (Slot* slot, uintptr_t* buffer, uintptr_t size); */

/** Calculates the used ring buffer space.
    @param  Start The start of the data.
    @param  Stop  The stop of the data.
    @param  Size  The size of the buffer. */
inline intptr_t SlotLength (Slot& slot) {
    if (slot.start > slot.stop) {
        return (slot.end - slot.begin) - (slot.start - slot.stop);
    }
    return slot.stop - slot.start;
}

/** Calculates the used ring buffer space.
    @param  Start The start of the data.
    @param  Stop  The stop of the data.
    @param  Size  The size of the buffer. */
inline intptr_t SlotLength (char* start, char* stop, uintptr_t size) {
    if (start > stop) {
        return size - (start - stop);
    }
    return stop - start;
}

/** Calculates the space left in the given ring buffer.
    @param  Start The start of the data.
    @param  Stop  The stop of the data.
    @param  Size  The size of the buffer. */
inline intptr_t SlotSpace (char* start, char* stop, uintptr_t size) {
    if (start > stop) {
        return start - stop;
    }
    return size - (stop - start);
}

/** Reads the given Operation input parameters from the slot to the args.
    @param  slot The slot to read from.
    @param  op   The Operation to get the in from.
    @param  args The args array of pointers to write to.
    @return Nil upon success and an Error Operation upon failure. */
KABUKI const Op* SlotRead (Slot* slot, const uint_t* params, void** args);

/** Reads the given Operation input parameters from the slot to the args.
    @param  slot The slot to read from.
    @param  op   The Operation to get the in from.
    @param  args The args array of pointers to write to.
    @return Nil upon success and an Error Operation upon failure. */
KABUKI const Op* SlotRead (Slot* slot, const Op& op, void** args);

/** Writes the given Operation output parameters from the slot to the args.
    @param  slot The slot to read from.
    @param  op   The Operation to get the in from.
    @param  args The args array of pointers to write to.
    @return Nil upon success and an Error Operation upon failure. */
KABUKI const Op* SlotWrite (Slot* slot, const Op& op, void** args);

/** Copies the contents of the other slot into the slot. */
KABUKI Slot& SlotWrite (Slot& slot, Slot& other);

KABUKI void SlotDisplay (Slot& slot);

/** Copies a char from the source to the text.
    @param begin     Beginning address of the buffer.
    @param end       The end address of the buffer.
    @param string    The potentially unsafe string to write.
    @param delimiter A NON-ZERO delimiter to separate the string text into
                     texts. */
KABUKI Slot& Print (const char* string, Slot& slot, char delimiter = 0);

/** Copies a char from the source to the text.
    @param begin     Beginning address of the buffer.
    @param end       The end address of the buffer.
    @param value     The potentially unsafe string to write.
    @param delimiter The last char to print . */
KABUKI Slot& Print (const char* text, const char* text_end, Slot& slot,
                    char delimiter = 0);

/** Writes the give value to the given buffer as an ASCII string.
    @param  value The value to write to the slot.
    @param  slot  The slot tow rite to.
    @return The slot. */
KABUKI Slot& Print (int32_t value, Slot& slot, char delimiter = 0);

/** Writes the give value to the given buffer as an ASCII string.
    @param  value The value to write to the slot.
    @param  slot  The slot to write to.
    @return The slot. */
KABUKI Slot& Print (uint32_t value, Slot& slot, char delimiter = 0);

/** Writes the give value to the given buffer as an ASCII string.
    @param  value The value to write to the slot.
    @param  slot  The slot tow rite to.
    @return The slot. */
KABUKI Slot& Print (int64_t value, Slot& slot, char delimiter = 0);

/** Writes the give value to the given buffer as an ASCII string.
    @param  value The value to write to the slot.
    @param  slot  The slot to write to.
    @return The slot. */
KABUKI Slot& Print (uint64_t value, Slot& slot, char delimiter = 0);

/** Writes the give value to the given buffer as an ASCII string.
    @param  value The value to write to the slot.
    @param  slot  The slot tow rite to.
    @return The slot. */
KABUKI Slot& Print (float value, Slot& slot, char delimiter = 0);

/** Writes the give value to the given buffer as an ASCII string.
    @param  value The value to write to the slot.
    @param  slot  The slot tow rite to.
    @return The slot. */
KABUKI Slot& Print (double value, Slot& slot, char delimiter = 0);

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
    Print (value, buffer, buffer + 16);
    return PrintRight (buffer, num_columns, slot, delimiter);
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
inline Slot& PrintRight (int64_t value, int num_columns, Slot& slot,
                         char delimiter = 0) {
    char buffer[24];
    Print (value, buffer, buffer + 24);
    return PrintRight (buffer, num_columns, slot, delimiter);
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
inline Slot& PrintRight (float value, int num_columns, Slot& slot,
                         char delimiter = 0) {
    char buffer[kkFloat32DigitsMax];
    Print (value, buffer, buffer + kkFloat32DigitsMax);
    return PrintRight (buffer, num_columns, slot, delimiter);
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
KABUKI Slot& PrintCentered (const char* text, int num_columns, Slot& slot,
                            char delimiter = 0);

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
inline Slot& PrintCentered (uint32_t value, int num_columns, Slot& slot,
                            char delimiter = 0) {
    char buffer[16];
    Print (value, buffer, buffer + 16);
    return PrintCentered (buffer, num_columns, slot, delimiter);
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
inline Slot& PrintCentered (uint64_t value, int num_columns, Slot& slot,
                            char delimiter = 0) {
    char buffer[24];
    Print (value, buffer, buffer + 24);
    return PrintCentered (buffer, num_columns, slot, delimiter);
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
inline Slot& PrintCentered (double value, int num_columns, Slot& slot,
                            char delimiter = 0) {
    char buffer[kFloat64DigitsMax];
    Print (value, buffer, buffer + kFloat64DigitsMax);
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

/** Prints the slot to the console. */
KABUKI void Print (Slot& slot);

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

#endif  //< #if MAJOR_SEAM >= 1 && MINOR_SEAM >= 3
#endif  //< HEADER_FOR_CRABS_SLOT

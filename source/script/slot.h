/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/slot.h
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

#ifndef HEADER_FOR_SCRIPT_SLOT
#define HEADER_FOR_SCRIPT_SLOT

#include "op.h"

namespace _ {

/** A Slot in a Door in a Chinese Room to pass messages through.
    A Slot is similar to a TCP port. It's a ring buffer. */
struct Slot {
    char* begin, //< Beginning of the ring buffer.
        * start, //< Start of the data in the ring buffer.
        * stop,  //< Stop of the data in the ring buffer.
        * end;   //< End of the ring buffer.
};

/** Initializes the ring buffer with the given buffer begin and size.
    @param slot  The slot to initialize.
    @param begin Pointer to the beginning of the ring buffer.
    @param size  The size of the ring buffer in bytes. */
KABUKI void SlotInit (Slot* slot, char* begin, uintptr_t size);

/** Calculates the used ring buffer space.
    @param  Start The start of the data.
    @param  Stop  The stop of the data.
    @param  Size  The size of the buffer. */
inline uint_t SlotLength (char* start, char* stop, uint_t size) {
    if (start > stop) {
        return (uint_t)(size - (start - stop));
    }
    return (uint_t)(stop - start);
}

/** Calculates the space left in the given ring buffer.
    @param  Start The start of the data.
    @param  Stop  The stop of the data.
    @param  Size  The size of the buffer. */
inline uint_t SlotSpace (char* start, char* stop, uint_t size) {
    if (start > stop) {
        return (uint_t)(start - stop);
    }
    return (uint_t)(size - (stop - start));
}

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

/** Writes the given strand to the text buffer.
    @param begin Beginning address of the write buffer.
    @param end   The end address of the buffer.
    @param value The potentially unsafe string to write. */
KABUKI Slot& SlotWrite (Slot& slot, const char* strand);

/** Copies a char from the source to the text.
    @param begin     Beginning address of the buffer.
    @param end       The end address of the buffer.
    @param value     The potentially unsafe string to write.
    @param delimiter A NON-ZERO delimiter to separate the string text into
    strands. 
KABUKI Slot& SlotWrite (Slot& slot, const char* strand,
                        char delimiter);*/

/** Copies a char from the source to the text.
    @param begin     Beginning address of the buffer.
    @param end       The end address of the buffer.
    @param value     The potentially unsafe string to write.
    @param delimiter A NON-ZERO delimiter to separate the string text into
                     strands. 
KABUKI Slot& SlotWrite (Slot& slot, const char* strand,
                        const char* strand_end, char delimiter);*/

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI Slot& SlotWrite (Slot& slot, int8_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI Slot& SlotWrite (Slot& slot, uint8_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI Slot& SlotWrite (Slot& slot, int16_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI Slot& SlotWrite (Slot& slot, uint16_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI Slot& SlotWrite (Slot& slot, int32_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI Slot& SlotWrite (Slot& slot, uint32_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI Slot& SlotWrite (Slot& slot, int64_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI Slot& SlotWrite (Slot& slot, uint64_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI Slot& SlotWrite (Slot& slot, float value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI Slot& SlotWrite (Slot& slot, double value);

/** Gets or sets the Write.
    @slot Reference to a static Slot and static buffer. */
KABUKI Slot& SlotWriteSet (Slot& slot);

/** The primary out slot.
    To set the Write () @see SlotWriteSet 
*/
KABUKI Slot& Out ();

/** The primary out slot.
    Upon system start Write will not be defined and will get defined using
*/
KABUKI Slot& WriteLine ();

/** Dumps the Write Slot to the Console.
    @return Returns the Write Slot. */
KABUKI Slot& Dump ();

/** Prints the given token a specified num_times. */
Slot& Line (Slot& slot, char token = '-', const char* left_column_strand = "\n",
                int num_columns = 80);

/** Prints an 80-char line of the char repeating with an underscore
    i.e. s_s_... */
Slot& LineString (Slot& slot, const char* strand, int num_columns = 80);

/** Prints a bunch of new lines followed by the given message and a line of
the specified token. */
Slot& LineBreak (Slot& slot, const char* message, int top_bottom_margin,
                    char c = '-', int num_columns = 80);

/** Prints a vertical tab with the given number of rows. */
Slot& Lines (Slot& slot, int numRows = 10);

/** Prints the given byte in Hex.
    This function prints the hex in big endian. */
Slot& Hex (Slot& slot, byte c);

/** Print's out the given pointer to the strand. */
Slot& Hex (Slot& slot, const void* pointer);

}       //< namespace _
        
/** Op << writes a nil-terminated UTF-8 or ASCII string to the slot. * /
inline _::Slot& operator<< (_::Slot& strand_a, _::Slot& strand_b) {
    return strand_a.Write (strand_b);
}

/** Op << writes a nil-terminated UTF-8 or ASCII string to the
    slot. */
inline _::Slot& operator<< (_::Slot& slot, const char* string) {
    return SlotWrite (slot, string);
}

/** Op << writes the given value to the slot. */
inline _::Slot& operator<< (_::Slot& slot, int8_t value) {
    return SlotWrite (slot, value);
}

/** Op << writes the given value to the slot. */
inline _::Slot& operator<< (_::Slot& slot, uint8_t value) {
    return SlotWrite (slot, value);
}

/** Op << writes the given value to the slot. */
inline _::Slot& operator<< (_::Slot& slot, int16_t value) {
    return SlotWrite (slot, value);
}

/** Op << writes the given value to the slot. */
inline _::Slot& operator<< (_::Slot& slot, uint16_t value) {
    return SlotWrite (slot, value);
}

/** Op << writes the given value to the slot. */
inline _::Slot& operator<< (_::Slot& slot, int32_t value) {
    return SlotWrite (slot, value);
}

/** Op << writes the given value to the slot. */
inline _::Slot& operator<< (_::Slot& slot, uint32_t value) {
    return SlotWrite (slot, value);
}

/** Op << writes the given value to the slot. */
inline _::Slot& operator<< (_::Slot& slot, int64_t value) {
    return SlotWrite (slot, value);
}

/** Op << writes the given value to the slot. */
inline _::Slot& operator<< (_::Slot& slot, uint64_t value) {
    return SlotWrite (slot, value);
}

/** Op << writes the given value to the slot. */
inline _::Slot& operator<< (_::Slot& slot, float value) {
    return SlotWrite (slot, value);
}

/** Op << writes the given value to the slot. */
inline _::Slot& operator<< (_::Slot& slot, double value) {
    return SlotWrite (slot, value);
}

#endif  //< HEADER_FOR_SCRIPT_SLOT

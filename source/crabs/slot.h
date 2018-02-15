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

#if CRABS_SEAM >= 1

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
    if (cursor <= end) {
        return cursor;
    }
    size_t size = end - begin;
    while (++end > cursor) {
        *(end - size) = *end;
    }
    return cursor - size;
}

/** Initializes the ring buffer with the given buffer begin and size.
    @param slot  The slot to initialize.
    @param begin Pointer to the beginning of the ring buffer.
    @param size  The size of the ring buffer in bytes. */
KABUKI void SlotInit (Slot* slot, uintptr_t* buffer, uintptr_t size);

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

/** Write the contents of the other slot into the slot. */
KABUKI Slot& SlotWrite (Slot& slot, Slot& other);

KABUKI void SlotDisplay (Slot& slot);

}       //< namespace _

#endif  //< CRABS_SEAM >= 1
#endif  //< HEADER_FOR_CRABS_SLOT

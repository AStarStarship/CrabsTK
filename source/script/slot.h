/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/slot.h
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

#ifndef HEADER_FOR_SCRIPT_SLOT
#define HEADER_FOR_SCRIPT_SLOT

#include "op.h"

namespace _ {

/** A Slot (i.e. Port) that uses pointers.
*/
struct Slot {
    char* begin, //< Beginning of the ring buffer.
        * start, //< Start of the data in the ring buffer.
        * stop,  //< Stop of the data in the ring buffer.
        * end;   //< End of the ring buffer. 
};

/** Initializes the ring buffer with the given buffer begin and size.
    @param slot  The slot to initialize.
    @param begin Pointer to the beginning of the ring buffer.
    @param size  Size of the ring buffer in bytes. */
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

KABUKI const Op* SlotRead (Slot* slot, const Op& op, void** args);

KABUKI const Op* SlotWrite (Slot* slot, const Op& op, void** args);

/** Pushes the args onto the Expr's args stack.
    @param  args_cursor Write cursor in the args FIFO queue.
    @param  args_end    Last byte in the args FIFO queue.
    @param  value       The value to push.
    @return Modified args_cursor or nil upon failure. */
KABUKI const char* SlotStar (Slot* slot, const uint_t* params, 
                             const char* input, const char* input_end);
}       //< namespace _

#endif  //< HEADER_FOR_SCRIPT_SLOT

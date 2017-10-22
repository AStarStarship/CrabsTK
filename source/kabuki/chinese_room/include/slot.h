/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/include/slot.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
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

#ifndef CHINESE_ROOM_SLOT_H
#define CHINESE_ROOM_SLOT_H

#include "config.h"

namespace _ {

/** Calculates the used ring buffer space.
    @param  Start The start of the data.
    @param  Stop  The stop of the data.
    @param  Size The size of the buffer. */
KABUKI uint_t SlotLength (byte* start, byte* stop, uint_t size);

/** Calculates the space left in the given ring buffer.
    @param  Start The start of the data.
    @param  Stop  The stop of the data.
    @param  Size  The size of the buffer. */
KABUKI uint_t SlotSpace (byte* start, byte* stop, uint_t size);

/** Clears the ring buffer by writing zeros to it. */
KABUKI void SlotClear (byte* const begin, uint_t rx_start,
                       byte* start, byte* const stop,
                       byte* const end, uint_t size);

/** Copies a block from a ring-buffer to the given destination. */
KABUKI byte* SlotWrite (void* source, byte* const begin,
                        byte* const start, byte* const stop,
                        byte* const end, size_t size);

/** Copies a block from a ring-buffer to the given destination. */
KABUKI byte* SlotRead (void* destination, byte* const begin,
                       byte* const start, byte* const stop,
                       byte* const end, size_t size);

/** Copies a block from a ring-buffer to the given destination. */
KABUKI byte* SlotWrite (void* source, byte* const begin,
                        byte* const start, byte* const stop,
                        byte* const end, size_t size);

/** Copies a block from a ring-buffer to the given destination. */
KABUKI byte* SlotRead (void* destination, byte* const begin,
                       byte* const start, byte* const stop,
                       byte* const end, size_t size);

/** A Slot in a Socket in a Door. */
struct KABUKI Slot {
    uint_t     size,        //< The size of the buffer.
               start,       //< The starting index of the ring buffer data.
               stop,        //< The stopping index of the ring buffer data.
               read;        //< The read variable.
    byte       buffer;      //< The first byte in the ring buffer.
};

enum {
    kSlotHeaderSize = 4 * sizeof (uint_t)   //< Size of a Slot Header.
};

/** Returns true if the given slot contains the given address. */
KABUKI bool SlotContains (Slot* slot, void* address);

}       //< namespace _
#endif  //< CHINESE_ROOM_SLOT_H

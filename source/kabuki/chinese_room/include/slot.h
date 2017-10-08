/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/include/slot.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough] (calemccollough.github.io)

                    All right reserved (R).

             Licensed under the Apache License, Version 2.0 (the "License"); 
             you may not use this file except in compliance with the License. 
             You may obtain a copy of the License at

                    http://www.apache.org/licenses/LICENSE-2.0

             Unless required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#ifndef CHINESE_ROOM_SLOT_H
#define CHINESE_ROOM_SLOT_H

#include "terminal.h"

namespace _ {

/** A group of Terminal(s) that create a Slot in a door in a Chinese room.
*/
struct Slot {
    byte is_dynamic,        //< Flag for if using dynamic memory.
         num_slots,         //< The number of slots in this group.
         max_num_slots,     //< The number of slots allocated in memory.
         reserved;          //< Reserved for memory alignment.
    Terminal* slot_one;     //< The slots in door that messages pass through.
};

/** Initializes a Slot at the beginning of the given buffer. */
KABUKI Slot* SlotInit (uint_t width, uint_t size, byte max_num_slots);

/** Initializes a Slot at the beginning of the given buffer. */
KABUKI Slot* SlotInit (byte* buffer, uint_t slot_size, byte max_num_slots);

/** Gets a pointer to the array of pointers to Terminal(s). */
KABUKI Terminal** SlotTerminals (Slot* s);

/** Adds a Terminal to the slot.
    @return Returns nullptr if the Terminal is full and a pointer to the Slot in the 
            buffer upon success. */
KABUKI ticket_t SlotAddTerminal (Slot* s, Terminal* t);

/** Gets the Terminal from the Slot at the given index. */
KABUKI Terminal* SlotGetTerminal (Slot* s, char_t index);

/** Gets the Terminal from the Slot at the given index. */
KABUKI Terminal* SlotFindTerminal (Slot* s, void* address);

/** Deletes the Terminal from the Slot at the given index. */
KABUKI void SlotDelete (Slot* s, char_t index);

/** Prints the given Slot to the stdout. */
KABUKI void SlotPrint (Slot* s);

}       //< namespace _
#endif  //< CHINESE_ROOM_SLOT_H

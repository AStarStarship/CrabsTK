/** The Chinese Room
    @version 0.x
    @file    /.../slot.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough] (calemccollough.github.io)

                    All right reserved (R).

    Licensed under the Apache License, Version 2.0 (the "License"); you may
    not use this file except in compliance with the License. You may obtain
    a copy of the License at

            http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef CHINESEROOM_SLOT_H
#define CHINESEROOM_SLOT_H

#include "terminal.h"

namespace _ {

/** A group of Terminals that create a Slot in a door in a Chinese room.
*/
struct Slot {
    byte num_slots_,                //< The number of slots in this group.
        max_num_slots_;             //< The number of slots allocated in memory.
    uint_t size;                    //< The size of the entire slot.
    Terminal* slot_one_;            //< The slots in door that messages pass through.
};

Slot* InitSlot (char* buffer, uint_t slot_size) {
    if (buffer == nullptr) return nullptr;
    if (slot_size < kMinSlotSize) return nullptr;
    
}

}       //< namespace _
#endif  //< CHINESEROOM_SLOT_H

/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/impl/door.cc
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

#include <stdafx.h>
#include "../include/door.h"

namespace _ {

Door::Door (const char* roomName, uint_t slot_size, bool is_dynamic) :
    is_dynamic_ (is_dynamic)
{
    //tx.SetBuffer (adjacentDoor->Rx ()->EndAddress () + 1), aSlotSize);
    //rx = new SerialSlot (tx.EndAddress (), aSlot, aSlotSize, 
    //  aTalkbackSize);
}

Door::~Door () {
    if (is_dynamic_ == 0) return;
    delete[] reinterpret_cast<byte*> (this);
}

Door* Door::Init (uint_t buffer_size) {
    if (buffer_size < kMinDoorSize)
    return this;
}

Slot* Door::GetSlot (index index) {
    return nullptr;
}

ticket_t Door::AddSlot (Slot* slot) {
    return SlotAddSlot (slot_, slot);
}

Slot* Door::Contains (void* address) {
    return SlotFindSlot (slot_, address);
}

Slot* Door::ContainsSlot (void* address) {
    return SlotFindSlot (slot_, address);
}

ticket_t Door::ExecAll () {
    return 0;
}

const Operation* Door::Star (index index, Expression* expr) {
    if (index < ' ') {
        static const Operation this_member = { "Door", NumOperations (0), 
            0, "A door in a Chinese room.", 0 };
        return &this_member;
    }
    index -= ' ';
    if (index >= slot_->num_slots) return InvalidOperation ();
    return nullptr;
}

KABUKI Door& Doors () {
    static Door front_door;
    return front_door;
}

/** Initializes a Door at the beginning of the given buffer. 
static Door* DoorInit (byte* buffer, uint_t slot_size) {
    if (buffer == nullptr) return nullptr;
    if (slot_size < kMinSlotSize) return nullptr;
    Wall* wall = reinterpret_cast<Door*>(buffer);
    w->is_dynamic = 0;
    w->num_doors = 0;
    w->max_num_doors;
    w->door_one = nullptr;
}*/

}       //< namespace _

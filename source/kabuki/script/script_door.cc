/** kabuki::script
#include <script_door.h>
    @version 0.x
    @file    ~/source/kabuki/script/impl/script_door.cc
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

#include <stdafx.h>
#include "../script/door.h"

using namespace std;

namespace _ {

const Operation* DoorResult (Door* door, Door::Error error) {
    return 0;
}

const char* DoorErrorString (Door::Error error) {
    static const char* strings[] = {
        "Invalid operation",
        "Room error"
    };

    //if (error < 0)
    //    return strings[Door::RoomError];
    return strings[error];
}

Door::Door (const char* room_name, uintptr_t* buffer, uintptr_t size_bytes) {

	if (buffer == nullptr) {
        if (size_bytes < kMinDoorSize) {
        	size_bytes = kMinDoorSize;
        }
	} else {
        if (size_bytes < kMinDoorSize) {
        	// @todo insert error code here
        	cout << "\n| Error: Door size_bytes < kMinDoorSize!";
        	return;
        }
	}
    //tx.SetBuffer (adjacentDoor->Rx ()->EndAddress () + 1), aSlotSize);
    //rx = new SerialSlot (tx.EndAddress (), aSlot, aSlotSize, 
    //  aTalkbackSize);
}

Door::~Door () {
    if (buffer_)
    	delete[] buffer_;
}

slot_t Door::GetSlot (slot_t index) {
    return StackGet<slot_t, slot_t> (slots_, index);
}

slot_t Door::AddSlot (slot_t slot) {
    return StackPush<slot_t, slot_t> (slots_, slot);
}

bool Door::Contains (void* address) {
    return StackContains<slot_t, slot_t> (slots_, address);
}

slot_t Door::FindSlot (void* address) {
	slot_t count = slots_->count;
    for (slot_t i = 0; i < count; ++i) {
        //Slot* slot = nullptr; //< @todo fix me!
        
        if (StackContains<slot_t, slot_t> (slots_, address))
            return i;
    }
    return count;
}

const Operation* Door::ExecAll () {
    return 0;
}

const Operation* Door::Star (char_t index, Expression* expr) {
    if (index < ' ') {
        static const Operation star = { "Door", 
            NumOperations (0), FirstOperation ('A'),
            "A door in a Chinese room.", 0 };
        return &star;
    }
    index -= ' ';
    if (((slot_t)index) >= slots_->count)
        return DoorResult (this, Door::InvalidOperationError);
    return nullptr;
}

/*
KABUKI Door& Doors () {
    static Door front_door;
    return front_door;
}*/

/** Initializes a Door at the beginning of the given buffer. 
static Door* DoorInit (uintptr_t* buffer, slot_t slot_size) {
    if (buffer == nullptr) return nullptr;
    if (slot_size < kMinSlotSize) return nullptr;
    Wall* wall = reinterpret_cast<Door*>(buffer);
    w->is_dynamic = 0;
    w->num_doors = 0;
    w->max_num_doors;
    w->door_one = nullptr;
}*/

}       //< namespace _

/** Kabuki Toolkit
#include <script_door.h>
    @version 0.x
    @file    ~/library/crabs/crabs_door.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-8 Cale McCollough <calemccollough@gmail.com>;
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
#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 4
// Dependencies:
#include "door.h"
// End dependencies.
#if MAJOR_SEAM == 1 && MINOR_SEAM == 4
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PUTCHAR(c) putchar(c);
#else
#define PRINTF(x, ...)
#define PUTCHAR(c)
#endif

namespace _ {

const Op* DoorResult (Door* door, Door::Error error) {
    return 0;
}

const char* DoorErrorText (Door::Error error) {
    static const char* strings[] = {
        "Invalid op",
        "Room error"
    };

    //if (error < 0)
    //    return strings[Door::kErrorImplementation];
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
#if CRABS_DEBUG
            PRINTF ("\nError: Door size_bytes < kMinDoorSize!";
#endif  //< CRABS_DEBUG
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

const Op* Door::ExecAll () {
    return 0;
}

const Op* Door::Star (wchar_t index, Expr* expr) {
    static const Op kThis = { "Door",
        OpFirst ('A'), OpFirst ('A' + slots_->count),
        "A door in a Chinese room with a bunch of slots in it where "
        "messages are passed.",'}', ';', ' ', false, nullptr, nullptr };
    if (index == '?') {
        return ExprQuery (expr, kThis);
    }
    index -= ' ';
    if (((slot_t)index) >= slots_->count) {
        return DoorResult (this, Door::kErrorInvalidOp);
    }
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
#undef PRINTF
#undef PUTCHAR
#endif  //> #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 4

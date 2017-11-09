/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/include/door.h
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

#ifndef SCRIPT_DOOR_H
#define SCRIPT_DOOR_H

#include "slot.h"
#include "tstack.h"

namespace _ {

/** An door that connects two Chinese rooms.
    @see https://en.wikipedia.org/wiki/Chinese_room

    A door can lead to one or more rooms. Each room has a Room. A door can 
    be locked, in which case all of the Rooms behind this door are locked 
    out. When a door is locked, no messages are processed or sent to or 
    from the door and its subsidiary agents.

    Rooms can be connected by one or more Portal (string). An example of a system 
    with one Door and multiple Portals is a WiFi plus USB/Serial connection.
    This is a commonly used configuration for debugging the system.

    Multiple doors also might share the same Portal. An example of this is a 
    connection to multiple systems over a WiFi connection.

    @code

    |=============|
    |    slots    |
    |=============|
    | Door struct |
    |=============|

    @endcode
*/
class Door : public Operand {
    public:

    typedef enum Errors {
        InvalidOperationError = 0,
        RoomError
    } Error;

    enum {
        kMinDoorSize = 248,   //< The min and default size of the door buffer.
    };

    /** A door in a Chinese room. */
    Door (const char* roomName = " (:-)>", uint_t slot_size = kMinDoorSize,
          bool is_dynamic = false);

    /** Destructor. */
    ~Door ();

    Door* Init (uint_t buffer_size);

    /** Gets the expr at the given index. */
    Slot* GetSlot (int index);

    /** Address the given expr to the Door. */
    uint_t AddSlot (Slot* t);

    /** Attempts to find a Slot or Door with the given address. */
    int_t Contains (void* address);

    /** Gets the Slot that contains the given address.
        @return Returns the doors_ stack count if the Door does not
                contain the given address. */
    uint_t ContainsSlot (void* address);

    /** Executes all of the queued escape sequences. */
    const Operation* ExecAll ();

    /** Script expressions. */
    virtual const Operation* Star (char_t index, Expression* expr);

    private:
                            //< 2-to-4-int vtable pointer here in memory!
    bool is_dynamic_;       //< Flag for if using dynamic memory.
    TStack<uint_t>* slots_; //< The slots in the door.
};

/** Returns a Static Error Operation. */
KABUKI const Operation* DoorResult (Door* door, Door::Error error);

KABUKI const char* DoorErrorString (Door::Error error);

/** Returns the main door of the room.
KABUKI Door& Doors (); */

/** Initializes a Door at the beginning of the given buffer. 
static Door* DoorInit (int* buffer, uint_t slot_size) {
    if (buffer == nullptr) return nullptr;
    if (slot_size < kMinSlotSize) return nullptr;
    Wall* wall = reinterpret_cast<Door*>(buffer);
    w->is_dynamic = 0;
    w->num_doors = 0;
    w->max_num_doors;
    w->door_one = nullptr;
}*/

}       //< namespace _
#endif  //< SCRIPT_DOOR_H

/** The Chinese Room
    @version 0.x
    @file    ~/chinses_room/include/Door.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>
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

#ifndef CHINESE_ROOM_DOOR_H
#define CHINESE_ROOM_DOOR_H

#include "slot.h"

namespace _ {

/** An door that connects two Chinese rooms, i.e. threads or systems.
    @see https://en.wikipedia.org/wiki/Chinese_room

    A door can lead to one or more rooms. Each room has a Room. A door can
    be locked, in which case all of the Rooms behind this door are locked
    out. When a door is locked, no messages are processed or sent to or
    from the door and its subsidiary agents.

    Rooms can be connected by one or more Portal (s). An example of a system
    with one Door and multiple Portals is a WiFi plus USB/Serial connection.
    This is a commonly used configuration for debugging the system.

    Multiple doors also might share the same Portal. An example of this is a
    connection to multiple systems over a WiFi connection.
*/
class Door: public Device {
    public:

    enum {
        kMinDoorSize = 248,   //< The min and default size of the door buffer.
    };

    /** A door in a Chinese room. */
    Door (const char* roomName = " (:-)>", uint_t slot_size = kMinDoorSize,
          bool is_dynamic = false);

    /** Destructor. */
    ~Door ();

    Door* Init (uint_t buffer_size);

    /** Gets the terminal at the given index. */
    Terminal* GetTerminal (int index);

    /** Address the given terminal to the Door. */
    ticket_t AddTerminal (Terminal* t);

    /** Attempts to find a Terminal or Door with the given address. */
    Terminal* Contains (void* address);

    /** Gets the Terminal that contains the given address.
        @return Returns null if this Door doesn't contain the given address.
    */
    Terminal* ContainsTerminal (void* address);

    ticket_t ExecAll ();

    /** I2P operations. */
    const Member* Op (byte index, Verifier* io) override;

    private:

    // 2-to-4-byte vtable pointer here in memory!
    uint size_;             //< Size of the entire door and all of it's slots.
    byte is_dynamic_,       //< Flag for if the door is static(0) or dynamic(1).
        door_number_,       //< This door's number.
        max_num_doors_,     //< The max number of doors in the system.
        num_doors_,         //< The number of doors this door is connected too.
        current_slot_,      //< Index of current Slot being iterated through.
        current_door_;      //< Index of current Door being iterated through.
    Slot* slot_;            //< The slot in the door in the Chinese Room.
};

/** Returns the main door of the room. */
KABUKI Door& Doors ();

/** Initializes a Door at the beginning of the given buffer. 
static Door* DoorInit (byte* buffer, uint_t slot_size) {
    if (buffer == nullptr) return nullptr;
    if (slot_size < kMinSocketSize) return nullptr;
    Wall* wall = reinterpret_cast<Door*>(buffer);
    w->is_dynamic = 0;
    w->num_doors = 0;
    w->max_num_doors;
    w->door_one = nullptr;
}*/

}       //< namespace _
#endif  //< CHINESE_ROOM_DOOR_H

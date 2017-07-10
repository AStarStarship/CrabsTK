/** The Chinese Room
    @version 0.x
    @file    /.../Door.h
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

#ifndef CHINESEROOM_DOOR_H
#define CHINESEROOM_DOOR_H

#include "terminal.h"

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
          bool is_dynamic = false)
        : type_ (is_dynamic) {
        //tx.SetBuffer (adjacentDoor->Rx ()->EndAddress () + 1), aSlotSize);
        //rx = new SerialTerminal (tx.EndAddress (), aSlot, aSlotSize, 
        //  aTalkbackSize);
    }

    /** Destructor. */
    ~Door () {
        if (type_ == 0) return;
        delete[] reinterpret_cast<byte*> (this);
    }

    Door* Init (uint_t buffer_size) {
        if (buffer_size < kMinDoorSize)
        return this;
    }

    /** Gets the terminal at the given index. */
    Terminal* GetTerminal (int index) {
        return nullptr;
    }

    /** Address the given terminal to the Door. */
    ticket_t AddTerminal (Terminal& t) {
        if (num_slots_ > max_num_slots_)
            return 0;//ReportError (0);

        return 0;
    }

    /** Attempts to find a Terminal or Door with the given address. */
    Terminal* Contains (byte* address) {
        if (num_slots_ == 0) return nullptr;

        byte numberOfSlots = num_slots_;

        Terminal* current = GetTerminal (0),
            *end = GetTerminal (numberOfSlots - 1);

        do {
            if (current->Contains (address)) return current;
            current += sizeof (Terminal*);
        } while (current != end);
        return nullptr;
    }

    /** Gets the Terminal that contains the given address.
        @return Returns null if this Door doesn't contain the given address.
    */
    Terminal* GetTerminal (void* address) {
        Terminal** terminals = reinterpret_cast<Terminal**>(slot_one_);
        for (byte i = 0; i < num_doors_; ++i) {
            Terminal* t = terminals[i];
            if (t->Contains (address))
                return t;
        }
    }

    /** I2P operations. */
    const Member* Op (Rx* rx, Tx& tx, char index) override {
        if (index == '?') {
            static const Member m0 = { "Door", SetNumMembers (0), 
                                       0, "A door in a Chinese room." };
            return &m0;
        }
        index -= ' ';
        if (index >= num_slots_) return InvalidMember ();
        return nullptr;
    }

    private:

    // 2-to-4-byte vtable pointer here in memory!
    uint size_;                     //< Size of the entire door and all of it's slots.
    byte type_,                     //< The type of door: static(1) or dynamic(0).
        door_number_,               //< This door's number.
        max_num_doors_,             //< The max number of doors in the system.
        num_doors_,                 //< The number of doors this door is connected too.
        max_num_slots_,             //< The number of slots allocated in memory.
        num_slots_,                 //< The number of slots in the door.
        current_slot_,              //< Index of current Slot being iterated through.
        current_door_;              //< Index of current Door being iterated through.
    Terminal* slot_one_;            //< The slots in door that messages pass through.
};

/** Returns the main door of the room. */
inline Door& doors () {
    static Door front_door;
    return front_door;
}

}       //< namespace _
#endif  //< CHINESEROOM_DOOR_H

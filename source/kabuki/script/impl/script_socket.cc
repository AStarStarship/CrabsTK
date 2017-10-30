/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/impl/script_socket.cc
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
    @desc    Slot implementation split into three files, slot.h, 
             monoid_rx.h, and monoid_tx.h because of the large read/write
             functions.
*/

#include <stdafx.h>
#include "../include/socket.h"

namespace _ {

const Operation* SocketResult (Socket* socekt, Socket::Error error) {
    return 0;
}

Socket* SocketInit (byte* buffer, uint_t slot_size, byte max_num_slots) {
    if (buffer == nullptr) return nullptr;
    if (slot_size < kMinSlotSize) return nullptr;
    Socket* socket = reinterpret_cast<Socket*>(buffer);
    socket->num_slots = 0;
    socket->max_num_slots = max_num_slots;
    return socket;
}

Slot** SocketSlots (Socket* socket) {
    byte* address = reinterpret_cast<byte*> (socket) + sizeof (Socket);
    return reinterpret_cast<Slot**> (address);
}

const Operation* SocketAddSlot (Socket* socket, Slot* slot) {
    if (socket == nullptr)
        return SocketResult (socket, Socket::RoomError);
    if (slot == nullptr)
        return SocketResult (socket, Socket::RoomError);
    auto num_slots = socket->num_slots;
    if (num_slots + 1 >= socket->max_num_slots)
        return SocketResult (socket, Socket::RoomError);   //< @todo Add new error.
    SocketSlots (socket)[num_slots] = slot;
    ++socket->num_slots;
    return 0;
}

Slot* SocketGetSlot (Socket* socket, uint_t index) {
    if (socket == nullptr) return nullptr;
    if (index >= socket->num_slots)
        return nullptr;
    return SocketSlots (socket)[index];
}
Slot* SocketFindSlot (Socket* socket, void* address) {
    Slot** slots = SocketSlots (socket);
    if (slots == nullptr) return nullptr;
    for (uint_t i = 0; i < socket->num_slots; ++i) {
        Slot* slot = slots[i];
        if (SlotContains (slot, address))
            return slot;
    }
    return nullptr;
}

void SocketRemoveSlot (Socket* socket, uint_t index) {
    if (socket == nullptr) return;
    if (index >= socket->num_slots)
        return;
    Slot** slots = SocketSlots (socket);
    Slot* slot = slots[index];
    slot->~Slot ();
    for (uint_t i = index; i < socket->num_slots; ++i)
        slots[index] = slots[index + 1];
    --socket->num_slots;
}

void SocketPrint (Socket* socket) {
    if (socket == nullptr) return;
    printf ("\nSlot:\nis_dynamic %s socket\nnum_slots: %u\nmax_num_slots: %u\n",
        socket->is_dynamic? "true" : "false", socket->num_slots, socket->max_num_slots);
}

}       //< namespace _

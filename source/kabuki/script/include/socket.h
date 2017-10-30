/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/include/socket.h
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

#pragma once
#include <stdafx.h>

#ifndef KABUKI_SCRIPT_SOCKET_H
#define KABUKI_SCRIPT_SOCKET_H

#include "slot.h"
#include "bin.h"
#include "bout.h"

namespace _ {

/** A Socket is an Array of Bin-Bout Slots pairs.
    Each B-In/Bout Pair is located at indexes (2K) and (2K + 1). In the Chinese Room
    thought experiment there is only a single door with a single slot, so 
    all data is conceptually all serialized into a single Bin and Bout.
*/
struct KABUKI Socket {

    typedef enum Errors {
        RoomError = 0
    } Error;

    Array<uint_t> offsets;  //< An Array of offsets to 
};

/** Returns a Statically Evaluated Operation Error. */
KABUKI const Operation* SocketResult (Socket* socket, Socket::Error error);

/** Initializes a Socket at the beginning of the given buffer. */
KABUKI Socket* SocketInit (byte* buffer, uint_t slot_size, byte max_num_slots);

/** Gets a pointer to the array of pointers to Slot(string). */
KABUKI Slot** SocketSlots (Socket* string);

/** Adds a Slot to the slot.
    @return Returns nullptr if the Slot is full and a pointer to the Socket
            in the buffer upon success. */
KABUKI const Operation* SocketAddSlot (Socket* socket, Slot* slot);

/** Deletes the Slot from the Socket at the given index. */
KABUKI void SocketRemoveSlot (Socket* socket, uint_t index);

/** Gets the Slot from the Socket at the given index. */
KABUKI Slot* SocketGetSlot (Socket* socket, uint_t index);

/** Gets the Slot from the Socket at the given index. */
KABUKI Slot* SocketFindSlot (Socket* socket, void* address);

/** Prints the given Socket to the stdout. */
KABUKI void SocketPrint (Socket* string);

}       //< namespace _
#endif  //< KABUKI_SCRIPT_SOCKET_H

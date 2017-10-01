/** The Chinese Room
    @version 0.x
    @file    ~/chinses_room/include/unityper.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough<calemccollough.github.io>
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

#ifndef CHINESE_ROOM_UNITYPER_H
#define CHINESE_ROOM_UNITYPER_H

#include "error.h"
#include "types.h"
#include "address.h"
#include "array.h"
#include "utils.h"

namespace _ {

/** Clears the ring buffer by writing zeros to it. */
KABUKI void RingBufferClear (byte* const begin, uint_t rx_start, 
                             byte* start, byte* const stop,
                             byte* const end, uint_t size);

/** Calculates the used ring buffer space.
    @param  Start The start of the data.
    @param  Stop  The stop of the data.
    @param  Size The size of the buffer. */
KABUKI uint_t RingBufferLength (byte* start, byte* stop, uint_t size);

/** Calculates the space left in the given ring buffer.
    @param  Start The start of the data.
    @param  Stop  The stop of the data.
    @param  Size  The size of the buffer. */
KABUKI uint_t RingBufferSpace (byte* start, byte* stop, uint_t size);

/** Copies a block from a ring-buffer to the given destination. */
KABUKI byte* RingBufferWrite (void* source, byte* const begin,
                              byte* const start, byte* const stop,
                              byte* const end, size_t size);

/** Copies a block from a ring-buffer to the given destination. */
KABUKI byte* RingBufferRead (void* destination, byte* const begin,
                             byte* const start, byte* const stop,
                             byte* const end, size_t size);

/** Copies a block from a ring-buffer to the given destination. */
KABUKI byte* SocketWrite (void* source, byte* const begin,
                          byte* const start, byte* const stop,
                          byte* const end, size_t size);

/** Copies a block from a ring-buffer to the given destination. */
KABUKI byte* RingBufferRead (void* destination, byte* const begin,
                             byte* const start, byte* const stop,
                             byte* const end, size_t size);

/** A Tx ring-buffer.
    A Tx ring-buffer is identical in structure to an Rx ring-buffer, but the stop becomes
    volatile and start is not volatile. */
struct Unityper {
    uint_t size;            //< The size of the ring buffers.
    volatile uint_t start;  //< The starting index of the ring-buffer data.
    uint_t stop,            //< The stopping index of the ring-buffer data.
        read;               //< The address that the Rx device is reading from.
};

enum {
    kSlotHeaderSize = sizeof (Unityper) + sizeof (uintptr_t) -
    sizeof (Unityper) % sizeof (uintptr_t),
    //< Offset to the start of the ring buffer.
    kMinSocketSize = 32 + kSlotHeaderSize,
};

/** Gets the start of the Tx ring buffer. */
KABUKI byte* UnityperSlot (Unityper* tx);

KABUKI Unityper* UnityperInit (byte* buffer, uint_t size);

/** Initializes the tx buffer with the given buffer size. */
KABUKI Unityper* UnityperInit (Unityper* buffer, uint_t size);

/** Calculates the space left in the given ring buffer.
    @param  tx The Tx buffer.
*/
KABUKI uint_t UnityperSpace (Unityper* tx);

/** Gets the tx buffer space. */
KABUKI uint_t UnityperTxBufferLength (Unityper* tx);

/** Gets the end address of the tx buffer. */
KABUKI byte* UnityperEndAddress (Unityper* tx);

/** Prints a message with the given params to the given Tx slot.
    @param tx The Tx socket to write to.
    @param params The escape sequence to write.
    @param args The array of pointers to the stuff to write. */
static ticket_t Write (Unityper* tx, const char* address, const uint_t* params, 
                       void** args);

/** Streams a tx byte.
    @param tx The tx Unityper*/
KABUKI byte UnityperStreamByte (Unityper* tx);

/** Scans a message with the given params to the given Terminal.
    @param rx The Unityper socket.
    @param params The parameters.
    @param args The arguments.
    @return Returns 0 upon success and an ErrorList ticket number upon failure. 
*/
KABUKI ticket_t Read (Unityper* rx, const uint_t* params, void** args);

/** Prints the given Tx to the stdout. */
KABUKI void Print (Unityper* tx);

}       //< namespace _
#endif  //< CHINESE_ROOM_UNITYPER_H

/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/include/monoid.h
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

#ifndef CHINESE_ROOM_MONOID_H
#define CHINESE_ROOM_MONOID_H

#include "set.h"

namespace _ {

/** Returns true if the given monoid contains the given address. */
//void* SlotContains ();

/*< A*B B-Input arguments.
    
*/
struct Bin {
    uint_t size,            //< The size of the buffer.
        start;              //< The starting index of the ring buffer data.
    volatile uint_t stop;   //< The stopping index of the ring buffer data.
    uint_t read;            //< The read variable.
};

/** A*B B-Output socket.
    A sequence of A*Bin expressions terminated by an ASCII CR char. All sequences get 
    evaluated down to an immutable set (by definition all sets are immutable), which 
    can be an empty set.

    # Slot Format
    By default, all sequences are a set of one byte, all sets are linear, and all sets 
    may repeat some number of times.

    @code
    ESC VU8 1 BS CR
    @endcode
    A Tx ring-buffer is identical in structure to an Rx ring-buffer, but the stop becomes
    volatile and start is not volatile. */
struct Bout {
    uint_t size;            //< The size of the monoid ring buffers.
    volatile uint_t start;  //< The starting index of the ring-buffer data.
    uint_t stop,            //< The stopping index of the ring-buffer data.
        read;               //< The address that the Rx device is reading from.
};

/** Initializes the Bin struct to an empty buffer. */
KABUKI Bin* BinInit (byte* buffer, uint_t size);

/** Initializes the Bin struct to an empty buffer. */
KABUKI Bin* BinInit (Bin* rx, uint_t size);

/** Gets the start of the Bin ring buffer. */
KABUKI byte* BinBaseAddress (Bin* ptr);

/** Gets the rx buffer length. */
KABUKI uint_t BinSpace (Bin* rx);

/** Gets the start of the Bin ring buffer. */
KABUKI byte* BinBaseAddress (void* ptr, uint_t rx_tx_offset);

/** Gets the end address of the rx buffer. */
KABUKI byte* BinEndAddress (Bin* rx);

/** Scans a message with the given params to the given Bin.
    The data in the Bin is word-aligned, unlike the Slot. It also
    doesn't have a hash with an escape sequence.
    
    @param rx The Bin socket.
    @param params The parameters.
    @param args The arguments.
    @return Returns 0 upon success and an ErrorList ticket number upon failure. */
KABUKI const Operation* BinRead (Bin* rx, const uint_t* params, void** args);

/** Scans a message with the given params to the given Slot.
    @param rx The monoid socket.
    @param params The parameters.
    @param args The arguments.
    @return Returns 0 upon success and an ErrorList ticket number upon failure. */
KABUKI const Operation* BoutRead (Bout* rx, const uint_t* params, void** args);

/** Returns true if the Bin buffer contains any data.
    @warning Function does not do any error checking for speed. */
KABUKI bool BinIsReadable (Bin* rx);

/** Prints out the given object to the std::out. */
KABUKI void BinPrint (Bin* rx);

enum {
    kSlotHeaderSize = sizeof (Bout) + sizeof (uintptr_t) -
    sizeof (Bout) % sizeof (uintptr_t),
    //< Offset to the start of the ring buffer.
    kMinSlotSize = 32 + kSlotHeaderSize,
};

/** Gets the start of the Tx ring buffer. */
KABUKI byte* BoutSlot (Bout* tx);

KABUKI Bout* BoutInit (byte* buffer, uint_t size);

/** Initializes the tx buffer with the given buffer size. */
KABUKI Bout* BoutInit (Bout* buffer, uint_t size);

/** Calculates the space left in the given ring buffer.
    @param  tx The Tx buffer. */
KABUKI uint_t BoutSpace (Bout* tx);

/** Gets the tx buffer space. */
KABUKI uint_t BoutBufferLength (Bout* tx);

/** Gets the end address of the tx buffer. */
KABUKI byte* BoutEndAddress (Bout* tx);

/** Prints a message with the given params to the given Tx slot.
    @param tx The Tx socket to write to.
    @param params The escape sequence to write.
    @param args The array of pointers to the stuff to write. */
KABUKI ticket_t Write (Bout* tx, const char* address, const uint_t* params,
                       void** args);

/** Streams a tx byte.
    @param tx The tx monoid. */
KABUKI byte BoutStreamByte (Bout* tx);

/** Prints the given Tx to the stdout. */
KABUKI void BoutPrint (Bout* tx);

}       //< namespace _
#endif  //< CHINESE_ROOM_MONOID_H

/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/bout.h
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

#ifndef HEADER_FOR_SCRIPT_BOUT
#define HEADER_FOR_SCRIPT_BOUT

#include "op.h"
#include "error.h"

namespace _ {

/** A*B B-Output socket with a packed byte stream.
    A sequence of A*BOut expressions terminated by an ASCII CR char. All 
    sequences get evaluated down to an immutable set (by definition all sets are
    immutable), which can be an empty set.

    # Slot Format
    By default, all sequences are a set of one byte, all sets are linear, and 
    all sets may repeat some number of times.

    @code
    ESC VU8 1 BS CR
    @endcode
    A B-Output ring-buffer is identical in structure to an BOut ring-buffer, but
    the stop becomes volatile and start is not volatile. */
struct KABUKI BOut {

    /** List of the Finite BOut States. */
    typedef enum States {
        kStateDisconnected = 0, //< BOut State 0: BOut locked.
        kStateWriting ,         //< BOut State 1: Most common state.
        kStateWritingBsc,       //< BOut State 2: Writing BSQ.
    } State;

    uint_t          size;  //< Size of the B-Output.
    volatile uint_t start; //< Starting index of the ring-buffer data.
    uint_t          stop,  //< Stopping index of the ring-buffer data.
                    read;  //< Address that the BOut device is reading from.
};

/** Get's the B-Output's buffer.*/
KABUKI char* BOutBuffer (BOut* bout);

#if USING_SCRIPT_TEXT

/** Gets a a char for printing out the bout_state. */
KABUKI const char** BOutState ();

#endif  //< USING_SCRIPT_TEXT

/** Initializes the B-Output buffer with the given buffer size. */
KABUKI BOut* BOutInit (uintptr_t* buffer, uint_t size);

/** Calculates the space left in the given ring buffer.
    @param  bout The B-Output buffer. */
KABUKI uint_t BOutSpace (BOut* bout);

/** Gets the B-Output. */
KABUKI uint_t BOutBufferLength (BOut* bout);

/** Gets the end address of the tx buffer. */
KABUKI char* BOutEndAddress (BOut* bout);

/** Streams a B-Output byte.
    @param bout A B-Output abstract byte stream. */
KABUKI int BOutStreamByte (BOut* bout);

/** Writes a message with the given params to the given B-Output slot.
    @param bout   The B-Output socket to write to.
    @param params The escape sequence to write.
    @param args   The array of pointers to the stuff to write. */
KABUKI const Op* BOutWrite (BOut* bout, const uint_t* params,  void** args);

/** Sends a connection message to the given address. */
KABUKI const Op* BOutConnect (BOut* bout, const char* address);

/** Sends a connection message to the given address. */
KABUKI void BOutRingBell (BOut* bout, const char* address);

/** Sends a connection message to the given address. */
KABUKI void BOutAckBack (BOut* bout, const char* address);

#if USING_SCRIPT_TEXT
/** Prints the BIn to the Text.
    @param  bin The pin to print.
    @param  text The Text to print the bin to.
    @return The text. */
KABUKI Strand& BOutPrint (BOut* bout, Strand& strand);
#endif  //< USING_SCRIPT_TEXT

}       //< namespace _

#if USING_SCRIPT_TEXT
/** Prints out the bin to the text. */
inline _::Strand& operator<< (_::Strand& strand, _::BOut* bout) {
    return _::BOutPrint (bout, strand);
}
#endif  //< USING_SCRIPT_TEXT

#endif  //< HEADER_FOR_SCRIPT_BOUT

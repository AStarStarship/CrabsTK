/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/crabs/bout.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#ifndef HEADER_FOR_CRABS_BOUT
#define HEADER_FOR_CRABS_BOUT

#include "op.h"

#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 3

#include "printer.h"

namespace _ {

/** List of the Finite BOut States. */
typedef enum BOutStates {
    kBOutStateDisconnected = 0, //< BOut State 0: BOut locked.
    kBOutStateWriting,          //< BOut State 1: Most common state.
    kBOutStateWritingBsc,       //< BOut State 2: Writing BSQ.
} BOutState;

/** B-Output ring buffer socket.

    The BOut ring buffer is defined as being contiguous     

    
    @code
    
    @endcode

    A B-Output ring-buffer is identical in structure to an BOut ring-buffer, but
    the stop becomes volatile and start is not volatile. */
struct KABUKI BOut {
    uint_t          size;  //< Size of the B-Output.
    volatile uint_t start; //< Starting index of the ring-buffer data.
    uint_t          stop,  //< Stopping index of the ring-buffer data.
                    read;  //< Address that the BOut device is reading from.
};

/** Get's the B-Output's buffer.*/
KABUKI char* BOutBuffer (BOut* bout);

#if USING_PRINTER

/** Gets a a char for printing out the bout_state. */
KABUKI const char** BOutStateStrings ();

#endif

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

/** . */
KABUKI void BInKeyStrokes ();

#if USING_PRINTER
/** Prints the BIn to the Text.
    @param  bout The bout to print.
    @param  text The Text to print to the bout.
    @return The slot. */
KABUKI Printer& PrintBOut (Printer& print, BOut* bout);
#endif

}       //< namespace _

#if USING_PRINTER
/** Prints out the bin to the text. */
inline _::Printer& operator<< (_::Printer& print, _::BOut* bout) {
    return _::PrintBOut (print, bout);
}
#endif

#endif  //< MAJOR_SEAM >= 1 && MINOR_SEAM >= 3
#endif  //< HEADER_FOR_CRABS_BOUT
#undef DEBUG_CRABS_BOUT

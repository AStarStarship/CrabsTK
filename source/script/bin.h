/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/kabuki/script/bin.h
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

#ifndef HEADER_FOR___BIN
#define HEADER_FOR___BIN

#include "operation.h"
#include "text.h"
#include "slot.h"
#include "error.h"

namespace _ {

/** A*B B-Input Slot.
    A B-Input Slot is functionally identical to a input port in TCP. */
struct KABUKI Bin {

    /** List of Finite Bin States.
        @see Script Protocol RFC for list of states. */
    typedef enum States {
        AddressState = 0,   //< State  0: Scanning address.
        ArgsState,          //< State  1: Scanning arguments.
        Utf8State,          //< State  2: Scanning STR.
        Utf16State,         //< State  3: Scanning ST2.
        Utf32State,         //< State  4: Scanning ST8.
        VarintState,        //< State  5: Scanning varint.
        ObjectState,        //< State  6: 8-bit OB1 state.
        HashState,          //< State  7: Stand the 32-bit hash.
        ErrorState,         //< State  8: Handling an error state.
        DisconnectedState,  //< State  9: Disconnected state.
        AckState,           //< State 10: Awaiting connection ACK.
        LockedState,        //< State 11: Locked state.
        PodState,           //< State 12: Scanning plain-old-data.
    } State;

    uint_t          size,   //< The size of the buffer.
                    start;  //< The starting index of the ring buffer data.
    volatile uint_t stop;   //< The stopping index of the ring buffer data.
    uint_t          read;   //< The read variable.
};

/** Get's the B-Input's buffer.*/
KABUKI byte* BinBuffer (Bin* bin);

KABUKI byte* BinEndAddress (Bin* bin);

#if SCRIPT_USING_TEXT
/** Gets a a char for printing out the bin_state. */
KABUKI const char** BinState ();
#endif  //< SCRIPT_USING_TEXT

/** Initializes the Bin struct KABUKI to an empty buffer. */
KABUKI Bin* BinInit (uintptr_t* buffer, uint_t size);

/** Gets the rx buffer length. */
KABUKI uint_t BinSpace (Bin* bin);

/** Gets the end address of the rx buffer. */
KABUKI byte* BinEndAddress (Bin* bin);

/** Returns true if the Bin buffer contains any data.
    @warning Function does not do any error checking for speed. */
KABUKI bool BinIsReadable (Bin* bin);

/** Scans a message with the given params to the given Bin.
    The data in the Bin is word-aligned, unlike the Slot. It also 
    doesn't have a hash with an escape sequence.
    
    @param rx The Bin socket.
    @param params The parameters.
    @param args   The arguments.
    @return       Returns 0 upon success and an ErrorList ticket number upon 
                  failure. */
KABUKI const Operation* BinRead (Bin* bin, const uint_t* params, void** args);

#if SCRIPT_USING_TEXT
/** Prints the Bin to the Text.
    @param  bin The pin to print.
    @param  txt The Text to print the bin to.
    @return The txt. */
KABUKI Text& BinPrint (Bin* bin, Text& txt);
#endif  //< SCRIPT_USING_TEXT
}       //< namespace _

#if SCRIPT_USING_TEXT
/** Prints out the bin to the txt. */
inline _::Text& operator<< (_::Text& txt, _::Bin* bin) {
    return _::BinPrint (bin, txt);
}
#endif  //< SCRIPT_USING_TEXT

#endif  //< HEADER_FOR___BIN

/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/include/bin.h
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

#ifndef SCRIPT_BIN_H
#define SCRIPT_BIN_H

#include "operation.h"

namespace _ {

/*< A*B B-Input Slot.
    A B-Input Slot is functionally identical to a input port in TCP.
*/
struct KABUKI Bin {

    /** List of B-Input Errors. */
    typedef enum {
        BufferOverflowError      = 0,
        BufferUnderflowError     = 1,
        VarintOverflowError      = 2,
        InvalidHashError         = 3,
        InvalidTypeError         = 4,
        InvalidIndexError        = 5,
        InvalidEnqueryError      = 6,
        InvalidArgumentError     = 7,
        InvalidDoorError         = 8,
        TooManyParametersError   = 9,
        StackOverflowError       = 10,
        InvalidSubetError        = 11,
        TooManyPopsError         = 12,
        StringOverflowError      = 13,
        InvalidErrorHandlerError = 14,
        InvalidOpeartionError    = 15,
        ArrayOverflowError       = 16,
        InvalidOperationError    = 17,
        RoomError                = 18,
    } Error;

    /** List of Finite Bin States.
        @see Script Protocol RFC for list of states. */
    typedef enum States {
        ScanningAddressState = 0,   //< State 0: Scanning address.
        ScanningArgsState,          //< State 1: Scanning arguments.
        ScanningStringState,        //< State 2: Scanning STR.
        ScanningVarintState,        //< State 3: Scanning varint.
        ScanningPodState,           //< State 4: Scanning plain-old-data.
        ScanningHashState,          //< State 5: Stand the 32-bit hash.
        HandlingErrorState,         //< State 6: Handling an error state.
        LockedState,                //< State N: Locked state.
    } State;

    uint_t          size,   //< The size of the buffer.
                    start;  //< The starting index of the ring buffer data.
    volatile uint_t stop;   //< The stopping index of the ring buffer data.
    uint_t          read;   //< The read variable.
    byte            state,  //< The state of the Bin.
                    buffer; //< The first byte in the ring buffer.
};

/** Gets a a char for printing out the bin_state. */
KABUKI const char* BinStateString (Bin::State state);

/** Used to return an erroneous result from a B-Input.

    @param error The error type.
    @return Returns a Static Error Operation Result. */
KABUKI const Operation* BinResult (Bin* bin, Bin::Error error);

/** Used to return an erroneous result from a B-Input.
    @param  bin     The source Bin.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
KABUKI const Operation* BinResult (Bin* bin, Bin::Error error,
                                   const uint_t* header);

/** Used to return an erroneous result from a B-Input.
    @param  bin     The source Bin.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
KABUKI const Operation* BinResult (Bin* bin, Bin::Error error,
                                   const uint_t* header,
                                   byte offset);

/** Used to return an erroneous result from a B-Input.
    @param  bin     The source Bin.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
KABUKI const Operation* BinResult (Bin* bin, Bin::Error error,
                                   const uint_t* header,
                                   byte offset,
                                   byte* address);

/** Initializes the Bin struct KABUKI to an empty buffer. */
KABUKI Bin* BinInit (uintptr_t* buffer, uint_t size);

/** Gets the rx buffer length. */
KABUKI uint_t BinSpace (Bin* bin);

/** Gets the end address of the rx buffer. */
KABUKI byte* BinEndAddress (Bin* bin);

/** Returns true if the Bin buffer contains any data.
    @warning Function does not do any error checking for speed. */
KABUKI bool BinIsReadable (Bin* bin);

/** Prints out the given object to the std::out. */
KABUKI void BinPrint (Bin* bin);

/** Scans a message with the given params to the given Bin.
    The data in the Bin is word-aligned, unlike the Slot. It also
    doesn't have a hash with an escape sequence.
    
    @param rx The Bin socket.
    @param params The parameters.
    @param args   The arguments.
    @return       Returns 0 upon success and an ErrorList ticket number upon 
                  failure. */
KABUKI const Operation* BinRead (Bin* bin, const uint_t* params, void** args);

}       //< namespace _
#endif  //< SCRIPT_BIN_H

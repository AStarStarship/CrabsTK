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

namespace _ {

/** A*B B-Input Slot.
    A B-Input Slot is functionally identical to a input port in TCP. */
struct KABUKI Bin {
    /** List of B-Input Errors. */
    typedef enum {
        kErrorBufferOverflow      = 0,
        kErrorBufferUnderflow = 1,
        kErrorVarintOverflow      = 2,
        kErrorInvalidHash         = 3,
        kErrorInvalidType         = 4,
        kErrorInvalidIndex        = 5,
        kErrorInvalidEnquery      = 6,
        kErrorInvalidArgument     = 7,
        kErrorInvalidDoor         = 8,
        kErrorTooManyParameters   = 9,
        kErrorStackOverflow       = 10,
        kErrorInvalidSubet        = 11,
        kErrorTooManyPops         = 12,
        kErrorTextOverflow      = 13,
        kErrorInvalidErrorHandler = 14,
        kErrorInvalidOperand      = 15,
        kErrorArrayOverflow       = 16,
        kErrorInvalidOperation    = 17,
        kErrorUtf8                = 18,
        kErrorUtf16               = 19,
        kErrorUtf32               = 20,
        kErrorLocked              = 21,
        kErrorInvalidArgs         = 22,
        kErrorAuthenticationError = 23,
        kErrorRoom                = 24,
    } Error;

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

/** Gets a human-readable description for given error. */
KABUKI const char* BinError (Bin::Error e);

/** Gets a a char for printing out the bin_state. */
KABUKI const char* BinState (Bin::State state);

inline const char* BinState (byte state) {
    return BinState ((Bin::State)state);
}

/** Used to return an erroneous result from a B-Input.

    @param error The error type.
    @return Returns a Static Error Operation Result. */
inline const Operation* BinResult (Bin* bin, Bin::Error error) {
#if SCRIPT_DEBUG
    std::cout << "\n| Bin " << BinError (error) << " Error!\n";
#endif  //< SCRIPT_DEBUG
    return reinterpret_cast<const Operation*> (1);
}
/** Used to return an erroneous result from a B-Input.
    @param  bin     The source Bin.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
inline const Operation* BinResult (Bin* bin, Bin::Error error,
                                   const uint_t* header) {
#if SCRIPT_DEBUG
    std::cout << "\n| Bin " << BinError (error) << " Error!\n";
#endif  //< SCRIPT_DEBUG
    return reinterpret_cast<const Operation*> (1);
}

/** Used to return an erroneous result from a B-Input.
    @param  bin     The source Bin.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
inline const Operation* BinResult (Bin* bin, Bin::Error error,
                                   const uint_t* header,
                                   uint_t offset) {
#if SCRIPT_DEBUG
    std::cout << "\n| Bin " << BinError (error) << " Error!\n";
#endif  //< SCRIPT_DEBUG
    return reinterpret_cast<const Operation*> (1);
}

/** Used to return an erroneous result from a B-Input.
    @param  bin     The source Bin.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
inline const Operation* BinResult (Bin* bin, Bin::Error error,
                                   const uint_t* header,
                                   uint_t offset,
                                   byte* address) {
#if SCRIPT_DEBUG
    std::cout << "\n| Bin " << BinError (error) << " Error!\n";
#endif  //< SCRIPT_DEBUG
    return reinterpret_cast<const Operation*> (1);
}

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

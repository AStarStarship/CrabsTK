/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/include/bout.h
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

#ifndef KABUKI_SCRIPT_BOUT_H
#define KABUKI_SCRIPT_BOUT_H

#include "operation.h"

namespace _ {

/** A*B B-Output socket with a packed byte stream.
    A sequence of A*Bout expressions terminated by an ASCII CR char. All sequences get 
    evaluated down to an immutable set (by definition all sets are immutable), which 
    can be an empty set.

    # Slot Format
    By default, all sequences are a set of one byte, all sets are linear, and all sets 
    may repeat some number of times.

    @code
    ESC VU8 1 BS CR
    @endcode
    A B-Output ring-buffer is identical in structure to an Bout ring-buffer, but the stop becomes
    volatile and start is not volatile. */
struct KABUKI Bout {

    /** List of B-Output Errors. */
    typedef enum {
        BufferOverflowError = 0,
        LockedError,
        RoomError,
    } Error;

    /** List of the Finite Bout States. */
    typedef enum TxStates {
        WritingState = 0,             //< TxState 0: Most common state.
        LockedState,                  //< TxState 1: Locked.
    } State;

    uint_t size;           //< Size of the B-Output.
    volatile uint_t start; //< Starting index of the ring-buffer data.
    uint_t stop,           //< Stopping index of the ring-buffer data.
           read;           //< Address that the Bout device is reading from.
    byte   state,          //< State of the B-Output.
           buffer;         //< First byte in the ring buffer.
};

/** Gets a a char for printing out the bout_state. */
KABUKI const char* BoutStateString (Bout::State state);

/** Used to return an erroneous result from a B-Output.

    @param error The error type.
    @return Returns a Static Error Operation Result. */
KABUKI const Operation* BoutResult (Bout* bout, Bout::Error error);

/** Used to return an erroneous result from a B-Output.
    @param  bout    The source Bout.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
KABUKI const Operation* BoutResult (Bout* bout, Bout::Error error,
                                    const uint_t* header);

/** Used to return an erroneous result from a B-Output.
    @param  bout    The source Bout.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
KABUKI const Operation* BoutResult (Bout* bout, Bout::Error error,
                                    const uint_t* header,
                                    byte offset);

/** Used to return an erroneous result from a B-Output.
    @param  bout    The source Bout.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
KABUKI const Operation* BoutResult (Bout* bout, Bout::Error error,
                                    const uint_t* header,
                                    byte offset,
                                    byte* address);

/** Initializes the B-Output buffer with the given buffer size. */
KABUKI Bout* BoutInit (uintptr_t* buffer, uint_t size);

/** Calculates the space left in the given ring buffer.
    @param  bout The B-Output buffer. */
KABUKI uint_t BoutSpace (Bout* bout);

/** Gets the B-Output. */
KABUKI uint_t BoutBufferLength (Bout* bout);

/** Gets the end address of the tx buffer. */
KABUKI byte* BoutEndAddress (Bout* bout);

/** Streams a B-Output byte.
    @param bout A B-Output abstract byte stream. */
KABUKI int BoutStreamByte (Bout* bout);

/** Prints the given B-Output to the stdout. */
KABUKI void BoutPrint (Bout* bout);

/** Prints a message with the given params to the given B-Output slot.
    @param bout   The B-Output socket to write to.
    @param params The escape sequence to write.
    @param args   The array of pointers to the stuff to write. */
KABUKI const Operation* BoutWrite (Bout* bout, const uint_t* params, void** args);

/** Returns a reference to the bout for this assembly. */
template<uint_t kNumber>
inline Bout& Logbag () {
    static uintptr_t* buffer = Buffer<kNumber, kLogSize> ();
    static Bout bout = LogInit (TxInit (buffer, kLogSize));
    return bout;
}

/** Operation + writes the given value to the bout. */
inline Bout& operator+ (Bout& bout, int8_t value);

/** Operation + writes the given value to the bout. */
inline Bout& operator+ (Bout& bout, uint8_t value);

/** Operation + writes the given value to the bout. */
inline Bout& operator+ (Bout& bout, int16_t value);

/** Operation + writes the given value to the bout. */
inline Bout& operator+ (Bout& bout, uint16_t value);

/** Operation + writes the given value to the bout. */
inline Bout& operator+ (Bout& bout, int32_t value);

/** Operation + writes the given value to the bout. */
inline Bout& operator+ (Bout& bout, uint32_t value);

/** Operation + writes the given value to the bout. */
inline Bout& operator+ (Bout& bout, int64_t value);

/** Operation + writes the given value to the bout. */
inline Bout& operator+ (Bout& bout, uint64_t value);

/** Operation + writes the given value to the bout. */
inline Bout& operator+ (Bout& bout, float value);

/** Operation + writes the given value to the bout. */
inline Bout& operator+ (Bout& bout, double value);

/** Operation + prints a char to the bout. */
inline Bout& operator+ (Bout& bout, const char* string);

}       //< namespace _
#endif  //< KABUKI_SCRIPT_BOUT_H

/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/bin.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 4

#ifndef HEADER_FOR_CRABS_BIN
#define HEADER_FOR_CRABS_BIN

#include "op.h"
#include "bout.h"

namespace _ {

/** List of BIn States.
    @see SCRIPT Specification RFC. */
typedef enum BInStates {
    kBInStateAddress = 0,   //< State  0: Scanning address.
    kBInStatePackedArgs,    //< State  1: Scanning arguments.
    kBInStatePackedUtf8,    //< State  2: Scanning UTF-8 string.
    kBInStatePackedUtf16,   //< State  3: Scanning UTF-16 string.
    kBInStatePackedUtf32,   //< State  4: Scanning UTF-32 string.
    kBInStatePackedVarint,  //< State  5: Scanning varint.
    kBInStatePackedObject,  //< State  6: 8-bit OB1 state.
    kBInStateVerifyingHash, //< State  7: Verifying the 32-bit hash.
    kBInStateHandlingError, //< State  8: Handling an error state.
    kBInStateDisconnected,  //< State  9: Disconnected state.
    kBInStateAck,           //< State 10: Awaiting connection ACK.
    kBInStateLocked,        //< State 11: Locked state.
    kBInStatePackedPod,     //< State 12: Scanning plain-old-data.
} BInState;

/** A*B B-Input Slot.
    A B-Input Slot is functionally identical to a input port in TCP. */
struct KABUKI BIn {
    uint_t          size,   //< The size of the buffer.
                    start;  //< The starting index of the ring buffer data.
    volatile uint_t stop;   //< The stopping index of the ring buffer data.
    uint_t          read;   //< The read variable.
};

/** Get's the B-Input's buffer. */
inline char* BInBegin (BIn* bin) {
    return reinterpret_cast<char*> (bin) + sizeof (BIn);
}

inline char* BInEnd (BIn* bin) {
    // This will never be nil.
    //if (!bin) {
    //    return nullptr;
    //}
    return reinterpret_cast<char*> (bin) + bin->size;
}

/** Calculates the used ring buffer space.
    @param  Start The start of the data.
    @param  Stop  The stop of the data.
    @param  Size  The size of the buffer. */
inline intptr_t SlotLength (char* start, char* stop, uintptr_t size) {
    if (start > stop) {
        return size - (start - stop);
    }
    return stop - start;
}

/** Calculates the space left in the given ring buffer.
    @param  Start The start of the data.
    @param  Stop  The stop of the data.
    @param  Size  The size of the buffer. */
inline intptr_t SlotSpace (char* start, char* stop, uintptr_t size) {
    if (start > stop) {
        return start - stop;
    }
    return size - (stop - start);
}

/** Gets the rx buffer length. */
inline uint_t BInSpace (BIn* bin) {
    if (!bin) {
        return 0;
    }
    char* txb_ptr = reinterpret_cast<char*>(bin);
    return (uint_t)SlotSpace (txb_ptr + bin->start, txb_ptr + bin->stop,
                              bin->size);
}

inline uint_t BinBufferLength (BIn* bin) {
    if (!bin) {
        return ~0;
    }
    char* base = BInBegin (bin);
    return (uint_t)SlotLength (base + bin->start, base + bin->stop, bin->size);
}

#if USING_PRINTER
/** Gets a a char for printing out the bin_state. */
KABUKI const char** BInStateStrings ();

/** Pulls the keyboard input into the ring buffer. */
//KABUKI void BInKeyboard ();

#endif

/** Initializes the BIn struct KABUKI to an empty buffer. */
KABUKI BIn* BInInit (uintptr_t* buffer, uint_t size);

/** Gets the end address of the rx buffer. */
KABUKI char* BInEnd (BIn* bin);

/** Returns true if the BIn buffer contains any data.
    @warning Function does not do any error checking for speed. */
KABUKI bool BInIsReadable (BIn* bin);

/** Scans a message with the given params to the given BIn.
    The data in the BIn is word-aligned, unlike the Slot. It also
    doesn't have a hash with an escape sequence.

    @param rx     The BIn socket.
    @param params The parameters.
    @param args   The arguments.
    @return       Returns 0 upon success and an ErrorList ticket number upon
                  failure. */
KABUKI const Op* BInRead (BIn* bin, const uint_t* params, void** args);

inline const Op* BOutRead (BOut* bout, const uint_t* params, void** args) {
    return BInRead (reinterpret_cast<BIn*> (bout), params, args);
}

#if USING_PRINTER
/** Prints the BIn to the Text.
    @param  bin The pin to print.
    @param  text The Text to print the bin to.
    @return The text. */
KABUKI Printer Print (Printer printer, BIn* bin);
#endif

}       //< namespace _

#if USING_PRINTER
/** Prints out the bin to the text. */
inline _::Printer& operator<< (_::Printer& print, _::BIn* bin) {
    return _::Print (print, bin);
}
#endif

#endif  //< HEADER_FOR_CRABS_BIN
#endif  //> #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 4

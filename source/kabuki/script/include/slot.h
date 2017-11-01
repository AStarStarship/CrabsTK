/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/include/slot.h
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

#ifndef SCRIPT_SLOT_H
#define SCRIPT_SLOT_H

#include "operation.h"
#include "bout.h"
#include "bin.h"

namespace _ {

/** A Slot in a Door in a Chinese Room. */
struct KABUKI Slot {
    uint_t size,  //< The size of the buffer.
           start, //< The starting index of the ring buffer data.
           stop,  //< The stopping index of the ring buffer data.
           read;  //< The read variable.
};
const Operation* SlotResult (Slot* slot, Bin::Error error);
const Operation* SlotResult (Slot* slot, Bout::Error error);

enum {
    kSlotHeaderSize = 4 * sizeof (uint_t)   //< Size of a Slot Header.
};

static const uintptr_t kMaxSlotSize = ~((uintptr_t)0);

/** Calculates the used ring buffer space.
    @param  Start The start of the data.
    @param  Stop  The stop of the data.
    @param  Size The size of the buffer. */
KABUKI uint_t SlotLength (byte* start, byte* stop, uint_t size);

/** Calculates the space left in the given ring buffer.
    @param  Start The start of the data.
    @param  Stop  The stop of the data.
    @param  Size  The size of the buffer. */
KABUKI uint_t SlotSpace (byte* start, byte* stop, uint_t size);

/** Clears the ring buffer by writing zeros to it. */
KABUKI void SlotWipe (Slot* slot);

/** Copies a block from a ring-buffer to the given destination. */
KABUKI byte* SlotWrite (Slot* slot, void* source, size_t size);

/** Copies a block from a ring-buffer to the given destination. */
KABUKI byte* SlotRead (Slot* slot, void* source, size_t size);

/** Gets the args from the b-sequence. */
KABUKI const Operation* BArgs (Slot* slot, const uint_t* params, void** args);

/** Posts a evaluated result to the b-sequence. */
KABUKI const Operation* BResult (Slot* slot, const uint_t* params, void** args);

KABUKI bool IsReadable (Slot* slot);

/** Used to return an erroneous result from a B-Input.
    @param  bin     The source Slot.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
KABUKI const Operation* SlotResult (Slot* bin, Bin::Error error,
                                   const uint_t* header);
/** Used to return an erroneous result from a B-Input.
    @param  bin     The source Slot.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
KABUKI const Operation* SlotResult (Slot* bin, Bout::Error error,
                                   const uint_t* header);

/** Used to return an erroneous result from a B-Input.
    @param  bin     The source Slot.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
KABUKI const Operation* SlotResult (Slot* bin, Bin::Error error,
                                   const uint_t* header,
                                   byte offset);

/** Used to return an erroneous result from a B-Input.
    @param  bin     The source Slot.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
KABUKI const Operation* SlotResult (Slot* bin, Bout::Error error,
                                   const uint_t* header,
                                   byte offset);

/** Used to return an erroneous result from a B-Input.
    @param  bin     The source Slot.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
KABUKI const Operation* SlotResult (Slot* bin, Bin::Error error,
                                   const uint_t* header,
                                   byte offset,
                                   byte* address);

/** Used to return an erroneous result from a B-Input.
    @param  bin     The source Slot.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
KABUKI const Operation* SlotResult (Slot* bin, Bout::Error error,
                                   const uint_t* header,
                                   byte offset,
                                   byte* address);
}       //< namespace _
#endif  //< SCRIPT_SLOT_H

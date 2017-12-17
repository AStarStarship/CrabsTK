/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/kabuki/script/mirror.h
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

#ifndef HEADER_FOR_SCRIPT_MIRROR
#define HEADER_FOR_SCRIPT_MIRROR

#include "operation.h"
#if SCRIPT_USING_MIRROR
#include "bin.h"
#include "bout.h"

namespace _ {

/** A Mirror in a Chinese Room.
    A Mirror */
struct KABUKI Mirror {
    uint_t size,    //< The size of the buffer.
           start,   //< The starting index of the ring buffer data.
           stop,    //< The stopping index of the ring buffer data.
           read;    //< The read variable.
};

const Operation* MirrorResult (Mirror* mirror, Error error);
const Operation* MirrorResult (Mirror* mirror, Error error);

enum {
    kMirrorHeaderSize = 4 * sizeof (uint_t)   //< Size of a Mirror Header.
};

static const uintptr_t kMaxMirrorSize = ~((uintptr_t)0);

/** Calculates the used ring buffer space.
    @param  Start The start of the data.
    @param  Stop  The stop of the data.
    @param  Size The size of the buffer. */
KABUKI uint_t MirrorLength (byte* start, byte* stop, uint_t size);

/** Calculates the space left in the given ring buffer.
    @param  Start The start of the data.
    @param  Stop  The stop of the data.
    @param  Size  The size of the buffer. */
KABUKI uint_t MirrorSpace (byte* start, byte* stop, uint_t size);

/** Checks if the given slot contains the specified address. */
KABUKI void* MirrorContains (Mirror* mirror, void* address);

/** Clears the ring buffer by writing zeros to it. */
KABUKI void MirrorWipe (Mirror* mirror);

/** Copies a block from a ring-buffer to the given destination. */
//KABUKI byte* MirrorWrite (Mirror* mirror, void* source, size_t size);

/** Copies a block from a ring-buffer to the given destination. */
//KABUKI byte* MirrorRead (Mirror* mirror, void* source, size_t size);

/** Gets the args from the b-sequence. */
KABUKI const Operation* MirrorRead (Mirror* mirror, const uint_t* params,
                                  void** args);

/** Posts a evaluated result to the b-sequence. */
//KABUKI const Operation* MirrorWrite (Mirror* mirror, const uint_t* params,
//                                   void** args);

KABUKI bool MirrorIsReadable (Mirror* mirror);

/** Used to return an erroneous result from a B-Input.
    @param  bin     The source Mirror.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
KABUKI const Operation* MirrorResult (Mirror* mirror, Error error,
                                      const uint_t* header);
/** Used to return an erroneous result from a B-Input.
    @param  bin     The source Mirror.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
KABUKI const Operation* MirrorResult (Mirror* mirror, Error error,
                                   const uint_t* header);

/** Used to return an erroneous result from a B-Input.
    @param  bin     The source Mirror.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
KABUKI const Operation* MirrorResult (Mirror* mirror, Error error,
                                   const uint_t* header,
                                   byte offset);

/** Used to return an erroneous result from a B-Input.
    @param  bin     The source Mirror.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
//KABUKI const Operation* MirrorResult (Mirror* mirror, Error error,
//                                   const uint_t* header,
//                                   uint_t offset);

/** Used to return an erroneous result from a B-Input.
    @param  bin     The source Mirror.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
KABUKI const Operation* MirrorResult (Mirror* mirror, Error error,
                                   const uint_t* header,
                                   uint_t offset,
                                   byte* address);

/** Used to return an erroneous result from a B-Input.
    @param  bin     The source Mirror.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
KABUKI const Operation* MirrorResult (Mirror* mirror, Error error,
                                   const uint_t* header,
                                   uint_t offset,
                                   byte* address);

}       //< namespace _
#endif  //< SCRIPT_USING_MIRROR
#endif  //< HEADER_FOR_SCRIPT_MIRROR

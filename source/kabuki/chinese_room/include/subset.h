/** The Chinese Room
    @version 0.x
    @file    ~/chinese_room/include/subset.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough<calemccollough.github.io>
                            All right reserved (R).
             Licensed under the Apache License, Version 2.0 (the "License"); 
             you may not use this file except in compliance with the License. 
             You may obtain a copy of the License at
                        http://www.apache.org/licenses/LICENSE-2.0
             Unless required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#ifndef CHINESE_ROOM_AUTOMA_H
#define CHINESE_ROOM_AUTOMA_H

#include "config.h"

namespace _ {

/*< A rx socket that prints results to any given memory address. */
struct B {
    uint_t size,            //< The size of the buffer.
        start;              //< The starting index of the ring buffer data.
    volatile uint_t stop;   //< The stopping index of the ring buffer data.
    uint_t read;            //< The read variable.
};

/** Initializes the B struct to an empty buffer. */
KABUKI B* BInit (byte* buffer, uint_t size);

/** Initializes the B struct to an empty buffer. */
KABUKI B* BInit (B* rx, uint_t size);

/** Gets the start of the B ring buffer. */
KABUKI byte* BBaseAddress (B* ptr);

/** Gets the rx buffer length. */
KABUKI uint_t BSpace (B* rx);

/** Gets the start of the B ring buffer. */
KABUKI byte* BBaseAddress (void* ptr, uint_t rx_tx_offset);

/** Gets the end address of the rx buffer. */
KABUKI byte* BEndAddress (B* rx);

/** Scans a message with the given params to the given B.
    The data in the B is word-aligned, unlike the Monoid. It also
    doesn't have a hash with an escape sequence.
    
    @param rx The B socket.
    @param params The parameters.
    @param args The arguments.
    @return Returns 0 upon success and an ErrorList ticket number upon failure. */
KABUKI ticket_t Read (B* rx, const uint_t* params, void** args);

/** Returns true if the B buffer contains any data.
    @warning Function does not do any error checking for speed. */
KABUKI bool IsReadable (B* rx);

/** Prints out the given object to the std::out. */
KABUKI void Print (B* rx);

}       //< namespace _
#endif  //< CHINESE_ROOM_AUTOMA_H

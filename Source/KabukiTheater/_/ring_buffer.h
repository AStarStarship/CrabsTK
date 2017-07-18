/** The Chinese Room
    @version 0.x
    @file    /.../socket.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough] (calemccollough.github.io)

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#ifndef CHINESEROOM_RINGBUFFER_H
#define CHINESEROOM_RINGBUFFER_H

#include "utils.h"

namespace _ {

inline void RingBufferClear (byte* const begin, uint_t rx_start, 
                              byte* start, byte* const stop,
                              byte* const end, uint_t size)
{
    byte* cursor = begin + rx_start;
    while (start != cursor) {
        *cursor = 0;
        if (++cursor >= end) cursor -= size;
    }
}

/** Calculates the used ring buffer space.
    @param  Start The start of the data.
    @param  Stop  The stop of the data.
    @param  Size The size of the buffer.
*/
inline uint_t RingBufferLength (byte* start, byte* stop, uint_t size) {
    uint_t delta_start_stop = static_cast<uint> (stop - start);
    return start <= stop ? delta_start_stop : size - delta_start_stop;
}

/** Calculates the space left in the given ring buffer.
    @param  Start The start of the data.
    @param  Stop  The stop of the data.
    @param  Size  The size of the buffer.
*/
inline uint_t RingBufferSpace (byte* start, byte* stop, uint_t size) {
    uint_t delta_start_stop = static_cast<uint> (stop - start);
    return start <= stop ? size - delta_start_stop : delta_start_stop;
}

/** Copies a block from a ring-buffer to the given destination. */
static byte* RingBufferWrite (void* source, byte* const begin,
                              byte* const start, byte* const stop,
                              byte* const end, size_t size) {
    if (source == nullptr) return start;

    // Now we can copy the book into memory.
    if ((start > stop) && (start + size >= end)) {
        // Calculate upper chunk size.
        uint_t top_chunk = end - stop;
        size -= top_chunk;

        memcpy (source, start, top_chunk);
        memcpy (reinterpret_cast<byte*>(source) + top_chunk, begin, size);
        return begin + size;
    }
    memcpy (source, stop, size);
    return start + size;
}

/** Copies a block from a ring-buffer to the given destination. */
static byte* RingBufferRead (void* destination, byte* const begin,
                             byte* const start, byte* const stop,
                             byte* const end, size_t size) {
    if (destination == nullptr) return start;

    // Now we can copy the book into memory.
    if ((start > stop) && (start + size >= end)) {
        // Calculate upper chunk size.
        uint_t top_chunk = end - stop;
        size -= top_chunk;

        memcpy (start, destination, top_chunk);
        memcpy (begin, reinterpret_cast<byte*>(destination) + top_chunk, size);
        return begin + size;
    }
    memcpy (stop, destination, size);
    return start + size;
}

}       //< namespace _

#endif  //< CHINESEROOM_RINGBUFFER_H

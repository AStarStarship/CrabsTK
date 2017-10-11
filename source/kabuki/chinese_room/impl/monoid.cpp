/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/impl/monoid.cpp
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
    @desc    Monoid implementation split into three files, monoid.h, 
             monoid_rx.h, and monoid_tx.h because of the large read/write
             functions.
*/

#include "../include/monoid.h"
#include "../include/error.h"

namespace _ {

void MonoidClear (byte* const begin, uint_t rx_start, 
                             byte* start, byte* const stop,
                             byte* const end, uint_t size)
{
    byte* cursor = begin + rx_start;
    while (start != cursor) {
        *cursor = 0;
        if (++cursor >= end) cursor -= size;
    }
}

uint_t MonoidLength (byte* start, byte* stop, uint_t size) {
    uint_t delta_start_stop = static_cast<uint> (stop - start);
    return start <= stop ? delta_start_stop : size - delta_start_stop;
}

uint_t MonoidSpace (byte* start, byte* stop, uint_t size) {
    uint_t delta_start_stop = static_cast<uint> (stop - start);
    return start <= stop ? size - delta_start_stop : delta_start_stop;
}

byte* MonoidWrite (void* source, byte* const begin,
                              byte* const start, byte* const stop,
                              byte* const end, size_t size) {
    if (source == nullptr) return start;

    // Now we can copy the bag into memory.
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

byte* MonoidRead (void* destination, byte* const begin,
                         byte* const start, byte* const stop,
                         byte* const end, size_t size) {
    if (destination == nullptr) return start;

    // Now we can copy the bag into memory.
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

byte* MonoidWrite (void* source, byte* const begin,
                          byte* const start, byte* const stop,
                          byte* const end, size_t size) {
    if (source == nullptr) return start;

    // Now we can copy the bag into memory.
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

byte* MonoidRead (void* destination, byte* const begin,
                             byte* const start, byte* const stop,
                             byte* const end, size_t size) {
    if (destination == nullptr) return start;

    // Now we can copy the bag into memory.
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

struct MonoidTx {
    uint_t size;            //< The size of the ring buffers.
    volatile uint_t start;  //< The starting index of the ring-buffer data.
    uint_t stop,            //< The stopping index of the ring-buffer data.
        read;               //< The address that the Rx device is reading from.
};

enum {
    kSlotHeaderSize = sizeof (MonoidTx) + sizeof (uintptr_t) -
    sizeof (MonoidTx) % sizeof (uintptr_t),
    //< Offset to the start of the ring buffer.
    kMinSocketSize = 32 + kSlotHeaderSize,
};

byte* MonoidTxSlot (MonoidTx* tx) {
    return reinterpret_cast<byte*>(tx) + kSlotHeaderSize;
}

MonoidTx* MonoidTxInit (byte* buffer, uint_t size) {
    if (size < kMinSocketSize) return nullptr;
    if (buffer == nullptr) return nullptr;

    MonoidTx* tx = reinterpret_cast<MonoidTx*> (buffer);
    tx->size = size - kSlotHeaderSize;
    tx->start = 0;
    tx->stop = 0;
    tx->read = 0;

#if DEBUG_CHINESE_ROOM
    memset (MonoidTxSlot (tx), '\0', size);
#endif
    return tx;
}

MonoidTx* MonoidInit (MonoidTx* buffer, uint_t size) {
    if (size < kMinSocketSize) return nullptr;
    if (buffer == nullptr)     return nullptr;

    MonoidTx* tx = reinterpret_cast<MonoidTx*> (buffer);
    tx->size = size - kSlotHeaderSize;
    tx->start = 0;
    tx->stop  = 0;
    tx->read  = 0;

#if DEBUG_CHINESE_ROOM
    memset (MonoidTxSlot (tx), '\0', size);
#endif
    return tx;
}

uint_t MonoidTxSpace (MonoidTx* tx) {
    if (tx == nullptr) return ~0;
    byte* txb_ptr = reinterpret_cast<byte*>(tx);
    return MonoidSpace (txb_ptr + tx->start, txb_ptr + tx->stop, 
                                tx->size);
}

uint_t MonoidTxBufferLength (MonoidTx* tx) {
    if (tx == nullptr) return ~0;
    byte* base = MonoidTxSlot (tx);
    return MonoidLength (base + tx->start, base + tx->stop, tx->size);
}

byte* MonoidTxEndAddress (MonoidTx* tx) {
    return reinterpret_cast<byte*>(tx) + kSlotHeaderSize + tx->size;
}

byte MonoidTxStreamByte (MonoidTx* tx) {

    byte* begin = MonoidTxSlot (tx),
        *end = begin + tx->size;
    byte* open = (byte*)begin + tx->read,
        *start = begin + tx->start,
        *cursor = start;

    int length = start < open ? open - start + 1 :
        (end - start) + (open - begin) + 2;

    if (length < 1)
        return Report (BufferOverflowError, Esc<1, STX> (), 2,
                       start);
    byte b = *cursor;
    tx->stop = (++cursor > end) ? static_cast<uint_t> (Diff (begin, end)) :
        static_cast<uint_t> (Diff (begin, cursor));
    return 0;
}

void Print (MonoidTx* tx) {
    PrintLine ('_');
    if (tx == nullptr) {
        printf ("| Monoid null\n");
        return;
    }
    uint_t size = tx->size;
    printf ("| Monoid 0x%p: size: %u, start: %u, stop: %u, read: %u\n", tx, size,
            tx->start, tx->stop, tx->read);
    PrintMemory (MonoidTxSlot (tx), size);
}

}       //< namespace _


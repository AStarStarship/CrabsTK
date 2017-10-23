/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/impl/script_slot.cc
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

#include <stdafx.h>
#include "../include/slot.h"

namespace _ {

void SlotClear (byte* const begin, uint_t rx_start,
    byte* start, byte* const stop,
    byte* const end, uint_t size)
{
    byte* cursor = begin + rx_start;
    while (start != cursor) {
        *cursor = 0;
        if (++cursor >= end) cursor -= size;
    }
}

uint_t SlotLength (byte* start, byte* stop, uint_t size) {
    uint_t delta_start_stop = static_cast<uint> (stop - start);
    return start <= stop?delta_start_stop:size - delta_start_stop;
}

uint_t SlotSpace (byte* start, byte* stop, uint_t size) {
    uint_t delta_start_stop = static_cast<uint> (stop - start);
    return start <= stop?size - delta_start_stop:delta_start_stop;
}

byte* SlotWrite (void* source, byte* const begin, byte* const start,
                 byte* const stop, byte* const end, size_t size)
{
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

byte* SlotRead (void* destination, byte* const begin, byte* const start,
                byte* const stop, byte* const end, size_t size)
{
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

bool SlotContains (Slot* slot, void* address) {
    uintptr_t addr = reinterpret_cast<uintptr_t> (address),
              base_address = reinterpret_cast<uintptr_t> (slot),
              upper_address = base_address + slot->size;
    if ((addr > base_address) && (addr < upper_address)) {
        // Optimize for the most common case first.
        return true;
    }
    return false;
}

}       //< namespace _

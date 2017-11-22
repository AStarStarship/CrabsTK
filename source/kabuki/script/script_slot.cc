/** kabuki::script
#include <script_slot.h>
    @version 0.x
    @file    ~/source/kabuki/script/impl/script_slot.cc
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

#include <stdafx.h>
#include "../script/slot.h"
#include "../script/memory.h"

namespace _ {

void* SlotContains (Slot* slot, void* address) {
    if (slot == nullptr) {
        return nullptr;
    }
    byte* begin = reinterpret_cast<byte*> (slot) + sizeof (Slot);
    if (address < begin) {
        return nullptr;
    }
    byte* end   = begin + slot->size;
    if (address > end) {
        return nullptr;
    }
    return address;
}

void SlotWipe (Slot* slot) {
    if (slot == nullptr)
        return;
    byte* begin  = reinterpret_cast<byte*> (slot) + sizeof (Slot),
        * start  = begin + slot->start,
        * stop   = begin + slot->stop,
        * temp;
    if (start > stop) {
        temp = start;
        start = stop;
        stop = temp;
    }
    while (start != stop) {
        *start++ = 0;
    }
}

byte* SlotWrite (void* source, byte* const begin, byte* const start,
                 byte* const stop, byte* const end, size_t size) {
    if (source == nullptr) return start;

    if ((start > stop) && (start + size >= end)) {
        // Calculate upper chunk size.
        size_t top_chunk = end - stop;
        size -= top_chunk;

        MemoryCopy (stop, top_chunk, source);
        MemoryCopy (begin, size, reinterpret_cast<byte*>(source) + top_chunk);
        return begin + size;
    }
    MemoryCopy (stop, size, source);
    return start + size;
}

byte* SlotRead (void* destination, byte* const begin, byte* const start,
                byte* const stop , byte* const end  , size_t size) {
    if (destination == nullptr) return start;

    if ((start > stop) && (start + size >= end)) {
        // Calculate upper chunk size.
        size_t top_chunk = end - stop;
        size -= top_chunk;

        MemoryCopy (destination, top_chunk, start);
        MemoryCopy (reinterpret_cast<byte*>(destination) + top_chunk, size,
                    begin);
        return begin + size;
    }
    MemoryCopy (destination, size, stop);
    return start + size;
}

//bool IsWritable (MirrorOut* mirror) {
//   return mirror->start != mirror->stop;
//}

const Operation* SlotResult (Slot* bin, Bin::Error error,
                             const uint_t* header) {
    return 0;
}

const Operation* SlotResult (Slot* bin, Bout::Error error,
                                    const uint_t* header) {
    return 0;
}

const Operation* SlotResult (Slot* bin, Bin::Error error,
                                    const uint_t* header,
                                    byte offset) {
    return 0;
}

const Operation* SlotResult (Slot* bin, Bout::Error error,
                                    const uint_t* header,
                                    byte offset) {
    return 0;
}

const Operation* SlotResult (Slot* bin, Bin::Error error,
                                    const uint_t* header,
                                    uint_t offset,
                                    byte* address) {
    return 0;
}

const Operation* SlotResult (Slot* bin, Bout::Error error,
                                    const uint_t* header,
                                    uint_t offset,
                                    byte* address)
{
    return 0;
}

}       //< namespace _

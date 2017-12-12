/** kabuki::script
#include <script_utils.h>
    @version 0.x
    @file    ~/source/kabuki/script/impl/script_utils.cc
    @author  Cale McCollough <https://calemccollough.github.io>
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
#include "memory.h"

using namespace std;

namespace _ {

int MemoryAlignToPowerOf2 (int value) {
    if (value < 0) {
        return 4;
    }
    // @cite https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
    uint v = (uint)value;
    --v;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    ++v;
    return (int)value;
}

byte MemoryNibbleToLowerCaseHex (byte b) {
    if (b > 15) return 'f';
    if (b > 9)
        return b + ('a' - 10);
    return b + '0';
}

byte MemoryNibbleToUpperCaseHex (byte b) {
    if (b > 15) return 'F';
    if (b > 9) return b + ('A' - 10);
    return b + '0';
}

uint16_t MemoryByteToLowerCaseHex (byte b) {
    uint16_t value = MemoryNibbleToLowerCaseHex (b & 0xf);
    value = value << 8;
    value |= MemoryNibbleToLowerCaseHex (b >> 4);
    return value;
}

uint16_t MemoryByteToUpperCaseHex (byte b) {
    uint16_t value = MemoryNibbleToUpperCaseHex (b & 0xf);
    value = value << 8;
    value |= MemoryNibbleToUpperCaseHex (b >> 4);
    return value;
}

int MemoryHexToByte (byte c) {
    //printf ("toByte (byte c): %c ", c);
    if (c < '0') return -1;
    if (c >= 'a') {
        if (c > 'f') return -1;
        //printf ("output!: %i \n", c - ('a' - 10));
        return c - ('a' - 10);
    }
    if (c >= 'A') {
        if (c > 'F') return -1;
        return c - ('A' - 10);
    }
    if (c > '9') return -1;
    //printf ("output: %i \n", c - ('a' - 10));
    return c - '0';
}

int MemoryHexToByte (uint16_t h) {
    // Note: This works but the upper and lower are switched... pay no
    // attention to the man behind the curtain #HandWave
    //printf ("toByte (uint16_t c): %c%c\n", (byte) (h << 8), (byte)h);
    int lowerValue = MemoryHexToByte (static_cast<byte> (h >> 8));
    //printf (" lowerValue: %i \n", lowerValue);
    if (lowerValue < 0) return -1;

    int upperValue = MemoryHexToByte (static_cast<byte> (h));
    if (upperValue < 0) return -1;
    //printf (" upperValue: %i \nlowerValue | (upperValue << 4): %i\n",
    //    upperValue, lowerValue | (upperValue << 4));

    //PrintLine ();
    return lowerValue | (upperValue << 4);
}

void MemoryClear (void* address, size_t size) {
    //memset (address, '0', size);
    byte* ptr = reinterpret_cast<byte*> (address);
    for (; size != 0; --size)
        *ptr++ = '\0';
    /* This code is designed to work like memcpy but is not working right now.
    uintptr_t lsb_mask = (1 << sizeof (long)) - 1,
    msb_mask = ~lsb_mask,
    adr_uint = reinterpret_cast<uintptr_t> (address),
    adr_lsb  = adr_uint & lsb_mask;
    long* ptr = reinterpret_cast<long*> (adr_uint & msb_mask),
    * end = reinterpret_cast<long*> ((adr_uint + size) & lsb_mask);
    // Example: address 0x1
    // Mask off 0x1
    long value = *ptr,
    value_mask = adr_uint & lsb_mask;
    value &= value_mask;
    *ptr = value;
    ++ptr;
    for (; ptr < end; ++ptr)
    *ptr = 0;
    value = *ptr;
    value_mask = adr_uint & lsb_mask;
    value &= value_mask;
    *ptr = value;*/
}

byte* MemoryCopy (void* target, void* target_end, const void* memory, size_t size) {
    // @todo Optimize to write in words.
    if (target == nullptr) {
        return 0;
    }
    if (target_end == nullptr) {
        return 0;
    }
    if (memory == nullptr) {
        return 0;
    }
    byte* write = reinterpret_cast<byte*> (target),
        * write_end = reinterpret_cast<byte*> (target_end);
    if ((write_end - write) < size) {
        return nullptr;
    }
    const byte* read = reinterpret_cast<const byte*> (memory);
    for (; size != 0; --size)
        *write++ = *read++;
    return write;
}

byte* MemoryCopy (void* target, void* target_end, const void* memory,
                   const void* memory_end) {
    // @todo Optimize to write in words.
    if (target == nullptr) {
        return 0;
    }
    if (target_end == nullptr) {
        return 0;
    }
    if (memory == nullptr) {
        return 0;
    }
    if (memory_end == nullptr) {
        return 0;
    }
    byte      * write     = reinterpret_cast<     byte*> (target    ),
              * write_end = reinterpret_cast<      byte*> (target_end);
    const byte* read      = reinterpret_cast<const byte*> (memory    ),
              * read_end  = reinterpret_cast<const byte*> (memory_end);
    size_t target_size = write_end - write,
           memory_size = read_end - read;
    if (target_size < memory_size) { // Buffer overflow!
        return nullptr;
    }
    for (; memory_size != 0; --memory_size) {
        *write++ = *read++;
    }
    return write;
}

byte* MemoryCopy (void* target, void* target_end, const void* memory,
                  const void* memory_end, size_t size) {
    // @todo Optimize to write in words.
    if (target == nullptr) {
        return 0;
    }
    if (target_end == nullptr) {
        return 0;
    }
    if (memory == nullptr) {
        return 0;
    }
    if (memory_end == nullptr) {
        return 0;
    }
    byte* write = reinterpret_cast<byte*> (target),
        *write_end = reinterpret_cast<byte*> (target_end);
    if ((write_end - write) < size) {
        return nullptr;
    }
    const byte* read = reinterpret_cast<const byte*> (memory),
        *read_end = reinterpret_cast<const byte*> (memory_end);
    if ((read_end - read) < size) {
        return nullptr;
    }
    for (; size != 0; --size) {
        *write++ = *read++;
    }
    return write;
}

}       //< namespace _

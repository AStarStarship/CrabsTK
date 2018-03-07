/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/crabs/crabs_utils.cc
    @author  Cale McCollough <https://calemccollough.github.io>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 2

#include "memory.h"

#if MAJOR_SEAM == 1 && MINOR_SEAM == 2
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PUTCHAR(c) putchar(c);
#else
#define PRINTF(x, ...)
#define PUTCHAR(c)
#endif

namespace _ {

uint32_t AlignPowerOf2 (uint32_t value) {
   // @cite https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
    if (value < 0) {
        return 4;
    }
    // 
    --value;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    ++value;
    return value;
}

int32_t AlignPowerOf2 (int32_t value) {
    if (value < 0) {
        return 4;
    }
    uint32_t v = (uint32_t)value;
    --v;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    ++v;
    return (int32_t)value;
}

void MemoryClear (void* address, size_t size) {
    //memset (address, '0', size);
    char* ptr = reinterpret_cast<char*> (address);
    for (; size; --size)
        *ptr++ = '\0';
    /* This code is designed to work like memcpy but is not working right now.
    uintptr_t lsb_mask = (1 << sizeof (long)) - 1,
    msb_mask  = ~lsb_mask,
    adr_uint  = reinterpret_cast<uintptr_t> (address),
    adr_lsb   = adr_uint & lsb_mask;
    long* ptr = reinterpret_cast<long*> (adr_uint & msb_mask),
    * end = reinterpret_cast<long*> ((adr_uint + size) & lsb_mask);
    // Example: address 0x1
    // Mask off 0x1
    long value = *ptr,
    value_mask = adr_uint & lsb_mask;
    value &= value_mask;
    *ptr = value;
    ++ptr;
    for (; ptr < end; ++ptr) {
        *ptr = 0;
    }
    value = *ptr;
    value_mask = adr_uint & lsb_mask;
    value &= value_mask;
    *ptr = value;*/
}

char* MemoryCopy (char* write, char* write_end, const char* read,
                  const char* read_end) {
    // @todo Optimize to write in words.
    if (!write) {
        return nullptr;
    }
    if (write > write_end) {
        return nullptr;
    }
    if (!read) {
        return nullptr;
    }
    if (read > read_end) {
        return nullptr;
    }

    size_t target_size = write_end - write,
           read_size   = read_end  - read;
    if (target_size < read_size) { // Buffer overflow!
        return nullptr;
    }
    for (; read_size != 0; --read_size) {
        *write++ = *read++;
    }
    return write;
}

char* MemoryCopy (char* write, char* write_end, const char* read,
                  int size) {
    // @todo Optimize to write in words.
    if (!write) {
        return nullptr;
    }
    if (write > write_end) {
        return nullptr;
    }
    if (!read) {
        return nullptr;
    }
    if ((write_end - write) < size) {
        return nullptr;
    }
    for (; size; --size) {
        *write++ = *read++;
    }
    return write;
}

char* MemoryCopy (char* write, char* write_end, const char* read,
                  const char* read_end, int size) {
    // @todo Optimize to write in words.
    if (!write) {
        return nullptr;
    }
    if (write > write_end) {
        return nullptr;
    }
    if (!read) {
        return nullptr;
    }
    if (read > read_end) {
        return nullptr;
    }
    // Check for room in both the read and write buffers.
    if ((write_end - write) < size) {
        return nullptr;
    }
    if ((read_end - read) < size) {
        return nullptr;
    }
    for (; size; --size) {
        *write++ = *read++;
    }
    return write;
}

}       //< namespace _
#undef PRINTF
#undef PUTCHAR
#endif  //< MAJOR_SEAM == 1 && MINOR_SEAM >= 1

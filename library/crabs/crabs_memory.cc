/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/crabs_utils.cc
    @author  Cale McCollough <https://calemccollough.github.io>
    @license Copyright (C) 2014-8 Cale McCollough <calemccollough@gmail.com>;
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

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 2

#include "memory.h"
#include "printer.h"
#include "right.h"
#include "hex.h"

#if MAJOR_SEAM == 1 && MINOR_SEAM == 2
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PUTCHAR(c) putchar(c);
#else
#define PRINTF(x, ...)
#define PUTCHAR(c)
#endif

namespace _ {

uintptr_t* MemoryCreate (uintptr_t size) {
    return new uintptr_t[size];
}

void MemoryDestroy (uintptr_t* buffer) {
    delete buffer;
}

void MemoryClear (void* address, size_t size) {
    //memset (address, '0', size);
    char* ptr = reinterpret_cast<char*> (address);
    for (; size; --size)
        *ptr++ = '\0';
    /* This code is designed to work like memcpy but is not working right now.
    uintptr_t lsb_mask = (1 << sizeof (uint32_t)) - 1,
    msb_mask  = ~lsb_mask,
    adr_uint  = reinterpret_cast<uintptr_t> (address),
    adr_lsb   = adr_uint & lsb_mask;
    uint32_t* ptr = reinterpret_cast<uint32_t*> (adr_uint & msb_mask),
    * end = reinterpret_cast<uint32_t*> ((adr_uint + size) & lsb_mask);
    // Example: address 0x1
    // Mask off 0x1
    uint32_t value = *ptr,
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

char* MemoryCopy (void* begin, void* end, const void* start,
                  const void* stop) {
    ASSERT (begin)
    ASSERT (start)

    char      * begin_ptr = reinterpret_cast<char      *> (begin),
              * end_ptr   = reinterpret_cast<char      *> (end);
    const char* start_ptr = reinterpret_cast<const char*> (start),
              * stop_ptr  = reinterpret_cast<const char*> (stop);

    if (begin_ptr >= end_ptr || start_ptr >= stop_ptr)
        return nullptr;

    // Debug stuff.
    char* begin_copy = begin_ptr,
        * end_copy   = end_ptr;

    // Algorithm:
    // 1.) Save return value.
    // 2.) Align write pointer up and copy the unaligned bytes in the lower 
    //     memory region.
    // 3.) Align write_end pointer down and copy the unaligned bytes in the  
    //     upper memory region.
    // 4.) Copy the word-aligned middle region.
    char* success = end_ptr ,
        * aligned_pointer = AlignUpPointer<> (begin_ptr);
    while (begin_ptr < aligned_pointer)
        *begin_ptr++ = *start_ptr++;
    aligned_pointer = AlignDownPointer<> (end_ptr);
    while (end_ptr > aligned_pointer)
        *end_ptr-- = *stop_ptr--;

    uintptr_t      * words     = reinterpret_cast<uintptr_t      *> (begin_ptr),
                   * words_end = reinterpret_cast<uintptr_t      *> (end_ptr);
    const uintptr_t* read_word = reinterpret_cast<const uintptr_t*> (start_ptr);

    while (words < words_end)
        *words++ = *read_word++;

    COUT << Socket (begin_copy, end_copy);

    return success;
}

bool MemoryCompare (const void* begin, const void* end, const void* start,
                     const void* stop) {
    const char* begin_ptr = reinterpret_cast<const char*> (begin),
              * end_ptr   = reinterpret_cast<const char*> (end),
              * start_ptr = reinterpret_cast<const char*> (start),
              * stop_ptr  = reinterpret_cast<const char*> (stop);
    while (begin_ptr < end_ptr) {
        char a = *begin_ptr++,
             b = *end_ptr++;
        if (a != b)
            return false;
    }
    return true;
}

char* PrintMemory (char* cursor, char* end, const void* start, 
                   const void* stop) {
    ASSERT (start)
    ASSERT (cursor)
    ASSERT (cursor < end)

    char      * buffer_begin    = cursor;
    const char* address_ptr     = reinterpret_cast<const char*> (start),
              * address_end_ptr = reinterpret_cast<const char*> (stop);
    size_t      size      = address_end_ptr - address_ptr,
                num_rows        = size / 64 + 
                                  (size % 64 != 0) ? 1 : 0;

    intptr_t num_bytes = 81 * (num_rows + 2);
    if ((end - cursor) <= num_bytes) {
        PRINTF ("\n    ERROR: buffer overflow trying to fit %i in %i bytes!",
                (int)num_bytes, (int)(end - cursor))
        return nullptr;
    }
    size += num_bytes;
    *cursor++ = '\n';
    *cursor++ = '|';

    //  columns
    *cursor++ = '0';
    cursor = PrintRight (cursor, end, 8, 8);
    *cursor++ = ' ';
    for (int i = 16; i <= 56; i += 8) {
        cursor = PrintRight (cursor, end, i, 8);
    }
    for (int j = 8; j > 0; --j)
        *cursor++ = ' ';
    *cursor++ = '|';
    *cursor++ = '\n';
    *cursor++ = '|';
    for (int j = 8; j > 0; --j) {
        *cursor++ = '+';
        for (int k = 7; k > 0; --k)
            *cursor++ = '-';
    }
    *cursor++ = '|';
    *cursor++ = ' ';
        
    cursor = PrintHex (cursor, end, address_ptr);

    PRINTF ("\nBuffer space left:%i", (int)(end - cursor))
    char c;
    while (address_ptr < address_end_ptr) {
        *cursor++ = '\n';
        *cursor++ = '|';
        for (int i = 0; i < 64; ++i) {
            c = *address_ptr++;
            if (address_ptr > address_end_ptr)
                c = 'x';
            if (c < ' ') {
                c = ' ';
            }
            *cursor++ = c;
        }
        *cursor++ = '|';
        *cursor++ = ' ';
        cursor = PrintHex (cursor, end, address_ptr);
    }
    *cursor++ = '\n';
    *cursor++ = '|';
    for (int j = 8; j > 0; --j) {
        *cursor++ = '+';
        for (int k = 7; k > 0; --k) {
            *cursor++ = '-';
        }
    }
    *cursor++ = '|';
    *cursor++ = ' ';
    return PrintHex (cursor, end, address_ptr + size);
}

Socket::Socket () {
    // Nothing to do here! ({:-)-+=<
}

Socket::Socket (void* begin, void* end) :
    begin (reinterpret_cast<char*> (begin)),
    end   (reinterpret_cast<char*> (end  )) {
    if (!begin || !end || begin > end) {
        begin = end = 0;
        return;
    }
}

Socket::Socket (void* begin, intptr_t size) :
    begin (reinterpret_cast<char*> (begin)),
    end   (reinterpret_cast<char*> (begin) + size) {
    if (!begin || size < 0) {
        end = reinterpret_cast<char*> (begin);
        return;
    }
}

Socket::Socket (const Socket& other) :
    begin (other.begin),
    end   (other.end) {
    // Nothing to do here! ({:-)-+=<
}

Socket& Socket::operator= (const Socket& other) {
    begin = other.begin;
    end   = other.end;
    return *this;
}

}       //< namespace _
#undef PRINTF
#undef PUTCHAR
#endif  //< MAJOR_SEAM == 1 && MINOR_SEAM >= 1

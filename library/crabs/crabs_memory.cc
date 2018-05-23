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

uintptr_t* MemoryCreate (uintptr_t size_bytes) {
    return new uintptr_t[size_bytes];
}

void MemoryDestroy (uintptr_t* buffer) {
    delete buffer;
}

void MemoryClear (void* address, size_t size_bytes) {
    //memset (address, '0', size);
    char* ptr = reinterpret_cast<char*> (address);
    for (; size_bytes; --size_bytes)
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

/** Copies memory in word-aligned chucks. */
char* MemoryCopyFast (char* write, char* write_end, const char* read,
                      const char* read_end) {
    // Algorithm:
    // 1.) Save return value.
    // 2.) Align write pointer up and copy the unaligned bytes in the lower 
    //     memory region.
    // 3.) Align write_end pointer down and copy the unaligned bytes in the  
    //     upper memory region.
    // 4.) Copy the word-aligned middle region.
    char* success = write_end + 1 ,
        * aligned_pointer = AlignUpPointer<> (write);
    while (write < aligned_pointer)
        *write++ = *read++;
    aligned_pointer = AlignDownPointer<> (write_end);
    while (write_end > aligned_pointer)
        *write_end-- = *read_end--;

    uintptr_t* words = reinterpret_cast<uintptr_t*> (write),
             * words_end = reinterpret_cast<uintptr_t*> (write_end);
    const uintptr_t* read_word = reinterpret_cast<const uintptr_t*> (read);
    while (words < words_end)
        *words++ = *read_word++;
    return success;
}

inline char* MemoryCopyFast (void* write, void* write_end, const void* read, 
                             const void* read_end) {
    return MemoryCopyFast (reinterpret_cast<char*> (write),
                           reinterpret_cast<char*> (write_end),
                           reinterpret_cast<const char*> (read),
                           reinterpret_cast<const char*> (read_end));
}

char* MemoryCopy (void* cursor, void* end, const void* read,
                  const void* read_end) {
    ASSERT (cursor)
    ASSERT (cursor > end)
    ASSERT (read)
    ASSERT (read > read_end)

    if (SocketSize (cursor, end) < SocketSize (read, read_end))
        return nullptr; //< Buffer overflow!
    return MemoryCopyFast (cursor, end, read, read_end);
}

char* MemoryCopy (void* write, void* write_end, const void* read,
                  size_t byte_count) {
    ASSERT (write)
    ASSERT ((write < write_end))
    ASSERT (read)

    if ((size_t)SocketSize (write, write_end) < byte_count) // Buffer overflow!
        return nullptr;

    return MemoryCopy (write, write_end, read, 
                       reinterpret_cast<const char*> (read) + byte_count);
}

char* MemoryCopy (void* write, size_t write_size, const void* read, 
                  size_t read_size) {
    ASSERT (write);
    ASSERT (read);
    if (write_size < read_size)
        return nullptr;

    return MemoryCopy (write, reinterpret_cast<char*> (write) + write_size,
                       read , reinterpret_cast<const char*> (read) + read_size);
}

char* PrintMemory (char* cursor, char* end, const void* start, 
                   const void* stop) {
    ASSERT (start)
    ASSERT (cursor)
    ASSERT (cursor < end)

    char      * buffer_begin    = cursor;
    const char* address_ptr     = reinterpret_cast<const char*> (start),
              * address_end_ptr = reinterpret_cast<const char*> (stop);
    size_t      size_bytes      = address_end_ptr - address_ptr,
                num_rows        = size_bytes / 64 + 
                                  (size_bytes % 64 != 0) ? 1 : 0;

    intptr_t num_bytes = 81 * (num_rows + 2);
    if ((end - cursor) <= num_bytes) {
        PRINTF ("\n    ERROR: buffer overflow trying to fit %i in %i bytes!",
                (int)num_bytes, (int)(end - cursor))
        return nullptr;
    }
    size_bytes += num_bytes;
    *cursor++ = '\n';
    *cursor++ = '|';

    //  columns
    *cursor++ = '0';
    cursor = PrintRight (cursor, end, 8, 7);
    for (int i = 16; i <= 64; i += 8) {
        cursor = PrintRight (cursor, end, i, 8);
    }
    *cursor++ = '|';
    *cursor++ = '\n';
    *cursor++ = '|';
    for (int j = 8; j > 0; --j) {
        for (int k = 7; k > 0; --k) {
            *cursor++ = '-';
        }
        *cursor++ = '+';
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
        //PRINT_HEADING
        //PRINTF ("\n%s", buffer_begin)
        //PRINT_HEADING
    }
    *cursor++ = '\n';
    *cursor++ = '|';
    for (int j = 8; j > 0; --j) {
        for (int k = 7; k > 0; --k) {
            *cursor++ = '-';
        }
        *cursor++ = '+';
    }
    *cursor++ = '|';
    *cursor++ = ' ';
    return PrintHex (cursor, end, address_ptr + size_bytes);
}

Socket::Socket (char* begin, char* end) :
    begin (begin),
    end (end) {
    if (!begin || !end || begin > end) {
        begin = end = 0;
        return;
    }
}

Socket::Socket (char* begin, intptr_t size_bytes) :
    begin (begin),
    end (begin + size_bytes) {
    if (!begin || size_bytes < 0) {
        end = begin;
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

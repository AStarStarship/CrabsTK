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
                  const char* read_end, intptr_t size) {
    // @todo Optimize to write in words.
    if (!write || write > write_end || !read || read > read_end ||
        (write_end - write) < size || (read_end - read) < size) {
        return nullptr;
    }
    while (--size >= 0) {
        *write++ = *read++;
    }
    return write;
}

char* PrintMemory (const void* token, const void* token_end, char* buffer,
                   char* buffer_end, char delimiter) {
    if (!token) {
        return nullptr;
    }
    if (!buffer) {
        return buffer;
    }
    if (buffer >= buffer_end) {
        return nullptr;
    }
    char      * buffer_begin    = buffer;
    const char* address_ptr     = reinterpret_cast<const char*> (token),
              * address_end_ptr = reinterpret_cast<const char*> (token_end);
    size_t      size            = address_end_ptr - address_ptr,
                num_rows        = size / 64;
        
    //PRINTF ("\n    Printing Buffer with length:%i", TextLength (token))

    if (size % 64 != 0) {
        ++num_rows;
    }
    size += 81 * (num_rows + 2);
    if (buffer + size >= buffer_end) {
        PRINTF ("\n    ERROR: buffer isn't big enough!")
        return nullptr;
    }
    *buffer++ = '\n';
    *buffer++ = '|';

    //  columns
    *buffer++ = '0';
    buffer = PrintRight (8, 7, buffer, buffer_end);
    for (int i = 16; i <= 64; i += 8) {
        buffer = PrintRight (i, 8, buffer, buffer_end);
    }
    *buffer++ = '|';
    *buffer++ = '\n';
    *buffer++ = '|';
    for (int j = 8; j > 0; --j) {
        for (int k = 7; k > 0; --k) {
            *buffer++ = '-';
        }
        *buffer++ = '+';
    }
    *buffer++ = '|';
    *buffer++ = ' ';
        
    buffer = PrintHex (address_ptr, buffer, buffer_end);
    char c;
    while (address_ptr < address_end_ptr) {
        *buffer++ = '\n';
        *buffer++ = '|';
        for (int i = 0; i < 64; ++i) {
            c = *address_ptr++;
            if (address_ptr > address_end_ptr)
                c = 'x';
            if (c < ' ') {
                c = ' ';
            }
            *buffer++ = c;
        }
        *buffer++ = '|';
        *buffer++ = ' ';
        buffer = PrintHex (address_ptr, buffer, buffer_end);
        //PRINT_HEADING
        //PRINTF ("\n%s", buffer_begin)
        //PRINT_HEADING
    }
    *buffer++ = '\n';
    *buffer++ = '|';
    for (int j = 8; j > 0; --j) {
        for (int k = 7; k > 0; --k) {
            *buffer++ = '-';
        }
        *buffer++ = '+';
    }
    *buffer++ = '|';
    *buffer++ = ' ';
    return PrintHex (address_ptr + size, buffer, buffer_end, delimiter);
}

Memory::Memory (const char* begin, const char* end) :
    begin (begin),
    end (end) {
    if (!begin || !end || begin > end) {
        begin = end = 0;
        return;
    }
}

Memory::Memory (const char* begin, intptr_t size) :
    begin (begin),
    end (begin + size) {
    if (!begin || size < 0) {
        end = begin;
        return;
    }
}

}       //< namespace _
#undef PRINTF
#undef PUTCHAR
#endif  //< MAJOR_SEAM == 1 && MINOR_SEAM >= 1

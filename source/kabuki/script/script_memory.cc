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
#include "../script/memory.h"

using namespace std;

namespace _ {

byte* WordAlign (byte* ptr) {
    return ptr + (((~reinterpret_cast<uintptr_t> (ptr))+1) & 
                  (sizeof (byte*)-1));
}

uintptr_t* WordAlign64 (uintptr_t* buffer) {
    byte* byte_ptr = reinterpret_cast<byte*> (buffer);
    uintptr_t offset = (((~reinterpret_cast<uintptr_t> (buffer)) + 1) &
        (sizeof (uint64_t) - 1));
    return reinterpret_cast<uintptr_t*> (byte_ptr + offset);
}

uintptr_t WordAlignSize (uintptr_t size) {
    // Algorithm works by inverting the bits, mask of the LSbs and adding 1.
    // This allows the algorithm to word align without any if statements.
    // The algorithm works the same for all memory widths as proven by the
    // truth table bellow.
    // ~000 = 111 => 000 + 111 + 1 = 0x1000
    // ~001 = 110 => 001 + 110 + 1 = 0x1000
    // ~010 = 101 => 010 + 101 + 1 = 0x1000
    // ~011 = 100 => 011 + 100 + 1 = 0x1000
    // ~100 = 011 => 100 + 011 + 1 = 0x1000
    // ~101 = 010 => 101 + 010 + 1 = 0x1000
    // ~110 = 001 => 110 + 001 + 1 = 0x1000
    // ~111 = 000 => 111 + 000 + 1 = 0x1000
    //                                    v-----Mask
    return size + (((~size) + 1) & (sizeof (uintptr_t) - 1));
}

uintptr_t AlignSize64 (uintptr_t size) {
    // Algorithm works by inverting the bits, mask of the LSbs and adding 1.
    // This allows the algorithm to word align without any if statements.
    // The algorithm works the same for all memory widths as proven by the
    // truth table bellow.
    // ~000 = 111 => 000 + 111 + 1 = 0x1000
    // ~001 = 110 => 001 + 110 + 1 = 0x1000
    // ~010 = 101 => 010 + 101 + 1 = 0x1000
    // ~011 = 100 => 011 + 100 + 1 = 0x1000
    // ~100 = 011 => 100 + 011 + 1 = 0x1000
    // ~101 = 010 => 101 + 010 + 1 = 0x1000
    // ~110 = 001 => 110 + 001 + 1 = 0x1000
    // ~111 = 000 => 111 + 000 + 1 = 0x1000
    //                                                        Mask
    return size + (((~size) + 1) & (sizeof (uint64_t) - 1));
}

uintptr_t Diff (void* begin, void* end) {
    return static_cast<uintptr_t>(reinterpret_cast<byte*> (end) -
                                  reinterpret_cast<byte*> (begin));
}

uintptr_t Diff (const void* begin, const void* end) {
    byte* start = static_cast<byte*> (const_cast<void*> (begin)),
        *stop = static_cast<byte*> (const_cast<void*> (end));
    return static_cast<uintptr_t> (stop - start);
}

void PrintMemory (const void* address, const void* end) {
    printf ("\n|%i", 0);
    // Print columns
    for (int i = 8; i <= 66; i += 8)
        printf ("%8i", i);
    cout << '\n' << '|';
    for (int i = 0; i < 65; ++i)
        cout << '_';

    const char* chars = reinterpret_cast<const char*> (address);
    char temp;
    while (chars < end) {
        cout << '\n' << '|';
        for (int i = 0; i < 64; ++i) {
            temp = *chars;
            if (chars >= end)
                temp = 'x';
            putchar (temp);
            ++chars;
        }
        printf ("| 0x%p", chars - 64);
    }
    cout << '\n' << '|';
    for (int i = 0; i < 64; ++i)
        cout << '_';
    printf ("| 0x%p\n", chars + Diff (address, end));
}

void PrintMemory (const void* address, size_t size) {
    const char* end = reinterpret_cast<const char*>(address) + size;
    PrintMemory (address, end);
}

void MemoryClear (void* address, size_t size) {
    byte* ptr = reinterpret_cast<byte*> (address);
    for (; size != 0; --size)
        *ptr++ = '\0';
    /*
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

size_t MemoryCopy (void* destination, size_t size, const void* source) {
    if (destination == nullptr)
        return 0;
    if (source == nullptr)
        return 0;
    //memcpy (destination, size, source);
    byte      * write = reinterpret_cast<byte*> (destination);
    const byte* read  = reinterpret_cast<const byte*> (source);
    for (size_t count = size; count != 0; --count)
        *write++ = *read++;
    return size;
}

}       //< namespace _

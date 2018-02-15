/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/crabs/crabs_console.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
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

#include "console.h"

#if CRABS_SEAM >= 2

#include "text.h"


#if CRABS_SEAM == 2
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PUTCHAR(c) putchar(c);
#else
#define PRINTF(x, ...)
#define PUTCHAR(c)
#endif

namespace _ {

#if USING_CRABS_TEXT

Slot& ConsoleSet (uintptr_t *buffer, uintptr_t buffer_size) {

#if USING_DYNAMIC_MEMORY
    static uintptr_t* the_buffer = nullptr;
    static uintptr_t  the_buffer_size = kSlotBufferSizeRx +
        kSlotBufferSizeTx + 1;
#else
    enum { kBufferSizeWords = buffer_size >> kWordSizeShift };
    static uintptr_t the_buffer[kBufferSizeWords];
#endif
}

Slot& Print () {
    static uintptr_t buffer;
    static Slot empty_slot = {
        reinterpret_cast<char*> (&buffer),
        reinterpret_cast<char*> (&buffer),
        reinterpret_cast<char*> (&buffer),
        reinterpret_cast<char*> (&buffer) + sizeof (uintptr_t) - 1
    };
    static Slot& slot = ConsoleSet (empty_slot);
    return slot;
}

Slot& PrintLine () {
    return Print () << '\n';
}

char ConsoleReadChar (const char* header) {
    Print () << header;
    char c = getchar ();
    Print () << '\n';
    return c;
}

int ConsoleReadInt (const char* header) {
    int number;
    Print () << header;
    std::cin.clear ();
    std::cin >> number;
    return number;
}

float ConsoleReadFloat (const char* header) {
    float number;
    Print () << header;
    std::cin.clear ();
    std::cin >> number;
    return number;
}

void ConsoleReadSlot (const char* header, char* target, char* target_end) {
    if (!target) {
        return;
    }
    if (target > target_end) {
        return;
    }
    Print () << header;
    std::cin.get (target, target_end - target, '\n');
    std::cin.clear ();
    std::cin.ignore (target_end - target, '\n');
}

void ConsoleWrite (const char* slot) {
    Print () << slot;
}

void ConsoleWrite (Slot& slot) {
    
}

void ConsoleDump (Slot& slot) {
    char* start = slot.start,
        * stop  = slot.stop,
        * end   = slot.end;
    while (start != stop) {
        if (start > stop) {
            start = slot.begin;
        }
        Print () << *start++;
    }
    slot.start = start;
}

Slot& Scan () {
    Slot& write = Print ();
    char* begin,
        *cursor = write.start,
        *stop = write.stop,
        *end = write.end;
    while (cursor != stop) {
        if (cursor > end) {
            cursor = begin;
        }
        std::cerr << *cursor++;
    }
}

#endif
}       //< namespace _
#undef PRINTF
#undef PUTCHAR
#endif  //< CRABS_SEAM >= 2

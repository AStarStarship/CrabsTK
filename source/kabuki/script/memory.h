/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/include/utils.h
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

#pragma once
#include <stdafx.h>

#ifndef SCRIPT_MEMORY_H
#define SCRIPT_MEMORY_H

#include "config.h"

namespace _ {

/** Creates/Gets a static buffer of the specified size. */
template<uint_t kBufferSize>
KABUKI uintptr_t* WordBuffer () {
    static uintptr_t buffer[(kBufferSize / sizeof (uintptr_t)) + 1];
    return buffer;
}

/** Creates/Gets one of n static buffers of the specified size. */
template<uint_t kBufferNumber,
         uint_t kBufferSize>
KABUKI uintptr_t* BufferNum () {
    static uintptr_t buffer[(kBufferSize / sizeof (uintptr_t)) + 1];
    return buffer;
}

/** Creates/Gets one of n static buffers of the specified size. */
template<typename T,
         uint_t kBufferNumber,
         uint_t kBufferSize>
KABUKI T* TypeBuffer () {
    static T buffer[(kBufferSize / sizeof (uintptr_t)) + 1];
    return buffer;
}

/** reinterpret_cast(string) a the given base and offset to an object pointer.
*/
template<typename T>
KABUKI T* PointTo (void* base, uint_t offset) {
    return reinterpret_cast<T*>(reinterpret_cast<byte*>(base) + offset);
}

/** Returns the number to add to word-align the given pointer to a uint_t-bit
    boundary.
    @param ptr The address to align.
    @return The offset to add to the ptr to word align it.
    */
template<typename T>
KABUKI uintptr_t WordAlignOffset (void* ptr) {
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
    // ~111 = 000 => 111 + 000 + 1 = 0x1000              v--- Mask
    return ((~reinterpret_cast<uintptr_t> (ptr)) + 1) & (sizeof (T) - 1);
}

KABUKI byte* WordAlign (byte* ptr);

KABUKI uintptr_t* WordAlign64 (uintptr_t* buffer);

//KABUKI uintptr_t AlignSize (uintptr_t size);
KABUKI uintptr_t AlignSize64 (uintptr_t size);

/** Calculates the difference between the begin and end address. */
KABUKI uintptr_t Diff (void* begin, void* end);

/** Calculates the difference between the begin and end address. */
KABUKI uintptr_t Diff (const void* begin, const void* end);

#if USE_MORE_ROM
/** Prints out the contents of the address to the debug stream. */
KABUKI void PrintMemory (const void* address, const void* end);

KABUKI void PrintMemory (const void* address, size_t size);
#endif  //< USE_MORE_ROM

/** Overwrites the memory with zeros. */
KABUKI void MemoryClear (void* address, size_t size);

/** Copies the source to the destination. */
KABUKI size_t MemoryCopy (void* destination, size_t size, const void* source);

/*< Creates a buffer from dynamic memory. */
template<typename T, typename U>
KABUKI T* New (U const size, U min_size) {
    if (size < min_size) return nullptr;
    byte* buffer;
    try {
        buffer = new byte[size];
#if DEBUG
        memset (buffer, '\0', size);
#endif
        return reinterpret_cast<T*>(buffer);
    } catch (...) {
        return nullptr;
    }
}

}       //< namespace _

#endif  //< SCRIPT_MEMORY_H

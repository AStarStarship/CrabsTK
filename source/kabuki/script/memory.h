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
inline uintptr_t* WordBuffer () {
    static uintptr_t buffer[(kBufferSize / sizeof (uintptr_t)) + 1];
    return buffer;
}

/** Creates/Gets one of n static buffers of the specified size. */
template<uint_t kBufferNumber,
         uint_t kBufferSize>
    inline uintptr_t* BufferNum () {
    static uintptr_t buffer[(kBufferSize / sizeof (uintptr_t)) + 1];
    return buffer;
}

/** Creates/Gets one of n static buffers of the specified size. */
template<typename T,
         uint_t kBufferNumber,
         uint_t kBufferSize>
inline T* TypeBuffer () {
    static T buffer[(kBufferSize / sizeof (uintptr_t)) + 1];
    return buffer;
}

/** reinterpret_cast(string) a the given base and offset to an object pointer.
*/
template<typename T>
inline T* PointTo (void* base, uint_t offset) {
    return reinterpret_cast<T*>(reinterpret_cast<byte*>(base) + offset);
}

/** Returns the number to add to word-align the given pointer to a uint_t-bit
    boundary.
    @param ptr The address to align.
    @return The offset to add to the ptr to word align it. */
template<typename T>
inline uintptr_t WordAlignOffset (void* ptr) {
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

/** Word aligns the given byte pointer up in addresses.
    @param ptr Pointer to align.
    @return Next word aligned up pointer. */
inline byte* WordAlign (byte* ptr) {
    return ptr + (((~reinterpret_cast<uintptr_t> (ptr)) + 1) &
        (sizeof (byte*) - 1));
}

inline uintptr_t* WordAlign64 (uintptr_t* buffer) {
    byte* byte_ptr = reinterpret_cast<byte*> (buffer);
    uintptr_t offset = (((~reinterpret_cast<uintptr_t> (buffer)) + 1) &
        (sizeof (uint64_t) - 1));
    return reinterpret_cast<uintptr_t*> (byte_ptr + offset);
}

inline uintptr_t WordAlignSize (uintptr_t size) {
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
    //                                       v----- Mask
    return size + (((~size) + 1) & (sizeof (uintptr_t) - 1));
}


//KABUKI uintptr_t AlignSize (uintptr_t size);
inline uintptr_t AlignSize64 (uintptr_t size) {
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
    //                                       v----- Mask
    return size + (((~size) + 1) & (sizeof (uint64_t) - 1));
}

/** Calculates the difference between the begin and end address. */
inline uintptr_t Diff (void* begin, void* end) {
    return static_cast<uintptr_t>(reinterpret_cast<byte*> (end) -
                                  reinterpret_cast<byte*> (begin));
}

/** Calculates the difference between the begin and end address. */
inline uintptr_t Diff (const void* begin, const void* end) {
    byte* start = static_cast<byte*> (const_cast<void*> (begin)),
        *stop = static_cast<byte*> (const_cast<void*> (end));
    return static_cast<uintptr_t> (stop - start);
}

/** Overwrites the memory with zeros functionally identical to memset.
    @warning Sharp knives: Function used to make L3 compiler warnings stop
             barking on known safe memory blocks. Newbs have been warned. */
KABUKI void MemoryClear (void* address, size_t size);

/** Copies the source to the destination functionally identical to memcpy.
    @warning Sharp knives: Function used to make L3 compiler warnings stop
             barking on known safe memory blocks. Newbs have been warned. */
KABUKI size_t MemoryCopy (void* destination, size_t size, const void* source);

#if USE_MORE_ROM
/** Prints out the contents of the address to the debug stream. */
KABUKI void PrintMemory (const void* address, const void* end);

inline void PrintMemory (const void* address, size_t size) {
    const char* end = reinterpret_cast<const char*>(address) + size;
    PrintMemory (address, end);
}
#endif  //< USE_MORE_ROM

}       //< namespace _

#endif  //< SCRIPT_MEMORY_H

/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/memory.h
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
             permissions and limitations under the License. */

#pragma once
#include <stdafx.h>

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 2

#ifndef HEADER_FOR_CRABS_ALIGN
#define HEADER_FOR_CRABS_ALIGN

#include "config.h"

namespace _ {

/** Compute the next highest power of 2. */
KABUKI uint32_t AlignPowerOf2 (uint32_t value);

/** Compute the next highest power of 2. */
KABUKI int32_t AlignPowerOf2 (int32_t value);

/** Aligns the given pointer up to a sizeof (T) byte boundary.
    @param  ptr The address to align.
    @return The aligned value. */
template<typename WordBoundary, typename I = uintptr_t>
inline I AlignUp (I value) {
    // Algorithm works by inverting the bits, mask of the LSbs and adding 1.
    // This allows the algorithm to word align without any if statements.
    // The algorithm works the same for all memory widths as proven by the
    // truth table bellow.
    //
    // 2-bit example:
    // ~00 = 11 => 00 + 11 + 1 = 0x100
    // ~01 = 10 => 01 + 10 + 1 = 0x100
    // ~10 = 01 => 10 + 01 + 1 = 0x100
    // ~11 = 00 => 11 + 00 + 1 = 0x100
    //
    // 3-bit example:
    // ~000 = 111 => 000 + 111 + 1 = 0x1000
    // ~001 = 110 => 001 + 110 + 1 = 0x1000
    // ~010 = 101 => 010 + 101 + 1 = 0x1000
    // ~011 = 100 => 011 + 100 + 1 = 0x1000
    // ~100 = 011 => 100 + 011 + 1 = 0x1000
    // ~101 = 010 => 101 + 010 + 1 = 0x1000
    // ~110 = 001 => 110 + 001 + 1 = 0x1000
    // ~111 = 000 => 111 + 000 + 1 = 0x1000
    printf ("\nAligning value %u", (uint)value);
    value += ((~value) + 1) & (sizeof (WordBoundary) - 1);
    printf (" with result %u.", (uint)value);
    return value;
}
/*
template<typename WordBoundary = intptr_t, typename T = char>
inline T* AlignUpPointer (void* pointer) {
    uintptr_t ptr = reinterpret_cast<uintptr_t> (pointer);
    return reinterpret_cast<T*> (AlignUp<WordBoundary> (ptr));
}*/

/** Aligns the given pointer to the sizeof (WordBoundary) down..
    @param  value The value to align.
    @return The aligned value. */
template<typename WordBoundary = intptr_t, typename I = uintptr_t>
inline I AlignDown (I value) { //        v--- LSb Mask
    return value & ~(sizeof (WordBoundary) - 1);
}

/** Aligns the given pointer to the sizeof (WordBoundary) down.
    @param  pointer The pointer to align.
    @return The aligned pointer. */
template<typename WordBoundary = intptr_t, typename T = char>
inline T* AlignDownPointer (T* pointer) {
    uintptr_t ptr = reinterpret_cast<uintptr_t> (pointer);
    return reinterpret_cast<T*> (AlignDown<WordBoundary> (ptr));
}

/** Returns the number to add to word-align the given pointer to a uint_t-bit
    boundary.
    @param  ptr The address to align.
    @return The aligned value. */
template<typename T = uintptr_t>
inline T AlignUp2 (T value) {
    return value + value & 0x1;
}

/** Returns the number to add to word-align the given pointer to a uint_t-bit 
    boundary.
    @param  ptr The address to align.
    @return The aligned value. */
template<typename T = uintptr_t>
inline T AlignUp4 (T value) {
    return AlignUp<uint32_t, T> (value);
}

/** Returns the number to add to word-align the given pointer to a uint_t-bit 
    boundary.
    @param  ptr The address to align.
    @return The aligned value. */
template<typename T = uintptr_t>
inline T AlignUp8 (T value) {
    return AlignUp<uint64_t, T> (value);
}

/** Calculates the offset to align the given pointer to a 16-bit word boundary.
    @return A vector you add to a pointer to align it. */
template<typename T = char>
inline T* AlignUpPointer2 (void* pointer) {
    // Mask off lower bit and add it to the ptr.
    uintptr_t ptr = reinterpret_cast<uintptr_t> (pointer);
    return reinterpret_cast<T*> (ptr + ptr & 0x1);
}

/** Calculates the offset to align the given pointer to a 16-bit word boundary.
    @return A vector you add to a pointer to align it. */
template<typename T = char>
inline T* AlignUpPointer2 (const void* pointer) {
    // Mask off lower bit and add it to the ptr.
    uintptr_t ptr = reinterpret_cast<uintptr_t> (pointer);
    return reinterpret_cast<T*> (ptr + ptr & 0x1);
}

/** Aligns the given pointer to a 32-bit word boundary.
    @return A vector you add to a pointer to align it. */
template<typename T = char>
inline T* AlignUpPointer4 (void* pointer) {
    int32_t* test = AlignUpPointer<int32_t> (pointer);
    return reinterpret_cast<T*> (test);
}

/** Aligns the given pointer to a 32-bit word boundary.
    @return A vector you add to a pointer to align it. */
template<typename T = char>
inline T* AlignUpPointer4 (const void* pointer) {
    return reinterpret_cast<const T*> (AlignUpPointer<int32_t> (pointer));
}

/** Aligns the given pointer to a 64-bit word boundary.
    @return A vector you add to a pointer to align it. */
template<typename T = char>
inline T* AlignUpPointer8 (void* pointer) {
    uintptr_t ptr = reinterpret_cast<uintptr_t> (pointer);
    return reinterpret_cast<T*> (AlignUp<int64_t> (ptr));
}

/** Aligns the given pointer to a 64-bit word boundary.
    @return A vector you add to a pointer to align it. */
template<typename T = char>
inline T* AlignUpPointer8 (const void* pointer) {
    uintptr_t ptr = reinterpret_cast<uintptr_t> (pointer);
    return reinterpret_cast<const T*> (AlignUp<int64_t> (ptr));
}

/** Aligns the given pointer to a word boundary.
    @return A vector you add to a pointer to align it. */
template<typename T = char>
inline T* AlignUpPointer (const void* pointer) {
    uintptr_t ptr = reinterpret_cast<uintptr_t> (pointer);
    ptr += ((~ptr) + 1) & (sizeof (T) - 1);
    return reinterpret_cast<T*> (ptr);
}

/** Aligns the given pointer to a word boundary.
    @return A vector you add to a pointer to align it. */
template<typename T = char>
inline T* AlignUpPointer (void* pointer) {
    uintptr_t ptr = reinterpret_cast<uintptr_t> (pointer);
    ptr += ((~ptr) + 1) & (sizeof (T) - 1);
    return reinterpret_cast<T*> (ptr);
}

}       //< namespace _
#endif  //< HEADER_FOR_CRABS_ALIGN

#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 2

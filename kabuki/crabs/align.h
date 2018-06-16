/* Kabuki Toolkit
    @version 0.x
    @file    ~/kabuki-toolkit/kabuki/crabs/memory.h
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
// Dependencies:
#include "config.h"
// End dependencies.

namespace _ {

/* Compute the next highest power of 2. */
KABUKI uint32_t AlignPowerOf2(uint32_t value);

/* Compute the next highest power of 2. */
KABUKI int32_t AlignPowerOf2(int32_t value);

/* Aligns the given pointer up to a sizeof (T) byte boundary.

    Algorithm works by inverting the bits, mask of the LSbs and adding 1.
    This allows the algorithm to word align without any if statements.
    The algorithm works the same for all memory widths as proven by the
    truth tables bellow.

    @param  ptr The address to align.
    @return The aligned value.

    @code
    // The convention KT uses is that the usigned size always omes first
    // becuase it's the first byte of an ASCII Object.
    int32_t signed_example = 7;
    signed_example = AlignUp<int64_t, uint32_t, int32_t> (signed_example);

    uint16_t unsigned_example = 3;
    unsgiend_example = AlignUp<int32_t, uint16_t, uint16_t> (unsigned_example);

    // 64-bit alignment example:
    // ~000 = 111 => 000 + 111 + 1 = 0x1000
    // ~001 = 110 => 001 + 110 + 1 = 0x1000
    // ~010 = 101 => 010 + 101 + 1 = 0x1000
    // ~011 = 100 => 011 + 100 + 1 = 0x1000
    // ~100 = 011 => 100 + 011 + 1 = 0x1000
    // ~101 = 010 => 101 + 010 + 1 = 0x1000
    // ~110 = 001 => 110 + 001 + 1 = 0x1000
    // ~111 = 000 => 111 + 000 + 1 = 0x1000
    // //
    // 32-bit alignment example:
    // ~00 = 11 => 00 + 11 + 1 = 0x100
    // ~01 = 10 => 01 + 10 + 1 = 0x100
    // ~10 = 01 => 10 + 01 + 1 = 0x100
    // ~11 = 00 => 11 + 00 + 1 = 0x100
    // //
    // 16-bit alignment example:
    // ~0 = 1 => 0 + 1 = 1
    // ~1 = 0 => 1 + 0 = 0
    // //
    // 8-bit example:
    // value + ((~value) + 1) & (sizeof (int8_t) - 1) = value
    @endcode */
template <typename WordBoundary, typename UI = uintptr_t,
          typename I = uintptr_t>
inline I AlignUpOffset(I value) {
  UI result = (~(UI)value) + 1;
  return result & (sizeof(WordBoundary) - 1);
}

template <typename WordBoundary, typename UI = uintptr_t, typename I = intptr_t>
inline I AlignUpSigned(I value) {
  UI offset = ((~(UI)value) + 1) & (sizeof(WordBoundary) - 1);
  return value + offset;
}

template <typename WordBoundary, typename UI = uintptr_t>
inline UI AlignUpUnsigned(UI value) {
  UI offset = ((~value) + 1) & (sizeof(WordBoundary) - 1);
  return value + offset;
}

template <typename WordBoundary>
inline uintptr_t AlignUpOffset(const void* value) {
  uintptr_t result = (~reinterpret_cast<uintptr_t>(value)) + 1;
  return result & (sizeof(WordBoundary) - 1);
}

inline uintptr_t AlignUpOffset2(const void* value) {
  return reinterpret_cast<uintptr_t>(value) & 1;
}

inline uintptr_t AlignUpOffset4(const void* value) {
  return ((~reinterpret_cast<uintptr_t>(value)) + 1) & 3;
}

inline uintptr_t AlignUpOffset8(const void* value) {
  return ((~reinterpret_cast<uintptr_t>(value)) + 1) & 7;
}

/* Aligns the given pointer to the sizeof (WordBoundary) down..
    @param  value The value to align.
    @return The aligned value. */
template <typename WordBoundary = intptr_t, typename I = uintptr_t>
inline I AlignDown(I value) {  //        v--- LSb Mask
  return value & ~(sizeof(WordBoundary) - 1);
}

/* Aligns the given pointer to the sizeof (WordBoundary) down.
    @param  pointer The pointer to align.
    @return The aligned pointer. */
template <typename WordBoundary = intptr_t, typename TextWord4 = char>
inline TextWord4* AlignDownPointer(TextWord4* pointer) {
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer);
  return reinterpret_cast<TextWord4*>(AlignDown<WordBoundary>(ptr));
}

/* Calculates the offset to align the given pointer to a 16-bit word boundary.
    @return A vector you add to a pointer to align it. */
template <typename TextWord4 = char>
inline TextWord4* AlignUpPointer2(void* pointer) {
  // Mask off lower bit and add it to the ptr.
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer);
  return reinterpret_cast<TextWord4*>(ptr + ptr & 0x1);
}

/* Calculates the offset to align the given pointer to a 16-bit word boundary.
    @return A vector you add to a pointer to align it. */
template <typename TextWord4 = char>
inline TextWord4* AlignUpPointer2(const void* pointer) {
  // Mask off lower bit and add it to the ptr.
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer);
  return reinterpret_cast<TextWord4*>(ptr + ptr & 0x1);
}

/* Aligns the given pointer to a 32-bit word boundary.
    @return A vector you add to a pointer to align it. */
template <typename TextWord4 = char>
inline TextWord4* AlignUpPointer4(void* pointer) {
  int32_t* test = AlignUpPointer<int32_t>(pointer);
  return reinterpret_cast<TextWord4*>(test);
}

/* Aligns the given pointer to a 32-bit word boundary.
    @return A vector you add to a pointer to align it. */
template <typename TextWord4 = char>
inline TextWord4* AlignUpPointer4(const void* pointer) {
  return reinterpret_cast<const TextWord4*>(AlignUpPointer<int32_t>(pointer));
}

/* Aligns the given pointer to a 64-bit word boundary.
    @return A vector you add to a pointer to align it. */
template <typename TextWord4 = char>
inline TextWord4* AlignUpPointer8(void* pointer) {
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer);
  return reinterpret_cast<TextWord4*>(AlignUpUnsigned<int64_t>(ptr));
}

/* Aligns the given pointer to a 64-bit word boundary.
    @return A vector you add to a pointer to align it. */
template <typename TextWord4 = char>
inline TextWord4* AlignUpPointer8(const void* pointer) {
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer);
  return reinterpret_cast<const TextWord4*>(AlignUpUnsigned<int64_t>(ptr));
}

/* Aligns the given pointer to a word boundary.
    @return A vector you add to a pointer to align it. */
template <typename TextWord4 = char>
inline TextWord4* AlignUpPointer(const void* pointer) {
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer);
  ptr += ((~ptr) + 1) & (sizeof(TextWord4) - 1);
  return reinterpret_cast<TextWord4*>(ptr);
}

/* Aligns the given pointer to a word boundary.
    @return A vector you add to a pointer to align it. */
template <typename TextWord4 = char>
inline TextWord4* AlignUpPointer(void* pointer) {
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer);
  ptr += ((~ptr) + 1) & (sizeof(TextWord4) - 1);
  return reinterpret_cast<TextWord4*>(ptr);
}

}       //< namespace _ {
#endif  //< HEADER_FOR_CRABS_ALIGN

#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 2

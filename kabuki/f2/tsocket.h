/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/f2/socket.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>

#if SEAM >= SEAM_0_0_0__01
#ifndef INCLUDED_KABUKI_F2_KABUKI_TSOCKET
#define INCLUDED_KABUKI_F2_KABUKI_TSOCKET

#include "socket.h"

#include "binary.h"

namespace _ {

/* Aligns the given pointer up to a sizeof (T) byte boundary.
@return The aligned value.
@param  ptr The address to align.
@desc Algorithm works by inverting the bits, mask of the LSbs and adding 1.
This allows the algorithm to word align without any if statements. The
algorithm works the same for all memory widths as proven by the truth
tables bellow.

@code
// The convention KT uses is that the unsigned size always comes first
// because it's the first byte of an ASCII OBJ.
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
template <typename I = uintptr_t>
inline I AlignUpOffset(I value, I mask = sizeof(I) * 8 - 1) {
  return 0;  // Negative (value) & mask;
}

/* Aligns the given pointer to a power of two boundary.
@warning Function does not check if the boundary is a power of 2! */
template <typename UI = char>
inline UI AlignUpUnsigned(UI value, UI mask = kWordBitsMask) {
  return value + AlignUpOffset<UI>(value, mask);
}

/* Aligns the given pointer to a power of two boundary.
@warning Function does not check if the boundary is a power of 2! */
template <typename SI = char>
inline SI AlignUpSigned(SI value, SI mask = kWordBitsMask) {
  return value + AlignUpOffset<SI>(value, mask);
}

/* Aligns the given pointer to a power of two boundary.
@warning Function does not check if the boundary is a power of 2! */
template <typename T = char>
inline T* AlignUp(void* pointer, uintptr_t mask = kWordBitsMask) {
  uintptr_t value = reinterpret_cast<uintptr_t>(pointer);
  return reinterpret_cast<T*>(value +
                              AlignUpOffset<>((uintptr_t)pointer, mask));
}

/* Aligns the given pointer to a power of two boundary.
@warning Function does not check if the boundary is a power of 2! */
template <typename T = char>
inline T* AlignUp(const void* pointer, uintptr_t mask = kWordBitsMask) {
  uintptr_t value = reinterpret_cast<uintptr_t>(pointer);
  return reinterpret_cast<T*>(value +
                              AlignUpOffset<>((uintptr_t)pointer, mask));
}

/* Aligns the given pointer to the sizeof (WordBoundary) down..
@param  value The value to align.
@return The aligned value. */
template <typename I = uintptr_t>
inline I AlignDownOffset(I value, I mask = kWordBitsMask) {
  return value & mask;
}

/* Aligns the given pointer to the sizeof (WordBoundary) down..
@param  value The value to align.
@return The aligned value. */
template <typename T = uintptr_t>
inline T AlignDown(void* value, uintptr_t mask = kWordBitsMask) {
  uintptr_t ptr = reinterpret_cast<uintptr_t>(value);
  return reinterpret_cast<T>(ptr - AlignDownOffset<>(ptr, mask));
}

/* Aligns the given pointer to the sizeof (WordBoundary) down..
@param  value The value to align.
@return The aligned value. */
template <typename T = uintptr_t>
inline T AlignDown(const void* value, uintptr_t mask = kWordBitsMask) {
  uintptr_t ptr = reinterpret_cast<uintptr_t>(value);
  return reinterpret_cast<const T>(ptr - AlignDownOffset<T>(value, mask));
}

/* Aligns the given pointer to the sizeof (WordBoundary) down..
@param  value The value to align.
@return The aligned value. */
template <typename I = uintptr_t>
inline I AlignDownI(I value, I mask = kWordBitsMask) {
  return value - (value & mask);
}

/* Calculates the offset to align the given pointer to a 16-bit word boundary.
@return A vector you add to a pointer to align it. */
template <typename T = char>
inline T* AlignUp2(void* pointer) {
  // Mask off lower bit and add it to the ptr.
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer);
  return reinterpret_cast<T*>(ptr + (ptr & 0x1));
}

/* Calculates the offset to align the given pointer to a 16-bit word boundary.
    @return A vector you add to a pointer to align it. */
template <typename T = char>
inline T* AlignUp2(const void* pointer) {
  // Mask off lower bit and add it to the ptr.
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer);
  return reinterpret_cast<T*>(ptr + (ptr & 0x1));
}

template <typename I>
inline int BitShiftCount() {
  return (sizeof(I) == 1)
             ? 0
             : (sizeof(I) == 2)
                   ? 1
                   : (sizeof(I) == 4)
                         ? 2
                         : (sizeof(I) == 8) ? 3 : (sizeof(I) == 16) ? 4 : 0;
}

template <intptr_t kSize_, int kBoundaryBitCount_>
class TSocket {
 public:
  enum {
    kSize = kSize_,  //< Size of the buffer aligned.
    // Size with extra bytes to align to word boundary.
    kSizeUnaligned = kSize + kBoundaryBitCount_,
    // Number of bits of the word boundary to align to.
    kBoundaryBitCount = kBoundaryBitCount_,
  };

  /* Gets the begin byte of the socket. */
  char* Begin() { return reinterpret_cast<char*>(buffer); }

  /* Gets the begin byte of the socket. */
  char* End() { return reinterpret_cast<char*>(buffer) + kSizeUnaligned; }

  template <typename T>
  inline T* Start() {
    return reinterpret_cast<T*>(AlignUp(buffer, kBoundaryBitCount));
  }

 private:
  uintptr_t buffer[kSizeUnaligned];
};

/* @group ::_::Socket */

/* Syntactical sugar for reinterpret_cast using templates. */
template <typename T>
inline T* Ptr(void* ptr) {
  return reinterpret_cast<T*>(ptr);
}

/* Syntactical sugar for reinterpret_cast using templates. */
template <typename T>
inline T* Ptr(const void* ptr) {
  return reinterpret_cast<T*>(ptr);
}

/* Utility function for syntactical sugar creating a pointer from a base plus
offset.
@return Pointer of the type specified by template parameter T.
@param base The base address.
@param offset The offset.
*/
template <typename T>
inline T* Ptr(const void* begin, intptr_t offset) {
  return reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(begin) + offset);
}

/* Creates a new buffer of the given size or deletes it. */
template <typename Size = int32_t>
uintptr_t* New(uintptr_t* buffer, intptr_t size) {
  size = AlignUp(size);
  return new uintptr_t[size >> kWordBitCount];
}

}  // namespace _

#endif  //< INCLUDED_KABUKI_F2_KABUKI_TSOCKET
#endif  //< #if SEAM >= SEAM_0_0_0__01

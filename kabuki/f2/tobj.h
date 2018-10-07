/* Kabuki Toolkit
@version 0.x
@file    /kabuki/f2/obj.h
@author  Cale McCollough <cale.mccollough@gmail.com>
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
#if SEAM >= SEAM_0_0_0__03
#ifndef INCLUDED_KABUKI_F2_TOBJ
#define INCLUDED_KABUKI_F2_TOBJ

#include "../f2/test.h"
#include "obj.h"

namespace _ {

template <typename SI>
inline SI ObjCountMax() {
  SI count_max = 0;
  return (~count_max) - 7;
}

template <typename SI>
inline bool ObjSizeIsValid(SI size, SI size_min) {
  return (size >= size_min) && (size <= ObjCountMax<SI>());
}

template <typename SI>
inline bool ObjCountIsValid(SI index, SI count_min) {
  return (index >= count_min) && (index < ObjCountMax<SI>());
}

/* Returns the ASCII OBJ size. */
template <typename Size>
inline Size ObjSize(uintptr_t* object) {
  ASSERT(object);
  return *reinterpret_cast<Size*>(object);
}

/* Returns the ASCII OBJ size. */
template <typename Size>
inline Size ObjSize(CObj obj) {
  ASSERT(obj);
  ASSERT(obj.begin);
  return *reinterpret_cast<Size*>(obj.begin);
}

template <typename Size>
uintptr_t* ObjNew(Size size, Size size_min) {
  if (size < size_min) return nullptr;

  if (!ObjSizeIsValid<Size>(size, size_min)) return nullptr;

  size = AlignUp<8, uint16_t, int16_t>(size);
  uintptr_t* buffer = new uintptr_t[size >> kWordBitCount];
  *reinterpret_cast<int16_t*>(buffer) = size;
  return buffer;
}

/* Clones the other ASCII OBJ. */
template <typename Size = int32_t>
uintptr_t* ObjClone(uintptr_t* buffer, Size size) {
  ASSERT(buffer);
  uintptr_t* clone = new uintptr_t[size >> kWordBitCount];
  // SocketCopy(clone, size, buffer, size);
  *reinterpret_cast<Size*>(buffer) = size;
  return clone;
}

template <typename T, typename UI = uint, typename SI = int>
inline SI ObjCountRound(SI count) {
  enum {
    kRoundEpochMask = (sizeof(SI) == 8)
                          ? 7
                          : (sizeof(SI) == 4) ? 3 : (sizeof(SI) == 2) ? 1 : 0,
  };
  SI count_aligned = AlignUpSigned<SI>(count);
}

/* A word-aligned ASCII OBJ with a size that is a positive integer multiple
of 8.

ASCII Objects may only use 16-bit, 32-bit, and 64-bit signed integers for their
size. The minimum and maximum bounds of size of ASCII objects are defined by the
minimum size required to store the header with minimum item count, and the
highest positive integer multiple of 8. The fastest way to covert the upper
bounds is to invert the bits and subtract 7 as follows:

@code
int16_t upper_bounds_si2 = (~(int16_t)0) - 7;
int32_t upper_bounds_si4 = (~(int32_t)0) - 7;
int64_t upper_bounds_si8 = (~(int64_t)0) - 7;
@endcode
*/
template <typename Size>
class TObj {
 public:
  /* Constructs a buffer with either statically or dynamically allocated memory
  based on if buffer is nil. */
  TObj() : Buffer(size, buffer), destructor(nullptr) {
    // Nothing to do here! ({:-)-/==<
  }

  /* Constructs a buffer with either statically or dynamically allocated memory
  based on if buffer is nil. */
  TObj(Destructor destructor) : Buffer(size, buffer), destructor(destructor) {
    // Nothing to do here! ({:-)-/==<
  }

  /* Constructs a buffer with either statically or dynamically allocated memory
  based on if buffer is nil. */
  TObj(Size size, Destructor destructor = nullptr) : Buffer(size, buffer) {}

  /* Constructs a buffer with either statically or dynamically allocated memory
  based on if buffer is nil. */
  TObj(Size size, uintptr_t* buffer, Destructor destructor = nullptr)
      : Buffer(size, buffer) {}

  /* Destructor deletes dynamic memory if is_dynamic_ is true. */
  ~TObj() { Destruct(obj_); }

  /* Returns the buffer_. */
  uintptr_t* Begin() { return obj_.begin; }

  /* Returns the buffer_. */
  uintptr_t* GetStart() { return obj_.begin; }

  char* GetStop() {
    Size size = ObjSize<Size>(begin);
    return reinterpret_cast<char*>(obj_.begin) + size - 1;
  }

  /* Gets the size_. */
  Size GetSize() { return ObjSize<Size>(begin); }

  /* Doubles the size of the buffer and copies the given byte_count.
  @return A positive size of the new buffer upon success and -1 upon failure.
  @param byte_count The number of bytes to copy after growing the buffer. */
  Size Grow(Size new_size) { return ObjGrow<Size>(obj_.begin, new_size); }

  inline CObj& Obj() { return begin; }

 private:
  CObj obj_;  //< ASCII OBJ harness.
};

}  // namespace _
#endif  //< #if SEAM >= SEAM_0_0_0__03
#endif  //< INCLUDED_KABUKI_F2_TOBJ

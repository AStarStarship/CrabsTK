/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/object.h
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
#include <stdafx.h>
#if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 0
#ifndef HEADER_FOR_CRABS_OBJECT
#define HEADER_FOR_CRABS_OBJECT
// Dependencies:
#include "align.h"
#include "assert.h"
// End dependencies.

typedef void (*Destructor)(uintptr_t* buffer);

namespace _ {

typedef void (*Destructor)(uintptr_t* buffer);

/* Deletes the buffer. */
KABUKI void Delete(uintptr_t* buffer);

/* C header for an ASCII Object with programmable destructor. */
struct CObj {
  uintptr_t* ascii_obj_;   //< Pointer to the contiguous ASCII Object.
  Destructor destructor_;  //< Pointer to the destructor.
};

/* Deletes an ASCII object. */
KABUKI void ObjectDelete(CObj object);

/* Checks if the value is a valid object index, that it's 7 less than the max
value or less. */
inline KABUKI bool ObjectCountIsValid(int8_t value, int8_t count_min = 1);

/* Checks if the value is a valid object index, that it's 7 less than the max
value or less. */
inline KABUKI bool ObjectCountIsValid(int16_t value, int16_t count_min = 1);

/* Checks if the value is a valid index, that it's 7 less than the max
value or less. */
inline KABUKI bool ObjectCountIsValid(int32_t value, int32_t count_min = 1);

/* Checks if the value is a valid index, that it's 7 less than the max
value or less. */
inline KABUKI bool ObjectCountIsValid(int64_t value, int64_t count_min = 1);

/* Checks if the value is a valid object size, that it's an even multiple of
8. */
inline KABUKI bool ObjectSizeIsValid(int16_t value, int16_t count_min = 1);

/* Checks if the value is a valid object size, that it's an even multiple of
8. */
inline KABUKI bool ObjectSizeIsValid(int32_t value, int32_t count_min = 1);

/* Checks if the value is a valid object size, that it's an even multiple of
8. */
inline KABUKI bool ObjectSizeIsValid(int64_t value, int64_t count_min = 1);

/* Clones the given ASCII Object. */
KABUKI uintptr_t* ObjectClone(uintptr_t* ascii_object, int8_t size);

/* Clones the given ASCII Object. */
KABUKI uintptr_t* ObjectClone(uintptr_t* ascii_object, int16_t size);

/* Clones the given ASCII Object. */
KABUKI uintptr_t* ObjectClone(uintptr_t* ascii_object, int32_t size);

/* Clones the given ASCII Object. */
KABUKI uintptr_t* ObjectClone(uintptr_t* ascii_object, int64_t size);

template <typename SI>
inline bool ObjectSizeIsValid(SI size, SI size_min) {
  SI bounds_upper = 0;
  bounds_upper = (~bounds_upper) - 7;
  return (size >= size_min) && (size <= bounds_upper);
}

template <typename SI>
inline bool ObjectCountIsValid(SI index, SI count_min) {
  return (index >= count_min) && ((size & 7) == 0) && ((size >> 3) == 0);
}

/* Returns the ASCII Object size. */
template <typename Size>
inline Size ObjectSize(const void* object) {
  ASSERT(object);
  return *reinterpret_cast<Size*>(object);
}

template <typename Size>
uintptr_t* ObjectNew(Size size, Size size_min) {
  if (size < size_min) return nullptr;

  if (!ObjectSizeIsValid<Size>(size, size_min)) return nullptr;

  size = AlignUp<8, uint16_t, int16_t>(size);
  uintptr_t* buffer = new uintptr_t[size >> kWordBitCount];
  *reinterpret_cast<int16_t*>(buffer) = size;
  return buffer;
}

/* Clones the other ASCII Object. */
template <typename Size = int32_t>
uintptr_t* ObjectClone(uintptr_t* buffer, Size size) {
  ASSERT(buffer);
  uintptr_t* clone = new uintptr_t[size >> kWordBitCount];
  SocketCopy(clone, size, buffer, size);
  *reinterpret_cast<Size*>(buffer) = size;
  return clone;
}

template <typename T, typename UI = uint, typename SI = int>
inline SI ObjectCountRound(SI count) {
  enum {
    kRoundEpochMask = (sizeof(SI) == 8)
                          ? 7
                          : (sizeof(SI) == 4) ? 3 : (sizeof(SI) == 2) ? 1 : 0,
  };
  SI count_aligned = AlignUpSigned<SI>(count);
}

/* A word-aligned ASCII Object with a size that is a positive integer multiple
of 8.
ASCII Objects may only use 16-bit, 32-bit, and 64-bit signed integers for their
size. The minimum and maximum bounds of size of ASCII objects are defined by the
minimu size required to store the header with minimum item count, and the
highest positive integer multiple of 8. The fastest way to covert the upper
bounds is to invert the bits and subtract 7 as follows:

@code
int16_t upper_bounds_si2 = (~(int16_t)0) - 7;
int32_t upper_bounds_si4 = (~(int32_t)0) - 7;
int64_t upper_bounds_si8 = (~(int64_t)0) - 7;
@endcode
*/
template <typename Size>
class KABUKI Obj {
 public:
  /* Constructs a buffer with either stically or dynamically allocated memory
  based on if buffer is nil. */
  Obj(Destructor destructor = nullptr)
      : Buffer(size, buffer), destructor_(destructor) {
    // Nothing to do here! ({:-)-/==<
  }

  /* Constructs a buffer with either stically or dynamically allocated memory
  based on if buffer is nil. */
  Obj(Size size, Destructor destructor = nullptr) : Buffer(size, buffer) {}

  /* Constructs a buffer with either stically or dynamically allocated memory
  based on if buffer is nil. */
  Obj(Size size, uintptr_t* buffer, Destructor destructor = nullptr)
      : Buffer(size, buffer) {}

  /* Destructor deletes dynamic memory if is_dynamic_ is true. */
  ~Obj() {
    if (destructor_) destructor_(ascii_obj_, size_);
  }

  /* Returns the buffer_. */
  uintptr_t* GetBuffer() { return ascii_obj_.ascii_obj_; }

  /* Gets the size_. */
  Size GetSize() { return ObjectSize<Size>(ascii_obj_.GetBuffer()); }

  /* Gets the buffer as a Socket. */
  Socket GetSocket() { return ObjectSocket<Size>(Object()); }

  /* Doubles the size of the buffer and copyes the given byte_count.
  @return A positive size of the new buffer upon success and -1 upon failure.
  @param byte_count The number of bytes to copy after growing the buffer. */
  Size Grow(Size new_size) {
    return ObjectGrow<Size>(ascii_obj_.ascii_obj_, new_size);
  }

  inline CObj& Object() { return ascii_obj_; }

 private:
  CObj ascii_obj_;  //< ASCII Object header.
};

}  // namespace _
#endif  //< #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 0
#endif  //< HEADER_FOR_CRABS_OBJECT

/* Kabuki Toolkit
@version 0.x
@file    ~/kabuki-toolkit/kabuki/crabs/memory.h
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
#include <stdafx.h>
#if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2
#ifndef HEADER_FOR_CRABS_MEMORY
#define HEADER_FOR_CRABS_MEMORY
// Dependencies:
#include "align.h"
// End dependencies.

namespace _ {

/* A managed general purpose (i.e. not just for networking) memory socket.
    A socket is just a hole in something for fitting something in, like a light
    or electric socket. A socket of memory is just a block of memory you fit
    something in. A network socket is a socket for interprocess communication,
    which is usually implemented with a ring buffer
    (@see ~/kabuki-toolkit/kabuki/crabs/slot.h).
*/
struct Socket {
  char *begin,  //< Begin of the socket.
      *end;     //< End of the socket.

  /* Constructs an uninitialized socket. */
  Socket();

  /* Constructor. */
  Socket(void* begin, void* end);

  /* Constructor. */
  Socket(void* begin, intptr_t size);

  /* Clones the other memory. */
  Socket(const Socket& other);

  /* C++ operator= overload. */
  Socket& operator=(const Socket& other);
};

typedef void (*Destructor)(uintptr_t* buffer, intptr_t size);

inline KABUKI void DestructorNoOp(uintptr_t* buffer, intptr_t size);

inline KABUKI void DestructorDeleteBuffer(uintptr_t* buffer, intptr_t size);

/* A word-aligned ASCII Object with a size that is a positive integer multiple
of 8.
All ASCII Objects have to be ASCII Objects may only use 16-bit, 32-bit, and
64-bit signed integers for their size. The minimum and maximum bounds of size of
ASCII objects are defined by the minimu size required to store the header with
minimum item count, and the highest positive integer multiple of 8. The fastest
way to covert the upper bounds is to invert the bits and subtract 7 as follows:

@code
int16_t upper_bounds_si2 = (~(int16_t)0) - 7;
int32_t upper_bounds_si4 = (~(int32_t)0) - 7;
int64_t upper_bounds_si8 = (~(int64_t)0) - 7;
@endcode
*/
template <typename Size>
class KABUKI TObject {
 public:
  /* Constructs a buffer with either stically or dynamically allocated memory
  based on if buffer is nil. */
  TObject(Size size, uintptr_t* buffer = nullptr) : Buffer(size, buffer) {}

  /* Destructor deletes dynamic memory if is_dynamic_ is true. */
  ~TObject() {
    if (destruct_) destruct_(object_, size_);
  }

  /* Returns the buffer_. */
  uintptr_t* GetBuffer() {
    ASSERT(size > 0);
    size_ = size;
    if (!(size & kWordBitCount)) {
      size = size >> kWordBitCount;
    } else {
      size = (size >> kWordBitCount) + 1;
    }
    if (!buffer) {
      buffer = new uintptr_t[size];
    }
    object_ = buffer;
  }

  /* Gets is_dynamic_. */
  bool IsDynamic() { return destruct_ != &BufferDestructorNoOp; }

  /* Gets the size_. */
  Size GetSize() { return *reinterpret_cast<SI*>(object_); }

  /* Gets the buffer as a Socket. */
  Socket GetSocket() { return object_; }

  /* Doubles the size of the buffer and copyes the given byte_count.
  @return A positive size of the new buffer upon success and -1 upon failure.
  @param byte_count The number of bytes to copy after growing the buffer. */
  Size Grow(Size byte_count);

  /* Shrinks the buffer to the given size and if the memory is dynamic copies
  it to a new heap block.
  @return A positive size of the new buffer size upon success and -1 upon
  failure.
  @param size The size in bytes. */
  Size Shrink(Size size) {
    if (size < 0 || size > SignedMax<Size>()) return -1;
  }

  /* Shrinks the buffer to the given size without copying it.
  @return A positive size of the new buffer size upon success and -1 upon
  failure.
  @param size The size in bytes. */
  Size ShrinkInPlace(Size size);

 private:
  uintptr_t* object_;    //< Pointer to the bBuffer.
  Destructor destruct_;  //< Pointer to the destructor.
};

/* Creates a dynamic word-aligned buffer of the given size in bytes. */
KABUKI uintptr_t* MemoryCreate(uintptr_t size);

/* Destroys a dynamic buffer. */
KABUKI void MemoryDestroy(uintptr_t* buffer);

/* Returns the given pointer for use with uncooperative overloaded
    operators. */
inline const void* Ptr(const void* pointer) { return pointer; }

/* Converts the pointer to a std::uintptr_t. */
inline std::uintptr_t MemoryValue(const void* value) {
  return reinterpret_cast<std::uintptr_t>(value);
}

/* Converts the std::uintptr_t to a pointer. */
inline void* MemoryValue(std::uintptr_t value) {
  return reinterpret_cast<void*>(value);
}

/* Converts the std::uintptr_t to a pointer. */
inline const void* MemoryValueConst(std::uintptr_t value) {
  return reinterpret_cast<const void*>(value);
}

inline char* MemoryAdd(void* a, void* b) {
  return (char*)((uintptr_t)a + (uintptr_t)b);
}

/* Calculates the difference between the begin and end address. */
inline intptr_t MemoryVector(void* begin, void* end) {
  return reinterpret_cast<char*>(end) - reinterpret_cast<char*>(begin);
}

/* Calculates the difference between the begin and end address. */
inline intptr_t MemoryVector(const void* begin, const void* end) {
  return reinterpret_cast<const char*>(end) -
         reinterpret_cast<const char*>(begin);
}

/* Overwrites the memory with zeros functionally identical to memset. */
KABUKI void MemoryClear(void* address, size_t size);

/* Copies the source to the target functionally identical to memcpy.
    @param  begin The begin of the write buffer.
    @param  end   The end of the write buffer.
    @param  start The start of the read buffer.
    @param  stop  The stop of the read buffer.
    @return Pointer to the last byte written or nil upon failure. */
KABUKI char* MemoryCopy(void* begin, void* end, const void* start,
                        const void* stop);

/* Copies the source to the target functionally identical to memcpy.
    @param  begin The begin of the write buffer.
    @param  end   The end of the write buffer.
    @param  start The start of the read buffer.
    @param  size      Number of bytes to copy.
    @return Pointer to the last byte written or nil upon failure. */
inline char* MemoryCopy(void* begin, void* end, const void* start,
                        uintptr_t read_size) {
  return MemoryCopy(begin, end, start,
                    reinterpret_cast<const char*>(start) + read_size);
}

inline char* MemoryCopy(void* begin, size_t write_size, const void* read,
                        uintptr_t read_size) {
  return MemoryCopy(begin, reinterpret_cast<char*>(begin) + write_size, read,
                    reinterpret_cast<const char*>(read) + read_size);
}

/* Compares the two memory sockets.
    @param  begin The beginning of buffer a.
    @param  end   The end of buffer a.
    @param  start The start of buffer b.
    @param  stop  The stop of buffer b.
    @return True if they are the same and false if they are not. */
KABUKI bool MemoryCompare(const void* begin, const void* end, const void* start,
                          const void* stop);

inline bool MemoryCompare(void* begin, void* end, const void* read,
                          uintptr_t read_size) {
  return MemoryCompare(begin, end, read,
                       reinterpret_cast<const char*>(read) + read_size);
}

inline bool MemoryCompare(void* begin, size_t write_size, const void* read,
                          uintptr_t read_size) {
  return MemoryCompare(begin, reinterpret_cast<char*>(begin) + write_size, read,
                       reinterpret_cast<const char*>(read) + read_size);
}

/* Checks if the given value is Not-a-Number.
@param value
*/
template <typename T>
inline bool MemoryIsNaN(T value) {
  static const T nan =
      (sizeof(T) == 8)
          ? (T)0xFF
          : sizeof(T) == 4
                ? (T)0xFFFF
                : sizeof(T) == 2 ? (T)0xFFFFFFFF
                                 : sizeof(T) == 1 ? (T)0xFFFFFFFFFFFFFFFF : 0;
  return value == nan;
}

/* Returns the inverse of the given value.
For code metadata purposes. */
template <typename UI>
inline UI UnsignedMax() {
  UI max = 0;
  return ~max;
}

/* reinterpret_cast(string) a the given base and offset to an object pointer.
 */
template <typename T>
inline T* MemoryOffset(void* base, uint_t offset) {
  return reinterpret_cast<T*>(reinterpret_cast<char*>(base) + offset);
}

}  // namespace _

#endif  //< HEADER_FOR_CRABS_MEMORY
#endif  //< #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2

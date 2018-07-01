/* Kabuki Toolkit
@version 0.x
@file    ~/kabuki-toolkit/kabuki/crabs/socket.h
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
#include "number.h"
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
  char *begin,  //< Beginning of the socket.
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

/* Converts the pointer to a std::uintptr_t. */
inline KABUKI uintptr_t UIntPtr(const void* value);

/* Converts the std::uintptr_t to a pointer. */
inline KABUKI void* VoidPtr(uintptr_t value);

/* Converts the std::uintptr_t to a pointer. */
inline KABUKI const void* ConstVoidPtr(uintptr_t value);

/* Calculates the difference between the begin and end address. */
inline KABUKI intptr_t SocketSize(const void* begin, const void* end);

/* Overwrites the memory with zeros functionally identical to memset. */
KABUKI char* SocketClear(char* begin, char* end, intptr_t size);

/* Overwrites the memory with zeros functionally identical to memset.
@return False upon failure. */
KABUKI bool SocketWipe(void* begin, void* end, intptr_t size);

inline KABUKI char* SocketCopy(void* begin, intptr_t size, const void* read,
                               intptr_t read_size);

/* Copies the source to the target functionally identical to memcpy.
    @param  begin The begin of the write buffer.
    @param  end   The end of the write buffer.
    @param  start The start of the read buffer.
    @param  stop  The stop of the read buffer.
    @return Pointer to the last byte written or nil upon failure. */
inline KABUKI char* SocketCopy(void* begin, void* end, const void* start,
                               const void* stop);

/* Copies the source to the target functionally identical to memcpy.
    @param  begin The begin of the write buffer.
    @param  end   The end of the write buffer.
    @param  start The start of the read buffer.
    @param  size      Number of bytes to copy.
    @return Pointer to the last byte written or nil upon failure. */
inline KABUKI char* SocketCopy(void* begin, void* end, const void* start,
                               intptr_t read_size);

/* Compares the two memory sockets.
    @param  begin The beginning of buffer a.
    @param  end   The end of buffer a.
    @param  start The start of buffer b.
    @param  stop  The stop of buffer b.
    @return True if they are the same and false if they are not. */
KABUKI bool SocketCompare(const void* begin, const void* end, const void* start,
                          const void* stop);

inline KABUKI bool SocketCompare(void* begin, void* end, const void* read,
                                 uintptr_t read_size);

inline KABUKI bool SocketCompare(void* begin, size_t write_size,
                                 const void* read, uintptr_t read_size);

template <typename T>
inline T* Ptr(const void* ptr) {
  return reinterpret_cast<T*>(ptr);
}

/* Utility function for syntactical sugar creating a pointer from a base plus
offset.
@return Pointer of the type specified by template paramter T.
@param base The base address.
@param offset The offset.
*/
template <typename T>
inline T* Ptr(const void* base, intptr_t offset) {
  return reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(base) + offset);
}

}  // namespace _

#endif  //< HEADER_FOR_CRABS_MEMORY
#endif  //< #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2

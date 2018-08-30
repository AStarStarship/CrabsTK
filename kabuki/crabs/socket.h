/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit
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
#if SEAM >= SEAM_0_0_1
#ifndef INCLUDED_CRABS_MEMORY
#define INCLUDED_CRABS_MEMORY
// Dependencies:
#include "align.h"
#include "binary_64.h"
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

/* Creates a block of dynamic memory. */
KABUKI inline uintptr_t* New(intptr_t size);

/* Converts the pointer to a std::uintptr_t. */
KABUKI inline uintptr_t UIntPtr(const void* value);

/* Converts the std::uintptr_t to a pointer. */
KABUKI inline void* VoidPtr(uintptr_t value);

/* Converts the std::uintptr_t to a pointer. */
KABUKI inline const void* ConstVoidPtr(uintptr_t value);

/* Calculates the difference between the begin and end address. */
KABUKI inline intptr_t SizeOf(const void* begin, const void* end);

/* Overwrites the memory with zeros functionally identical to memset. */
KABUKI char* SocketClear(char* begin, char* end, intptr_t size);

/* Overwrites the memory with zeros functionally identical to memset. */
KABUKI char* SocketClear(void* begin, intptr_t size, intptr_t byte_count);

/* Overwrites the memory with zeros functionally identical to memset.
@return False upon failure. */
KABUKI bool SocketWipe(void* begin, void* end, intptr_t size);

KABUKI char* SocketCopy(void* begin, intptr_t size, const void* read,
                        intptr_t read_size);

/* Copies the source to the target functionally identical to memcpy.
@param  begin The begin of the write buffer.
@param  end   The end of the write buffer.
@param  start The start of the read buffer.
@param  size      Number of bytes to copy.
@return Pointer to the last byte written or nil upon failure. */
KABUKI inline char* SocketCopy(void* begin, void* end, const void* start,
                               intptr_t read_size);

/* Copies the source to the target functionally identical to memcpy.
    @param  begin The begin of the write buffer.
    @param  end   The end of the write buffer.
    @param  start The start of the read buffer.
    @param  stop  The stop of the read buffer.
    @return Pointer to the last byte written or nil upon failure. */
KABUKI inline char* SocketCopy(void* begin, void* end, const void* start,
                               const void* stop);

/* Compares the two memory sockets.
    @param  begin_a The beginning of Socket A.
    @param  end_a   The end of Socket A.
    @param  begin_b The start of Socket B.
    @param  stop_b  The stop of Socket B.
    @return True if they are the same and false if they are not. */
KABUKI bool SocketCompare(const void* begin, const void* end, const void* start,
                          const void* stop);

/* Compares the two memory sockets.
@param  begin_a The beginning of Socket A.
@param  end_a   The end of Socket A.
@param  begin_a The start of Socket B.
@param  size_b  The size of Socket B.
@return True if they are the same and false if they are not. */
KABUKI inline bool SocketCompare(const void* begin_a, void* end_a,
                                 const void* begin_b, intptr_t read_size);

/* Compares the two memory sockets.
@param  begin_a The beginning of buffer a.
@param  size_a  The size of Socket A .
@param  begin_a The start of buffer b.
@param  size_b  The size of Socket B.
@return True if they are the same and false if they are not. */
KABUKI inline bool SocketCompare(const void* begin_a, intptr_t size_a,
                                 const void* begin_b, intptr_t size_b);

}  // namespace _

#endif  //< INCLUDED_CRABS_MEMORY
#endif  //< #if SEAM >= SEAM_0_0_1

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

#ifndef INCLUDED_KABUKI_F2_KABUKI_SOCKET
#define INCLUDED_KABUKI_F2_KABUKI_SOCKET

namespace _ {

enum {
  kCpuCacheLineSize = 64,
  kWordBitCount = sizeof(void*) == 8 ? 3 : sizeof(void*) == 4 ? 2 : 0
};

/* Aligns the given buffer pointer up to a cache line boundary (64 bytes). */
API inline uintptr_t* AlignUp(uintptr_t* buffer,
                              uintptr_t mask = kWordBitsMask);

/* Aligns the given pointer up to a word boundary. */
API char* AlignUp(char* pointer, uintptr_t mask = sizeof(void*));

/* Aligns the given pointer up to a word boundary. */
API const char* AlignUp(const char* pointer, uintptr_t mask = sizeof(void*));

/* Aligns the given value up to an 8-byte boundary. */
API inline uint8_t AlignUp(uint8_t value, uint8_t mask = kWordBitsMask);

/* Aligns the given value up to an 8-byte boundary. */
API inline int8_t AlignUp(int8_t value, int8_t mask = kWordBitsMask);

/* Aligns the given value up to an 16-byte boundary. */
API uint16_t AlignUp(uint16_t value, uint16_t mask = kWordBitsMask);

/* Aligns the given value up to an 16-byte boundary. */
API int16_t AlignUp(int16_t value, int16_t mask = kWordBitsMask);

/* Aligns the given value up to an 32-byte boundary. */
API inline uint32_t AlignUp(uint32_t value, uint32_t mask = kWordBitsMask);

/* Aligns the given value up to an 32-byte boundary. */
API inline int32_t AlignUp(int32_t value, int32_t mask = kWordBitsMask);

/* Aligns the given value up to an 64-byte boundary. */
API inline uint64_t AlignUp(uint64_t value, uint64_t mask = kWordBitsMask);

/* Aligns the given value up to an 64-byte boundary. */
API inline int64_t AlignUp(int64_t value, int64_t mask = kWordBitsMask);

/* A managed general purpose (i.e. not just for networking) memory socket.
    A socket is just a hole in something for fitting something in, like a light
    or electric socket. A socket of memory is just a block of memory you fit
    something in. A network socket is a socket for interprocess communication,
    which is usually implemented with a ring buffer
    (@see ~/kabuki-toolkit/kabuki/f2/slot.h).
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
API inline uintptr_t* New(intptr_t size);

/* Destructor deletes the socket. */
API inline void Delete(uintptr_t* socket);

/* Converts the pointer to a std::uintptr_t. */
API inline uintptr_t UIntPtr(const void* value);

/* Converts the std::uintptr_t to a pointer. */
API inline void* VoidPtr(uintptr_t value);

/* Converts the std::uintptr_t to a pointer. */
API inline const void* ConstVoidPtr(uintptr_t value);

/* Calculates the difference between the begin and end address. */
API inline intptr_t SizeOf(const void* begin, const void* end);

/* Overwrites the memory with fill_char; functionally identical to memset. */
API char* SocketFill(char* begin, char* end, intptr_t size, char fill_char = 0);

/* Overwrites the memory with fill_char; functionally identical to memset. */
API char* SocketFill(void* begin, intptr_t size, char fill_char = 0);

/* Copies the source to the target functionally identical to memcpy.
@param  begin     The begin of the write buffer.
@param  size      The end of the write buffer.
@param  start     The start of the read buffer.
@param  read_size Number of bytes to copy.
@return Pointer to the last byte written or nil upon failure. */
API char* SocketCopy(void* begin, intptr_t size, const void* read,
                     intptr_t read_size);

/* Copies the source to the target functionally identical to memcpy.
@param  begin The begin of the write buffer.
@param  end   The end of the write buffer.
@param  start The start of the read buffer.
@param  size      Number of bytes to copy.
@return Pointer to the last byte written or nil upon failure. */
API inline char* SocketCopy(void* begin, void* end, const void* start,
                            intptr_t read_size);

/* Copies the source to the target functionally identical to memcpy.
    @param  begin The begin of the write buffer.
    @param  end   The end of the write buffer.
    @param  start The start of the read buffer.
    @param  stop  The stop of the read buffer.
    @return Pointer to the last byte written or nil upon failure. */
API inline char* SocketCopy(void* begin, void* end, const void* start,
                            const void* stop);

/* Compares the two memory sockets.
    @param  begin_a The beginning of Socket A.
    @param  end_a   The end of Socket A.
    @param  begin_b The start of Socket B.
    @param  stop_b  The stop of Socket B.
    @return True if they are the same and false if they are not. */
API bool SocketCompare(const void* begin, const void* end, const void* start,
                       const void* stop);

/* Compares the two memory sockets.
@param  begin_a The beginning of Socket A.
@param  end_a   The end of Socket A.
@param  begin_a The start of Socket B.
@param  size_b  The size of Socket B.
@return True if they are the same and false if they are not. */
API inline bool SocketCompare(const void* begin_a, void* end_a,
                              const void* begin_b, intptr_t read_size);

/* Compares the two memory sockets.
@param  begin_a The beginning of buffer a.
@param  size_a  The size of Socket A .
@param  begin_a The start of buffer b.
@param  size_b  The size of Socket B.
@return True if they are the same and false if they are not. */
API inline bool SocketCompare(const void* begin_a, intptr_t size_a,
                              const void* begin_b, intptr_t size_b);

}  // namespace _

#endif  //< INCLUDED_KABUKI_F2_KABUKI_SOCKET
#endif  //< #if SEAM >= SEAM_0_0_0__01

/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/crabs/slot.h
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
#if SEAM >= SEAM_0_0_3
#ifndef INCLUDED_CRABS_SLOT
#define INCLUDED_CRABS_SLOT
// Dependencies:
#include "bin.h"
#include "bout.h"
#include "op.h"
#include "socket.h"
#include "str1.h"
// End dependencies.

namespace _ {

/* A Slot in a Door in a Chinese Room to pass messages through.
    A Slot is Ring Buffer Socket similar to a TCP port. The operation of the
    Slot is similar to the Text class except that it introduces two more
    pointers for the (buffer) begin and (data) start of the ring buffer and
    you may write packed data.

*/
struct Slot {
  char *stop,  //< Stop of the data in the ring buffer.
      *end,    //< End of the ring buffer.
      *begin,  //< Beginning of the ring buffer.
      *start;  //< Start of the data in the ring buffer.

  /* Initializes the ring buffer with the given buffer begin and size.
      @param begin Pointer to the beginning of the ring buffer.
      @param size  The size of the ring buffer in bytes. */
  Slot(uintptr_t* buffer, uintptr_t size);

  /* Initializes the slot from the BIn. */
  Slot(BIn* bin);

  /* Initializes the slot from the BIn. */
  Slot(BOut* bout);

  /* Sets the ring buffer to the given buffer begin and size.
      @param begin Pointer to the beginning of the ring buffer.
      @param size  The size of the ring buffer in bytes. */
  inline bool Set(uintptr_t* buffer, uintptr_t size) {
    if (!buffer) return true;
    char* l_begin = reinterpret_cast<char*>(buffer);
    begin = start = stop = l_begin;
    end = l_begin + size;
    return false;
  }

  /* Checks if this slot contains the given address.
      @return Returns inputed address if this Slot contains the given address
              and nil else wise. */
  void* Contains(void* address);

  /* Clears the buffer without zeroing it out. */
  inline void Clear() { start = end = begin; }

  /* Zeros out the Slot. */
  void Wipe();

  /* Checks if there is space in the buffer.
      @return True if the buffer has space. */
  bool IsWritable();

  /* Checks if there is data in the buffer.
      @return True if the buffer has data. */
  bool IsReadable();

  /* Reads the given Operation input parameters from the slot to the args.
      @param  slot The slot to read from.
      @param  op   The Operation to get the in from.
      @param  args The args array of pointers to write to.
      @return Nil upon success and an Error Operation upon failure. */
  const Op* Read(const uint_t* params, void** args);

  /* Reads the given Operation input parameters from the slot to the args.
      @param  slot The slot to read from.
      @param  op   The Operation to get the in from.
      @param  args The args array of pointers to write to.
      @return Nil upon success and an Error Operation upon failure. */
  const Op* Read(const Op& op, void** args);

  /* Writes the given Operation output parameters from the slot to the args.
      @param  slot The slot to read from.
      @param  op   The Operation to get the in from.
      @param  args The args array of pointers to write to.
      @return Nil upon success and an Error Operation upon failure. */
  const Op* Write(const uint_t* params, void** args);

  /* Writes the given Operation output parameters from the slot to the args.
      @param  slot The slot to read from.
      @param  op   The Operation to get the in from.
      @param  args The args array of pointers to write to.
      @return Nil upon success and an Error Operation upon failure. */
  const Op* Write(const Op& op, void** args);

  /* Writes a single string to the slot buffer.
      @param message The string message to send.
      @return Nil upon success and an Error Operation upon failure. */
  const Op* Write(const char* message);

  /* Copies the contents of the other slot into the slot. */
  const Op* Write(Slot& other);

#if CRABS_TEXT
  /* Prints a string rep of this object to the printer. */
  Utf8& Print(Utf8& printer);
#endif
};

}  // namespace _

#if CRABS_TEXT
/* Prints out the bin to the text. */
inline _::Utf8& operator<<(_::Utf8& printer, _::Slot& slot) {
  return slot.Print(printer);
}
#endif

#endif  //< #if SEAM >= SEAM_0_0_3
#endif  //< INCLUDED_CRABS_SLOT

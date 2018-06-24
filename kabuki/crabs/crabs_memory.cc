/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/crabs_utils.cc
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <stdafx.h>
#if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2
// Dependencies:
#include "ascii.h"
#include "hex.h"
#include "memory.h"
#include "utf8.h"
// End dependencies.
#if SEAM_MAJOR == 0 && SEAM_MINOR == 3
#define PRINTF(format, ...) printf(format, __VA_ARGS__)
#define PUTCHAR(c) putchar(c)
#define PRINT_MEMORY(begin, end) \
  { Console<>().Out() << Socket(begin, end); }
#else
#define PRINTF(x, ...)
#define PUTCHAR(c)
#define PRINT_MEMORY(begin, end)
#endif

namespace _ {

uintptr_t* MemoryCreate(uintptr_t size) { return new uintptr_t[size]; }

void MemoryDestroy(uintptr_t* buffer) { delete buffer; }

void MemoryClear(void* address, size_t size) {
  // memset (address, '0', size);
  char* ptr = reinterpret_cast<char*>(address);
  for (; size; --size) *ptr++ = '\0';
  /* This code is designed to work like memcpy but is not working right now.
  uintptr_t lsb_mask = (1 << sizeof (uint32_t)) - 1,
  msb_mask  = ~lsb_mask,
  adr_uint  = reinterpret_cast<uintptr_t> (address),
  adr_lsb   = adr_uint & lsb_mask;
  uint32_t* ptr = reinterpret_cast<uint32_t*> (adr_uint & msb_mask),
  * end = reinterpret_cast<uint32_t*> ((adr_uint + size) & lsb_mask);
  // Example: address 0x1
  // Mask off 0x1
  uint32_t value = *ptr,
  value_mask = adr_uint & lsb_mask;
  value &= value_mask;
  *ptr = value;
  ++ptr;
  for (; ptr < end; ++ptr) {
      *ptr = 0;
  }
  value = *ptr;
  value_mask = adr_uint & lsb_mask;
  value &= value_mask;
  *ptr = value;*/
}

char* MemoryCopy(void* begin, void* end, const void* start, const void* stop) {
  ASSERT(begin)
  ASSERT(start)

  char *begin_ptr = reinterpret_cast<char*>(begin),
       *end_ptr = reinterpret_cast<char*>(end);
  const char *start_ptr = reinterpret_cast<const char*>(start),
             *stop_ptr = reinterpret_cast<const char*>(stop);

  PRINTF("\nCopying %i bytes from %p and writing to %p",
         (int)(stop_ptr - start_ptr), begin_ptr, stop_ptr);

  if (begin_ptr >= end_ptr || start_ptr >= stop_ptr) return nullptr;
  intptr_t read_size = stop_ptr - start_ptr, write_size = end_ptr - begin_ptr;
  if (write_size < read_size) return nullptr;

  if (read_size < (2 * sizeof(void*) + 1)) {
    while (start_ptr < stop_ptr) *begin_ptr++ = *start_ptr++;
    return begin_ptr;
  }

  // Debug stuff.
  char *begin_debug = begin_ptr, *end_debug = end_ptr;

  // Algorithm:
  // 1.) Save return value.
  // 2.) Align write pointer up and copy the unaligned bytes in the lower
  //     memory region.
  // 3.) Align write_end pointer down and copy the unaligned bytes in the
  //     upper memory region.
  // 4.) Copy the word-aligned middle region.
  char *success = end_ptr, *aligned_pointer = AlignUpPointer<>(begin_ptr);
  PRINTF("\n  AlignUpPointer<> (begin_ptr):0x%p", aligned_pointer);
  while (begin_ptr < aligned_pointer) *begin_ptr++ = *start_ptr++;
  aligned_pointer = AlignDownPointer<>(end_ptr);
  PRINTF("\n  AlignDownPointer<> (begin_ptr):0x%p", aligned_pointer);
  while (end_ptr > aligned_pointer) *end_ptr-- = *stop_ptr--;
  PRINTF("\n  Down-stage pointers are now begin_ptr:0x%p end_ptr:0x%p",
         begin_ptr, end_ptr);

  uintptr_t *words = reinterpret_cast<uintptr_t*>(begin_ptr),
            *words_end = reinterpret_cast<uintptr_t*>(end_ptr);
  const uintptr_t* read_word = reinterpret_cast<const uintptr_t*>(start_ptr);

  while (words < words_end) *words++ = *read_word++;

  PRINT_MEMORY(begin_debug, end_debug)

  return success;
}

bool MemoryCompare(const void* begin, const void* end, const void* start,
                   const void* stop) {
  const char *begin_ptr = reinterpret_cast<const char*>(begin),
             *end_ptr = reinterpret_cast<const char*>(end),
             *start_ptr = reinterpret_cast<const char*>(start),
             *stop_ptr = reinterpret_cast<const char*>(stop);
  while (begin_ptr < end_ptr) {
    char a = *begin_ptr++, b = *end_ptr++;
    if (a != b) return false;
  }
  return true;
}

Socket::Socket() {
  // Nothing to do here! ({:-)-+=<
}

Socket::Socket(void* begin, void* end)
    : begin(reinterpret_cast<char*>(begin)), end(reinterpret_cast<char*>(end)) {
  if (!begin || !end || begin > end) {
    begin = end = 0;
    return;
  }
}

Socket::Socket(void* begin, intptr_t size)
    : begin(reinterpret_cast<char*>(begin)),
      end(reinterpret_cast<char*>(begin) + size) {
  if (!begin || size < 0) {
    end = reinterpret_cast<char*>(begin);
    return;
  }
}

Socket::Socket(const Socket& other) : begin(other.begin), end(other.end) {
  // Nothing to do here! ({:-)-+=<
}

Socket& Socket::operator=(const Socket& other) {
  begin = other.begin;
  end = other.end;
  return *this;
}

}  // namespace _
#undef PRINTF
#undef PUTCHAR
#endif  //< SEAM_MAJOR == 0 && SEAM_MINOR >= 2

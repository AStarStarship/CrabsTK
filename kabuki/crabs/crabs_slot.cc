/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/crabs_slot.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <stdafx.h>
#if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 4
// Dependencies:
#include "ascii_data_types.h"
#include "debug.h"
#include "hex.h"
#include "line.h"
#include "tinteger.h"
#include "slot.h"
#include "text.h"
// End dependencies.

#if SEAM_MAJOR == 0 && SEAM_MINOR == 4

#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT(c) Print(c)
#define PRINT_BSQ(bsq) Console<>().Out() << header << '\n' << Bsq(bsq);
#else
#define PRINTF(x, ...)
#define PRINT(c)
#define PRINT_BSQ
#endif

namespace _ {

const Op* ReturnError(Slot* slot, Error error) {
  PRINTF("\n%s", ErrorStrings()[error])
  return reinterpret_cast<const Op*>(error);
}

const Op* ReturnError(Slot* slot, Error error, const uint_t* header) {
  PRINTF("\n%s", ErrorStrings()[error])
  return reinterpret_cast<const Op*>(error);
}

const Op* ReturnError(Slot* slot, Error error, const uint_t* header,
                      byte offset) {
  PRINTF("\n%s", ErrorStrings()[error])
  return reinterpret_cast<const Op*>(error);
}

const Op* ReturnError(Slot* slot, Error error, const uint_t* header,
                      uint_t offset, char* address) {
  PRINTF("\n%s", ErrorStrings()[error])
  return reinterpret_cast<const Op*>(error);
}

Slot::Slot(uintptr_t* buffer, uintptr_t size) {
  ASSERT(buffer);
  ASSERT(size >= kSlotSizeMin);
  char* l_begin = reinterpret_cast<char*>(buffer);
  begin = l_begin;
  start = l_begin;
  stop = l_begin;
  end = l_begin + size - 1;
}

Slot::Slot(BIn* bin) {
  ASSERT(bin);
  char* l_begin = reinterpret_cast<char*>(bin) + sizeof(BIn);
  begin = l_begin;
  start = l_begin + bin->start;
  stop = l_begin + bin->stop;
  end = l_begin + bin->size;
}

Slot::Slot(BOut* bout) {
  ASSERT(bout);
  char* l_begin = reinterpret_cast<char*>(bout) + sizeof(BIn);
  begin = l_begin;
  start = l_begin + bout->start;
  stop = l_begin + bout->stop;
  end = l_begin + bout->size;
}

void* Slot::Contains(void* address) {
  char* begin = reinterpret_cast<char*>(this) + sizeof(Slot);
  if (address < begin) {
    return nullptr;
  }
  char* l_end = end;
  if (address > l_end) {
    return nullptr;
  }
  return address;
}

void Slot::Wipe() {
  char *l_begin = reinterpret_cast<char*>(this) + sizeof(Slot),
       *l_start = start, *l_stop = stop, *temp;
  if (l_start > l_stop) {
    temp = l_start;
    start = l_stop;
    stop = temp;
  }
  while (start != stop) *start++ = 0;
}

const Op* Slot::Write(const uint_t* params, void** args) {
  ASSERT(params);
  ASSERT(args);

  ASSERT(false);
  // @todo Write me!
  return nullptr;
}

bool Slot::IsWritable() {
  char* l_stop = start;
  if (l_stop == begin) {
    if (l_stop != end) {
      return false;
    }
    return true;
  }
  return start != stop - 1;
}

bool Slot::IsReadable() { return start != stop; }

/*char* SlotRead (Slot* slot, char* write, void* write_end, char* const begin,
                    char* const start, char* const stop , char* const end,
                    size_t size) {
    if (!slot) {
        return nullptr;
    }
    if (!write) {
        return nullptr;
    }
    if (write > write_end) {
        return nullptr;
    }

    if ((start > stop) && (start + size >= end)) {
        // Calculate upper chunk size.
        size_t top_chunk = end - stop;
        size -= top_chunk;

        MemoryCopy (target, target_end, start, top_chunk);
        MemoryCopy (reinterpret_cast<char*>(target) + top_chunk, size,
                    begin);
        return begin + size;
    }
    MemoryCopy (target, target_end, stop, size);
    return start + size;
}*/

const Op* Slot::Read(const uint_t* params, void** args) {
  ASSERT(params);
  ASSERT(args);
  byte ui1;      //< Temp variable to load most types.
  uint16_t ui2;  //< Temp variable for working with UI2 types.
#if USING_CRABS_4_BYTE_TYPES
  uint32_t ui4;
#endif
#if USING_CRABS_8_BYTE_TYPES
  uint64_t ui8;  //< Temp UI8 variable.
#endif
  char* ui1_ptr;             //< Pointer to a UI1.
  uint16_t* ui2_ptr;         //< Pointer to a UI2.
  uint32_t* ui4_ptr;         //< Pointer to a UI4.
  uint64_t* ui8_ptr;         //< Pointer to a UI8.
  uint_t type,               //< Current type being read.
      index,                 //< Index in the escape sequence.
      num_params = *params;  //< Number of params.
  ASSERT(num_params);
  uintptr_t offset;  //< Offset to word align the current type.
  intptr_t length,   //< Length of the data in the buffer.
      count,         //< Argument length.
      size;          //< Size of the ring buffer.

  PRINTF("\n\nReading BIn: ")

  char *l_begin = begin,             //< Beginning of the buffer.
      *l_end = end,                  //< end of the buffer.
          *l_start = start,          //< start of the data.
              *l_stop = stop;        //< stop of the data.
  const uint_t* param = params + 1;  //< current param.

  size = l_end - l_begin;

  length = SlotLength(l_start, l_stop, size);

  PRINTF("\n\nReading %i bytes.", (int)length)
  // PRINT_BSQ (params)
  // When we scan, we are reading from the beginning of the BIn buffer.

  for (index = 0; index < num_params; ++index) {
    type = (byte)*param;
    ++param;
    PRINTF("\nindex:%u:\"%s\", start:0x%i, stop:0x%i", (uint)index,
           TypeString(type), (int)Size(l_begin, l_start),
           (int)Size(l_begin, l_stop))

    switch (type) {
      case NIL:
        return ReturnError(this, kErrorInvalidType);
      case ADR:  //< _R_e_a_d__S_t_r_i_n_g_-_1______________________
      case STR:
        // Load buffered-type argument length and increment the
        // index.
        count = *param;
        ++param;

        // std::cout << "\nReading char with max length " << count;

        // Load next pointer and increment args.
        ui1_ptr = reinterpret_cast<char*>(args[index]);
        if (!ui1_ptr) {
          break;
        }

        // Read from slot and write to the slot:
        ui1 = *l_start;
        if (++l_start > l_end) l_start -= size;
        *ui1_ptr = ui1;
        ++ui1_ptr;

        while (ui1 && count) {
          if (count-- == 0)
            return ReturnError(this, kErrorBufferUnderflow, params, index,
                               l_start);
          PRINT(ui1)

          ui1 = *l_start;  // Read byte from ring-buffer.
          if (++l_start > l_end) l_start -= size;
          *ui1_ptr = ui1;  // Write byte to destination.
          ++ui1_ptr;
        }

        PRINTF(" done!\n")

        if (type == 0) {
          *ui1_ptr = ui1;
        }
        break;
      case SI1:  //< _R_e_a_d__1__B_y_t_e__T_y_p_e_s________________
      case UI1:
      case BOL:
#if USING_CRABS_1_BYTE_TYPES
        if (length == 0) {
          return ReturnError(this, kErrorBufferUnderflow, params, index,
                             l_start);
        }
        --length;

        // Read from buffer and write to the stack:
        ui1 = *l_start;
        if (++l_start > l_end) {
          l_start -= size;
        }
        // Load next pointer and increment args.
        ui1_ptr = reinterpret_cast<char*>(args[index]);
        if (!ui1_ptr) {
          break;
        }
        *ui1_ptr = ui1;  //< Write
        if (++l_start > l_end) {
          l_start -= size;
        }
        break;
#else
        return ReturnError(this, kErrorInvalidType);
#endif
      case SI2:  //< _R_e_a_d__1_6_-_b_i_t__T_y_p_e_s______________
      case UI2:
      case HLF:
#if WORD_SIZE <= 16
      case SVI:
      case UVI:
#endif
#if USING_CRABS_2_BYTE_TYPES
        // Read2ByteType:{
        // Word-align
        offset = AlignUpOffset2(l_start);
        if ((uintptr_t)length < offset + 2) {
          return ReturnError(this, kErrorBufferUnderflow, params, index,
                             l_start);
        }
        length -= (uint_t)offset + 2;
        l_start += offset;
        if (l_start > l_end) {
          l_start -= size;
        }
        // Read from buffer and write to the stack:
        ui2_ptr = reinterpret_cast<uint16_t*>(l_start);
        ui2 = *ui2_ptr;
        l_start += sizeof(uint16_t);
        if (l_start > l_end) {
          l_start -= size;
        }
        // Load next pointer and increment args.
        ui2_ptr = reinterpret_cast<uint16_t*>(args[index]);
        if (!ui2_ptr) {
          break;
        }
        *ui2_ptr = ui2;
        //}
        break;
#else
        return ReturnError(this, kErrorInvalidType);
#endif
#if USING_CRABS_VARINT2
        goto Read2ByteType;
#else
        return ReturnError(this, kErrorInvalidType);
#endif
#if WORD_SIZE > 16
      case SVI:
      case UVI:
#endif
      case SI4:  //< _R_e_a_d__3_2_-_b_i_t__T_y_p_e_s______________
      case UI4:
      case FLT:
      case TMS:
#if USING_CRABS_4_BYTE_TYPES
        // Read4ByteType:{
        // Word-align
        offset = AlignUpOffset4(l_start);
        if ((uintptr_t)length < offset + 4) {
          return ReturnError(this, kErrorBufferUnderflow, params, index,
                             l_start);
        }
        length -= (uint_t)offset + 4;
        l_start += offset;
        if (l_start > l_end) {
          l_start -= size;  //< Bound
        }
        // Read from buffer and write to the stack:
        ui4_ptr = reinterpret_cast<uint32_t*>(l_start);
        ui4 = *ui4_ptr;              //< Read
        l_start += sizeof(int32_t);  //< Increment
        if (l_start > l_end) {
          l_start -= size;  //< Bound
        }
        // Load next pointer and increment args.
        ui4_ptr = reinterpret_cast<uint32_t*>(args[index]);
        if (!ui4_ptr) {
          break;
        }
        *ui4_ptr = ui1;  //< Write
        break;
//}
#else
        return ReturnError(this, kErrorInvalidType);
#endif
      case SI8:  //< _R_e_a_d__6_4_-_b_i_t__T_y_p_e_s______________
      case UI8:
      case DBL:
      case TMU:
#if USING_CRABS_8_BYTE_TYPES
        // Read8ByteType:{
        // Word-align
        offset = AlignUpOffset8(l_start);
        if ((uintptr_t)length < offset + sizeof(int64_t)) {
          return ReturnError(this, kErrorBufferUnderflow, params, index,
                             l_start);
        }
        length -= offset + sizeof(int64_t);
        l_start += offset;
        if (l_start > l_end) {
          l_start -= size;  //< Bound
        }
        // Read from buffer and write to the stack:
        ui8_ptr = reinterpret_cast<uint64_t*>(l_start);
        ui8 = *ui8_ptr;               //< Read
        l_start += sizeof(uint64_t);  //< Increment
        if (l_start > l_end) {
          l_start -= size;  //< Bound
        }
        // Load next pointer and increment args.
        ui8_ptr = reinterpret_cast<uint64_t*>(args[index]);
        if (ui8_ptr == 0) break;
        *ui8_ptr = ui8;  //< Write.
        break;
//}
#else
        return ReturnError(this, kErrorInvalidType);
#endif
      default: {
#if USING_CRABS_OBJ
        count = type >> 5;  //< count is now the array type bits.
        type &= 0x1f;       //< Now type is the type 0-31
        if (count && (type >= OBJ)) {
          // Can't make arrays out of objects!
          return ReturnError(this, kErrorInvalidType, params, index, l_start);
        }
        // We don't care if it's a multidimensional array anymore.
        ui1_ptr = reinterpret_cast<char*>(args[index]);
        if (ui1_ptr == nullptr)
          return ReturnError(this, kErrorImplementation, params, index,
                             l_start);
        count &= 0x3;
        switch (count) {
          case 0: {  // It's a 8-bit count.
            if (type >= LST) {
              // LST, BOK, DIC, and MAP can't be 8-bit!
              return ReturnError(this, kErrorInvalidType, params, index,
                                 l_start);
            }
            count = (uintptr_t)*ui1_ptr;
            break;
          }
          case 1: {  // It's a 16-bit count.
            if (length < 3) {
              return ReturnError(this, kErrorBufferUnderflow, params, index,
                                 l_start);
            }
            count -= 2;
            ui2_ptr = reinterpret_cast<uint16_t*>(ui1_ptr);
            count = (uintptr_t)*ui2_ptr;
            if (count > length) {
              return ReturnError(this, kErrorBufferOverflow, params, index,
                                 l_start);
            }
            break;
          }
          case 2: {  // It's a 32-bit count.
            if (length < 5) {
              return ReturnError(this, kErrorBufferUnderflow, params, index,
                                 l_start);
            }
            count -= 4;
            ui4_ptr = reinterpret_cast<uint32_t*>(ui1_ptr);
            count = (uintptr_t)*ui4_ptr;
            if (count > length) {
              return ReturnError(this, kErrorBufferOverflow, params, index,
                                 l_start);
            }
            break;
          }
          case 3: {  // It's a 64-bit count.
            if (length < 9) {
              return ReturnError(this, kErrorBufferUnderflow, params, index,
                                 l_start);
            }
            count -= 8;
            ui8_ptr = reinterpret_cast<uint64_t*>(ui1_ptr);
            count = (uintptr_t)*ui8_ptr;
            if (count > length) {
              return ReturnError(this, kErrorBufferOverflow, params, index,
                                 l_start);
            }
            break;
          }
          default: {
            return ReturnError(this, kErrorImplementation, params, index,
                               l_start);
          }
        }
        if (length < count) {
          return ReturnError(this, kErrorBufferOverflow, params, index,
                             l_start);
        }
        if (!count) {
          return ReturnError(this, kErrorBufferOverflow, params, index,
                             l_start);
        }
        if (l_start + count >= l_end) {
          for (; size - count > 0; --count) {
            ui1 = *l_start;
            if (++l_start > l_end) {
              l_start -= size;
            }
            *ui1_ptr = ui1;
            ++ui1_ptr;
          }
          l_stop = l_begin - 1;
          for (; count > 0; --count) {
            ui1 = *l_start;
            if (++l_start > l_end) {
              l_start -= size;
            }
            *ui1_ptr = ui1;
            ++ui1_ptr;
          }
          break;
        }
        for (; count > 0; --count) {
          ui1 = *l_start;
          if (++l_start > l_end) {
            l_start -= size;
          }
          *ui1_ptr = ui1;
          ++ui1_ptr;
        }
        break;
#endif
      }
        PRINTF(" |")
    }
  }
  PRINTF("\nDone reading.")
  // SlotWipe (slot);

  // Convert pointer back to offset
  start = l_start;

  return 0;
}

const Op* Slot::Read(const Op& op, void** args) { return Read(op.in, args); }

const Op* Slot::Write(const Op& op, void** args) { return Write(op.out, args); }

const Op* Slot::Write(Slot& other) { return nullptr; }

const Op* Slot::Write(const char* message) { return nullptr; }

#if CRABS_TEXT
Utf8& Slot::Print(Utf8& print) {
  char *l_begin = begin, *l_end = end;
  return print << Line('_', 80) << "\nSlot: begin:" << Hex<>(l_begin)
               << " start:" << Hex<>(start) << "\nstop:" << Hex<>(stop)
               << " end:" << Hex<>(l_end) << Socket(l_begin, l_end);
}
#endif

}  // namespace _
#undef PRINTF
#undef PRINT
#endif  //> #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 4

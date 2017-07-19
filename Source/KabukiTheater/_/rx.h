/** The Chinese Room
    @version 0.x
    @file    /.../rx.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough<calemccollough.github.io>

                        All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

                http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#ifndef CHINESEROOM_RXSLOT_H
#define CHINESEROOM_RXSLOT_H

#include "log.h"

namespace _ {

/*< A Rx socket.
*/
struct Rx
{
    uint_t size,            //< The size of the buffer.
        start;              //< The starting index of the ring buffer data.
    volatile uint_t stop;   //< The stopping index of the ring buffer data.
    uint_t read;            //< The read variable.
};

/** Initializes the Rx struct to an empty buffer. */
inline Rx* RxInit (byte* buffer, uint_t size) {
    if (buffer == nullptr)
        return nullptr;
    Rx* rx = reinterpret_cast<Rx*>(rx);
    rx->size = size - sizeof (Rx);
    rx->start = 0;
    rx->stop = 0;
    rx->read = 0;
    return rx;
}

/** Initializes the Rx struct to an empty buffer. */
inline Rx* RxInit (Rx* rx, uint_t size) {
    if (rx == nullptr)
        return nullptr;
    rx->size = size - sizeof (Rx);
    rx->start = 0;
    rx->stop = 0;
    rx->read = 0;
    return rx;
}

/** Gets the start of the Rx ring buffer. */
inline byte* SocketBaseAddress (void* ptr) {
    return reinterpret_cast <byte*>(ptr) + kSlotHeaderSize;
}

/** Gets the start of the Rx ring buffer. */
inline byte* SocketBaseAddress (Rx* ptr) {
    return reinterpret_cast <byte*>(ptr) + kSlotHeaderSize;
}

/** Gets the rx buffer length. */
inline uint_t GetRxSpace (Rx* rx) {
    if (rx == nullptr) return ~0;

    byte* base = SocketBaseAddress (rx);
    return RingBufferSpace (base + rx->start, base + rx->stop, rx->size);
}

/** Gets the start of the Rx ring buffer. */
inline byte* SocketBaseAddress (void* ptr, uint_t rx_tx_offset) {
    enum {
        kSlotHeaderSize = sizeof (Rx) + sizeof (uintptr_t) -
        sizeof (Rx) % sizeof (uintptr_t),
        //< Offset to the start of the ring buffer.
    };

    return reinterpret_cast <byte*>(ptr) + rx_tx_offset + kSlotHeaderSize;
}

/** Gets the end address of the rx buffer. */
inline byte* SlotEndAddress (Rx* rx) {
    return reinterpret_cast<byte*>(rx) + kSlotHeaderSize + rx->size;
}

#if DEBUG_CHINESEROOM
static void Print (Rx* rx) {
    if (rx == nullptr) return;
    uint_t size = rx->size;
    PrintLine ('_');
    printf ("| Rx %p: size: %u, start: %u, stop: %u, read: %u\n", rx, size,
            rx->start, rx->stop, rx->read);
    PrintMemory (SocketBaseAddress (rx), size);
}
#endif  //< DEBUG_CHINESEROOM

/** Scans a message with the given params to the given Terminal.
    @param rx The Rx socket.
    @param params The parameters.
    @param args The arguments.
    @return Returns 0 upon success and an ErrorList ticket number upon failure. 
*/
static ticket_t Read (Rx* rx, const uint_t* params, void** args) {
    if (rx == nullptr)
        Report (NullPointerError, 0, 0, 0);
    if (params == nullptr)
        Report (NullHeaderError, 0, 0, 0);
    if (args == nullptr)
        Report (NullHeaderError, 0, 0, 0);
#if DEBUG_CHINESEROOM
    Print (rx);
#endif

    byte type,              //< The current type we're reading.
        ui1;                //< Temp variable to load most types.
    uint16_t ui2;           //< Temp variable for working with UI2 types.
#if USING_VARINT4 || USING_AR4 || USING_BK4
    uint32_t ui4;
#endif
#if USING_VARINT8 || USING_AR8 || USING_BK8
    uint64_t ui8;
#endif

    byte*     ui1_ptr;          //< Pointer to a UI1.
#if  USING_VARINT2 || USING_BK2
    uint16_t* ui2_ptr;          //< Pointer to a UI2.
#endif
#if USING_VARINT4 || USING_AR4 || USING_BK4
    uint32_t* ui4_ptr;          //< Pointer to a UI4.
#endif
#if USING_VARINT8 || USING_AR8 || USING_BK8
    uint64_t* ui8_ptr;          //< Pointer to a UI1.
#endif
    uint_t size,                //< The size of the ring buffer.
        length,                 //< The length of the data in the buffer.
        count,                  //< The argument length.
        index,                  //< The index of the Esc.
        num_params = *params,   //< The number of params.
        array_type;             //< The type of being read.
    hash16_t hash;

    if (num_params == 0) return 0;   //< Nothing to do.

    hash = 0;
    size = rx->size;

    byte* begin = SocketBaseAddress (rx),   //< The beginning of the buffer.
        *end = begin + size,                //< The end of the buffer.
        *start = begin + rx->start,         //< The start of the data.
        *stop = begin + rx->stop;           //< The stop of the data.
    const uint_t* param = params + 1;       //< The current param.

    length = RingBufferLength (start, stop, size);

#if DEBUG_CHINESEROOM
    printf ("| Reading: ");
    PrintParams (params);
    printf ("| begin: 0x%p start : %u stop : %u end : %u "
            "length: %u ", begin, Diff (begin, start), 
            Diff (begin, stop), Diff (begin, end), length);
#endif
    // When we scan, we are reading from the beginning of the Rx buffer.

    for (index = 0; index < num_params; ++index) {
        type = *param;
        ++param;

#if DEBUG_CHINESEROOM
        printf ("\n| Index %2u: %s  start: %u, stop: %u hash: ", index,  
                TypeString (type), Diff (begin, start), Diff (begin, stop));
#endif

        switch (type)
        {
          case NIL:
          case SOH: //< _R_e_a_d__S_t_r_i_n_g_-_8_______________________________________
          case STX:
              // Load buffered-type argument length and increment the index.
              count = *param;
              ++param;
#if DEBUG_CHINESEROOM
              printf ("\n|           Reading string with max length %u: ", count);
#endif
            // Load next pointer and increment args.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == nullptr) break;

            // Read string.
            ui1 = *start;
            hash = Hash16 (ui1, hash);
            if (++start >= end) start -= size;
            *ui1_ptr = ui1;
            ++ui1_ptr;

            while (ui1 != 0 && count != 0) {
                if (count-- == 0)
                    return Report (BufferUnderflowError, params, index,
                                        start);
#if DEBUG_CHINESEROOM
                putchar (ui1);
#endif
                ui1 = *start;       // Read byte from ring-buffer.
                hash = Hash16 (ui1, hash);
                if (++start >= end) start -= size;
                *ui1_ptr = ui1;     // Write byte to destination.
                ++ui1_ptr;
            }
#if DEBUG_CHINESEROOM
            printf (" done!\n");
#endif
            if (type != SOH) {
                hash = Hash16 (ui1, hash);
                *ui1_ptr = ui1;
            }
            break;
          case SI1: //< _R_e_a_d__1__B_y_t_e__T_y_p_e_s_________________________________
          case UI1:
          case BOL:
#if USING_1_BYTE_TYPES
            if (length == 0) 
                return Report (BufferUnderflowError, params, index, start);
            --length;

            // Load next pointer and increment args.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == 0) break;

            // Read type;

            // Byte 1
            ui1 = *start;                       //< Read
            hash = Hash16 (ui1, hash);          //< Hash
            if (++start >= end) start -= size;  //< Increment
            *ui1_ptr = ui1;                     //< Write
            break;
#else
            return Report (InvalidRxTypeError, params, index, start);
#endif
          case SI2: //< _R_e_a_d__1_6_-_b_i_t__T_y_p_e_s________________________________
          case UI2:
          case HLF:
#if USING_2_BYTE_TYPES
            if (length < 2)
                return Report (BufferUnderflowError, params, index, start);
            length -= 2;

            // Load next pointer and increment args.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == 0) break;

            // Read type

            // Byte 1
            ui1 = *start;                       //< Read
            hash = Hash16 (ui1, hash);          //< Hash
            if (++start >= end) start -= size;  //< Increment
            *ui1_ptr = ui1;                     //< Write

            // Byte 2
            ui1 = *start;                       //< Read
            hash = Hash16 (ui1, hash);          //< Hash
            if (++start >= end) start -= size;  //< Increment
            *(ui1_ptr + 1) = ui1;               //< Write
            break;
#else
            return Report (InvalidRxTypeError, params, index,
                                start);
#endif
          case SI4: //< _R_e_a_d__3_2_-_b_i_t__T_y_p_e_s________________________
          case UI4:
          case FLT:
          case TMS:
#if USING_4_BYTE_TYPES
            if (length < 4) 
                return Report (BufferUnderflowError, params, index, start);
            length -= 4;

            // Load next pointer and increment args.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == 0) break;

            // Read type

            // Byte 1
            ui1 = *start;                       //< Read
            hash = Hash16 (ui1, hash);          //< Hash
            if (++start >= end) start -= size;  //< Increment
            *ui1_ptr = ui1;                     //< Write

            // Byte 2
            ui1 = *start;                       //< Read
            hash = Hash16 (ui1, hash);          //< Hash
            if (++start >= end) start -= size;  //< Increment
            *(ui1_ptr + 1) = ui1;               //< Write

            // Byte 3
            ui1 = *start;                       //< Read
            hash = Hash16 (ui1, hash);          //< Hash
            if (++start >= end) start -= size;  //< Increment
            *(ui1_ptr + 2) = ui1;               //< Write

            // Byte 4
            ui1 = *start;                       //< Read
            hash = Hash16 (ui1, hash);          //< Hash
            if (++start >= end) start -= size;  //< Increment
            *(ui1_ptr + 3) = ui1;               //< Write
            break;
#else
            return Report (InvalidRxTypeError, params, index,
                                start);
#endif
          case TMU: //< _R_e_a_d__6_4_-_b_i_t__T_y_p_e_s________________________
          case SI8:
          case UI8:
          case DBL:
#if USING_8_BYTE_TYPES
            if (length < 8)
                  return Report (BufferUnderflowError, params, index, start);
            length -= 8;

            // Load next pointer and increment args.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == 0) break;

            // Read type

            // Byte 1
            ui1 = *start;                       //< Read
            hash = Hash16 (ui1, hash);          //< Hash
            if (++start >= end) start -= size;  //< Increment
            *ui1_ptr = ui1;                     //< Write

            // Byte 2
            ui1 = *start;                       //< Read
            hash = Hash16 (ui1, hash);          //< Hash
            if (++start >= end) start -= size;  //< Increment
            *(ui1_ptr + 1) = ui1;               //< Write

            // Byte 3
            ui1 = *start;                       //< Read
            hash = Hash16 (ui1, hash);          //< Hash
            if (++start >= end) start -= size;  //< Increment
            *(ui1_ptr + 2) = ui1;               //< Write

            // Byte 4
            ui1 = *start;                       //< Read
            hash = Hash16 (ui1, hash);          //< Hash
            if (++start >= end) start -= size;  //< Increment
            *(ui1_ptr + 3) = ui1;               //< Write

            // Byte 5
            ui1 = *start;                       //< Read
            hash = Hash16 (ui1, hash);          //< Hash
            if (++start >= end) start -= size;  //< Increment
            *(ui1_ptr + 4) = ui1;               //< Write

            // Byte 6
            ui1 = *start;                       //< Read
            hash = Hash16 (ui1, hash);          //< Hash
            if (++start >= end) start -= size;  //< Increment
            *(ui1_ptr + 5) = ui1;               //< Write

            // Byte 7
            ui1 = *start;                       //< Read
            hash = Hash16 (ui1, hash);          //< Hash
            if (++start >= end) start -= size;  //< Increment
            *(ui1_ptr + 6) = ui1;               //< Write

            // Byte 8
            ui1 = *start;                       //< Read
            hash = Hash16 (ui1, hash);          //< Hash
            if (++start >= end) start -= size;  //< Increment
            *(ui1_ptr + 7) = ui1;               //< Write
            break;
#else
            return Report (InvalidRxTypeError, params, index, start);
#endif
          case SV2: //< _R_e_a_d__2_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t_________

#if USING_VARINT2
            // Load next pointer and increment args.
            ui2_ptr = reinterpret_cast<uint16_t*> (args[index]);
            if (ui2_ptr == nullptr)
                return Report (NullPointerError, params, index, start);

            // Read byte 1
            if (length-- == 0)
                return Report (BufferUnderflowError, params, index, start);
            ui1 = *start;
            if (++start >= end) start -= size;
            hash = Hash16 (ui1, hash);
            if ((ui1 >> 7) != 0)
            {
                ui2 = ui1 & 0x7F;   //< Mask off end-of-varint MSb.
                *ui2_ptr = UnpackSignedVarint<uint16_t>(ui2);
                break;
            }
            ui2 = ui1;

            // Read byte 2
            if (--length == 0)
                return Report (BufferUnderflowError, params, index, start);
            ui1 = *start;
            if (++start >= end) start -= size;
            hash = Hash16 (ui1, hash);
            if ((ui1 >> 7) != 0) {
                // Mask off end-of-varint MSb.
                ui2 |= ((uint16_t)(ui1 & 0x7F)) << 7;
                *ui2_ptr = UnpackSignedVarint<uint16_t> (ui2);
                break;
            }
            ui2 |= ((uint16_t)ui1) << (2 * 7);

            // Read byte 3
            if (--length == 0)
                return Report (BufferUnderflowError, params, index, start);
            ui1 = *start;
            if (++start >= end) start -= size;
            hash = Hash16 (ui1, hash);
            if ((ui1 >> 7) == 0)
                return Report (VarintOverflowError, params, index, start);
            ui2 |= ((uint16_t)(ui1 & 0x7F) << 14);
            *ui2_ptr = UnpackSignedVarint<uint16_t> (ui2);
            break;

          case UV2: //< _R_e_a_d__2_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t_____
            // Load next pointer and increment args.
              ui2_ptr = reinterpret_cast<uint16_t*> (args[index]);
              if (ui2_ptr == nullptr)
                  return Report (NullPointerError, params, index, start);

              // Read byte 1
              if (length-- == 0)
                  return Report (BufferUnderflowError, params, index, start);
              ui1 = *start;
              if (++start >= end) start -= size;
              hash = Hash16 (ui1, hash);
              if ((ui1 >> 7) != 0) {
                  ui2 = ui1 & 0x7F;   //< Mask off end-of-varint MSb.
                  *ui2_ptr = ui2;
                  break;
              }
              ui2 = ui1;

              // Read byte 2
              if (--length == 0)
                  return Report (BufferUnderflowError, params, index, start);
              ui1 = *start;
              if (++start >= end) start -= size;
              hash = Hash16 (ui1, hash);
              if ((ui1 >> 7) != 0) {
                  // Mask off end-of-varint MSb.
                  ui2 |= ((uint16_t)(ui1 & 0x7F)) << 7;
                  *ui2_ptr = ui2;
                  break;
            }
            ui2 |= ((uint16_t)ui1) << (2 * 7);

            // Read byte 3
            if (--length == 0)
                return Report (BufferUnderflowError, params, index, start);
            ui1 = *start;
            if (++start >= end) start -= size;
            hash = Hash16 (ui1, hash);
            if ((ui1 >> 7) == 0)
                return Report (VarintOverflowError, params, index, start);
            ui2 |= ((uint16_t)(ui1 & 0x7F) << 14);
            *ui2_ptr = ui2;
            break;
#else
          case UV2:
            return Report (InvalidRxTypeError, params, index, start);
#endif
          case SV4: //< _R_e_a_d__4_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t_________
          case UV4: //< _R_e_a_d__4_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t_____
#if USING_VARINT4
            // Load next pointer and increment args.
            ui4_ptr = reinterpret_cast<uint32_t*> (args[index]);
            if (ui4_ptr == nullptr)
                return Report (NullPointerError, params, index, start);

            // Scan byte 1.
            ui1 = *start;
            if (++start >= end) start -= size;
            hash = Hash16 (ui1, hash);
            ui4 = ui1;
            ui2 = 7;        //< Number of bits to shift ui1 to the left.
            count = 5; //< The max number of Varint4 bytes.
            while (ui1 >> 7 == 0)
            {
                if (length-- == 0)
                    return Report (BufferUnderflowError, params, index, start);
                ui1 = *start;
                if (++start >= end) start -= size;
                hash = Hash16 (ui1, hash);
                ui4 |= ((uint32_t)(ui1 & 0x7F)) << ui2;
                //< @todo I'm starting to second guess if we need to mask ui1 
                //< because we're packing them up and will overwrite.
                ui2 += 7;
                if (--count == 0)
                    return Report (VarintOverflowError, params, index, start);
            }
            if (count == 5)    //< If there is only one byte we need to
                ui4 &= 0x7F;        //< mask off the terminating varint bit.
            if (type == SV4) 
                ui4 = UnpackSignedVarint<uint32_t> (ui4);
            *ui4_ptr = ui4;
          break;
#else
            return Report (InvalidRxTypeError, params, index, start);
#endif
          case SV8: //< _R_e_a_d__V_a_r_i_n_t__8________________________________
          case UV8:
#if USING_VARINT8
              // Load next pointer and increment args.
              ui8_ptr = reinterpret_cast<uint64_t*> (args[index]);
              if (ui8_ptr == nullptr)
                  return Report (NullPointerError, params, index, start);

              // Scan byte 1.
              ui1 = *start;
              if (++start >= end) start -= size;
              hash = Hash16 (ui1, hash);
              ui8 = ui1;
              ui2 = 7;        //< Number of bits to shift ui1 to the left.
              count = 9; //< The max number of Varint8 bytes.
              while (ui1 >> 7 == 0) {
                  if (length-- == 0)
                      return Report (BufferUnderflowError, params, index, start);
                  ui1 = *start;
                  if (++start >= end) start -= size;
                  hash = Hash16 (ui1, hash);
                  if (count == 1) {
                      // Varint 8 differs from Varint 2 and 4 in that on the 
                      // last byte, byte 9 of 9, there is no terminating varint 
                      // bit.
                      ui8 |= ((uint64_t)(ui1)) << ui2;
                      break;
                  }
                  ui8 |= ((uint64_t)(ui1 & 0x7F)) << ui2;
                  //< @todo I'm starting to second guess if we need to mask ui1 
                  //< because we're packing them up and will overwrite.
                  ui2 += 7;
                  if (--count == 0)
                      return Report (VarintOverflowError, params, index, start);
              }
              if (count == 9)    //< If there is only one byte we need to
                  ui8 &= 0x7F;        //< mask off the terminating varint bit.
              if (type == SV8)
                  ui8 = UnpackSignedVarint<uint64_t> (ui8);
              *ui8_ptr = ui8;
            break;
#else
            return Report (InvalidRxTypeError, param, 
                                              index, start);
#endif
          case AR1:  //< _R_e_a_d__A_r_r_a_y_-_1________________________________
#if USING_AR1
            // Load next pointer and increment args.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == nullptr)
                return Report (NullPointerError, params, index, start);
            count = *param++;
            count *= SizeOf (*param++);
            goto ReadBlock;
#else
            return Report (InvalidRxTypeError, params, index, start);
#endif
          case AR2:  //< _R_e_a_d__A_r_r_a_y_-_2________________________________
#if USING_AR2
              // Load the pointer to the destination.
              ui1_ptr = reinterpret_cast<byte*> (args[index]);
              if (ui1_ptr == nullptr)
                  return Report (NullPointerError, params, index, start);
              count = *param++;
              count *= SizeOf (*param++);
              goto ReadBlock;
#else
            return Report (InvalidRxTypeError, params, index, start);
#endif
          case AR4:  //< _R_e_a_d__A_r_r_a_y_-_4________________________________
#if USING_AR4
            // Load the pointer to the destination.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == nullptr)
                return Report (NullPointerError, params, index, start);
            count = *param++;
            count *= SizeOf (*param++);
            goto ReadBlock;
#else
            return Report (InvalidRxTypeError, params, index, start);
#endif
          case AR8:  //< _R_e_a_d__A_r_r_a_y_-_8________________________________
#if USING_AR8
            // Load the pointer to the destination.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == nullptr)
                return Report (NullPointerError, params, index, start);
            count = *param++;
            count *= SizeOf (*param++);
            goto ReadBlock;
#else
            return Report (InvalidRxTypeError, params, index, start);
#endif
          case ESC: //< _R_e_a_d__E_s_c_a_p_e__S_e_q_u_e_n_c_e__________________
            // I'm not sure exactly how this should work. I can't do recursion
            // because of embedded limitations.
            break;
          case BK8: //< _R_e_a_d__B_o_o_k_8_____________________________________
            if (length <= 128)
                return Report (BufferUnderflowError, params, index, start);
            // Load next pointer and increment args.
            ui8_ptr = reinterpret_cast<uint64_t*> (args[index]);
            if (ui8_ptr == nullptr)
                return Report (NullPointerError, params, index, start);

            for (ui2 = 0; ui2 <= sizeof (uint64_t); ui2 += 8) {
                ui1 = *start;
                if (++start >= end) start -= size;
                hash = Hash16 (ui1, hash);
                ui8 |= ((uint64_t)ui1) << ui2;
            }
            count = (uint_t)ui8;
            break;
#if USING_AR8
#else
            return Report (InvalidRxTypeError, params, index, start);
#endif
          case BK4: //< _R_e_a_d__B_o_o_k_4_______________________________________
            if (length <= 64)
                return Report (BufferUnderflowError, params, index, start);
            // Load the pointer to the destination.
            ui4_ptr = reinterpret_cast<uint32_t*> (args[index]);
            if (ui4_ptr == nullptr)
                return Report (NullPointerError, params, index, start);

            for (ui2 = 0; ui2 <= sizeof (uint32_t); ui2 += 8) {
                ui1 = *start;
                if (++start >= end) start -= size;
                hash = Hash16 (ui1, hash);
                ui4 |= ((uint32_t)ui1) << ui2;
            }
            count = (uint_t)ui4;
            break;
#if USING_BK4
#else
            return Report (InvalidRxTypeError, params, index, start);
#endif
          case BK2: //< _R_e_a_d__B_o_o_k_2_______________________________________
            if (length <= 32)
                return Report (BufferUnderflowError, params, index, start);
            // Load the pointer to the destination.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == nullptr)
                return Report (NullPointerError, params, index, start);
            ui1 = *start;
            if (++start >= end) start -= size;
            hash = Hash16 (ui1, hash);
            ui1 = *start;
            if (++start >= end) start -= size;
            hash = Hash16 (ui1, hash);
            ui2 |= ((uint16_t)ui1) << 8;
            count = (uint_t)ui2;
            goto ReadBlock;
            break;
#if USING_BK2
#else
            return Report (InvalidRxTypeError, params, index, start);
#endif
          case US: //< _R_e_a_d__U_n_i_t__S_e_p_e_r_a_t_o_r_____________________
            ReadBlock:
            {
                if (length < count)
                    return Report (BufferOverflowError, params, index, start);
                if (count == 0)
                    break;          //< Not sure if this is an error.
                if (start + count >= end) {
                    for (; size - count > 0; --count) {
                        ui1 = *start;
                        if (++start >= end) start -= size;
                        hash = Hash16 (ui1, hash);
                        *ui1_ptr = ui1;
                        ++ui1_ptr;
                    }
                    stop = begin - 1;
                    for (; count > 0; --count) {
                        ui1 = *start;
                        if (++start >= end) start -= size;
                        hash = Hash16 (ui1, hash);
                        *ui1_ptr = ui1;
                        ++ui1_ptr;
                    }
                    break;
                }
               for (; count > 0; --count) {
                    ui1 = *start;
                   if (++start >= end) start -= size;
                    hash = Hash16 (ui1, hash);
                    *ui1_ptr = ui1;
                    ++ui1_ptr;
                }
                break;
            }
          default:
            RxInvalidType:
            {
                printf ("\n!!!Read invalid type %u\n", type);
                return Report (ReadInvalidTypeError, params, index, start);
            }
        }
        std::cout << " |";
    }
    printf ("\n| Hash expected: %x ", hash);
    ui2 = *start;
    if (++start >= end) start -= size;
    ui1 = *start;
    if (++start >= end) start -= size;
    ui2 |= (((uint16_t)ui1) << 8);
    printf ("found: %x\n", ui2);
    if (hash != ui2)
        return Report (InvalidHashError, params, index, start);

#if DEBUG_CHINESEROOM
    printf ("| Done reading\n");
    RingBufferClear (begin, rx->start, start, stop, end, size);
#endif

    // Convert pointer back to offset
    rx->start = Diff (begin, start);

    return 0;
}

/** Scans a message with the given params to the given Terminal. */
inline ticket_t Read (Tx* tx, const uint_t* params, void** args) {
    return Read (reinterpret_cast<Rx*>(tx), params, args);
}

/** Returns true if the Rx buffer contains any data.
    @warning Function does not do any error checking for speed.
*/
inline bool IsReadable (Rx* rx) {
    return rx->start != rx->stop;
}

}       //< namespace _
#endif  //< CHINESEROOM_RXSLOT_H

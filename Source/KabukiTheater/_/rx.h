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

#include "tx.h"

namespace _ {

/*< A Rx socket. */
struct Rx
{
    uint_t size,            //< The size of the buffer.
        start;              //< The starting index of the ring buffer data.
    volatile uint_t stop;   //< The stopping index of the ring buffer data.
    uint_t read;            //< The read variable.
};

/** Initializes the Rx struct to an empty buffer. */
inline Rx* InitRx (Rx* rx, uint_t size) {
    if (rx == nullptr)
        return nullptr;
    rx->size = size - sizeof (Rx);
    rx->start = 0;
    rx->stop = 0;
    rx->read = 0;
    return rx;
}

/** Calculates the used ring buffer space.
    @param  rx The Rx buffer. */
inline uint_t CalcRingSegmentLength (Rx* rx) {
    if (rx == nullptr) return ~0;
    byte* rxb_ptr = reinterpret_cast<byte*>(rx);
    return CalcRingSegmentLength (rxb_ptr + rx->start, rxb_ptr + rx->stop, rx->size);
}

/** Gets the start of the Rx ring buffer. */
inline byte* SlotBaseAddress (void* ptr) {
    return reinterpret_cast <byte*>(ptr) + kSlotHeaderSize;
}

/** Gets the start of the Rx ring buffer. */
inline byte* SlotBaseAddress (Rx* ptr) {
    return reinterpret_cast <byte*>(ptr) + kSlotHeaderSize;
}

/** Gets the start of the Rx ring buffer. */
inline byte* SlotBaseAddress (Rx& ptr) {
    return reinterpret_cast <byte*>(&ptr) + kSlotHeaderSize;
}

/** Gets the rx buffer length. */
inline uint_t GetRxSpace (Rx* rx) {
    if (rx == nullptr) return ~0;

    byte* base = SlotBaseAddress (rx);
    return CalcRingBufferSpace (base + rx->start, base + rx->stop, rx->size);
}

/** Gets the start of the Rx ring buffer. */
inline byte* SlotBaseAddress (void* ptr, uint_t rx_tx_offset) {
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

/** Scans a message with the given header to the given Terminal. */
static ticket_t Read (Rx* rx, const uint_t* header, void** ptrs) {
    if (rx == nullptr)
        ReportError (NullPointerError, 0, 0, 0);
    if (header == nullptr)
        ReportError (NullHeaderError, 0, 0, 0);

    // Temp variables packed into groups of 8 bytes for memory alignment.
    byte type,
        ui1;
#if USING_2_BYTE_TYPES
    uint16_t ui2;
#endif
#if USING_4_BYTE_TYPES
    uint32_t ui4;
#endif
#if USING_8_BYTE_TYPES
    uint64_t ui8;
#endif

    byte*     ui1_ptr;      //< Pointer to a UI1, not ui1.
#if USING_2_BYTE_TYPES
    uint16_t* ui2_ptr;      //< Pointer to a UI1, not ui1.
#endif
#if USING_4_BYTE_TYPES
    uint32_t* ui4_ptr;      //< Pointer to a UI1, not ui1.
#endif
#if USING_4_BYTE_TYPES
    uint64_t* ui8_ptr;      //< Pointer to a UI1, not ui1.
#endif

    uint_t num_params = *header,
        size,
        length,
        temp,
        index;
    //collisionsSize;
    //byte alignOffset;

    if (num_params == 0) return 0;   //< Nothing to do.

    size = rx->size;

    byte* begin = SlotBaseAddress (rx),
        *end = begin + size,
        *start = begin + rx->start,
        *stop = begin + rx->stop;
    //* temp_ptr;     //< A temporary pointer for storing memory alignment.

    length = CalcRingSegmentLength (start, stop, size);

    printf ("Reading: begin: %u start %u stop:%u end:%u\n"
        "length: %u\nnumParameters: %u Params: ",
        Diff (rx, begin), Diff (rx, start), Diff (rx, stop),
        Diff (rx, end), length, num_params);
    PrintParams (header);

    // When we scan, we are reading from the beginning of the Rx buffer.

    for (index = num_params; index < num_params; ++index) {
        ++header;
        type = *header;

        printf ("Reading type: %s\n", TypeString (type));

        temp = GetAlignment (type);

        switch (type)
        {
          case NIL:
          case SOH: //< _R_e_a_d__S_t_r_i_n_g_-_8_______________________________
          case STX:
            // Load next pointer and increment ptrs.
            ui1_ptr = reinterpret_cast<byte*> (ptrs[index]);

            if (ui1_ptr == nullptr) break;

            ++header;
            temp = *header;

            printf ("Reading: ");

            if (temp == 0) break;

            ui1 = *start;
            *ui1_ptr = ui1;

            putchar (static_cast<char> (ui1));

            while (ui1 != 0) {
                if (--temp == 0) 
                    return ReportError (BufferUnderflowError, 
                                                      header, index, start);
                ++start;
                ui1 = *start;       // Read byte
                ++ui1_ptr;
                *ui1_ptr = ui1;     // Write byte.

                putchar (static_cast<char> (ui1));

                if (++start >= end) start -= size;
            }
            if (type == SOH) break;
            putchar ('\0');
            break;

          case SI1: //< _R_e_a_d__1__B_y_t_e__T_y_p_e_s_________________________
          case UI1:
          case BOL:
#if USING_1_BYTE_TYPES
            if (length == 0) 
                return ReportError (BufferUnderflowError, header, 
                                                  index, start);
            --length;

            // No need to memory align 1 byte.

            // Load next pointer and increment ptrs.
            ui1_ptr = reinterpret_cast<byte*> (ptrs[index]);


            if (ui1_ptr == 0) break;
            ui1 = *start;
            if (++start >= end) start -= size;
            break;
#else
            return ReportError (UnsupportedTypeError, header, 
                                              index, start);
#endif

          case SI2: //< _R_e_a_d__1_6_-_b_i_t__T_y_p_e_s________________________
          case UI2:
#if USING_2_BYTE_TYPES
            if (length < 2) return ReportError 
                (BufferUnderflowError, header, index, start);


            length -= 2;

            // Memory align and check for zero padding.
            if (( (uintptr_t)stop) & 0x1)
            {
                if ((ui1 = *stop) != 0)
                {

                }
            }

            // Load next pointer and increment ptrs.
            ui2_ptr = reinterpret_cast<uint16_t*> (ptrs[index]);


            if (ui2_ptr == 0) break; //< Not sure if we allow null or not yet.
            *ui2_ptr = *start;
            if ((start += 2) >= end) start -= size;
            break;
#else
            return ReportError (UnsupportedTypeError, header, 
                                              index, start);
#endif

          case SI4: //< _R_e_a_d__3_2_-_b_i_t__T_y_p_e_s________________________
          case UI4:
          case FLT:
          case TMS:
#if USING_4_BYTE_TYPES
            if (length < 4) 
                return ReportError (BufferUnderflowError, header, 
                                                  index, start);
            length -= 4;

            // Load next pointer and increment ptrs.
            ui4_ptr = reinterpret_cast<uint32_t*> (ptrs[index]);


            *ui4_ptr = *((uint32_t*)stop);
            if ((stop += 4) > end) start -= size;
            break;
#else
            return ReportError (UnsupportedTypeError, header, 
                                              index, start);
#endif

          case TMU: //< _R_e_a_d__6_4_-_b_i_t__T_y_p_e_s________________________
          case SI8:
          case UI8:
          case DBL:
#if USING_8_BYTE_TYPES
            if (length < 8) 
                return ReportError (BufferUnderflowError, 
                                                  header, index, start);
            length -= 8;

            // Load next pointer and increment ptrs.
            ui8_ptr = reinterpret_cast<uint64_t*> (ptrs[index]);


            *ui8_ptr = * ((uint64_t*)stop);
            if ((stop += 8) >= end) start -= size;
            break;
#else
            return ReportError (UnsupportedTypeError, header, 
                                              index, start);
#endif

          case SV2: //< _R_e_a_d__2_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t_________

#if USING_VARINT2
                    // Load next pointer and increment ptrs.
            ui4_ptr = reinterpret_cast<uint32_t*> (ptrs[index]);


            if (ui4 >> 15) ui4 = 0x1 & (( (~ui4) + 1) << 1);
            else               ui4 = ui4 << 1;

            // Scan byte 1
            ui1 = *start;
            ui4 = ui1;
            if (++start >= end) start -= size;

            while (ui1 >> 7 != 0)
            {
                if (--length == 0)
                    return ReportError (BufferUnderflowError, 
                                                      header, index, start);
                ui1 = * (++start);
                ui4 |= ((uint32_t)ui1) << (1 * 7);
                if (++start >= end) start -= size;
            }
            *ui4_ptr = (ui4 << 31) == 0 ? ui4 : (~ui4) - 1;
            break;

          case UV2: //< _R_e_a_d__2_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t_____
            if (length == 0) 
                return ReportError (BufferUnderflowError, header, 
                                                  index, start);

            // Load next pointer and increment ptrs.
            ui4_ptr = reinterpret_cast<uint32_t*> (ptrs[index]);


            // Scan byte 1.
            ui1 = *start;
            ui4 = ui1;
            if (++start >= end) start -= size;

            while (ui1 >> 7 != 0)
            {
                if (--length == 0) 
                    return ReportError (BufferUnderflowError, 
                                                      header, index, start);
                ui1 = * (++start);
                ui4 |= ((uint32_t)ui1) << (1 * 7);
                if (++start >= end) start -= size;
            }
            *ui4_ptr = ui4;
            break;
#else
          case UV2:
            return ReportError (UnsupportedTypeError, header, 
                                              index, start);
#endif

          case SV4: //< _R_e_a_d__4_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t_________

#if USING_VARINT4
                    // Load next pointer and increment ptrs.
            ui4_ptr = reinterpret_cast<uint32_t*> (ptrs[index]);


            if (ui4 >> 15) ui4 = 0x1 & (( (~ui4) + 1) << 1);
            else               ui4 = ui4 << 1;

            // Scan byte 1
            ui1 = *start;
            ui4 = ui1;
            if (++start >= end) start -= size;

            while (ui1 >> 7 != 0)
            {
                if (--length == 0)
                    return ReportError (BufferUnderflowError, 
                                                      header, index, start);
                ui1 = * (++start);
                ui4 |= ((uint32_t)ui1) << (1 * 7);
                if (++start >= end) start -= size;
            }
            *ui4_ptr = (ui4 << 31) == 0 ? ui4 : (~ui4) - 1;
            break;

          case UV4: //< _R_e_a_d__4_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t_____
            if (length == 0) 
                return ReportError (BufferUnderflowError, header, 
                                                  index, start);

            // Load next pointer and increment ptrs.
            ui4_ptr = reinterpret_cast<uint32_t*> (ptrs[index]);


            // Scan byte 1.
            ui1 = *start;
            ui4 = ui1;
            if (++start >= end) start -= size;

            while (ui1 >> 7 != 0)
            {
                if (--length == 0) 
                    return ReportError (BufferUnderflowError, 
                                                      header, index, start);
                ui1 = * (++start);
                ui4 |= ((uint32_t)ui1) << (1 * 7);
                if (++start >= end) start -= size;
            }
            *ui4_ptr = ui4;
            break;
#else
          case UV4:
            return ReportError (UnsupportedTypeError, header, 
                                              index, start);
#endif

          case SV8: //< _R_e_a_d__8_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t_________

#if USING_VARINT8
                    // Load next pointer and increment ptrs.
            ui8_ptr = reinterpret_cast<uint64_t*> (ptrs[index]);


            if (ui8 >> 15) ui8 = 0x1 & (( (~ui8) + 1) << 1);
            else               ui8 = ui8 << 1;

            // Byte 1
            ui1 = *start;
            ui8 = ui1;
            if (++start >= end) start -= size;

            while (ui1 >> 7 != 0)
            {
                if (--length == 0)
                    return ReportError (BufferUnderflowError, 
                                                      header, index, start);
                ui1 = * (++start);
                ui8 |= ((uint64_t)ui1) << (1 * 7);
                if (++start >= end) start -= size;
            }
            *ui8_ptr = (ui8 << 63) == 0 ? ui8 : (~ui8) - 1;
            break;

          case UV8: //< _R_e_a_d__8_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t_____
            if (length == 0) 
                return ReportError (BufferUnderflowError, header, 
                                                  index, start);

            // Load next pointer and increment ptrs.
            ui8_ptr = reinterpret_cast<uint64_t*> (ptrs[index]);


            // Scan byte 1
            ui1 = *start;
            ui8 = ui1;
            if (++start >= end) start -= size;

            while (ui1 >> 7 != 0)
            {
                if (--length == 0) 
                    return ReportError (BufferUnderflowError, header, 
                                                      index, start);
                ui1 = * (++start);
                ui8 |= ((uint64_t)ui1) << (1 * 7);
                if (++start >= end) start -= size;
                if (ui1 >> 7 == 0) { *ui8_ptr = ui8; break; }
            }
            *ui8_ptr = ui8;
            break;
#else
          case UV8:
            return ReportError (UnsupportedTypeError, header, 
                                              index, start);
#endif
            
          case AR1:  //< _R_e_a_d__A_r_r_a_y_-_1________________________________
#if USING_AR1
            if (--length == 0) 
                return ReportError (BufferUnderflowError, header, 
                                                  index, start);

            // Load next pointer and increment ptrs.
            ui1_ptr = reinterpret_cast<byte*> (ptrs[index]);


            // Load array size.
            ui1 = *start;

            if ((start > stop) && (start + ui1 >= end))
            {
                // Find length of upper chunk.
                temp = (uint_t) (end - start);

                memcpy (ui1_ptr, start, temp);
                memcpy (ui1_ptr + temp, begin, (size_t) (ui1 - temp));
                break;
            }
            memcpy (ui1_ptr, ui1_ptr, (size_t)ui1);
            break;
#else
            return ReportError (UnsupportedTypeError, header, 
                                              index, start);
#endif

          case AR2:  //< _R_e_a_d__A_r_r_a_y_-_2________________________________
#if USING_AR2
            alignOffset = WordAlignOffset (start);
            if (length < sizeof (uint16_t) + alignOffset) 
                return ReportError (BufferUnderflowError, header, 
                                                  index, start);
            start -= (start + alignOffset >= end) * size;

            // Load next pointer and increment ptrs.
            ui2_ptr = reinterpret_cast<uint16_t*> (ptrs[index]);


            ui2 = *ui2_ptr;

            // Copy array into memory.
            if ((start > stop) && (start + ui2 >= end))
            {
                // Find length of upper chunk.
                temp = (uint_t) (end - start);

                memcpy (ui2_ptr, start, temp);
                memcpy (ui2_ptr + temp, begin, (size_t) (ui2 - temp));
                break;
            }
            memcpy (ui2_ptr, ui2_ptr + ui2, (size_t)ui2);
            break;
#else
            return ReportError (UnsupportedTypeError, header, 
                                              index, start);
#endif

          case AR4:  //< _R_e_a_d__A_r_r_a_y_-_4________________________________
#if USING_AR2
#else
            return ReportError (UnsupportedTypeError, header, 
                                              index, start);
#endif

          case AR8:  //< _R_e_a_d__A_r_r_a_y_-_8________________________________
#if USING_AR8
#else
            return ReportError (UnsupportedTypeError, header, 
                                              index, start);
#endif

          case ESC: //< _R_e_a_d__E_s_c_a_p_e__S_e_q_u_e_n_c_e__________________
            if (length == 0) 
                return ReportError (BufferUnderflowError, header, 
                                                  index, start);
            --length;

            // Load next pointer and increment ptrs.
            ui1_ptr = static_cast<byte*> (ptrs[index]);


            if (ui1_ptr == 0)
            {
                // This isn't an error, but we need to adjust the start pointer 
                // to just over the data.
                break;
            }
            ui1 = *start;
            if (++start >= end) start -= size;
            break;

          case BK8: //< _R_e_a_d__B_o_o_k_8_____________________________________

#if USING_AR8
#else
            return ReportError (UnsupportedTypeError, header, 
                                              index, start);
#endif

          case BK4: //< _R_e_a_d__B_o_o_k_4_______________________________________

#if USING_BK4
#else
            return ReportError (UnsupportedTypeError, header, 
                                              index, start);
#endif

          case BK2: //< _R_e_a_d__B_o_o_k_2_______________________________________

#if USING_BK2
            alignOffest = WordAlignOffset (start);
            start
                if ((start += sizeof (Book2)) >= end)
                {
                    start -= size;
                }

            // To save stack space, we have to manually load the book header 
            // and calculate size.

            start = WordAlignOffset (start, begin, end, sizeof (Book2));

            // Load numKeys.
            ui2 = *start;
            temp = ui2; // Store temp variable

            // Load bufferSize.
            ui2 = * (start + sizeof (byte));
            temp += ui2;    // Add the stored temp variable.
            temp *= (sizeof (uint16_t) + sizeof (uint16_t) + 
                (collisionsSize > 0) * (sizeof (byte) + sizeof (primeHash)));
            temp += collisionsSize;


            ui2_ptr = reinterpret_cast<uint16_t*> (start);
            // Load headerSize.
            ui2 = * (ui2_ptr + 2 * sizeof (uint16_t));
            temp += ui2;
            start += sizeof (uint16_t);

            // Load dataSize.
            ui2 = * (ui2_ptr + 3 * sizeof (uint16_t));
            temp += ui2;
            start += sizeof (uint16_t);

            // add the size of the header and we're done.
            temp += 5 * sizeof (uint16_t);

            if (length < temp)
                return ReportError (BufferUnderflowError, header, 
                                                  index, start);
            length -= (uint_t)temp;

            ui2_ptr = reinterpret_cast<uint16_t*> (ptrs);
            if(ui2_ptr == nullptr) {
                // This isn't an error because the user might want to pass in a 
                // nullptr.
                break;
            }

            // Now we can copy the book into memory.
            if ((start > stop) && (start + temp >= end))
            {
                // Calculate upper chunk size.
                temp = end - stop;
                temp -= temp;

                memcpy (start, ui2_ptr, temp);
                memcpy (begin, ui2_ptr + temp, (size_t)temp);
                start = begin + temp;
                break;
            }
            memcpy (stop, ui2_ptr, (size_t)temp);
            start += temp;
            break;
#else
            return ReportError (UnsupportedTypeError, header, 
                                              index, start);
#endif
          case US: //< _R_e_a_d__U_n_i_t__S_e_p_e_r_a_t_o_r_____________________
            break;

        default:
            return ReportError (ReadInvalidTypeError, header, 
                                              index, start);
        }
    }

    rx->start = Diff (stop, begin);
    return 0;
}

/** Scans a message with the given header to the given Terminal. */
inline ticket_t Read (Tx& tx, const uint_t* header, void** ptrs) {
    return Read (reinterpret_cast<Rx*>(&tx), header, ptrs);
}

/** Scans a message with the given header to the given Terminal. */
inline ticket_t Read (Tx* tx, const uint_t* header, void** ptrs) {
    return Read (reinterpret_cast<Rx*>(&tx), header, ptrs);
}

}       //< namespace _
#endif  //< CHINESEROOM_RXSLOT_H

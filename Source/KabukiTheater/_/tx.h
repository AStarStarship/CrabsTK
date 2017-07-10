/** The Chinese Room
    @version 0.x
    @file    /.../tx.h
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

#ifndef CHINESEROOM_TX_H
#define CHINESEROOM_TX_H

#include "error.h"
#include "types.h"
#include "address.h"
#include "utils.h"

namespace _ {

/** A Tx socket.
    A Tx socket is identical in structure to an Rx socket, but the stop becomes
    volatile and start is not volatile.
*/
struct Tx {
    uint_t size;            //< The size of the ring buffers.
    volatile uint_t start;  //< The starting index of the ring buffer data.
    uint_t stop,            //< The stopping index of the ring buffer data.
        read;               //< The address that the Rx device is reading from.
};

/** Initializes the Tx given struct to an empty buffer. */
inline void InitTx (Tx* tx, uint_t size)
{
    if (tx == nullptr) return;
    tx->size = size;
    tx->start = 0;
    tx->stop = 0;
    tx->read = 0;
}

enum {
    kSlotHeaderSize = sizeof (Tx) + sizeof (uintptr_t) -
        sizeof (Tx) % sizeof (uintptr_t),
    //< Offset to the start of the ring buffer.
    kMinSlotSize = kMinTerminalSize / 2,
};

/** Gets the start of the Tx ring buffer. */
inline byte* SlotBaseAddress (Tx& tx) {
    return reinterpret_cast<byte*>(&tx) + kSlotHeaderSize;
}

/** Gets the start of the Tx ring buffer. */
inline byte* SlotBaseAddress (Tx* tx) {
    return reinterpret_cast<byte*>(tx) + kSlotHeaderSize;
}

/** Initializes the Tx buffer struct at the start of the buffer. */
inline Tx* Init (Tx* tx, uint_t size) {
    if (tx == nullptr)
        return nullptr;
    tx->size = size - kSlotHeaderSize;
    tx->start = 0;
    tx->stop = 0;
    tx->read = 0;
    return tx;
}

inline Tx* ConstructTx (uint_t size) {
    byte* buffer = New<uint_t, byte> (size, kMinSlotSize);
    if (buffer == nullptr) return nullptr;

    if (buffer == nullptr) return nullptr;

    Tx* tx = reinterpret_cast<Tx*> (buffer);
    tx->size = size - kSlotHeaderSize;
    tx->start = 0;
    tx->stop = 0;
    tx->read = 0;

#if DEBUG
    memset (SlotBaseAddress(tx), '\0', size);
#endif
    return tx;
}

inline Tx* ConstructTx (byte* buffer, uint_t size) {
    if (size < kMinSlotSize) return nullptr;
    if (buffer == nullptr) return nullptr;

    Tx* tx = reinterpret_cast<Tx*> (buffer);
    tx->size = size - kSlotHeaderSize;
    tx->start = 0;
    tx->stop = 0;
    tx->read = 0;

#if DEBUG
    memset (SlotBaseAddress (tx), '\0', size);
#endif
    return tx;
}

/** Returns a reference to the log for this assembly. */
static Tx& Log () {
    static byte buffer[kLogSize];
    static Tx* tx = ConstructTx (kLogSize);
    return reinterpret_cast<Tx&>(tx);
}

/** Calculates the space left in the given ring buffer.
    @param  tx The Tx buffer.
*/
inline uint_t CalcRingBufferSpace (Tx* tx) {
    if (tx == nullptr) return ~0;
    byte* txb_ptr = reinterpret_cast<byte*>(tx);
    return CalcRingBufferSpace (txb_ptr + tx->start, txb_ptr + tx->stop, tx->size);
}

/** Gets the tx buffer space. */
inline uint_t GetTxBufferLength (Tx* tx) {
    if (tx == nullptr) return ~0;
    byte* base = SlotBaseAddress (tx);
    return CalcRingSegmentLength (base + tx->start, base + tx->stop, tx->size);
}

/** Gets the end address of the tx buffer. */
inline byte* GetEndAddress (Tx* tx) {
    return reinterpret_cast<byte*>(tx) + kSlotHeaderSize + tx->size;
}

#if DEBUG_CHINESEROOM

static void Print (Tx* tx) {

}

#endif  //< DEBUG_CHINESEROOM

/** Prints a message with the given header to the given Tx slot. */
static ticket_t Write (Tx& tx, const uint_t header[], const void** ptrs) {
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

    const uint_t num_params = *header;

    if (num_params == 0) return 0;    //< Nothing to do.

    uint_t size,
        buffer_space,
        index;

    size = tx.size;

    byte* begin = SlotBaseAddress (tx),
        *end = begin + size,
        *start = begin + tx.start,
        *stop = begin + tx.stop;
    //  * temp_ptr;
    const byte* ui1_ptr;
#if USING_2_BYTE_TYPES
    const uint16_t* ui2_ptr;
#endif
#if USING_4_BYTE_TYPES
    const uint32_t* ui4_ptr;
#endif
#if USING_8_BYTE_TYPES
    const uint64_t* ui8_ptr;
#endif

    buffer_space = CalcRingBufferSpace (start, stop, size);
    printf ("start: %p stop: %p size: %u buffer_space: %u\n", start, stop,
            size, buffer_space);

    ++header;

    //const char** strings = reinterpret_cast<const char**> (ptrs);
    //printf ("\ntestStrings at start\nstring1: %s\nstring2: %s\n", strings[0],
    //        strings[1]);

    printf ("write num_params: %u\n", num_params);

    // Write data.
    for (index = 0; index < num_params; ++index) {
        ++header;
        type = *header;
        printf ("%i:%u:%s\n", index, type, TypeString (type));

        switch (type) {
            case NIL:
                break;

            case SOH: //< _W_r_i_t_e__A_d_d_r_e_s_s__S_t_r_i_n_g________________
            case STX: //< _W_r_i_t_e__S_t_r_i_n_g_______________________________
                        // Check if the buffer has enough room.
                if (buffer_space == 0)
                    return ReportError (BufferUnderflowError, header, index,
                                        start);

                --buffer_space;

                //strings = reinterpret_cast<const char**> (ptrs);
                //printf ("\ntestStrings at after: 0x%p\nstring1: %s\n"
                //        "string2: %s\n", ptrs, strings[0], strings[1]);

                // Load the source data pointer and increment ptrs.fs
                ui1_ptr = reinterpret_cast<const byte*> (ptrs[index]);

                //printf ("Before trying to print 0x%p: %s\n", ui1_ptr, ui1_ptr);
                //print ();
                //printf ("\nWriting chars: ");

                // We know we will always write at least one null-term char.
                ui1 = *ui1_ptr;
                *stop = ui1;

                putchar (static_cast<char> (ui1));

                while (ui1 != 0) {
                    if (buffer_space-- == 0)
                        return ReportError (BufferUnderflowError, header,
                                            index, start);

                    ++ui1_ptr;
                    ui1 = *ui1_ptr;     // Read byte.

                    putchar (static_cast<char> (ui1));

                    *stop = ui1;        // Write byte
                    ++stop;

                    if (stop >= end) stop -= size;
                }
                //printf ("\nAfter\n");
                //print ();
                if (type == SOH) {
                    *stop = ui1;        // Write byte
                    ++stop;
                    break;
                }

                break;

            case SI1: //< _W_r_i_t_e__8_-_b_i_t__T_y_p_e_s______________________
            case UI1:
            case BOL:
#if USING_1_BYTE_TYPES
                // Check if the buffer has enough room.
                if (buffer_space-- == 0)
                    return ReportError (BufferUnderflowError,
                                        header, index, start);

                // Load next pointer and increment ptrs.
                ui1_ptr = reinterpret_cast<const byte*> (ptrs[index]);


                // Write the data and increment the stop to wrap around to the
                // beginning.
                *stop = *ui1_ptr;
                if (++stop >= end) stop -= size;
                break;
#else
                return ReportError (UnsupportedTypeError, header,
                                    index, start);
#endif

            case SI2: //< _W_r_i_t_e__1_6_-_b_i_t__T_y_p_e_s____________________
            case UI2:
#if USING_2_BYTE_TYPES
                // Align the buffer to a word boundary and check if the 
                // buffer has enough room.
                alignOffset = WordAlignOffset16 (stop);
                if (buffer_space < alignOffset + sizeof (uint16_t))
                    return ReportError (BufferUnderflowError,
                                        header, index, start);
                buffer_space -= alignOffset + sizeof (uint16_t);
                stop += alignOffset;

                // Load value to write and increment ptrs.
                ui2_ptr = reinterpret_cast<const uint16_t*> (ptrs[index]);
                ui2 = *ui2_ptr;

                // Write the data and increment the stop to wrap around to the 
                // beginning.
                if ((stop + sizeof (uint16_t)) >= end) stop -= size;
                ui2_ptr = reinterpret_cast<uint16_t*> (stop);
                *ui2_ptr = ui2;
                stop += sizeof (uint16_t);
                break;
#else
                return ReportError (UnsupportedTypeError, header,
                                    index, start);
#endif

            case SI4: //< _W_r_i_t_e__3_2_-_b_i_t__T_y_p_e_s______________________
            case UI4:
            case FLT:
            case TMS:
#if USING_4_BYTE_TYPES
                // Align the buffer to a word boundary and check if the buffer
                // has enough room.
#if WORD_SIZE == 16
                alignOffset = WordAlignOffset16 (stop);
#else
                alignOffset = WordAlignOffset32 (stop);
#endif
                if (buffer_space < alignOffset + sizeof (uint32_t))
                    return ReportError (BufferUnderflowError, header, index,
                                        start);
                buffer_space -= alignOffset + sizeof (uint32_t);
                stop += alignOffset;

                // Load value to write and increment ptrs.
                ui4_ptr = reinterpret_cast<const uint32_t*> (ptrs[index]);
                ui4 = *ui4_ptr;

                // Write the data and increment the stop to wrap around to the 
                // beginning.
                if ((stop + sizeof (uint32_t)) >= end) stop -= size;
                ui4_ptr = reinterpret_cast<uint32_t*> (stop);
                *ui4_ptr = ui4;
                stop += sizeof (uint32_t);
                break;
#else
                return ReportError (UnsupportedTypeError, header,
                                    index, start);
#endif

            case TMU: //< _W_r_i_t_e__6_4_-_b_i_t__T_y_p_e_s______________________
            case SI8:
            case UI8:
            case DBL:
#if USING_8_BYTE_TYPES
                // Align the buffer to a word boundary and check if the buffer
                // has enough room.
                alignOffset = WordAlignOffset (stop);
                if (buffer_space < alignOffset + sizeof (uint64_t))
                    return ReportError (BufferUnderflowError, header, index,
                                        start);
                buffer_space -= alignOffset + sizeof (uint64_t);
                stop += alignOffset;

                // Load value to write and increment ptrs.
                ui8_ptr = reinterpret_cast<const uint64_t*> (ptrs[index]);
                ui8 = *ui8_ptr;

                // Write the data and increment the stop to wrap around to the
                // beginning.
                if ((stop + sizeof (uint64_t)) >= end) stop -= size;
                ui8_ptr = reinterpret_cast<uint64_t*> (stop);
                *ui8_ptr = ui8;
                stop += sizeof (uint64_t);
                break;
#else
                return ReportError (UnsupportedTypeError, header,
                                    index, start);
#endif

            case SV2: //< _W_r_i_t_e__2_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t_____

#if USING_2_BYTE_VARINT
// Check to see we at least have one byte left in the buffer.
                if (buffer_space == 0)
                    return ReportError (BufferUnderflowError, header, index,
                                        start);

                // Load number to write and increment ptrs.
                ui2_ptr = reinterpret_cast<const uint16_t*> (ptrs[index]);

                ui2 = *ui2_ptr;

                // We are using the same code to print both signed and unsigned 
                // varints. In order to convert from a negative 2's complement 
                // signed integer to a transmittable format, we need to invert the 
                // bits and add 1. Then we just shift the bits left one and put the
                // sign bit in the LSB.

                if (ui2 >> ((sizeof (ui2) * 8 - 1)))
                    ui2 = 0x1 & (((~ui2) + 1) << 1);
                else
                    ui2 = ui2 << 1;

                goto WriteUV2;

            case UV2: //< _W_r_i_t_e__2_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t_

                if (buffer_space == 0)
                    return ReportError (BufferUnderflowError, header, index,
                                        start);

                // Load next pointer and increment ptrs.
                ui2_ptr = reinterpret_cast<const uint16_t*> (ptrs[index]);


                ui4 = *ui2_ptr;

                WriteUV2:
                if (--buffer_space == 0)
                    return ReportError (BufferUnderflowError, header, index,
                                        start);

                ui1 = ((byte)ui2) & 0x7f;
                ui2 = ui2 >> 7;
                if (++stop >= end) stop -= size;
                if (ui2 == 0) {
                    *stop = ui1 | 0x80;
                    break;
                }
                *stop = ui1;

                ui1 = ((byte)ui2) & 0x7f;
                ui2 = ui2 >> 7;
                if (++stop >= end) stop -= size;
                if (ui2 == 0) {
                    *stop = ui1 | 0x80;
                    break;
                }
                *stop = ui1;

                break;
#else
            case UV2:
                return ReportError (UnsupportedTypeError, header, index,
                                    start);
#endif

            case SV4: //< _W_r_i_t_e__4_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t_______

#if USING_4_BYTE_VARINT
        // Load next pointer and increment ptrs.
                ui4_ptr = reinterpret_cast<const uint32_t*> (ptrs[index]);


                ui4 = *ui4_ptr;

                // We are using the same code to print both signed and unsigned 
                // varints. In order to convert from a negative 2's complement 
                // signed integer to a transmittable format, we need to invert the 
                // bits and add 1. Then we just shift the bits left one and put the
                // sign bit in the LSB.

                if (ui4 >> ((sizeof (ui4) * 8 - 1)))
                    ui4 = 0x1 & (((~ui4) + 1) << 1);
                else
                    ui4 = ui4 << 1;

                goto WriteUV4;


            case UV4: //< _W_r_i_t_e__4_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t___

                if (buffer_space == 0)
                    return ReportError (BufferUnderflowError, header, index,
                                        start);

                // Load next pointer and increment ptrs.
                ui4_ptr = reinterpret_cast<const uint32_t*> (ptrs[index]);


                ui4 = *ui4_ptr;

                WriteUV4:   //< Manual do while loop to optimize loop exit.
                {

                    if (--buffer_space == 0)
                        return ReportError (BufferUnderflowError, header,
                                            index, start);

                    ui1 = ((byte)ui4) & 0x7f;
                    ui4 = ui4 >> 7;
                    if (++stop >= end) stop -= size;
                    if (ui4 == 0) {
                        *stop = ui1 | 0x80;
                        break;
                    }
                    *stop = ui1;

                    goto WriteUV4;
                }
#else
            case UV4:
                return ReportError (UnsupportedTypeError, header, index,
                                    start);
#endif

            case SV8: //< _W_r_i_t_e__8_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t_____

#if USING_VARINT8
        // Load next pointer and increment ptrs.
                ui8_ptr = reinterpret_cast<const uint64_t*> (ptrs[index]);


                ui8 = *ui8_ptr;

                // We are using the same code to print both signed and unsigned
                // varints. In order to convert from a negative 2's complement
                // the signed integer to a transmittable format, we need to
                // invert bits and add 1. Then we just shift the bits left one
                // and put the sign bit in the LSB.

                if (ui8 >> ((sizeof (ui8) * 8 - 1)))
                    ui8 = 0x1 & (((~ui8) + 1) << 1);
                else
                    ui8 = ui8 << 1;

                goto WriteUV8;

            case UV8: //< _W_r_i_t_e__8_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t_

                if (buffer_space == 0)
                    return ReportError (BufferUnderflowError, header, index,
                                        start);

                // Load next pointer and increment ptrs.
                ui8_ptr = reinterpret_cast<const uint64_t*> (ptrs[index]);


                ui8 = *ui8_ptr;

                WriteUV8:   //< Manual do while loop to optimize loop exit.
                {

                    if (--buffer_space == 0)
                        return ReportError (BufferUnderflowError, header,
                                            index, start);

                    ui1 = ((byte)ui8) & 0x7f;
                    ui8 = ui8 >> 7;
                    if (++stop >= end) stop -= size;
                    if (ui8 == 0) {
                        *stop = ui1 | 0x80;
                        break;
                    }
                    *stop = ui1;

                    goto WriteUV8;
                }
#else
            case UV8:
                return ReportError (UnsupportedTypeError, header, index,
                                    start);
#endif

            case AR1:  //< _W_r_i_t_e__A_r_r_a_y________________________________

#if USING_AR1
        // Load next pointer and increment ptrs.
                ui1_ptr = reinterpret_cast<const byte*> (ptrs[index]);

                ui1 = *ui1_ptr;

                // Copy the book into memory.
                if ((start > stop) && (start + ui1 >= end)) {
                    // Calculate upper chunk size.
                    temp = end - stop;
                    temp -= temp;

                    memcpy (stop, ui2_ptr, temp);
                    memcpy (begin, ui2_ptr + temp, (size_t)temp);
                    stop = begin + temp;
                    break;
                }
                memcpy (stop, ui2_ptr, (size_t)temp);
                stop += temp;
                break;
#else
                return ReportError (UnsupportedTypeError, header, index,
                                    start);
#endif

            case AR2:  //< _W_r_i_t_e__A_r_r_a_y_2______________________________

#if USING_AR2
        // Load next pointer and increment ptrs.
                ui2_ptr = reinterpret_cast<const uint16_t*> (ptrs[index]);

                ui2 = *ui2_ptr;

                // Copy the book into memory.
                if ((start > stop) && (start + ui2 >= end)) {
                    // Calculate upper chunk size.
                    temp = end - stop;
                    temp -= temp;

                    memcpy (stop, ui2_ptr, temp);
                    memcpy (begin, ui2_ptr + temp, temp);
                    stop = begin + temp;
                    break;
                }
                memcpy (stop, ui2_ptr, temp);
                stop += temp;
                break;
#else
                return ReportError (UnsupportedTypeError, header, index,
                                    start);
#endif

            case AR4:  //< _W_r_i_t_e__A_r_r_a_y_4______________________________

#if USING_AR2
#else
                return ReportError (UnsupportedTypeError, header, index,
                                    start);
#endif

            case AR8:  //< _W_r_i_t_e__A_r_r_a_y_8______________________________

#if USING_AR8
#else
                return ReportError (UnsupportedTypeError, header, index,
                                    start);
#endif

            case ESC: //< _W_r_i_t_e__E_s_c_a_p_e_S_e_q_u_e_n_c_e_______________________
#if USING_ESC
                break;
#else
                return ReportError (UnsupportedTypeError, header, index,
                                    start);
#endif

            case BK8:  //< _W_r_i_t_e__6_4_-_B_i_t__B_o_o_k_____________________

# if USING_BK2
                ui4_ptr = reinterpret_cast<const uint32_t*> (ptrs[index]);
                if (ui4_ptr == nullptr) break;
                //< This isn't an error because the user might want to pass in 
                //< a nullptr.

                // To save stack space, we have to manually load the book 
                // header and calculate size.

                // Load num_Keys.
                ui4 = *ui4_ptr;
                temp = ui4; // Store temp variable

                            // Load bufferSize.
                ui4 = *(ui4_ptr + sizeof (uint16_t));
                temp += ui4;

                // Check the number of pages and buffered number of pages is 
                // valid.
                if (temp >= MaxNumPagesBook8 || (temp & 0x7) != 0)
                    return ReportError (InvalidNumBookPagesError, header, index,
                                        start);

                // Load collisionsSize.
                ui4 = *(ui4_ptr + 2 * sizeof (uint16_t));
                // and put aside till after we calc page indexes size.
                collisionsSize = static_cast<uint32_t> (ui4);

                // Calculate indexed size
                temp *= (sizeof (uint32_t) + sizeof (uint32_t) +
                    (collisionsSize > 0) * (sizeof (byte) + sizeof (hash16_t)));
                temp += collisionsSize;

                // Load headerSize.
                ui4 = *(ui4_ptr + 2 * sizeof (uint16_t) + sizeof (uint32_t));
                temp += ui4;
                ui4_ptr += sizeof (uint32_t);

                // Load dataSize.
                ui8_ptr = reinterpret_cast<uint64_t*> (ui4_ptr);
                ui8 = *(ui8_ptr + 2 * sizeof (uint16_t) + 2 *
                        sizeof (uint32_t));
                temp += ui8;
                ui8_ptr += sizeof (uint64_t);

                // add the size of the header and we're done.
                temp += sizeof (Book8);

                if (buffer_space < temp)
                    return ReportError (BufferUnderflowError,
                                        header, index, start);
                buffer_space -= (uint_t)temp;

                // Now we can copy the book into memory.
                if ((start > stop) && (start + ui1 >= end)) {
                    // Calculate upper chunk size.
                    temp = end - stop;
                    temp -= temp;

                    memcpy (stop, ui4_ptr, temp);
                    memcpy (begin, ui4_ptr + temp, temp);
                    stop = begin + temp;
                    break;
                }
                memcpy (stop, ui4_ptr, temp);
                stop += temp;
                break;
#else
                return ReportError (UnsupportedTypeError,
                                    header, index, start);
#endif

            case BK4:  //< _W_r_i_t_e__B_o_o_k_4________________________________

# if USING_BK4
                ui2_ptr = reinterpret_cast<const uint16_t*> (ptrs[index]);
                if (ui2_ptr == nullptr) {
                    // This isn't an error because the user might want to pass 
                    // in a nullptr.
                    break;
                }
                // To save stack space, we have to manually load the book header
                // and calculate size.

                // Load numKeys.
                ui2 = *ui2_ptr;
                temp = ui2; // Store temp variable

                            // Load bufferSize.
                ui2 = *(ui2_ptr + sizeof (uint16_t));
                temp += ui2;    // Add the stored temp variable.

                // Check the number of pages and buffered number of pages is
                // valid.
                if (temp >= MaxNumPagesBook4 || (temp & 0x7) != 0)
                    return ReportError (InvalidNumBookPagesError,
                                        header, index, start);

                // Load collisionsSize.
                ui2 = *(ui2_ptr + 2 * sizeof (uint16_t));
                // put aside till after we calc page indexes size.
                collisionsSize = static_cast<uint16_t> (ui2);

                // Calculate indexed size
                temp *= (sizeof (uint16_t) + sizeof (uint16_t) +
                    (collisionsSize > 0) * (sizeof (uint16_t) +
                                            sizeof (primeHash)));
                temp += collisionsSize;

                // Load headerSize.
                ui2 = *(ui2_ptr + 3 * sizeof (uint16_t));
                temp += ui2;
                ui2_ptr += sizeof (uint16_t);

                // Load dataSize.
                ui2 = *(ui2_ptr + 4 * sizeof (uint16_t));
                temp += ui2;
                ui2_ptr += sizeof (uint16_t);

                // add the size of the header and we're done.
                temp += sizeof (Book4);

                if (buffer_space < temp)
                    return ReportError (BufferUnderflowError,
                                        header, index, start);
                buffer_space -= (uint_t)temp;

                // Copy the book into memory.
                if ((start > stop) && (start + ui1 >= end)) {
                    // Calculate upper chunk size.
                    temp = end - stop;
                    temp -= temp;

                    memcpy (stop, ui4_ptr, temp);
                    memcpy (begin, ui4_ptr + temp, temp);
                    stop = begin + temp;
                    break;
                }
                memcpy (stop, ui4_ptr, temp);
                stop += temp;
                break;
#else
                return ReportError (UnsupportedTypeError,
                                    header, index, start);
#endif

            case BK2:  //< _W_r_i_t_e__B_o_o_k_2________________________________

# if USING_BK2
                ui2_ptr = reinterpret_cast<const uint16_t*> (ptrs[index]);
                if (ui2_ptr == nullptr) {
                    // This isn't an error because the user might want to pass 
                    // in a nullptr.
                    break;
                }

                // To save stack space, we have to manually load the book header
                // and calculate size.

                // Load numKeys.
                ui2 = *ui2_ptr;
                temp = ui2; // Store temp variable

                // Load bufferSize.
                ui2 = *(ui2_ptr + sizeof (byte));
                temp += ui2;    // Add the stored temp variable.

                // Check the number of pages and buffered number of pages is
                // valid.
                if (temp >= MaxNumPagesBook2 || (temp & 0x7) != 0)
                    return ReportError (InvalidNumBookPagesError,
                                        header, index, start);

                // Load collisionsSize.
                ui2 = *(ui2_ptr + 2 * sizeof (byte));
                // put aside till after we calc page indexes size.
                collisionsSize = static_cast<uint16_t> (ui2);

                // Calculate index sizes
                temp *= (sizeof (uint16_t) + sizeof (uint16_t) +
                    (collisionsSize > 0)* (sizeof (byte) + sizeof (primeHash)));
                temp += collisionsSize;

                // Load headerSize.
                ui2 = *(ui2_ptr + 2 * sizeof (byte) + sizeof (uint16_t));
                temp += ui2;
                ui2_ptr += sizeof (uint16_t);

                // Load dataSize.
                ui2 = *(ui2_ptr + 2 * sizeof (byte) + 2 * sizeof (uint16_t));
                temp += ui2;
                ui2_ptr += sizeof (uint16_t);

                // add the size of the header and we're done.
                temp += sizeof (Book2);

                if (buffer_space < temp)
                    return ReportError (BufferUnderflowError,
                                        header, index, start);
                buffer_space -= (uint_t)temp;

                // Copy the book into memory.
                if ((start > stop) && (start + ui1 >= end)) {
                    // Calculate upper chunk size.
                    temp = end - stop;
                    temp -= temp;

                    memcpy (stop, ui2_ptr, temp);
                    memcpy (begin, ui2_ptr + temp, temp);
                    stop = begin + temp;
                    break;
                }
                memcpy (stop, ui2_ptr, temp);
                stop += temp;
                break;
#else
                return ReportError (UnsupportedTypeError, header, index, start);
#endif

            case US:
                break;

            default:
                return ReportError (ReadInvalidTypeError, header, index, start);
        }
    }

    tx.stop = Diff (stop, begin);
    return 0;
}


/** Prints a message with the given header to the given Tx slot. */
inline ticket_t Write (Tx* tx, const uint_t header[], const void** ptrs) {
    return Write (reinterpret_cast<Tx&>(tx), header, ptrs);
}

/** Operator << prints the given value as a string to the log. */
inline Tx& operator<< (Tx& tx, int8_t value) {
    void* ptrs[2];
    char buffer[5];
    sprintf_s (buffer, 5, "%i", value);
    Write (tx, Params<STX> (), Args (ptrs, Address<'?'> (), buffer));
    return tx;
}

/** Operator << prints the given value as a string to the log. */
inline Tx& operator<< (Tx& tx, uint8_t value) {
    void* ptrs[2];
    char buffer[4];
    sprintf_s (buffer, 4, "%u", value);
    Write (tx, Params<STX> (), Args (ptrs, Address<'?'>(), buffer));
    return tx;
}

/** Operator << prints the given value as a string to the log. */
inline Tx& operator<< (Tx& tx, int16_t value) {
    void* ptrs[2];
    char buffer[7];
    sprintf_s (buffer, 7, "%i", value);
    Write (tx, Params<STX> (), Args (ptrs, Address<'?'> (), buffer));
    return tx;
}

/** Operator << prints the given value as a string to the log. */
inline Tx& operator<< (Tx& tx, uint16_t value) {
    void* ptrs[2];
    char buffer[6];
    sprintf_s (buffer, 6, "%u", value);
    Write (tx, Params<STX> (), Args (ptrs, Address<'?'> (), buffer));
    return tx;
}

/** Operator << prints the given value as a string to the log. */
inline Tx& operator<< (Tx& tx, int32_t value) {
    void* ptrs[2];
    char buffer[12];
    sprintf_s (buffer, 12, "%i", value);
    Write (tx, Params<STX> (), Args (ptrs, Address<'?'> (), buffer));
    return tx;
}

/** Operator << prints the given value as a string to the log. */
inline Tx& operator<< (Tx& tx, uint32_t value) {
    void* ptrs[2];
    char buffer[11];
    sprintf_s (buffer, 11, "%u", value);
    Write (tx, Params<STX> (), Args (ptrs, Address<'?'> (), buffer));
    return tx;
}

/** Operator << prints the given value as a string to the log. */
inline Tx& operator<< (Tx& tx, int64_t value) {
    void* ptrs[2];
    char buffer[22];
    sprintf_s (buffer, 22, "%I64d", value);
    Write (tx, Params<STX> (), Args (ptrs, Address<'?'> (), buffer));
    return tx;
}

/** Operator << prints the given value as a string to the log. */
inline Tx& operator<< (Tx& tx, uint64_t value) {
    void* ptrs[2];
    char buffer[21];
    sprintf_s (buffer, 21, "%llu", value);
    Write (tx, Params<STX> (), Args (ptrs, Address<'?'> (), buffer));
    return tx;
}

/** Operator << prints the given value as a string to the log. */
inline Tx& operator<< (Tx& tx, float value) {
    void* ptrs[2];
    char buffer[FLT_MAX_10_EXP + 2];
    sprintf_s (buffer, FLT_MAX_10_EXP + 2, "%f", value);
    Write (tx, Params<STX> (), Args (ptrs, Address<'?'> (), buffer));
    return tx;
}

/** Operator << prints the given value as a string to the log. */
inline Tx& operator<< (Tx& tx, double value) {
    void* ptrs[2];
    char buffer[DBL_MAX_10_EXP + 2];
    sprintf_s (buffer, DBL_MAX_10_EXP + 2, "%f", value);
    Write (tx, Params<STX> (), Args (ptrs, Address<'?'> (), buffer));
    return tx;
}

/** Operator << prints a string to the log. */
inline Tx& operator<< (Tx& tx, const char* s) {
    void* ptrs[2];
    Write (tx, Params<STX> (), Args (ptrs, Address<'?'> (), s));
    return tx;
}

}       //< namespace _
#endif  //< CHINESEROOM_TX_H

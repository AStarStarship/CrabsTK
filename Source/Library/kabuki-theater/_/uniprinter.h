/** The Chinese Room
    @version 0.x
    @file    ~/uniscanner.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough<calemccollough.github.io>
                            All right reserved (R).
             Licensed under the Apache License, Version 2.0 (the "License"); 
             you may not use this file except in compliance with the License. 
             You may obtain a copy of the License at
                        http://www.apache.org/licenses/LICENSE-2.0
             Unless required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#ifndef CHINESEROOM_UNIPRINTER_H
#define CHINESEROOM_UNIPRINTER_H

#include "log.h"

namespace _ {

/*< A rx socket that prints results to any given memory address.
*/
struct Uniprinter
{
    uint_t size,            //< The size of the buffer.
        start;              //< The starting index of the ring buffer data.
    volatile uint_t stop;   //< The stopping index of the ring buffer data.
    uint_t read;            //< The read variable.
};

/** Initializes the Uniprinter struct to an empty buffer. */
INLINE Uniprinter* UniprinterInit (byte* buffer, uint_t size) {
    if (buffer == nullptr)
        return nullptr;
    Uniprinter* rx = reinterpret_cast<Uniprinter*>(rx);
    rx->size = size - sizeof (Uniprinter);
    rx->start = 0;
    rx->stop = 0;
    rx->read = 0;
    return rx;
}

/** Initializes the Uniprinter struct to an empty buffer. */
INLINE Uniprinter* UniprinterInit (Uniprinter* rx, uint_t size) {
    if (rx == nullptr)
        return nullptr;
    rx->size = size - sizeof (Uniprinter);
    rx->start = 0;
    rx->stop = 0;
    rx->read = 0;
    return rx;
}

/** Gets the start of the Uniprinter ring buffer. 
INLINE byte* UniprinterBaseAddress (void* ptr) {
    return reinterpret_cast <byte*>(ptr) + kSlotHeaderSize;
}*/

/** Gets the start of the Uniprinter ring buffer. */
INLINE byte* UniprinterBaseAddress (Uniprinter* ptr) {
    return reinterpret_cast <byte*>(ptr) + kSlotHeaderSize;
}

/** Gets the rx buffer length. */
INLINE uint_t UniprinterSpace (Uniprinter* rx) {
    if (rx == nullptr) return ~0;

    byte* base = UniprinterBaseAddress (rx);
    return RingBufferSpace (base + rx->start, base + rx->stop, rx->size);
}

/** Gets the start of the Uniprinter ring buffer. */
INLINE byte* UniprinterBaseAddress (void* ptr, uint_t rx_tx_offset) {
    enum {
        kSlotHeaderSize = sizeof (Uniprinter) + sizeof (uintptr_t) -
        sizeof (Uniprinter) % sizeof (uintptr_t),
        //< Offset to the start of the ring buffer.
    };

    return reinterpret_cast <byte*>(ptr) + rx_tx_offset + kSlotHeaderSize;
}

/** Gets the end address of the rx buffer. */
INLINE byte* UniprinterEndAddress (Uniprinter* rx) {
    return reinterpret_cast<byte*>(rx) + kSlotHeaderSize + rx->size;
}

/** Scans a message with the given params to the given Uniprinter.
    The data in the Uniprinter is word-aligned, unlike the Unityper. It also
    doesn't have a hash with an escape sequence.
    @param rx The Uniprinter socket.
    @param params The parameters.
    @param args The arguments.
    @return Returns 0 upon success and an ErrorList ticket number upon failure. */
INLINE ticket_t Read (Uniprinter* rx, const uint_t* params, void** args) {
    if (rx == nullptr)
        Report (NullPointerError, 0, 0, 0);
    if (params == nullptr)
        Report (NullHeaderError, 0, 0, 0);
    if (args == nullptr)
        Report (NullHeaderError, 0, 0, 0);

    byte type,                  //< The current type we're reading.
        ui1;                    //< Temp variable to load most types.
    uint16_t ui2;               //< Temp variable for working with UI2 types.
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
        index,                  //< The index in the escape sequence.
        num_params = *params;   //< The number of params.

    uintptr_t offset;           //< The offset to word align the current type.

    if (num_params == 0) {

#if DEBUG_CHINESEROOM
        std::cout << "\n\n| Reading Uniprinter: ";
#endif
        return 0;
    }

    size = rx->size;

    byte* begin = UniprinterBaseAddress (rx),   //< The beginning of the buffer.
        * end   = begin + size,                 //< The end of the buffer.
        * start = begin + rx->start,            //< The start of the data.
        * stop  = begin + rx->stop;             //< The stop of the data.
    const uint_t* param = params + 1;           //< The current param.

    length = RingBufferLength (start, stop, size);

#if DEBUG_CHINESEROOM
    std::cout << "\n\n| Reading Uniprinter: \n";
    //PrintEsc (params);
    printf ("| begin: 0x%p start : %u stop : %u end : %u "
            "length: %u ", begin, Diff (begin, start), 
            Diff (begin, stop), Diff (begin, end), length);
#endif
    // When we scan, we are reading from the beginning of the Uniprinter buffer.

    for (index = 0; index < num_params; ++index) {
        type = *param;
        ++param;
#if DEBUG_CHINESEROOM
        printf ("\n| index %2u: %s  start: %u, stop: %u hash: ", index,  
                TypeString (type), Diff (begin, start), Diff (begin, stop));
#endif

        switch (type) {
          case NIL:
              goto RxInvalidType;
          case SOH:    //< _R_e_a_d__S_t_r_i_n_g_-_8____________________________
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

            // Read from buffer and write to the stack:
            ui1 = *start;
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
                if (++start >= end) start -= size;
                *ui1_ptr = ui1;     // Write byte to destination.
                ++ui1_ptr;
            }
#if DEBUG_CHINESEROOM
            printf (" done!\n");
#endif
            if (type != SOH) {
                *ui1_ptr = ui1;
            }
            break;
          case SI1:     //< _R_e_a_d__1__B_y_t_e__T_y_p_e_s_____________________
          case UI1:
          case BOL:
#if USING_1_BYTE_TYPES
            if (length == 0) 
                return Report (BufferUnderflowError, params, index, start);
            --length;

            // Read from buffer and write to the stack:
            ui1 = *start;                       //< Read
            if (++start >= end) start -= size;  //< Increment
            // Load next pointer and increment args.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == 0)
                break;
            *ui1_ptr = ui1;                     //< Write
            break;
#else
            goto RxInvalidType;
#endif
          case SI2:     //< _R_e_a_d__1_6_-_b_i_t__T_y_p_e_s____________________
          case UI2:
          case HLF:
#if USING_2_BYTE_TYPES
            Read2ByteType:
            // Word-align
            offset = TypeAlign2 (start);
            if (length < offset + 2)
                return Report (BufferUnderflowError, params, index, start);
            length -= offset + 2;
            start  += offset;
            if (start >= end) start -= size;    //< Bound
            // Read from buffer and write to the stack:
            ui2_ptr = reinterpret_cast<uint16_t*>(start);
            ui2 = *ui2_ptr;                     //< Read
            start += sizeof (uint16_t);         //< Increment
            if (start >= end) start -= size;    //< Bound
            // Load next pointer and increment args.
            ui2_ptr = reinterpret_cast<uint16_t*> (args[index]);
            if (ui2_ptr == 0) break;
            *ui2_ptr = ui2;                     //< Write
            break;
#else
            goto RxInvalidType;
#endif
          case SI4:     //< _R_e_a_d__3_2_-_b_i_t__T_y_p_e_s____________________
          case UI4:
          case FLT:
          case TMS:
#if USING_4_BYTE_TYPES
            Read4ByteType:
            // Word-align
            offset = TypeAlign4 (start);
            if (length < offset + 4)
                return Report (BufferUnderflowError, params, index, start);
            length -= offset + 4;
            start += offset;
            if (start >= end) start -= size;    //< Bound
            // Read from buffer and write to the stack:
            ui4_ptr = reinterpret_cast<uint32_t*>(start);
            ui4 = *ui4_ptr;                     //< Read
            start += sizeof (int32_t);          //< Increment
            if (start >= end) start -= size;    //< Bound
            // Load next pointer and increment args.
            ui4_ptr = reinterpret_cast<uint32_t*> (args[index]);
            if (ui4_ptr == 0) break;
            *ui4_ptr = ui1;                     //< Write
            break;
#else
            goto RxInvalidType;
#endif
          case TMU:     //< _R_e_a_d__6_4_-_b_i_t__T_y_p_e_s____________________
          case SI8:
          case UI8:
          case DBL:
#if USING_8_BYTE_TYPES
            Read8ByteType:
            // Word-align
            offset = TypeAlign8 (start);
            if (length < offset + 8)
                return Report (BufferUnderflowError, params, index, start);
            length -= offset + 8;
            start += offset;
            if (start >= end) start -= size;    //< Bound
            // Read from buffer and write to the stack:
            ui8_ptr = reinterpret_cast<uint64_t*>(start);
            ui8 = *ui8_ptr;                     //< Read
            start += sizeof (uint64_t);         //< Increment
            if (start >= end) start -= size;    //< Bound
            // Load next pointer and increment args.
            ui8_ptr = reinterpret_cast<uint64_t*> (args[index]);
            if (ui8_ptr == 0) break;
            *ui8_ptr = ui8;                     //< Write
            break;
#else
            goto RxInvalidType;
#endif
          case SV2:     //< _R_e_a_d__V_a_r_i_n_t__2____________________________
          case UV2:
#if USING_VARINT2
            goto Read2ByteType;
#else
            goto RxInvalidType;
#endif
          case SV4:     //< _R_e_a_d__V_a_r_i_n_t__4____________________________
          case UV4:
#if USING_VARINT4
              goto Read4ByteType;
#else
            goto RxInvalidType;
#endif
          case SV8:     //< _R_e_a_d__V_a_r_i_n_t__8____________________________
          case UV8:
#if USING_VARINT8
            goto Read8ByteType;
#else
            goto RxInvalidType;
#endif
          case AR1:  //< _R_e_a_d__A_r_r_a_y_-_1________________________________
#if USING_AR1
            // Load next pointer and increment args.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == nullptr)
                return Report (NullPointerError, params, index, start);
            count = *param++;

            // Word-align
            offset = TypeAlign (start, count);
            if (length < offset)
                return Report (BufferUnderflowError, params, index, start);
            length -= offset;
            start += offset;
            if (start >= end) start -= size;

            count *= SizeOf (*param++);
            goto ReadBlock;
#else
            goto RxInvalidType;
#endif
          case AR2:  //< _R_e_a_d__A_r_r_a_y_-_2________________________________
#if USING_AR2
            // Load the pointer to the destination.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == nullptr)
                return Report (NullPointerError, params, index, start);
            count = *param++; //< Get type from header
              
            // Word-align
            offset = TypeAlign (start, count);
            if (length < offset)
                return Report (BufferUnderflowError, params, index, start);
            length -= offset;
            start += offset;
            if (start >= end) start -= size;

            count *= SizeOf (*param++);
            goto ReadBlock;
#else
            goto RxInvalidType;
#endif
          case AR4:  //< _R_e_a_d__A_r_r_a_y_-_4________________________________
#if USING_AR4
            // Load the pointer to the destination.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == nullptr)
                return Report (NullPointerError, params, index, start);
            count = *param++;   //< Read type

            // Word-align
            offset = TypeAlign (start, count);
            if (length < offset)
                return Report (BufferUnderflowError, params, index, start);
            length -= offset;
            start += offset;
            if (start >= end) start -= size;

            count *= SizeOf (*param++);
            goto ReadBlock;
#else
            goto RxInvalidType;
#endif
          case AR8:  //< _R_e_a_d__A_r_r_a_y_-_8________________________________
#if USING_AR8
            // Load the pointer to the destination.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == nullptr)
                return Report (NullPointerError, params, index, start);
            count = *param++;   //< Read Type
              
            // Word-align
            offset = TypeAlign (start, count);
            if (length < offset)
                return Report (BufferUnderflowError, params, index, start);
            length -= offset;
            start += offset;
            if (start >= end) start -= size;

            count *= SizeOf (*param++);
            goto ReadBlock;
#else
              goto RxInvalidType;
#endif
          case ESC: //< _R_e_a_d__E_s_c_a_p_e__S_e_q_u_e_n_c_e__________________
            // I'm not sure exactly how this should work. I can't do recursion
            // because of embedded limitations.
            break;
          case BK8: //< _R_e_a_d__B_o_o_k_8_____________________________________
            // Word-align
              offset = TypeAlign8 (start);
              if (length < offset + 128)
                  return Report (BufferUnderflowError, params, index, start);
              length -= offset;
              start += offset;
              if (start >= end) start -= size;

            // Load the pointer to the destination.
            ui8_ptr = reinterpret_cast<uint64_t*> (args[index]);
            if (ui8_ptr == nullptr)
                return Report (NullPointerError, params, index, start);
            count = (uint_t)*ui8_ptr;
            ui1_ptr = reinterpret_cast<byte*> (ui8_ptr + 1);
            goto ReadBlock;
#if USING_AR8
#else
            goto RxInvalidType;
#endif
          case BK4: //< _R_e_a_d__B_o_o_k_4_______________________________________
            // Word-align
              offset = TypeAlign4 (start);
              if (length < offset + 64)
                  return Report (BufferUnderflowError, params, index, start);
              length -= offset;
              start += offset;
              if (start >= end) start -= size;

            // Load the pointer to the destination.
            ui4_ptr = reinterpret_cast<uint32_t*> (args[index]);
            if (ui4_ptr == nullptr)
                return Report (NullPointerError, params, index, start);
            count = (uint_t)*ui4_ptr;
            ui1_ptr = reinterpret_cast<byte*> (ui4_ptr + 1);
            goto ReadBlock;
#if USING_BK4
#else
            goto RxInvalidType;
#endif
          case BK2: //< _R_e_a_d__B_o_o_k_2_______________________________________
            // Word-align
            offset = TypeAlign2 (start);
            if (length < offset + 32)
                return Report (BufferUnderflowError, params, index, start);
            length -= offset;
            start += offset;
            if (start >= end) start -= size;

            // Load the pointer to the destination.
            ui2_ptr = reinterpret_cast<uint16_t*> (args[index]);
            if (ui2_ptr == nullptr)
                return Report (NullPointerError, params, index, start);
            count = (uint_t)*ui2_ptr;
            ui1_ptr = reinterpret_cast<byte*> (ui2_ptr + 1);
            goto ReadBlock;
#if USING_BK2
#else
            goto RxInvalidType;
#endif
          case US: //< _R_e_a_d__U_n_i_t__S_e_p_e_r_a_t_o_r_____________________
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            count = kUnitSize;
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
                        *ui1_ptr = ui1;
                        ++ui1_ptr;
                    }
                    stop = begin - 1;
                    for (; count > 0; --count) {
                        ui1 = *start;
                        if (++start >= end) start -= size;
                        *ui1_ptr = ui1;
                        ++ui1_ptr;
                    }
                    break;
                }
               for (; count > 0; --count) {
                    ui1 = *start;
                   if (++start >= end) start -= size;
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

#if DEBUG_CHINESEROOM
    printf ("| Done reading\n");
    RingBufferClear (begin, rx->start, start, stop, end, size);
#endif

    // Convert pointer back to offset
    rx->start = Diff (begin, start);

    return 0;
}

/** Returns true if the Uniprinter buffer contains any data.
    @warning Function does not do any error checking for speed.
*/
INLINE bool IsReadable (Uniprinter* rx) {
    return rx->start != rx->stop;
}

INLINE void Print (Uniprinter* rx) {
    if (rx == nullptr) return;
    uint_t size = rx->size;
    PrintLine ('_');
    printf ("| Uniprinter %p: size: %u, start: %u, stop: %u, read: %u\n", rx, size,
            rx->start, rx->stop, rx->read);

    PrintMemory (UniprinterBaseAddress (rx), size);
}

}       //< namespace _
#endif  //< CHINESEROOM_UNIPRINTER_H

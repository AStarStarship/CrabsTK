    /** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/impl/script_slot_in.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (R). Licensed under the Apache License, Version 
             2.0 (the "License"); you may not use this file except in 
             compliance with the License. You may obtain a copy of the License 
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless 
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#include <stdafx.h>
#include "../include/slot.h"
#include "../include/types.h"

namespace _ {

bool IsReadable (Slot* slot) {
    return (slot->start != slot->stop);
}

const Operation* BArgs (Slot* slot, const uint_t* params, void** args) {
    if (slot == nullptr)
        return SlotResult (slot, Bin::RoomError);
    if (params == nullptr)
        return SlotResult (slot, Bin::RoomError);
    if (args == nullptr)
        return SlotResult (slot, Bin::RoomError);
    Bin* bin = reinterpret_cast<Bin*> (slot);

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
        index,                  //< The index in the escape sequence.
        num_params = *params;   //< The number of params.

    uintptr_t offset,           //< The offset to word align the current type.
        count;            //< The argument length.

    if (num_params == 0) {

#if DEBUG_SCRIPT
        std::cout << "\n\n| Reading Bin: ";
#endif
        return 0;
    }

    size = bin->size;

    byte* begin = reinterpret_cast<byte*> (slot) + sizeof (slot),
        //< The beginning of the buffer.
        *end = begin + size,                //< The end of the buffer.
        *start = begin + bin->start,          //< The start of the data.
        *stop = begin + bin->stop;           //< The stop of the data.
    const uint_t* param = params + 1;          //< The current param.

    length = SlotLength (start, stop, size);

#if DEBUG_SCRIPT
    std::cout << "\n\n| Reading Bin: \n";
    //ParamsPrint (params);
    printf ("| begin: 0x%p start : %u stop : %u end : %u "
            "length: %u ", begin, Diff (begin, start),
            Diff (begin, stop), Diff (begin, end), length);
#endif
    // When we scan, we are reading from the beginning of the Bin buffer.

    for (index = 0; index < num_params; ++index) {
        type = *param;
        ++param;
#if DEBUG_SCRIPT
        printf ("\n| index %2u: %s  start: %u, stop: %u hash: ", index,
                TypeString (type), Diff (begin, start), Diff (begin, stop));
#endif

        switch (type) {
            case NIL:
                goto InvalidType;
            case ADR:    //< _R_e_a_d__S_t_r_i_n_g_-_8____________________________
            case STR:
                // Load buffered-type argument length and increment the index.
                count = *param;
                ++param;
#if DEBUG_SCRIPT
                printf ("\n|           Reading char with max length %u: ", count);
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
                        return SlotResult (slot, Bin::BufferUnderflowError,
                                           params, index, start);
#if DEBUG_SCRIPT
                    putchar (ui1);
#endif
                    ui1 = *start;       // Read byte from ring-buffer.
                    if (++start >= end) start -= size;
                    *ui1_ptr = ui1;     // Write byte to destination.
                    ++ui1_ptr;
                }
#if DEBUG_SCRIPT
                printf (" done!\n");
#endif
                if (type != ADR) {
                    *ui1_ptr = ui1;
                }
                break;
            case SI1:     //< _R_e_a_d__1__B_y_t_e__T_y_p_e_s_____________________
            case UI1:
            case BOL:
#if USING_1_BYTE_TYPES
                if (length == 0)
                    return SlotResult (slot, Bin::BufferUnderflowError, params,
                                       index, start);
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
                goto InvalidType;
#endif
            case SI2:     //< _R_e_a_d__1_6_-_b_i_t__T_y_p_e_s____________________
            case UI2:
            case HLF:
#if USING_2_BYTE_TYPES
                Read2ByteType:{
                    // Word-align
                    offset = TypeAlign2 (start);
                    if (length < offset + 2)
                        return SlotResult (slot, Bin::BufferUnderflowError,
                                           params, index, start);
                    length -= offset + 2;
                    start += offset;
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
                }
#else
                goto InvalidType;
#endif
            case SI4:     //< _R_e_a_d__3_2_-_b_i_t__T_y_p_e_s____________________
            case UI4:
            case FLT:
            case TMS:
#if USING_4_BYTE_TYPES
                Read4ByteType:{
                    // Word-align
                    offset = TypeAlign4 (start);
                    if (length < offset + 4)
                        return SlotResult (slot, Bin::BufferUnderflowError,
                                           params, index, start);
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
                }
#else
                goto InvalidType;
#endif
            case TMU:     //< _R_e_a_d__6_4_-_b_i_t__T_y_p_e_s____________________
            case SI8:
            case UI8:
            case DBL:
#if USING_8_BYTE_TYPES
                Read8ByteType:{
                    // Word-align
                    offset = TypeAlign8 (start);
                    if (length < offset + 8)
                        return SlotResult (slot, Bin::BufferUnderflowError, params, index, start);
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
                }
#else
                goto InvalidType;
#endif
            case SV2:     //< _R_e_a_d__V_a_r_i_n_t__2____________________________
            case UV2:
#if USING_VARINT2
                goto Read2ByteType;
#else
                goto InvalidType;
#endif
            case SV4:     //< _R_e_a_d__V_a_r_i_n_t__4____________________________
            case UV4:
#if USING_VARINT4
                goto Read4ByteType;
#else
                goto InvalidType;
#endif
            case SV8:     //< _R_e_a_d__V_a_r_i_n_t__8____________________________
            case UV8:
#if USING_VARINT8
                goto Read8ByteType;
#else
                goto InvalidType;
#endif
            default:
            {
                count = type >> 5;  //< count is now the array type bits.
                type &= 0x1f;       //< Now type is the type 0-31
                if (count && (type >= OBJ)) {
                    // Can't make arrays out of objects!
                    return SlotResult (slot, Bin::InvalidTypeError, params, index, start);
                }
                // We don't care if it's a multidimensional array anymore.
                ui1_ptr = reinterpret_cast<uint8_t*> (args[index]);
                if (ui1_ptr == nullptr)
                    return SlotResult (slot, Bin::RoomError, params, index, start);
                count &= 0x3;
                switch (count) {
                    case 0: { // It's a 8-bit count.
                        if (type >= LST) {
                            // LST, BAG, BOK, and MAP cannot take a 8-bit count!
                            return SlotResult (slot, Bin::InvalidTypeError, params, index, start);
                        }
                        count = (uintptr_t)*ui1_ptr;
                        break;
                    }
                    case 1: { // It's a 16-bit count.
                        if (length < 3)
                            return SlotResult (slot, Bin::BufferUnderflowError, params, index, start);
                        ui2_ptr = reinterpret_cast<uint16_t*> (ui1_ptr);
                        count = (uintptr_t)*ui2_ptr;
                        if (count > length - 2)
                            return SlotResult (slot, Bin::BufferOverflowError, params, index, start);
                        break;
                    }
                    case 2: { // It's a 32-bit count.
                        if (length < 5)
                            return SlotResult (slot, Bin::BufferUnderflowError, params, index, start);
                        ui4_ptr = reinterpret_cast<uint32_t*> (ui1_ptr);
                        count = (uintptr_t)*ui4_ptr;
                        if (count > length - 4)
                            return SlotResult (slot, Bin::BufferOverflowError, params, index, start);
                        break;
                    }
                    case 3: { // It's a 64-bit count.
                        if (length < 9)
                            return SlotResult (slot, Bin::BufferUnderflowError, params, index, start);
                        ui8_ptr = reinterpret_cast<uint64_t*> (ui1_ptr);
                        count = (uintptr_t)*ui8_ptr;
                        if (count > length - 8)
                            return SlotResult (slot, Bin::BufferOverflowError, params, index, start);
                        break;
                    }
                    default:
                    {
                        return SlotResult (slot, Bin::RoomError, params, index, start);
                    }
                }
                if (length < count)
                    return SlotResult (slot, Bin::BufferOverflowError, params, index, start);
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
                InvalidType:
                {
                    printf ("\n!!!Read invalid type %u\n", type);
                    return SlotResult (slot, Bin::InvalidTypeError, params, index, start);
                }
            }
            std::cout << " |";
        }
    }
#if DEBUG_SCRIPT
    printf ("| Done reading\n");
    SlotWipe (slot);
#endif

    // Convert pointer back to offset
    bin->start = Diff (begin, start);

    return 0;
}

}       //< namespace _

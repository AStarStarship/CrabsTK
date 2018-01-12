/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/script_bout.cc
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
#include "bout.h"
#include "bin.h"
#include "type.h"
#include "bsq.h"
#include "args.h"
#include "address.h"
#include "text.h"
#include "hash.h"
#include "slot.h"



namespace _ {

#if SCRIPT_DEBUG

/** Used to return an erroneous result from a B-Output.

    @param error The error type.
    @return Returns a Static Error Op Result. */
inline const Op* BOutError (BOut* bout, Error error) {
#if SCRIPT_DEBUG == SCRIPT_BOUT
    std::cerr << "\n| BOut " << ErrorString (error) << " Error!" << text.Print ();
#endif
    return reinterpret_cast<const Op*> (1);
}

/** Used to return an erroneous result from a B-Output.
    @param  bout    The source BOut.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Op Result. */
inline const Op* BOutError (BOut* bout, Error error,
                            const uint_t* header) {
#if SCRIPT_DEBUG
    std::cerr << "\n| BOut " << ErrorString (error) << " Error!";
#endif  //< SCRIPT_DEBUG
    return reinterpret_cast<const Op*> (1);
}

/** Used to return an erroneous result from a B-Output.
    @param  bout    The source BOut.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Op Result. */
inline const Op* BOutError (BOut* bout, Error error,
                            const uint_t* header,
                            uint_t offset) {
#if SCRIPT_DEBUG
    std::cerr << "\n| BOut " << ErrorString (error) << " Error!";
#endif  //< SCRIPT_DEBUG
    return reinterpret_cast<const Op*> (1);
}

/** Used to return an erroneous result from a B-Output.
    @param  bout    The source BOut.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Op Result. */
inline const Op* BOutError (BOut* bout, Error error,
                            const uint_t* header,
                            uint_t offset,
                            char* address) {
    std::cerr << "\n| BOut " << ErrorString (error) << " Error!";
    return reinterpret_cast<const Op*> (1);
}

const char** BOutState () {
    static const char* strings[] = {
        "WritingState",
        "LockedState"
    };
    return strings;
}
#endif  //< SCRIPT_DEBUG

char* BOutBuffer (BOut* bout) {
    if (!bout)
        return nullptr;
    char* ptr = reinterpret_cast<char*> (bout);
    return ptr + sizeof (BOut);
}

BOut* BOutInit (uintptr_t* buffer, uint_t size) {
    if (size < kMinSlotSize)
        return nullptr;
    if (buffer == nullptr)
        return nullptr;

    BOut* bout  = reinterpret_cast<BOut*> (buffer);
    bout->size  = size - sizeof (BIn);
    bout->start = 0;
    bout->stop  = 0;
    bout->read  = 0;

#if SCRIPT_DEBUG
    MemoryClear (BOutBuffer (bout), size);
   // BOutPrint (bout);
#endif
    return bout;
}

uint_t BOutSpace (BOut* bout) {
    if (!bout) return ~0;
    char* txb_ptr = reinterpret_cast<char*>(bout);
    return SlotSpace (txb_ptr + bout->start, txb_ptr + bout->stop,
                      bout->size);
}

uint_t BOutBufferLength (BOut* bout) {
    if (!bout) return ~0;
    char* base = BOutBuffer (bout);
    return SlotLength (base + bout->start, base + bout->stop, bout->size);
}

char* BOutEndAddress (BOut* bout) {
    return reinterpret_cast<char*>(bout) + (4 * sizeof (uint_t)) + bout->size;
}

int BOutStreamByte (BOut* bout) {

    char* begin  = BOutBuffer (bout),
        * end    = begin + bout->size;
    char* open   = (char*)begin + bout->read,
        * start  = begin + bout->start,
        * cursor = start;

    intptr_t length = (int)(start < open) ? open - start + 1
                                          : (end - start) + (open - begin) + 2;

    if (length < 1) {
        BOutError (bout, kErrorBufferOverflow, Bsq<1, STR> (), 2,
                   start);
        return -1;
    }
    //byte b = *cursor;
    bout->stop = (++cursor > end) ? static_cast<uint_t> (MemoryVector (begin, end))
                                  :  static_cast<uint_t> (MemoryVector (begin, cursor));
    return 0;
}

const Op* BOutWrite (BOut* bout, const uint_t* params, void** args) {
    
#if SCRIPT_DEBUG
    Text<> text;
    text << "\n|\n|Writing "
         << BsqPrint (params, text)
         << " to B-Output:" << text.Pointer (bout)
         << BOutPrint (bout, text) << std::cout;
#endif  //< SCRIPT_DEBUG
    if (!bout)
        return BOutError (bout, kErrorImplementation);
    if (!params)
        return BOutError (bout, kErrorImplementation);
    if (!args)
        return BOutError (bout, kErrorImplementation);

    // Temp variables packed into groups of 8 bytes for memory alignment.
    byte //type,
        ui1;
#if USING_SCRIPT_2_BYTE_TYPES
    uint16_t ui2;
#endif
#if USING_SCRIPT_4_BYTE_TYPES
    uint32_t ui4;
#endif
#if USING_SCRIPT_8_BYTE_TYPES
    uint64_t ui8;
#endif

    uint_t   num_params,                //< Num params in the b-sequence.
             type,                      //< Current type.
             size,                      //< Size of the buffer.
             space,                     //< Space in the buffer.
             index,                     //< Index in the params.
             arg_index,                 //< Index in the args.
             length,                    //< Length of a type to write.
             value;                     //< Temp variable.
    num_params = params[0];
    if (num_params == 0) {
        return 0;      //< Nothing to do.
    }
    arg_index = 0;
    size = bout->size;
    const uint_t* param = params,       //< Pointer to the current param.
                * bsc_param;            //< Pointer to the current BSQ param.
    // Convert the socket offsets to pointers.
    char* begin = BOutBuffer (bout),    //< Beginning of the buffer.
        * end   = begin + size,         //< End of the buffer.
        * start = begin + bout->start,  //< Start of the data.
        * stop  = begin + bout->stop;   //< Stop of the data.
    const char* ui1_ptr;                //< Pointer to a 1-byte type.
#if USING_SCRIPT_2_BYTE_TYPES
    const uint16_t* ui2_ptr;            //< Pointer to a 2-byte type.
#endif
#if USING_SCRIPT_4_BYTE_TYPES
    const uint32_t* ui4_ptr;            //< Pointer to a 4-byte type.
#endif
#if USING_SCRIPT_8_BYTE_TYPES
    const uint64_t* ui8_ptr;            //< Pointer to a 8-byte type.
#endif
    hash16_t hash = kLargest16BitPrime; //< Reset hash to largest 16-bit prime.

    space = SlotSpace (start, stop, size);

    // Check if the buffer has enough room.
    if (space == 0)
        return BOutError (bout, kErrorBufferOverflow);
    --space;
    length = params[0];   //< Load the max char length.
    ++param;

    // Write data.
    for (index = 1; index <= num_params; ++index) {
        type = params[index];
#if SCRIPT_DEBUG
        text << "\n| param:" << arg_index + 1 << " type:" << TypeString (type) 
             << " start:" << MemoryVector (begin, start) 
             << " stop:" << MemoryVector (begin, stop) 
             << " space:" << space << std::cout;
             //<< " value:" << value;
#endif
        switch (type) {
            case NIL:
                break;

            case ADR: //< _W_r_i_t_e__A_d_d_r_e_s_s__S_t_r_i_n_g________________
            case STR: //< _W_r_i_t_e__U_T_F_-_8__S_t_r_i_n_g____________________
                if (space == 0)
                    return BOutError (bout, kErrorBufferOverflow, params,
                                       index, start);
                if (type != ADR) {
                    // We might not need to write anything if it's an ADR with 
                    // nil string.
                    length = params[++index]; //< Load the max char length.
                    ++num_params;
                } else {
                    length = kAddressLengthMax;
                }
                // Load the source data pointer and increment args.fs
                ui1_ptr = reinterpret_cast<const char*> (args[arg_index]);
#if SCRIPT_DEBUG
                text << "\"" << ui1_ptr << "\"";
#endif  //< SCRIPT_DEBUG

                // We know we will always have at least one nil-term char.
                ui1 = *ui1_ptr;
                while (ui1 != 0) {
                    if (space-- == 0)
                        return BOutError (bout, kErrorBufferOverflow,
                                           params, index, start);
                    hash = Hash16 (ui1, hash);

                    *stop = ui1;        // Write byte
                    if (++stop >= end) stop -= size;
                    ++ui1_ptr;
                    ui1 = *ui1_ptr;     // Read byte.
                }
                if (type != ADR) {  //< 1 is faster to compare than 2
                                    // More likely to have ADR than STR
                    *stop = 0;      // Write nil-term char.
                    if (++stop >= end) stop -= size;
                    break;
                }

                break;
            case SI1: //< _W_r_i_t_e__8_-_b_i_t__T_y_p_e_s______________________
            case UI1:
            case BOL:
#if USING_SCRIPT_1_BYTE_TYPES
                // Check if the buffer has enough room.
                if (space-- == 0)
                    return BOutError (bout, kErrorBufferOverflow, params,
                                      index, start);

                // Load pointer and read data to write.
                ui1_ptr = reinterpret_cast<const char*> (args[arg_index]);
                ui1 = *ui1_ptr;

                // Write data.
                *stop = ui1;
                hash = Hash16 (ui1, hash);
                if (++stop >= end) stop -= size;
                break;
#else
                return BOutError (bout, kErrorInvalidType);
#endif
            case SI2: //< _W_r_i_t_e__1_6_-_b_i_t__T_y_p_e_s____________________
            case UI2:
            case HLF:
#if USING_SCRIPT_2_BYTE_TYPES
                // Align the buffer to a word boundary and check if the 
                // buffer has enough room.
                if (space < sizeof (uint16_t))
                    return BOutError (bout, kErrorBufferOverflow,
                                      params, index, start);
                space -= sizeof (uint16_t);

                // Load pointer and value to write.
                ui2_ptr = reinterpret_cast<uint16_t*> (args[arg_index]);
                ui2 = *ui2_ptr;

                // Write data.

                // Byte 1
                ui1 = (byte)ui2;
                *stop = ui1;
                if (++stop >= end) stop -= size;
                hash = Hash16 (ui1, hash);

                // Byte 2
                ui1 = (byte)(ui2 >> 8);
                *stop = ui1;
                if (++stop >= end) stop -= size;
                hash = Hash16 (ui1, hash);
                break;

            case SVI: //< _W_r_i_t_e__2_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t_____

                // Load number to write and increment args.
                ui2_ptr = reinterpret_cast<const uint16_t*> (args[arg_index]);
                ui2 = *ui2_ptr;
                // We are using the same code to print both signed and unsigned 
                // varints. In order to convert from a negative 2's complement 
                // signed integer to a transmittable format, we need to invert 
                // the bits and add 1. Then we just shift the bits left one and 
                // put the sign bit in the LSB.
                ui2 = TypePackVarint<uint16_t> (ui2);
                goto WriteVarint2;
            case UVI: //< _W_r_i_t_e__2_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t_
                // Load next pointer value to write.
                ui2_ptr = reinterpret_cast<const uint16_t*> (args[arg_index]);
                if (ui2_ptr == nullptr)
                    return BOutError (bout, kErrorImplementation, params, index,
                                      start);
                ui2 = *ui2_ptr;

                WriteVarint2: {
                    // Byte 1
                    if (space-- == 0)
                        return BOutError (bout, kErrorBufferOverflow,
                                          params, index, start);
                    ui1 = ui2 & 0x7f;
                    ui2 = ui2 >> 7;
                    if (ui2 == 0) {
                        ui1 |= 0x80;
                        *stop = ui1;
                        if (++stop >= end) stop -= size;
                        hash = Hash16 (ui1, hash);
                        break;
                    }
                    *stop = ui1;
                    if (++stop >= end) stop -= size;
                    hash = Hash16 (ui1, hash);

                    // Byte 2
                    if (--space == 0)
                        return BOutError (bout, kErrorBufferOverflow,
                                          params, index, start);
                    ui1 = ui2 & 0x7f;
                    ui2 = ui2 >> 7;
                    if (ui2 == 0) {
                        ui1 |= 0x80;
                        *stop = ui1;
                        if (++stop >= end) stop -= size;
                        hash = Hash16 (ui1, hash);
                        break;
                    }
                    *stop = ui1;
                    if (++stop >= end) stop -= size;
                    hash = Hash16 (ui1, hash);

                    // Byte 3
                    if (--space == 0)
                        return BOutError (bout, kErrorBufferOverflow,
                                          params, index, start);
                    ui1 = ui2 & 0x7f;
                    ui1 |= 0x80;
                    *stop = ui1;
                    if (++stop >= end) stop -= size;
                    hash = Hash16 (ui1, hash);
                }
                break;
            #else
            case SVI:
            case UV2:
                return BOutError (bout, kErrorInvalidType);
            #endif

            case SI4: //< _W_r_i_t_e__3_2_-_b_i_t__T_y_p_e_s____________________
            case UI4:
            case FLT:
            case TMS:
#if USING_SCRIPT_4_BYTE_TYPES
                // Align the buffer to a word boundary and check if the buffer
                // has enough room.

                if (space < sizeof (uint32_t))
                    return BOutError (bout, kErrorBufferOverflow, params,
                                       index, start);
                space -= sizeof (uint64_t);

                // Load pointer and value to write.
                ui4_ptr = reinterpret_cast<uint32_t*> (args[arg_index]);
                ui4 = *ui4_ptr;


                // Byte 1
                ui1 = (byte)ui4;
                *stop = ui1;
                hash = Hash16 (ui1, hash);
                if (++stop >= end) stop -= size;

                // Byte 2
                ui1 = (byte)(ui4 >> 8);
                *stop = ui1;
                hash = Hash16 (ui1, hash);
                if (++stop >= end) stop -= size;

                // Byte 3
                ui1 = (byte)(ui4 >> 16);
                *stop = ui1;
                hash = Hash16 (ui1, hash);
                if (++stop >= end) stop -= size;

                // Byte 4
                ui4 = (byte)(ui4 >> 24);
                *stop = ui1;
                hash = Hash16 (ui1, hash);
                if (++stop >= end) stop -= size;
                break;
            case SV4: //< _W_r_i_t_e__4_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t_____

                // Load number to write and increment args.
                ui4_ptr = reinterpret_cast<const uint32_t*> (args[arg_index]);
                ui4 = *ui4_ptr;
                ui4 = TypePackVarint<uint32_t> (ui4);
                goto WriteVarint4;
            case UV4: //< _W_r_i_t_e__4_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t_
                // Load the 4-byte type to write to the buffer.
                ui4_ptr = reinterpret_cast<const uint32_t*> (args[arg_index]);
                ui4 = *ui4_ptr;
                WriteVarint4: { //< Optimized manual do while loop.
                    ui2 = 5;
                    if (space == 0) //< @todo Benchmark to space--
                        return BOutError (bout, kErrorBufferOverflow,
                                           params, index, start);
                    --space;    //< @todo Benchmark to space--
                    ui1 = ui4 & 0x7f;
                    ui4 = ui4 >> 7;
                    if (ui4 == 0) {
                        ui1 |= 0x80;
                        *stop = ui1;
                        if (++stop >= end) stop -= size;
                        hash = Hash16 (ui1, hash);
                        break;
                    }
                    *stop = ui1;
                    if (++stop >= end) stop -= size;
                    hash = Hash16 (ui1, hash);
                    // This wont happen I don't think.
                    //if (--ui2 == 0)
                    //    return BOutError (kErrorVarintOverflow, params, index,
                    //                       start);

                    goto WriteVarint4;
                }
                break;
#else
            case SV4:
            case UV4:
                return BOutError (bout, kErrorInvalidType);
#endif
            case SI8: //< _W_r_i_t_e__6_4_-_b_i_t__T_y_p_e_s____________________
            case UI8:
            case DBL:
            case TMU:
#if USING_SCRIPT_8_BYTE_TYPES
                // Align the buffer to a word boundary and check if the buffer
                // has enough room.
                if (space < sizeof (uint64_t))
                    return BOutError (bout, kErrorBufferOverflow, params,
                                       index, start);
                space -= sizeof (uint64_t);

                // Load pointer and value to write.
                ui8_ptr = reinterpret_cast<uint64_t*> (args[arg_index]);
                ui8 = *ui8_ptr;

                // Write data.

                // Byte 1
                ui1 = (byte)ui8;
                hash = Hash16 (ui1, hash);
                *stop = ui1;
                if (++stop >= end) stop -= size;

                // Byte 2
                ui1 = (byte)(ui8 >> 8);
                hash = Hash16 (ui1, hash);
                *stop = ui1;
                if (++stop >= end) stop -= size;

                // Byte 3
                ui1 = (byte)(ui8 >> 16);
                hash = Hash16 (ui1, hash);
                *stop = ui1;
                if (++stop >= end) stop -= size;

                // Byte 4
                ui8 = (byte)(ui8 >> 24);
                hash = Hash16 (ui1, hash);
                *stop = ui1;
                if (++stop >= end) stop -= size;

                // Byte 5
                ui1 = (byte)(ui8 >> 32);
                hash = Hash16 (ui1, hash);
                *stop = ui1;
                if (++stop >= end) stop -= size;

                // Byte 6
                ui1 = (byte)(ui8 >> 40);
                hash = Hash16 (ui1, hash);
                *stop = ui1;
                if (++stop >= end) stop -= size;

                // Byte 7
                ui1 = (byte)(ui8 >> 48);
                hash = Hash16 (ui1, hash);
                *stop = ui1;
                if (++stop >= end) stop -= size;

                // Byte 8
                ui8 = (byte)(ui8 >> 56);
                hash = Hash16 (ui1, hash);
                *stop = ui1;
                if (++stop >= end) stop -= size;
                break;
            case SV8: //< _W_r_i_t_e__8_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t_____
                // Load number to write and increment args.
                ui8_ptr = reinterpret_cast<const uint64_t*> (args[arg_index]);
                ui8 = *ui8_ptr;
                ui8 = TypePackVarint<uint64_t> (ui8);
                goto WriteVarint8;
            case UV8: //< _W_r_i_t_e__8_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t_
                // Load the 4-byte type to write to the buffer.
                ui8_ptr = reinterpret_cast<const uint64_t*> (args[arg_index]);
                ui8 = *ui8_ptr;
                WriteVarint8: {       //< Optimized manual do while loop.
                    ui2 = 8;          //< The max number of varint bytes - 1.
                    if (space <= 9) { //< @todo Benchmark to space--
                        return BOutError (bout, kErrorBufferOverflow,
                                          params, index, start);
                    }
                    --space;          //< @todo Benchmark to space--
                    if (--ui2 == 0) { //< It's the last byte not term bit.
                        ui1 = ui8 & 0xFF;
                        *stop = ui1;
                        if (++stop >= end) stop -= size;
                        hash = Hash16 (ui1, hash);
                        break;
                    }
                    ui1 = ui8 & 0x7f;   //< Take 7 bits at a time.
                    ui8 = ui8 >> 7;     //< 
                    if (ui8 == 0) {
                        ui1 |= 0x80;
                        *stop = ui1;
                        if (++stop >= end) stop -= size;
                        hash = Hash16 (ui1, hash);
                        break;
                    }
                    *stop = ui1;
                    if (++stop >= end) stop -= size;
                    hash = Hash16 (ui1, hash);

                    goto WriteVarint8;
                }
                break;
#else
            case SV8:
            case UV8:
                return BOutError (bout, kErrorInvalidType);
#endif
            default: {
                value = type >> 5;
                if ((type >> 5) && type > OBJ) {
                    return BOutError (bout, kErrorImplementation, params,
                                      index);
                }
                if ((type >> 7) && ((type & 0x1f) >= OBJ)) {
                    // Cannot have multi-dimensional arrays of objects!
                    type &= 0x1f;
                    return BOutError (bout, kErrorImplementation, params, index,
                                      start);
                }
                type = type & 0x1f;   //< Mask off lower 5 bits.
                switch (value) {
                    case 0: {
                        ui1_ptr = reinterpret_cast<const char*> 
                                  (args[arg_index]);
                        if (ui1_ptr == nullptr)
                            return BOutError (bout, kErrorImplementation, 
                                              params, index, start);
                    }
                    #if USING_SCRIPT_2_BYTE_TYPES
                    case 1: {
                        ui2_ptr = reinterpret_cast<const uint16_t*>
                            (args[arg_index]);
                        if (ui2_ptr == nullptr)
                            return BOutError (bout, kErrorImplementation, params,
                                              index, start);
                        ui2 = *ui2_ptr;
                        length = static_cast<uint_t>(ui2);
                        ui1_ptr = reinterpret_cast<const char*> (ui2_ptr);
                    }
                    #endif
                    #if USING_SCRIPT_4_BYTE_TYPES
                    case 2: {
                        ui4_ptr = reinterpret_cast<const uint32_t*>
                            (args[arg_index]);
                        if (ui4_ptr == nullptr)
                            return BOutError (bout, kErrorImplementation, params,
                                               index, start);
                        ui4 = *ui4_ptr;
                        length = static_cast<uint_t>(ui4);
                        ui1_ptr = reinterpret_cast<const char*> (ui4_ptr);
                    }
                    #endif
                    #if USING_SCRIPT_8_BYTE_TYPES
                    case 3: {
                        ui8_ptr = reinterpret_cast<const uint64_t*>
                            (args[arg_index]);
                        if (ui8_ptr == nullptr)
                            return BOutError (bout, kErrorImplementation, params,
                                               index, start);
                        ui8 = *ui8_ptr;
                        length = static_cast<uint_t>(ui8);
                        ui1_ptr = reinterpret_cast<const char*> (ui8_ptr);
                    }
                    #endif  //< USING_SCRIPT_8_BYTE_TYPES
                    default: {
                       // This wont happen due to the & 0x3 bit mask
                       // but it stops the compiler from barking.
                        return BOutError (bout, kErrorImplementation, params,
                                           index, start);
                    }

                }
                if (space < length) {
                    return BOutError (bout, kErrorBufferOverflow,
                                       params, index, start);
                }
                if (length == 0) {
                    break;          //< Not sure if this is an error.
                }
                if (start + length >= end) {
                    for (; size - length > 0; --length) {
                        ui1 = *(ui1_ptr++);
                        hash = Hash16 (ui1, hash);
                        *stop = ui1;
                        ++stop;
                    }
                    stop = begin - 1;
                    for (; length > 0; --length) {
                        ui1 = *(ui1_ptr++);
                        hash = Hash16 (ui1, hash);
                        *stop = ui1;
                        ++stop;
                    }
                    break;
                }
                for (; length > 0; --length) {
                    ui1 = *(ui1_ptr++);
                    hash = Hash16 (ui1, hash);
                    *stop = ui1;
                    ++stop;
                }
                break;
            }
        }
        ++arg_index;
    }
    if (space < 3)
        return BOutError (bout, kErrorBufferOverflow, params, index,
                           start);
    //space -= 2;   //< We don't need to save this variable.
    *stop = (byte)hash;
    if (++stop >= end) stop -= size;
    *stop = (byte)(hash >> 8);
    if (++stop >= end) stop -= size;
    bout->stop = MemoryVector (begin, stop);
#if SCRIPT_DEBUG
    text << "\n| Done writing to B-Output with the hash 0x" << text.Hex (hash)
         << '.' << std::cout;
#endif  //< SCRIPT_DEBUG
    return 0;
}

void BOutRingBell (BOut* bout, const char* address) {
    if (!bout) {
        return;
    }
    if (address == nullptr) {
        address = "";
    }
#if SCRIPT_DEBUG
    std::cout << "\n|\n| Ringing BEL to address:" << address;
#endif  //< SCRIPT_DEBUG

    // Temp variables packed into groups of 8 bytes for memory alignment.
    byte c;

    uint_t size = bout->size,               //< Size of the buffer.
        space;                              //< Space in the buffer.
    // Convert the Slot offsets to pointers.
    char* begin = BOutBuffer (bout),        //< Beginning of the buffer.
        * end   = begin + size,             //< End of the buffer.
        * start = begin + bout->start,      //< Start of the data.
        * stop  = begin + bout->stop;       //< Stop of the data.
    space = SlotSpace (start, stop, size);
    if (space == 0) {
#if SCRIPT_DEBUG
        std::cout << "\n| Buffer overflow!";
#endif  //< SCRIPT_DEBUG
        return;
    }
    *stop = BEL;
    if (++stop >= end) stop -= size;

    c = *address;
    while (c) {
        if (space == 0) {
#if SCRIPT_DEBUG
            std::cout << "\n| Buffer overflow!";
#endif  //< SCRIPT_DEBUG
            return;
        }
        *stop = c;
        if (++stop >= end) stop -= size;
        ++address;
        c = *address;
    }
    bout->stop = MemoryVector (begin, stop);
}

void BOutAckBack (BOut* bout, const char* address) {
    if (!bout) {
        return;
    }
    if (address == nullptr) {
        address = "";
    }
#if SCRIPT_DEBUG
    std::cout << "\n|\n| Ringing BEL to address:" << address;
#endif  //< SCRIPT_DEBUG

    // Temp variables packed into groups of 8 bytes for memory alignment.
    byte c;

    uint_t size = bout->size,               //< Size of the buffer.
        space;                              //< Space in the buffer.
    // Convert the Slot offsets to pointers.
    char* begin = BOutBuffer (bout),        //< Beginning of the buffer.
        * end   = begin + size,             //< End of the buffer.
        * start = begin + bout->start,      //< Start of the data.
        * stop  = begin + bout->stop;       //< Stop of the data.
    space = SlotSpace (start, stop, size);
    if (space == 0) {
#if SCRIPT_DEBUG
        std::cout << "\n| Buffer overflow!";
#endif  //< SCRIPT_DEBUG
        return;
    }
    *stop = ACK;
    if (++stop >= end) stop -= size;

    c = *address;
    while (c) {
        if (space == 0) {
#if SCRIPT_DEBUG
            std::cout << "\n| Buffer overflow!";
#endif  //< SCRIPT_DEBUG
            return;
        }
        *stop = c;
        if (++stop >= end) stop -= size;
        ++address;
        c = *address;
    }
    bout->stop = MemoryVector (begin, stop);
}

const Op* BOutConnect (BOut* bout, const char* address) {
    void* args[2];
    return BOutWrite (bout, Params <2, ADR, ADR> (),
                      Args (args, address, Address<_::BEL> ()));
}

#if USING_SCRIPT_TEXT
Strand& BOutPrint (BOut* bout, Strand& strand) {
    strand << strand.Line ('_');
    if (!bout) {
        return strand << "\n| BOut: NIL" << strand.Line ('_');
    }
    int size = bout->size;
    return strand << "\n| BOut:0x" << strand.Pointer (bout) << " size:" << size
                  << " start:" << bout->start << " stop:" << bout->stop
                  << " read:" << bout->read
                  << strand.Memory (BOutBuffer (bout), size + 64);
    //< @todo remove the + 64.);
}
#endif  //< USING_SCRIPT_TEXT

}       //< namespace _

/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/impl/script_bin.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
             All right reserved (R). Licensed under the Apache License, Version 
             2.0 (the "License"); you may not use this file except in 
             compliance with the License. You may obtain a copy of the License 
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless 
             required by applicable law or agreed to in writing, software 
             distributed under the License is distributed on an "AS IS" BASIS, 
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
    @desc    Slot implementation split into three files, slot.h, 
             monoid_rx.h, and monoid_tx.h because of the large read/write
             functions.
*/

#include <stdafx.h>
#include "../include/bin.h"
#include "../include/operation.h"
#include "../include/utils.h"
#include "../include/args.h"
#include "../include/types.h"

namespace _ {

uint_t SlotLength (byte* start, byte* stop, uint_t size) {
    if (start > stop)
        return size - (start - stop);
    return stop - start;
}

uint_t SlotSpace (byte* start, byte* stop, uint_t size) {
    if (start > stop)
        return start - stop;
    return size - (stop - start);
}

const char* BinErrorString (Bin::Error e) {
#if USING_CONSOLE
    static const char* strings[] = {
        "Buffer overflow",              //<  0
        "Buffer underflow",             //<  1
        "Varint overflow",              //<  2
        "Invalid hash",                 //<  3
        "Invalid type",                 //<  4
        "Invalid index",                //<  5
        "Invalid equerry",              //<  6
        "Invalid argument number",      //<  7
        "Invalid door",                 //<  8
        "Too many parameters",          //<  9
        "Stack overflow",               //< 10
        "Invalid subset",               //< 11
        "Too many pops",                //< 12
        "String buffer overflow",       //< 13
        "Invalid error handler",        //< 14
        "Invalid operation",            //< 15
        "Array overflow",               //< 16
        "Invalid Set",                  //< 17
        "Room Error"                    //< 18
    };
    if (e < 0 || e > Bin::RoomError)
        return strings[Bin::RoomError];
    return strings[e];
#else
    return kEmptyString;
#endif
}

const char* BinStateString (Bin::State state) {
    static const char* strings[] = {
        "Scanning char ",
        "Scanning varint",
        "Scanning address",
        "Scanning POD",
        "Scanning args",
        "Handling error",
        "Invalid operation",
        "Scanning hash",
        "Locked"
    };
    if (state >= Bin::LockedState)
        return strings[Bin::LockedState];
    return strings[state];
}

const Operation* BinResult (Bin* bin, Bin::Error error) {
    std::cout << "\nBin " << BinErrorString (error) << " Error!\n";
    return reinterpret_cast<const Operation*> (1);
}

const Operation* BinResult (Bin* bin, Bin::Error error, const uint_t* header) {
    std::cout << "\nBin Bin " << BinErrorString (error) << " Error!\n";
    return reinterpret_cast<const Operation*> (1);
}

const Operation* BinResult (Bin* bin, Bin::Error error, const uint_t* header,
                            byte offset) {
    std::cout << "\nBin " << BinErrorString (error) << " Error!\n";
    return reinterpret_cast<const Operation*> (1);
}

const Operation* BinResult (Bin* bin, Bin::Error error, const uint_t* header,
                            byte offset, byte* address) {
    std::cout << "\nBin " << BinErrorString (error) << " Error!\n";
    return reinterpret_cast<const Operation*> (1);
}

Bin* BinInit (uintptr_t* buffer, uint_t size) {
    if (size < kMinSlotSize)
        return nullptr;
    if (buffer == nullptr)
        return nullptr;

    Bin* bin   = reinterpret_cast<Bin*> (buffer);
    bin->size  = size - (4 * sizeof (uint_t));
    bin->start = 0;
    bin->stop  = 0;
    bin->read  = 0;

#if DEBUG_SCRIPT
    memset (&bin->buffer, '\0', size);
#endif
    return bin;
}

uint_t BinSpace (Bin* bin) {
    if (bin == nullptr) return ~0;
    byte* txb_ptr = reinterpret_cast<byte*>(bin);
    return SlotSpace (txb_ptr + bin->start, txb_ptr + bin->stop,
        bin->size);
}

uint_t BinBufferLength (Bin* bin) {
    if (bin == nullptr) return ~0;
    byte* base = &bin->buffer;
    return SlotLength (base + bin->start, base + bin->stop, bin->size);
}

byte* BinEndAddress (Bin* bin) {
    return reinterpret_cast<byte*>(bin) + (4 * sizeof (uint_t)) + bin->size;
}

int BinStreamByte (Bin* bin) {

    byte* begin = &bin->buffer,
        *end = begin + bin->size;
    byte* open = (byte*)begin + bin->read,
        *start = begin + bin->start,
        *cursor = start;

    int length = start < open?open - start + 1:
        (end - start) + (open - begin) + 2;

    if (length < 1) {
        BinResult (bin, Bin::BufferOverflowError, Params<1, STR> (), 2, start);
        return -1;
    }
    byte b = *cursor;
    bin->stop = (++cursor > end)?static_cast<uint_t> (Diff (begin, end)):
        static_cast<uint_t> (Diff (begin, cursor));
    return 0;
}

bool BinIsReadable (Bin* bin) {
    return BinBufferLength (bin) > 0;
}

void BinPrint (Bin* bin) {
    PrintLine ('_');
    if (bin == nullptr) {
        printf ("| Bin null\n");
        return;
    }
    uint_t size = bin->size;
    printf ("| Bin 0x%p: size: %u, start: %u, stop: %u, read: %u\n", bin, size,
        bin->start, bin->stop, bin->read);
    PrintMemory (&bin->buffer, size + 64); // @todo remove the + 64.
}

const Operation* BinRead (Bin* bin, const uint_t* params, void** args) {
#if DEBUG_SCRIPT
    std::cout << "\n\Reading ";
    ParamsPrint (params);
    std::cout << " from B-Input:";
    printf ("%p\n", bin);
    BinPrint (bin);
#endif
    if (bin == nullptr)
        return BinResult (bin, Bin::RoomError);
    if (params == nullptr)
        return BinResult (bin, Bin::RoomError);
    if (args == nullptr)
        return BinResult (bin, Bin::RoomError);
    byte     type,                  //< The current type being read.
             ui1;                   //< Temp variable.
    uint16_t ui2;                   //< Temp variable.
#if USING_VARINT4 || USING_AR4 || USING_BK4
    uint32_t ui4;
#endif
#if USING_VARINT8 || USING_AR8 || USING_BK8
    uint64_t ui8;
#endif

    byte*     ui1_ptr;              //< Pointer to a UI1.
#if  USING_VARINT2 || USING_BK2
    uint16_t* ui2_ptr;              //< Pointer to a UI2.
#endif
#if USING_VARINT4 || USING_AR4 || USING_BK4
    uint32_t* ui4_ptr;              //< Pointer to a UI4.
#endif
#if USING_VARINT8 || USING_AR8 || USING_BK8
    uint64_t* ui8_ptr;              //< Pointer to a UI1.
#endif
    uint_t size,                    //< Size of the ring buffer.
           length,                  //< Length of the data in the buffer.
           count,                   //< Argument length.
           index,                   //< Index in the params.
           arg_index = 0,           //< Index in the args.
           temp,
           num_params = *params;    //< Number of params.
    hash16_t hash;

    if (num_params == 0) return 0;  //< Nothing to do.

    hash = 0;
    size = bin->size;

    byte* begin = &bin->buffer,       //< The beginning of the buffer.
        * end   = begin + size,       //< The end of the buffer.
        * start = begin + bin->start, //< The start of the data.
        * stop  = begin + bin->stop;  //< The stop of the data.
    const uint_t* param = params + 1; //< The current param.

    length = SlotLength (start, stop, size);
    
    // When we scan, we are reading from the beginning of the Slot buffer.

    for (index = 1; index <= num_params; ++index) {
        type   = params[index];
#if DEBUG_SCRIPT
        printf ("\n| param:%2u TType:%s  start:%u, stop:%u length:%u",
                arg_index + 1,  TypeString (type), Diff (begin, start),
                Diff (begin, stop), length);
#endif
        switch (type) {
            case NIL:
                return BinResult (bin, Bin::InvalidTypeError, params, index,
                                  start);
            case ADR:
            case STR: //< _R_e_a_d__S_t_r_i_n_g_-_8_____________________________
                // Load buffered-type argument length and increment the index.
                ++num_params;
                count = params[++index];
                //< @todo Replace with pointer arithmetic.
                // Load next pointer and increment args.
                ui1_ptr = reinterpret_cast<byte*> (args[arg_index]);
                if (ui1_ptr == nullptr)
                    return BinResult (bin, Bin::RoomError, params, index, 
                                      start);
#if DEBUG_SCRIPT
                printf ("\n| Reading STR:%p with max length:%u \"", ui1_ptr, 
                        count);
#endif
                // Read char.
                ui1 = *start;
                hash = Hash16 (ui1, hash);
                if (++start >= end) start -= size;
                *ui1_ptr = ui1;
                ++ui1_ptr;
#if DEBUG_SCRIPT
                std::cout << ui1;
#endif
                while ((ui1 != 0) && (count != 0)) {
                    --count;
                    if (count == 0) //< Reached count:0 before nil-term char.
                        return BinResult (bin, Bin::BufferUnderflowError,
                                          params, index, start);
                    ui1 = *start;       // Read byte from ring-buffer.
                    hash = Hash16 (ui1, hash);
                    if (++start >= end) start -= size;
                    *ui1_ptr++ = ui1;   // Write byte to destination.
#if DEBUG_SCRIPT
                    std::cout << ui1;
#endif
                }
                #if DEBUG_SCRIPT
                std::cout << "\" success!\n";
                #endif
                if (type != ADR) {
                    *ui1_ptr = 0;
                    // No need to hash 0.
                }
                break;
            case ST2: //< _R_e_a_d__S_t_r_i_n_g_-_16____________________________
#if USING_ST2
                // Load buffered-type argument length and increment the index.
                ++num_params;
                count = params[++index];
                //#if DEBUG_SCRIPT
                //printf ("\n|           Reading char with max length %u: ",
                //        count);
                //#endif
                // Load next pointer and increment args.
                ui2_ptr = reinterpret_cast<uint16_t*> (args[arg_index]);
                if (ui2_ptr == nullptr) break;

                // Read char.
                ui2 = *start;
                hash = Hash16UI2 (ui2, hash);
                if (++start >= end) start -= size;
                *ui2_ptr = ui2;
                ++ui2_ptr;

                while (ui2 != 0 && count != 0) {
                    if (count == 1)
                        return BinResult (bin, Bin::BufferUnderflowError, 
                                          params, index, start);
                    --count;
                    //#if DEBUG_SCRIPT
                    //std::cout << ui2;
                    //#endif
                    ui2 = *start;       // Read byte from ring-buffer.
                    hash = Hash16UI2 (ui2, hash);
                    if (++start >= end) start -= size;
                    *ui2_ptr++ = ui2;   // Write byte to destination.
                }
                //#if DEBUG_SCRIPT
                //            //printf (" done!\n");
                //#endif
                hash = Hash16UI2 (ui2, hash);
                *ui2_ptr = ui2;
                break;
#else
                return BinResult (bin, Bin::InvalidTypeError, params, 
                                  index, start);
#endif
            case SI1: //< _R_e_a_d__1__B_y_t_e__T_y_p_e_s_______________________
            case UI1:
            case BOL:
#if USING_1_BYTE_TYPES
                if (length-- == 0)
                    return BinResult (bin, Bin::BufferUnderflowError, params,
                                      index, start);

                // Load next pointer and increment args.
                ui1_ptr = reinterpret_cast<byte*> (args[arg_index]);
                if (ui1_ptr == 0)
                    break;

                // Read type;

                // Byte 1
                ui1 = *start;                       //< Read
                std::cout << " \'" << ui1 << "\',";
                hash = Hash16 (ui1, hash);          //< Hash
                if (++start >= end) start -= size;  //< Increment
                *ui1_ptr = ui1;                     //< Write
                break;
#else
                return BinResult (bin, Bin::InvalidTypeError, params, 
                                  index, start);
#endif
            case SI2: //< _R_e_a_d__1_6_-_b_i_t__T_y_p_e_s______________________
            case UI2:
            case HLF:
#if USING_2_BYTE_TYPES
                if (length < 2)
                    return BinResult (bin, Bin::BufferUnderflowError, params,
                                      index, start);
                length -= 2;

                // Load next pointer and increment args.
                ui1_ptr = reinterpret_cast<byte*> (args[arg_index]);
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
                return BinResult (bin, Bin::InvalidTypeError, params, 
                                  index, start);
#endif
            case SI4: //< _R_e_a_d__3_2_-_b_i_t__T_y_p_e_s______________________
            case UI4:
            case FLT:
            case TMS:
#if USING_4_BYTE_TYPES
                if (length < 4)
                    return BinResult (bin, Bin::BufferUnderflowError, params, 
                                      index, start);
                length -= 4;

                // Load next pointer and increment args.
                ui1_ptr = reinterpret_cast<byte*> (args[arg_index]);
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
                return BinResult (bin, Bin::InvalidTypeError, params,
                                  index, start);
#endif
            case TMU: //< _R_e_a_d__6_4_-_b_i_t__T_y_p_e_s______________________
            case SI8:
            case UI8:
            case DBL:
#if USING_8_BYTE_TYPES
                if (length < 8)
                    return BinResult (bin, Bin::BufferUnderflowError, params,
                                      index, start);
                length -= 8;

                // Load next pointer and increment args.
                ui1_ptr = reinterpret_cast<byte*> (args[arg_index]);
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
                return BinResult (bin, Bin::InvalidTypeError, params,
                                  index, start);
#endif
            case SV2: //< _R_e_a_d__2_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t_______

#if USING_VARINT2
                      // Load next pointer and increment args.
                ui2_ptr = reinterpret_cast<uint16_t*> (args[arg_index]);
                if (ui2_ptr == nullptr)
                    return BinResult (bin, Bin::RoomError, params, index,
                                      start);

                // Read byte 1
                if (length-- == 0)
                    return BinResult (bin, Bin::BufferUnderflowError, params,
                                      index, start);
                ui1 = *start;
                if (++start >= end) start -= size;
                hash = Hash16 (ui1, hash);
                if ((ui1 >> 7) != 0) {
                    ui2 = ui1 & 0x7F;   //< Mask off end-of-varint MSb.
                    *ui2_ptr = UnpackSignedVarint<uint16_t> (ui2);
                    break;
                }
                ui2 = ui1;

                // Read byte 2
                if (--length == 0)
                    return BinResult (bin, Bin::BufferUnderflowError, params,
                                      index, start);
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
                    return BinResult (bin, Bin::BufferUnderflowError, params,
                                      index, start);
                ui1 = *start;
                if (++start >= end) start -= size;
                hash = Hash16 (ui1, hash);
                if ((ui1 >> 7) == 0)
                    return BinResult (bin, Bin::VarintOverflowError, params,
                                      index, start);
                ui2 |= ((uint16_t)(ui1 & 0x7F) << 14);
                *ui2_ptr = UnpackSignedVarint<uint16_t> (ui2);
                break;

            case UV2: //< _R_e_a_d__2_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t___
                      // Load next pointer and increment args.
                ui2_ptr = reinterpret_cast<uint16_t*> (args[arg_index]);
                if (ui2_ptr == nullptr)
                    return BinResult (bin, Bin::RoomError, params, index,
                                      start);

                // Read byte 1
                if (length-- == 0)
                    return BinResult (bin, Bin::BufferUnderflowError, params,
                                      index, start);
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
                    return BinResult (bin, Bin::BufferUnderflowError, params,
                                      index, start);
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
                    return BinResult (bin, Bin::BufferUnderflowError, params,
                                      index, start);
                ui1 = *start;
                if (++start >= end) start -= size;
                hash = Hash16 (ui1, hash);
                if ((ui1 >> 7) == 0)
                    return BinResult (bin, Bin::VarintOverflowError, params,
                                      index, start);
                ui2 |= ((uint16_t)(ui1 & 0x7F) << 14);
                *ui2_ptr = ui2;
                break;
#else
            case UV2:
                return BinResult (bin, Bin::InvalidTypeError, params, index,
                                  start);
#endif
            case SV4: //< _R_e_a_d__4_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t_______
            case UV4: //< _R_e_a_d__4_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t___
#if USING_VARINT4
                      // Load next pointer and increment args.
                ui4_ptr = reinterpret_cast<uint32_t*> (args[arg_index]);
                if (ui4_ptr == nullptr)
                    return BinResult (bin, Bin::RoomError, params, index,
                                      start);

                // Scan byte 1.
                ui1 = *start;
                if (++start >= end) start -= size;
                hash = Hash16 (ui1, hash);
                ui4 = ui1;
                ui2 = 7;        //< Number of bits to shift ui1 to the left.
                count = 5; //< The max number of Varint4 bytes.
                while (ui1 >> 7 == 0) {
                    if (length-- == 0)
                        return BinResult (bin, Bin::BufferUnderflowError,
                                          params, index, start);
                    ui1 = *start;
                    if (++start >= end) start -= size;
                    hash = Hash16 (ui1, hash);
                    ui4 |= ((uint32_t)(ui1 & 0x7F)) << ui2;
                    //< @todo I'm starting to second guess if we need to mask ui1 
                    //< because we're packing them up and will overwrite.
                    ui2 += 7;
                    if (--count == 0)
                        return BinResult (bin, Bin::VarintOverflowError, params,
                                          index, start);
                }
                if (count == 5)    //< If there is only one byte we need to
                    ui4 &= 0x7F;        //< mask off the terminating varint bit.
                if (type == SV4)
                    ui4 = UnpackSignedVarint<uint32_t> (ui4);
                *ui4_ptr = ui4;
                break;
#else
                return BinResult (bin, Bin::InvalidTypeError, params, index,
                                  start);
#endif
            case SV8: //< _R_e_a_d__V_a_r_i_n_t__8______________________________
            case UV8:
#if USING_VARINT8
                // Load next pointer and increment args.
                ui8_ptr = reinterpret_cast<uint64_t*> (args[arg_index]);
                if (ui8_ptr == nullptr)
                    return BinResult (bin, Bin::RoomError, params, index,
                                      start);

                // Scan byte 1.
                ui1 = *start;
                if (++start >= end) start -= size;
                hash = Hash16 (ui1, hash);
                ui8 = ui1;
                ui2 = 7;        //< Number of bits to shift ui1 to the left.
                count = 9; //< The max number of Varint8 bytes.
                while (ui1 >> 7 == 0) {
                    if (length-- == 0)
                        return BinResult (bin, Bin::BufferUnderflowError,
                                          params, index, start);
                    ui1 = *start;
                    if (++start >= end) start -= size;
                    hash = Hash16 (ui1, hash);
                    if (count == 1) {
                        // Varint 8 differs from Varint 2 and 4 in that on the 
                        // last byte, byte 9 of 9, there is no terminating 
                        // varint bit.
                        ui8 |= ((uint64_t)(ui1)) << ui2;
                        break;
                    }
                    ui8 |= ((uint64_t)(ui1 & 0x7F)) << ui2;
                    //< @todo I'm starting to second guess if we need to mask 
                    //< ui1 because we're packing them up and will overwrite.
                    ui2 += 7;
                    if (--count == 0)
                        return BinResult (bin, Bin::VarintOverflowError, params,
                                          index, start);
                }
                if (count == 9)    //< If there is only one byte we need to
                    ui8 &= 0x7F;        //< mask off the terminating varint bit.
                if (type == SV8)
                    ui8 = UnpackSignedVarint<uint64_t> (ui8);
                *ui8_ptr = ui8;
                break;
#else
                return BinResult (bin, Bin::InvalidTypeError, params, index,
                                  start);
#endif
            case BSC: //< _B_-_S_e_q_u_e_n_c_e__S_t_r_i_n_g_____________________
#if USING_BSC
            ui1_ptr = reinterpret_cast<byte*> (args[arg_index]);
            if (ui1_ptr == nullptr)
                return BinResult (bin, Bin::RoomError, params, index,
                                  start);
            ui1 = *start;
#endif 
            default: {  //< It's an Array
                std::cout << "\nIt's an array!\n";
                switch ((type >> 5) & 0x3) {
                    case 0: {

                    if ((type < LST) && (type < MAP))
                        return BinResult (bin, Bin::InvalidTypeError, params, 
                                          index, start);
                        if (length < 1)
                            return BinResult (bin, Bin::BufferUnderflowError,
                                              params, index, start);
                        ui1_ptr = reinterpret_cast<byte*> (args[arg_index]);
                        if (ui1_ptr == nullptr)
                            return BinResult (bin, Bin::RoomError, params,
                                              index, start);

                        ui1 = *start;
                        if (++start >= end) start -= size;
                        hash = Hash16 (ui1, hash);
                        if (ui1 > length - 1)
                            return BinResult (bin, Bin::BufferOverflowError,
                                              params, index, start);
                        length = length - count - 1;
                        count = (uintptr_t)ui1;
                        break;
                    }
                    case 1:
                    {
                        if (length < 2)
                            return BinResult (bin, Bin::BufferUnderflowError,
                                              params, index, start);
                        ui2_ptr = reinterpret_cast<uint16_t*> 
                                    (args[arg_index]);
                        if (ui2_ptr == nullptr)
                            return BinResult (bin, Bin::RoomError, params,
                                              index, start);

                        for (temp = 0; temp <= sizeof (uint16_t); temp += 8) {
                            ui1 = *start;
                            if (++start >= end) start -= size;
                            hash = Hash16 (ui1, hash);
                            ui2 |= ((uint16_t)ui1) << temp;
                        }
                        if (ui2 > length - 2)
                            return BinResult (bin, Bin::BufferOverflowError,
                                              params, index, start);
                        length = length - count - 2;
                        count = (uintptr_t)ui2;
                        ui1_ptr = reinterpret_cast<byte*> (ui2_ptr);
                        break;
                    }
                    case 2:
                    {
                        if (length < 4)
                            return BinResult (bin, Bin::BufferUnderflowError,
                                              params, index, start);
                        ui4_ptr = reinterpret_cast<uint32_t*> (args[arg_index]);
                        if (ui4_ptr == nullptr)
                            return BinResult (bin, Bin::RoomError, params,
                                              index, start);

                        for (temp = 0; temp <= sizeof (uint32_t); temp += 8) {
                            ui1 = *start;
                            if (++start >= end) start -= size;
                            hash = Hash16 (ui1, hash);
                            ui4 |= ((uint32_t)ui1) << temp;
                        }
                        if (ui4 >= length - 4)
                            return BinResult (bin, Bin::BufferOverflowError,
                                              params, index, start);
                        length = length - count - 5;
                        count = (uintptr_t)ui4;
                        ui1_ptr = reinterpret_cast<byte*> (ui4_ptr);
                        break;
                    }
                    case 3:
                    {
                        if (length < 8)
                            return BinResult (bin, Bin::BufferUnderflowError,
                                              params, index, start);
                        ui8_ptr = reinterpret_cast<uint64_t*> (args[arg_index]);
                        if (ui8_ptr == nullptr)
                            return BinResult (bin, Bin::RoomError, params,
                                              index, start);

                        for (temp = 0; temp <= sizeof (uint64_t); temp += 8) {
                            ui1 = *start;
                            if (++start >= end) start -= size;
                            hash = Hash16 (ui1, hash);
                            ui8 |= ((uint64_t)ui1) << temp;
                        }
                        if (ui8 > length - 8)
                            return BinResult (bin, Bin::BufferOverflowError,
                                              params, index, start);
                        length = length - count - 8;
                        count = (uintptr_t)ui8;
                        ui1_ptr = reinterpret_cast<byte*> (ui8_ptr);
                        break;

                    }
                    default: return BinResult (bin, Bin::RoomError, params,
                                               index, start);
                }

                if (length < count)
                    return BinResult (bin, Bin::BufferOverflowError, params,
                                      index, start);
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
                
        }
        ++arg_index;
        std::cout << " |";
    }
    #if DEBUG_SCRIPT
    printf ("\n| Hash expected: %x ", hash);
    #endif
    if (length < 2)
        return BinResult (bin, Bin::BufferUnderflowError, params, index,
                          start);
    ui2 = *start;
    if (++start >= end) start -= size;
    ui1 = *start;
    if (++start >= end) start -= size;
    ui2 |= (((uint16_t)ui1) << 8);
    #if DEBUG_SCRIPT
    printf ("found: %x\n", ui2);
    #endif
    if (hash != ui2)
        return BinResult (bin, Bin::InvalidHashError, params, index,
                          start);

//#if DEBUG_SCRIPT
//    //printf ("| Done reading\n");
//    SlotClear (begin, bin->start, start, stop, end, size);
//#endif

    // Convert pointer back to offset
    bin->start = Diff (begin, start);

    return 0;
}

}       //< namespace _

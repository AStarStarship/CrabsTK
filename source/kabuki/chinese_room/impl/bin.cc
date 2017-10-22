/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/impl/bin.cc
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
#include "../include/slot.h"
#include "../include/operation.h"

namespace _ {

const char* BinErrorString (Bin::Error e) {
#if USING_CONSOLE
    static const char* strings[] = {
        "Buffer underflow",             //< 0
        "Buffer overflow",              //< 1
        "Varint overflow",              //< 2
        "Invalid hash",                 //< 3
        "Invalid type",                 //< 4
        "Invalid index",                //< 5
        "Invalid equerry",              //< 6
        "Invalid argument number",      //< 7
        "Invalid door",                 //< 8
        "Too many parameters",          //< 9
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
        "Scanning string ",
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
    // @todo Write me.
    return 0;
}

const Operation* BinResult (Bin* bin, Bin::Error error, params_t* header) {
    // @todo Write me.
    return 0;
}

const Operation* BinResult (Bin* bin, Bin::Error error, params_t* header,
                            byte offset) {
    // @todo Write me.
    return 0;
}

const Operation* BinResult (Bin* bin, Bin::Error error, params_t* header,
                            byte offset, byte* address) {
    // @todo Write me.
    return 0;
}

Bin* BinInit (byte* buffer, uint_t size) {
    if (size < kMinSlotSize) return nullptr;
    if (buffer == nullptr) return nullptr;

    Bin* bin = reinterpret_cast<Bin*> (buffer);
    bin->size = size - (4 * sizeof (uint_t));
    bin->start = 0;
    bin->stop = 0;
    bin->read = 0;

#if DEBUG_CHINESE_ROOM
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
        BinResult (bin, Bin::BufferOverflowError, Params<1, STX> (), 2, start);
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
    PrintMemory (&bin->buffer, size);
}
    
const Operation* BinRead (Bin* bin, params_t* params, void** args) {
    if (bin == nullptr)
        return BinResult (bin, Bin::RoomError);
    if (params == nullptr)
        return BinResult (bin, Bin::RoomError);
    if (args == nullptr)
        return BinResult (bin, Bin::RoomError);

    byte type,                          //< The current type being read.
        ui1;                            //< Temp variable.
    uint16_t ui2;                       //< Temp variable.
#if USING_VARINT4 || USING_AR4 || USING_BK4
    uint32_t ui4;
#endif
#if USING_VARINT8 || USING_AR8 || USING_BK8
    uint64_t ui8;
#endif

    byte*     ui1_ptr;                  //< Pointer to a UI1.
#if  USING_VARINT2 || USING_BK2
    uint16_t* ui2_ptr;                  //< Pointer to a UI2.
#endif
#if USING_VARINT4 || USING_AR4 || USING_BK4
    uint32_t* ui4_ptr;                  //< Pointer to a UI4.
#endif
#if USING_VARINT8 || USING_AR8 || USING_BK8
    uint64_t* ui8_ptr;                  //< Pointer to a UI1.
#endif
    uint_t size,                        //< Size of the ring buffer.
           length,                      //< Length of the data in the buffer.
           count,                       //< Argument length.
           index,                       //< Index in the escape sequence.
           num_params = *params;        //< Number of params.
    hash16_t hash;

    if (num_params == 0) return 0;      //< Nothing to do.

    hash = 0;
    size = bin->size;

    byte* begin = &bin->buffer,         //< The beginning of the buffer.
        * end = begin + size,           //< The end of the buffer.
        * start = begin + bin->start,   //< The start of the data.
        * stop = begin + bin->stop;     //< The stop of the data.
    params_t* param = params + 1;       //< The current param.

    length = SlotLength (start, stop, size);
    //
    //#if DEBUG_CHINESE_ROOM
    //    //printf ("\n\n| Reading: ");
    //   ParamsPrint (params);
    //    //printf ("| begin: 0x%p start : %u stop : %u end : %u "
    //            "length: %u ", begin, Diff (begin, start), 
    //            Diff (begin, stop), Diff (begin, end), length);
    //#endif
    // When we scan, we are reading from the beginning of the Slot buffer.

    for (index = 0; index < num_params; ++index) {
         type = *param;
         ++param;
         start += TypeAlign (start, type);
//#if DEBUG_CHINESE_ROOM
//        //printf ("\n| Index %2u: %s  start: %u, stop: %u hash: ", index,  
//                TypeString (type), Diff (begin, start), Diff (begin, stop));
//#endif

        switch (type) {
          case NIL:
              goto BoutInvalidType;
          case SOH: //< _R_e_a_d__S_t_r_i_n_g_-_8_______________________________________
          case STX:
              // Load buffered-type argument length and increment the index.
              count = *param;
              ++param;
//#if DEBUG_CHINESE_ROOM
//              //printf ("\n|           Reading string with max length %u: ", count);
//#endif
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
                if (count == 1)
                    return BinResult (bin, Bin::BufferUnderflowError, params, index, start);
                --count;
//#if DEBUG_CHINESE_ROOM
//                std::cout << ui1;
//#endif
                ui1 = *start;       // Read byte from ring-buffer.
                hash = Hash16 (ui1, hash);
                if (++start >= end) start -= size;
                *ui1_ptr++ = ui1;   // Write byte to destination.
            }
//#if DEBUG_CHINESE_ROOM
//            //printf (" done!\n");
//#endif
            if (type != SOH) {
                hash = Hash16 (ui1, hash);
                *ui1_ptr = ui1;
            }
            break;
          case SI1: //< _R_e_a_d__1__B_y_t_e__T_y_p_e_s_________________________________
          case UI1:
          case BOL:
#if USING_1_BYTE_TYPES
            if (length-- == 0) 
                return BinResult (bin, Bin::BufferUnderflowError, params, index, start);

            // Load next pointer and increment args.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == 0)
                break;

            // Read type;

            // Byte 1
            ui1 = *start;                       //< Read
            hash = Hash16 (ui1, hash);          //< Hash
            if (++start >= end) start -= size;  //< Increment
            *ui1_ptr = ui1;                     //< Write
            break;
#else
            goto BoutInvalidType;
#endif
          case SI2: //< _R_e_a_d__1_6_-_b_i_t__T_y_p_e_s________________________
          case UI2:
          case HLF:
#if USING_2_BYTE_TYPES
            if (length < 2)
                return BinResult (bin, Bin::BufferUnderflowError, params, index,
                                  start);
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
            goto BoutInvalidType;
#endif
          case SI4: //< _R_e_a_d__3_2_-_b_i_t__T_y_p_e_s________________________
          case UI4:
          case FLT:
          case TMS:
#if USING_4_BYTE_TYPES
            if (length < 4) 
                return BinResult (bin, Bin::BufferUnderflowError, params, index, start);
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
            goto BoutInvalidType;
#endif
          case TMU: //< _R_e_a_d__6_4_-_b_i_t__T_y_p_e_s________________________
          case SI8:
          case UI8:
          case DBL:
#if USING_8_BYTE_TYPES
            if (length < 8)
                  return BinResult (bin, Bin::BufferUnderflowError, params, index, start);
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
            goto BoutInvalidType;
#endif
          case SV2: //< _R_e_a_d__2_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t_________

#if USING_VARINT2
            // Load next pointer and increment args.
            ui2_ptr = reinterpret_cast<uint16_t*> (args[index]);
            if (ui2_ptr == nullptr)
                return BinResult (bin, Bin::RoomError, params, index, start);

            // Read byte 1
            if (length-- == 0)
                return BinResult (bin, Bin::BufferUnderflowError, params, index, start);
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
                return BinResult (bin, Bin::BufferUnderflowError, params, index, start);
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
                return BinResult (bin, Bin::BufferUnderflowError, params, index, start);
            ui1 = *start;
            if (++start >= end) start -= size;
            hash = Hash16 (ui1, hash);
            if ((ui1 >> 7) == 0)
                return BinResult (bin, Bin::VarintOverflowError, params, index, start);
            ui2 |= ((uint16_t)(ui1 & 0x7F) << 14);
            *ui2_ptr = UnpackSignedVarint<uint16_t> (ui2);
            break;

          case UV2: //< _R_e_a_d__2_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t_____
            // Load next pointer and increment args.
              ui2_ptr = reinterpret_cast<uint16_t*> (args[index]);
              if (ui2_ptr == nullptr)
                  return BinResult (bin, Bin::RoomError, params, index, start);

              // Read byte 1
              if (length-- == 0)
                  return BinResult (bin, Bin::BufferUnderflowError, params, index, start);
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
                  return BinResult (bin, Bin::BufferUnderflowError, params, index, start);
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
                return BinResult (bin, Bin::BufferUnderflowError, params, index, start);
            ui1 = *start;
            if (++start >= end) start -= size;
            hash = Hash16 (ui1, hash);
            if ((ui1 >> 7) == 0)
                return BinResult (bin, Bin::VarintOverflowError, params, index, start);
            ui2 |= ((uint16_t)(ui1 & 0x7F) << 14);
            *ui2_ptr = ui2;
            break;
#else
          case UV2:
            goto BoutInvalidType;
#endif
          case SV4: //< _R_e_a_d__4_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t_________
          case UV4: //< _R_e_a_d__4_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t_____
#if USING_VARINT4
            // Load next pointer and increment args.
            ui4_ptr = reinterpret_cast<uint32_t*> (args[index]);
            if (ui4_ptr == nullptr)
                return BinResult (bin, Bin::RoomError, params, index, start);

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
                    return BinResult (bin, Bin::BufferUnderflowError, params, index, start);
                ui1 = *start;
                if (++start >= end) start -= size;
                hash = Hash16 (ui1, hash);
                ui4 |= ((uint32_t)(ui1 & 0x7F)) << ui2;
                //< @todo I'm starting to second guess if we need to mask ui1 
                //< because we're packing them up and will overwrite.
                ui2 += 7;
                if (--count == 0)
                    return BinResult (bin, Bin::VarintOverflowError, params, index, start);
            }
            if (count == 5)    //< If there is only one byte we need to
                ui4 &= 0x7F;        //< mask off the terminating varint bit.
            if (type == SV4) 
                ui4 = UnpackSignedVarint<uint32_t> (ui4);
            *ui4_ptr = ui4;
          break;
#else
            goto BoutInvalidType;
#endif
          case SV8: //< _R_e_a_d__V_a_r_i_n_t__8________________________________
          case UV8:
#if USING_VARINT8
              // Load next pointer and increment args.
              ui8_ptr = reinterpret_cast<uint64_t*> (args[index]);
              if (ui8_ptr == nullptr)
                  return BinResult (bin, Bin::RoomError, params, index, start);

              // Scan byte 1.
              ui1 = *start;
              if (++start >= end) start -= size;
              hash = Hash16 (ui1, hash);
              ui8 = ui1;
              ui2 = 7;        //< Number of bits to shift ui1 to the left.
              count = 9; //< The max number of Varint8 bytes.
              while (ui1 >> 7 == 0) {
                  if (length-- == 0)
                      return BinResult (bin, Bin::BufferUnderflowError, params, index, start);
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
                      return BinResult (bin, Bin::VarintOverflowError, params, index, start);
              }
              if (count == 9)    //< If there is only one byte we need to
                  ui8 &= 0x7F;        //< mask off the terminating varint bit.
              if (type == SV8)
                  ui8 = UnpackSignedVarint<uint64_t> (ui8);
              *ui8_ptr = ui8;
            break;
#else
              goto BoutInvalidType;
#endif
          case AR1:  //< _R_e_a_d__A_r_r_a_y_-_1________________________________
#if USING_AR1
            // Load next pointer and increment args.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == nullptr)
                return BinResult (bin, Bin::RoomError, params, index, start);
            count = *param++;
            count *= SizeOf (*param++);
            goto ReadBlock;
#else
            goto BoutInvalidType;
#endif
          case AR2:  //< _R_e_a_d__A_r_r_a_y_-_2________________________________
#if USING_AR2
              // Load the pointer to the destination.
              ui1_ptr = reinterpret_cast<byte*> (args[index]);
              if (ui1_ptr == nullptr)
                  return BinResult (bin, Bin::RoomError, params, index, start);
              count = *param++;
              count *= SizeOf (*param++);
              goto ReadBlock;
#else
            goto BoutInvalidType;
#endif
          case AR4:  //< _R_e_a_d__A_r_r_a_y_-_4________________________________
#if USING_AR4
            // Load the pointer to the destination.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == nullptr)
                return BinResult (bin, Bin::RoomError, params, index, start);
            count = *param++;
            count *= SizeOf (*param++);
            goto ReadBlock;
#else
            goto BoutInvalidType;
#endif
          case AR8:  //< _R_e_a_d__A_r_r_a_y_-_8________________________________
#if USING_AR8
            // Load the pointer to the destination.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == nullptr)
                return BinResult (bin, Bin::RoomError, params, index, start);
            count = *param++;
            count *= SizeOf (*param++);
            goto ReadBlock;
#else
              goto BoutInvalidType;
#endif
          case ESC: //< _R_e_a_d__E_s_c_a_p_e__S_e_q_u_e_n_c_e__________________
            // I'm not sure exactly how this should work. I can't do recursion
            // because of embedded limitations.
            break;
          case FS: //< _R_e_a_d__B_o_o_k_8_____________________________________
            if (length <= 128)
                return BinResult (bin, Bin::BufferUnderflowError, params, index, start);
            // Load next pointer and increment args.
            ui8_ptr = reinterpret_cast<uint64_t*> (args[index]);
            if (ui8_ptr == nullptr)
                return BinResult (bin, Bin::RoomError, params, index, start);

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
            goto BoutInvalidType;
#endif
          case GS: //< _R_e_a_d__B_o_o_k_4_______________________________________
            if (length <= 64)
                return BinResult (bin, Bin::BufferUnderflowError, params, index, start);
            // Load the pointer to the destination.
            ui4_ptr = reinterpret_cast<uint32_t*> (args[index]);
            if (ui4_ptr == nullptr)
                return BinResult (bin, Bin::RoomError, params, index, start);

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
            goto BoutInvalidType;
#endif
          case RS: //< _R_e_a_d__B_o_o_k_2_______________________________________
            if (length <= 32)
                return BinResult (bin, Bin::BufferUnderflowError, params, index, start);
            // Load the pointer to the destination.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == nullptr)
                return BinResult (bin, Bin::RoomError, params, index, start);
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
            goto BoutInvalidType;
#endif
          case US: //< _R_e_a_d__U_n_i_t__S_e_p_e_r_a_t_o_r_____________________
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            count = kUnitSize;
            ReadBlock:
            {
                if (length < count)
                    return BinResult (bin, Bin::BufferOverflowError, params, index, start);
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
            BoutInvalidType:
            {
#if DEBUG
                //printf ("\n!!!Read invalid type %u\n", type);
#endif
                return BinResult (bin, Bin::InvalidTypeError, params, index,
                                  start);
            }
        }
        std::cout << " |";
    }
    //#if DEBUG_CHINESE_ROOM
    //printf ("\n| Hash expected: %x ", hash);
    //#endif
    ui2 = *start;
    if (++start >= end) start -= size;
    ui1 = *start;
    if (++start >= end) start -= size;
    ui2 |= (((uint16_t)ui1) << 8);
    //#if DEBUG_CHINESE_ROOM
    //printf ("found: %x\n", ui2);
    //#endif
    if (hash != ui2)
        return BinResult (bin, Bin::InvalidHashError, params, index, start);

//#if DEBUG_CHINESE_ROOM
//    //printf ("| Done reading\n");
//    SlotClear (begin, bin->start, start, stop, end, size);
//#endif

    // Convert pointer back to offset
    bin->start = Diff (begin, start);

    return 0;
}

}       //< namespace _

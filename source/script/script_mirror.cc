/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/kabuki/script/mirror.h
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
#include "mirror.h"

#if SCRIPT_USING_MIRROR
#include "types.h"

namespace _ {

/*
const Operation* MirrorResult (Mirror* mirror, Error error) {
    return BinResult (reinterpret_cast<Bin*> (mirror), error);
}

const Operation* MirrorResult (Mirror* mirror, Error error) {
    return Error (reinterpret_cast<Bout*> (mirror), error);
}


const Operation* MirrorWrite (Mirror* mirror, const uint_t* params, void** args) {
    if (mirror == nullptr)     //< Is this a double check?
        return MirrorResult (mirror, kErrorImplementation);
    if (!params)
        return MirrorResult (mirror, kErrorImplementation);
    if (!args)
        return MirrorResult (mirror, kErrorImplementation);
    return MirrorResult (mirror, kErrorImplementation);
}
//bool IsWritable (MirrorOut* mirror) {
//   return mirror->start != mirror->stop;
//}*/

bool MirrorIsReadable (Mirror* mirror) {
    return (mirror->start != mirror->stop);
}

const Operation* MirrorRead (Mirror* mirror, const uint_t* params, void** args) {
    if (mirror == nullptr)
        return MirrorResult (mirror, kErrorImplementation);
    if (!params)
        return MirrorResult (mirror, kErrorImplementation);
    if (!args)
        return MirrorResult (mirror, kErrorImplementation);
    Bin* bin = reinterpret_cast<Bin*> (mirror);

    byte //array_type,            //< The current array type.
        ui1;                    //< Temp variable to load most types.
    uint16_t ui2;               //< Temp variable for working with UI2 types.
#if SCRIPT_USING_4_BYTE_TYPES || SCRIPT_USING_VARINT4
    uint32_t ui4;
#endif  //< SCRIPT_USING_4_BYTE_TYPES
#if SCRIPT_USING_8_BYTE_TYPES || SCRIPT_USING_VARINT8
    uint64_t ui8;
#endif  //< SCRIPT_USING_8_BYTE_TYPES || SCRIPT_USING_VARINT8

    byte*     ui1_ptr;          //< Pointer to a UI1.
                                //#if  SCRIPT_USING_VARINT2 || USING_BK2
    uint16_t* ui2_ptr;          //< Pointer to a UI2.
                                //#endif
                                //#if SCRIPT_USING_VARINT4 || USING_AR4 || USING_BK4
    uint32_t* ui4_ptr;          //< Pointer to a UI4.
                                //#endif
                                //#if SCRIPT_USING_VARINT8 || USING_8_BIT_TYPES
    uint64_t* ui8_ptr;          //< Pointer to a UI8.
                                //#endif
    uint_t type,                //< The current type being read.
        size,                   //< The size of the ring buffer.
        length,                 //< The length of the data in the buffer.
        index,                  //< The index in the escape sequence.
        num_params = *params;   //< The number of params.

    uintptr_t offset,           //< The offset to word align the current type.
        count;                  //< The argument length.

    if (num_params == 0) {

    //DEBUG_PRINT ("\n\n| Reading Bin: ");

        size = bin->size;

        byte* begin = reinterpret_cast<byte*> (mirror) + sizeof (mirror),
            //< The beginning of the buffer.
            *end = begin + size,                //< The end of the buffer.
            *start = begin + bin->start,          //< The start of the data.
            *stop = begin + bin->stop;           //< The stop of the data.
        const uint_t* param = params + 1;          //< The current param.

        length = MirrorLength (start, stop, size);

#if SCRIPT_DEBUG
    //Text txt ("\n\n| Reading Bin:");
    //    << ParamsPrint (params)
    //Txt << "\n| begin: 0x" << Pointer (txt, begin) << " start:" << Diff (begin, start)
    //    << " stop:" << Diff (begin, stop) << " end:" << Diff (begin, end)
    //    << " length:" << length;
#endif
    // When we scan, we are reading from the beginning of the Bin buffer.

        for (index = 0; index < num_params; ++index) {
            type = (byte)*param;
            ++param;
#if SCRIPT_DEBUG
            printf ("\n| index %2u: %s  start: %u, stop: %u hash: ", index,
                    TypeText (type), Diff (begin, start), Diff (begin, stop));
#endif

            switch (type) {
                case NIL:
                    goto InvalidType;
                case ADR:    //< _R_e_a_d__S_t_r_i_n_g_-_8____________________________
                case STR:
                    // Load buffered-type argument length and increment the index.
                    count = *param;
                    ++param;

                    //std::cout << "\n|           Reading char with max length " << count;

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
                            return MirrorResult (mirror, kErrorBufferUnderflow,
                                                 params, index, start);

                          //putchar (ui1);

                        ui1 = *start;       // Read byte from ring-buffer.
                        if (++start >= end) start -= size;
                        *ui1_ptr = ui1;     // Write byte to destination.
                        ++ui1_ptr;
                    }

                    //cout << " done!\n";

                    if (type != ADR) {
                        *ui1_ptr = ui1;
                    }
                    break;
                case SI1:     //< _R_e_a_d__1__B_y_t_e__T_y_p_e_s_____________________
                case UI1:
                case BOL:
#if SCRIPT_USING_1_BYTE_TYPES
                    if (length == 0)
                        return MirrorResult (mirror, kErrorBufferUnderflow, params,
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
#if SCRIPT_USING_2_BYTE_TYPES
                    Read2ByteType:{
                        // Word-align
                        offset = TypeAlign2 (start);
                        if (length < offset + 2)
                            return MirrorResult (mirror, kErrorBufferUnderflow,
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
                    }
                                  break;
#else
                    goto InvalidType;
#endif
                case SI4:     //< _R_e_a_d__3_2_-_b_i_t__T_y_p_e_s____________________
                case UI4:
                case FLT:
                case TMS:
#if SCRIPT_USING_4_BYTE_TYPES
                    Read4ByteType:{
                        // Word-align
                        offset = TypeAlign4 (start);
                        if (length < offset + 4)
                            return MirrorResult (mirror, kErrorBufferUnderflow,
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
#if SCRIPT_USING_8_BYTE_TYPES
                    Read8ByteType:{
                        // Word-align
                        offset = TypeAlign8 (start);
                        if (length < offset + 8)
                            return MirrorResult (mirror, kErrorBufferUnderflow, params, index, start);
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
#if SCRIPT_USING_VARINT2
                    goto Read2ByteType;
#else
                    goto InvalidType;
#endif
                case SV4:     //< _R_e_a_d__V_a_r_i_n_t__4____________________________
                case UV4:
#if SCRIPT_USING_VARINT4
                    goto Read4ByteType;
#else
                    goto InvalidType;
#endif
                case SV8:     //< _R_e_a_d__V_a_r_i_n_t__8____________________________
                case UV8:
#if SCRIPT_USING_VARINT8
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
                        return MirrorResult (mirror, kErrorInvalidType, params, index, start);
                    }
                    // We don't care if it's a multidimensional array anymore.
                    ui1_ptr = reinterpret_cast<uint8_t*> (args[index]);
                    if (ui1_ptr == nullptr)
                        return MirrorResult (mirror, kErrorImplementation, params, index, start);
                    count &= 0x3;
                    switch (count) {
                        case 0:
                        { // It's a 8-bit count.
                            if (type >= LST) {
                                // LST, BAG, BOK, and MAP cannot take a 8-bit count!
                                return MirrorResult (mirror, kErrorInvalidType,
                                                     params, index, start);
                            }
                            count = (uintptr_t)*ui1_ptr;
                            break;
                        }
                        case 1:
                        { // It's a 16-bit count.
                            if (length < 3)
                                return MirrorResult (mirror, kErrorBufferUnderflow,
                                                     params, index, start);
                            count -= 2;
                            ui2_ptr = reinterpret_cast<uint16_t*> (ui1_ptr);
                            count = (uintptr_t)*ui2_ptr;
                            if (count > length)
                                return MirrorResult (mirror, kErrorBufferOverflow,
                                                     params, index, start);
                            break;
                        }
                        case 2:
                        { // It's a 32-bit count.
                            if (length < 5)
                                return MirrorResult (mirror, kErrorBufferUnderflow,
                                                     params, index, start);
                            count -= 4;
                            ui4_ptr = reinterpret_cast<uint32_t*> (ui1_ptr);
                            count = (uintptr_t)*ui4_ptr;
                            if (count > length)
                                return MirrorResult (mirror, kErrorBufferOverflow,
                                                     params, index, start);
                            break;
                        }
                        case 3:
                        { // It's a 64-bit count.
                            if (length < 9)
                                return MirrorResult (mirror, kErrorBufferUnderflow,
                                                     params, index, start);
                            count -= 8;
                            ui8_ptr = reinterpret_cast<uint64_t*> (ui1_ptr);
                            count = (uintptr_t)*ui8_ptr;
                            if (count > length)
                                return MirrorResult (mirror, kErrorBufferOverflow,
                                                     params, index, start);
                            break;
                        }
                        default:
                        {
                            return MirrorResult (mirror, kErrorImplementation, params, index, start);
                        }
                    }
                    if (length < count)
                        return MirrorResult (mirror, kErrorBufferOverflow, params, index, start);
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
                        std::cout << "\n| !!!Read invalid type " << type;
                        return MirrorResult (mirror, kErrorInvalidType, params,
                                             index, start);
                    }
                }
                //std::cout << " |";
            }
        }
        //cout << "\n| Done reading";
        //MirrorWipe (mirror);

        // Convert pointer back to offset
        bin->start = Diff (begin, start);

        return 0;
    }
}
const Operation* MirrorResult (Mirror* mirror, Error error) {
    return BinResult (reinterpret_cast<Bin*> (mirror), error);
}

const Operation* MirrorResult (Mirror* mirror, Error error) {
    return Error (reinterpret_cast<Bout*> (mirror), error);
}


const Operation* Mirrorwrite (Mirror* mirror, const uint_t* params, void** args) {
    if (mirror == nullptr)     //< Is this a double check?
        return MirrorResult (mirror, kErrorImplementation);
    if (!params)
        return MirrorResult (mirror, kErrorImplementation);
    if (!args)
        return MirrorResult (mirror, kErrorImplementation);
    return MirrorResult (mirror, kErrorImplementation);
}
//bool IsWritable (MirrorOut* mirror) {
//   return mirror->start != mirror->stop;
//}

void* MirrorContains (Mirror* mirror, void* address) {
    if (mirror == nullptr) {
        return nullptr;
    }
    byte* begin = reinterpret_cast<byte*> (mirror) + sizeof (Mirror);
    if (address < begin) {
        return nullptr;
    }
    byte* end = begin + mirror->size;
    if (address > end) {
        return nullptr;
    }
    return address;
}

void MirrorWipe (Mirror* mirror) {
    if (mirror == nullptr)
        return;
    byte* begin = reinterpret_cast<byte*> (mirror) + sizeof (Mirror),
        *start = begin + mirror->start,
        *stop = begin + mirror->stop,
        *temp;
    if (start > stop) {
        temp = start;
        start = stop;
        stop = temp;
    }
    while (start != stop) {
        *start++ = 0;
    }
}
/*
byte* MirrorWrite (void* target, void* target_end, byte* const begin, byte* const start,
byte* const stop, byte* const end, size_t size) {
if (target == nullptr) return start;

if ((start > stop) && (start + size >= end)) {
// Calculate upper chunk size.
size_t top_chunk = end - stop;
size -= top_chunk;

MemoryCopy (stop, top_chunk, target);
MemoryCopy (begin, size, reinterpret_cast<byte*>(target) + top_chunk);
return begin + size;
}
MemoryCopy (stop, size, target);
return start + size;
}

byte* MirrorRead (void* target, void* target_end, byte* const begin, byte* const start,
byte* const stop , byte* const end  , size_t size) {
if (target == nullptr) return start;

if ((start > stop) && (start + size >= end)) {
// Calculate upper chunk size.
size_t top_chunk = end - stop;
size -= top_chunk;

MemoryCopy (target, target_end, start, top_chunk);
MemoryCopy (reinterpret_cast<byte*>(target) + top_chunk, size,
begin);
return begin + size;
}
MemoryCopy (target, target_end, stop, size);
return start + size;
}*/

//bool IsWritable (MirrorOut* mirror) {
//   return mirror->start != mirror->stop;
//}

const Operation* MirrorResult (Mirror* bin, Error error,
                             const uint_t* header) {
    return 0;
}

const Operation* MirrorResult (Mirror* bin, Error error,
                             const uint_t* header) {
    return 0;
}

const Operation* MirrorResult (Mirror* bin, Error error,
                             const uint_t* header,
                             byte offset) {
    return 0;
}

const Operation* MirrorResult (Mirror* bin, Error error,
                             const uint_t* header,
                             byte offset) {
    return 0;
}

const Operation* MirrorResult (Mirror* bin, Error error,
                             const uint_t* header,
                             uint_t offset,
                             byte* address) {
    return 0;
}

const Operation* MirrorResult (Mirror* bin, Error error,
                             const uint_t* header,
                             uint_t offset,
                             byte* address) {
    return 0;
}

}       //< namespace _
#endif  //< SCRIPT_USING_MIRROR

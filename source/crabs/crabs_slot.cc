/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/crabs/crabs_slot.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
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
#include "slot.h"

#if MAJOR_SEAM == 1 && MINOR_SEAM >= 4

#include "type.h"
#include "text.h"


#if MAJOR_SEAM == 1 && MINOR_SEAM == 1
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PUTCHAR(c) putchar(c);
#define PRINT_BSQ(bsq)\ {\
    enum { kSize = 64*1024 }\
    char bsq_buffer[kSize];\
    \
}
#else
#define PRINTF(x, ...)
#define PUTCHAR(c)
#endif

namespace _ {

void SlotInit (Slot* slot, uintptr_t* begin, uintptr_t size) {
    if (!slot) {
        slot->begin = 0;
        slot->start = 0;
        slot->stop  = 0;
        slot->end   = 0;
        return;
    }
    if (!begin) {
        slot->begin = 0;
        slot->start = 0;
        slot->stop  = 0;
        slot->end   = 0;
        return;
    }
    char* begin_char = reinterpret_cast<char*> (Align8 (begin));
    slot->begin = begin_char;
    slot->start = begin_char;
    slot->stop  = begin_char;
    slot->end   = begin_char + size;
}

inline const Op* SlotError (Slot* slot, Error error,
                            const uint_t* header) {
    PRINTF ("\n%s", ErrorStrings ()[error])
    return reinterpret_cast<const Op*> (error);
}

inline const Op* SlotError (Slot* slot, Error error,
                            const uint_t* header,
                            byte offset) {
    PRINTF ("\n%s", ErrorStrings ()[error])
    return reinterpret_cast<const Op*> (error);
}

inline const Op* SlotError (Slot* slot, Error error,
                            const uint_t* header,
                            uint_t offset,
                            char* address) {
    PRINTF ("\n%s", ErrorStrings ()[error])
    return reinterpret_cast<const Op*> (error);
}

inline const Op* SlotError (Slot* slot, Error error) {
    PRINTF ("\n%s", ErrorStrings ()[error])
    return reinterpret_cast<const Op*> (error);
}

void* SlotContains (Slot* slot, void* address) {
    if (!slot) {
        return nullptr;
    }
    char* begin = reinterpret_cast<char*> (slot) + sizeof (Slot);
    if (address < begin) {
        return nullptr;
    }
    char* end = slot->end;
    if (address > end) {
        return nullptr;
    }
    return address;
}

void SlotWipe (Slot* slot) {
    if (!slot) {
        return;
    }
    char* begin = reinterpret_cast<char*> (slot) + sizeof (Slot),
        * start = slot->start,
        * stop  = slot->stop,
        * temp;
    if (start > stop) {
        temp = start;
        start = stop;
        stop = temp;
    }
    while (start != stop) {
        *start++ = 0;
    }
}

const Op* SlotWrite (Slot* slot, const uint_t* params, void** args) {
    if (!slot)     //< Is this a double check?
        return SlotError (slot, kErrorImplementation);
    if (!params)
        return SlotError (slot, kErrorImplementation);
    if (!args)
        return SlotError (slot, kErrorImplementation);
    return SlotError (slot, kErrorImplementation);
}

bool SlotIsWritable (Slot* slot) {
    char* stop = slot->start;
    if (stop == slot->begin) {
        if (stop != slot->end) {
            return false;
        }
        return true;
    }
    return slot->start != slot->stop - 1;
}

bool SlotIsReadable (Slot* slot) {
    return (slot->start != slot->stop);
}

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

const Op* SlotRead (Slot* slot, const Op& op, void** args) {
    if (!slot) {
        return SlotError (slot, kErrorImplementation);
    }
    if (!args) {
        return SlotError (slot, kErrorImplementation);
    }
    const     uint_t* params = op.in;
    byte      ui1;     //< Temp variable to load most types.
    uint16_t  ui2;     //< Temp variable for working with UI2 types.
    #if USING_CRABS_4_BYTE_TYPES
    uint32_t  ui4;
    #endif
    #if USING_CRABS_8_BYTE_TYPES
    uint64_t  ui8;     //< Temp UI8 variable.
    #endif
    char*     ui1_ptr; //< Pointer to a UI1.
    uint16_t* ui2_ptr; //< Pointer to a UI2.
    uint32_t* ui4_ptr; //< Pointer to a UI4.
    uint64_t* ui8_ptr; //< Pointer to a UI8.
    uint_t    type,    //< Current type being read.
              index,   //< Index in the escape sequence.
              num_params = *params; //< Number of params.
    uintptr_t  offset; //< Offset to word align the current type.
    intptr_t length,   //< Length of the data in the buffer.
              count,   //< Argument length.
              size;    //< Size of the ring buffer.

    if (!num_params) {
        return nullptr;
    }

    PRINTF ("\n\nReading BIn: ")

    char* begin = slot->begin,        //< Beginning of the buffer.
        * end   = slot->end,          //< end of the buffer.
        * start = slot->start,        //< start of the data.
        * stop  = slot->stop;         //< stop of the data.
    const uint_t* param = params + 1; //< current param.

    size = end - begin;

    length = SlotLength (start, stop, size);

    PRINTF ("\n\nReading %u bytes.", length)
    //PrintBsq (params)
    // When we scan, we are reading from the beginning of the BIn buffer.

    for (index = 0; index < num_params; ++index) {
        type = (byte)*param;
        ++param;
        PRINTF ("\nindex:%u:\"%s\", start:0x%I64i, stop:0x%I64i", index, 
                TypeString (type), MemoryVector (begin, start),
                MemoryVector (begin, stop))

        switch (type) {
            case NIL:
                return SlotError (slot, kErrorInvalidType);
            case ADR:    //< _R_e_a_d__S_t_r_i_n_g_-_1______________________
            case STR:
                // Load buffered-type argument length and increment the 
                // index.
                count = *param;
                ++param;

                //std::cout << "\nReading char with max length " << count;

                // Load next pointer and increment args.
                ui1_ptr = reinterpret_cast<char*> (args[index]);
                if (!ui1_ptr) {
                    break;
                }

                // Read from slot and write to the slot:
                ui1 = *start;
                if (++start > end) start -= size;
                *ui1_ptr = ui1;
                ++ui1_ptr;

                while (ui1 && count) {
                    if (count-- == 0) {
                        return SlotError (slot, kErrorBufferUnderflow,
                                          params, index, start);
                    }
                    PUTCHAR (ui1)

                    ui1 = *start;       // Read byte from ring-buffer.
                    if (++start > end) {
                        start -= size;
                    }
                    *ui1_ptr = ui1;     // Write byte to destination.
                    ++ui1_ptr;
                }
                    
                PRINTF (" done!\n")

                if (type == 0) {
                    *ui1_ptr = ui1;
                }
                break;
            case SI1:    //< _R_e_a_d__1__B_y_t_e__T_y_p_e_s________________
            case UI1:
            case BOL:
                #if USING_CRABS_1_BYTE_TYPES
                if (length == 0) {
                    return SlotError (slot, kErrorBufferUnderflow, params, 
                                      index, start);
                }
                --length;

                // Read from buffer and write to the stack:
                ui1 = *start;
                if (++start > end) {
                    start -= size;
                }
                // Load next pointer and increment args.
                ui1_ptr = reinterpret_cast<char*> (args[index]);
                if (!ui1_ptr) {
                    break;
                }
                *ui1_ptr = ui1;                     //< Write
                if (++start > end) {
                    start -= size;
                }
                break;
                #else
                return SlotError (slot, kErrorInvalidType);
                #endif
            case SI2:     //< _R_e_a_d__1_6_-_b_i_t__T_y_p_e_s______________
            case UI2:
            case HLF:
            #if WORD_SIZE <= 16
            case SVI:
            case UVI:
            #endif
                #if USING_CRABS_2_BYTE_TYPES
                //Read2ByteType:{
                    // Word-align
                    offset = Align2 (start);
                    if ((uintptr_t)length < offset + 2) {
                        return SlotError (slot, kErrorBufferUnderflow,
                                                params, index, start);
                    }
                    length -= (uint_t)offset + 2;
                    start += offset;
                    if (start > end) {
                        start -= size;
                    }
                    // Read from buffer and write to the stack:
                    ui2_ptr = reinterpret_cast<uint16_t*>(start);
                    ui2 = *ui2_ptr;
                    start += sizeof (uint16_t);
                    if (start > end) {
                        start -= size;
                    }
                    // Load next pointer and increment args.
                    ui2_ptr = reinterpret_cast<uint16_t*> (args[index]);
                    if (!ui2_ptr) {
                        break;
                    }
                    *ui2_ptr = ui2;
                //}
                break;
                #else
                return SlotError (slot, kErrorInvalidType);
                #endif
                #if USING_CRABS_VARINT2
                goto Read2ByteType;
                #else
                return SlotError (slot, kErrorInvalidType);
                #endif
            #if WORD_SIZE > 16
            case SVI:
            case UVI:
            #endif
            case SI4:     //< _R_e_a_d__3_2_-_b_i_t__T_y_p_e_s______________
            case UI4:
            case FLT:
            case TMS:
                #if USING_CRABS_4_BYTE_TYPES
                //Read4ByteType:{
                    // Word-align
                    offset = Align4 (start);
                    if ((uintptr_t)length < offset + 4) {
                        return SlotError (slot, kErrorBufferUnderflow,
                                          params, index, start);
                    }
                    length -= (uint_t)offset + 4;
                    start += offset;
                    if (start > end) {
                        start -= size;    //< Bound
                    }
                    // Read from buffer and write to the stack:
                    ui4_ptr = reinterpret_cast<uint32_t*>(start);
                    ui4 = *ui4_ptr;                     //< Read
                    start += sizeof (int32_t);          //< Increment
                    if (start > end) {
                        start -= size;    //< Bound
                    }
                    // Load next pointer and increment args.
                    ui4_ptr = reinterpret_cast<uint32_t*> (args[index]);
                    if (!ui4_ptr) {
                        break;
                    }
                    *ui4_ptr = ui1;                     //< Write
                    break;
                //}
                #else
                return SlotError (slot, kErrorInvalidType);
                #endif
            case SI8:     //< _R_e_a_d__6_4_-_b_i_t__T_y_p_e_s______________
            case UI8:
            case DBL:
            case TMU:
            case SV8:
            case UV8:
                #if USING_CRABS_8_BYTE_TYPES
                //Read8ByteType:{
                    // Word-align
                    offset = Align8 (start);
                    if ((uintptr_t)length < offset + sizeof (int64_t)) {
                        return SlotError (slot, kErrorBufferUnderflow, 
                                            params, index, start);
                    }
                    length -= offset + sizeof (int64_t);
                    start  += offset;
                    if (start > end) {
                        start -= size; //< Bound
                    }
                    // Read from buffer and write to the stack:
                    ui8_ptr = reinterpret_cast<uint64_t*>(start);
                    ui8 = *ui8_ptr;             //< Read
                    start += sizeof (uint64_t); //< Increment
                    if (start > end) {
                        start -= size;          //< Bound
                    }
                    // Load next pointer and increment args.
                    ui8_ptr = reinterpret_cast<uint64_t*> (args[index]);
                    if (ui8_ptr == 0) break;
                    *ui8_ptr = ui8;                     //< Write
                    break;
                //}
                #else
                return SlotError (slot, kErrorInvalidType);
                #endif
            default: {
                #if USING_CRABS_OBJ
                count = type >> 5;  //< count is now the array type bits.
                type &= 0x1f;       //< Now type is the type 0-31
                if (count && (type >= OBJ)) {
                    // Can't make arrays out of objects!
                    return SlotError (slot, kErrorInvalidType, params, index, start);
                }
                // We don't care if it's a multidimensional array anymore.
                ui1_ptr = reinterpret_cast<char*> (args[index]);
                if (ui1_ptr == nullptr)
                    return SlotError (slot, kErrorImplementation, params, index, start);
                count &= 0x3;
                switch (count) {
                    case 0: { // It's a 8-bit count.
                        if (type >= LST) {
                            // LST, BOK, DIC, and MAP can't be 8-bit!
                            return SlotError (slot, kErrorInvalidType,
                                                params, index, start);
                        }
                        count = (uintptr_t)*ui1_ptr;
                        break;
                    }
                    case 1: { // It's a 16-bit count.
                        if (length < 3) {
                            return SlotError (slot, kErrorBufferUnderflow,
                                              params, index, start);
                        }
                        count -= 2;
                        ui2_ptr = reinterpret_cast<uint16_t*> (ui1_ptr);
                        count = (uintptr_t)*ui2_ptr;
                        if (count > length) {
                            return SlotError (slot, kErrorBufferOverflow,
                                              params, index, start);
                        }
                        break;
                    }
                    case 2: { // It's a 32-bit count.
                        if (length < 5) {
                            return SlotError (slot, kErrorBufferUnderflow,
                                              params, index, start);
                        }
                        count -= 4;
                        ui4_ptr = reinterpret_cast<uint32_t*> (ui1_ptr);
                        count = (uintptr_t)*ui4_ptr;
                        if (count > length) {
                            return SlotError (slot, kErrorBufferOverflow,
                                              params, index, start);
                        }
                        break;
                    }
                    case 3: { // It's a 64-bit count.
                        if (length < 9) {
                            return SlotError (slot, kErrorBufferUnderflow,
                                              params, index, start);
                        }
                        count -= 8;
                        ui8_ptr = reinterpret_cast<uint64_t*> (ui1_ptr);
                        count = (uintptr_t)*ui8_ptr;
                        if (count > length) {
                            return SlotError (slot, kErrorBufferOverflow,
                                              params, index, start);
                        }
                        break;
                    }
                    default: {
                        return SlotError (slot, kErrorImplementation, 
                                          params, index, start);
                    }
                }
                if (length < count) {
                    return SlotError (slot, kErrorBufferOverflow, params, 
                                      index, start);
                }
                if (!count) {
                    return SlotError (slot, kErrorBufferOverflow, params, 
                                      index, start);
                }
                if (start + count >= end) {
                    for (; size - count > 0; --count) {
                        ui1 = *start;
                        if (++start > end) {
                            start -= size;
                        }
                        *ui1_ptr = ui1;
                        ++ui1_ptr;
                    }
                    stop = begin - 1;
                    for (; count > 0; --count) {
                        ui1 = *start;
                        if (++start > end) {
                            start -= size;
                        }
                        *ui1_ptr = ui1;
                        ++ui1_ptr;
                    }
                    break;
                }
                for (; count > 0; --count) {
                    ui1 = *start;
                    if (++start > end) {
                        start -= size;
                    }
                    *ui1_ptr = ui1;
                    ++ui1_ptr;
                }
                break;
                #endif
            }
            PRINTF (" |")
        }
    }
    PRINTF ("\nDone reading.")
    //SlotWipe (slot);

    // Convert pointer back to offset
    slot->start = start;

    return 0;
}

const Op* SlotWrite (Slot* slot, const Op& op, void** args) {
    return nullptr;
}

Slot& SlotWrite (Slot& slot, Slot& other) {
    return slot;
}

void SlotDisplay (Slot& slot) {
    char* begin,
        * start = slot.start,
        * stop  = slot.stop,
        * end;
    if (stop < start) {
        begin = slot.begin;
        end   = slot.end;
        while (start <= end) {
            std::cerr << *start++;
        }
        while (begin <= stop) {
            std::cerr << *begin++;
        }
    }
    while (start <= stop) {
        std::cerr << *start++;
    }
}

Slot& Print (const char* text, Slot& slot, char delimiter) {
    if (!text) {
        return slot;
    }
    char* cursor = slot.stop + 1,
        *end = slot.end;
    char c = *text;
    ++text;
    while (c > delimiter) {
        if (cursor > end) {
            cursor = slot.begin;
            end = slot.start;
            if (cursor == end) {
                return slot;
            }
        }
        *cursor++ = c;
        c = *text++;
    }
    *cursor = 0;
    return slot;
}

Slot& Print (const char* text, const char* text_end, Slot& slot, char delimiter) {
    if (!text) {
        return slot;
    }
    if (text > text_end) {
        return slot;
    }
    char* cursor = slot.stop + 1,
        * end = slot.end;
    char c = *text;
    ++text;
    while (c > delimiter) {
        if (cursor > end) {
            cursor = slot.begin;
            end    = slot.start;
            if (cursor == end) {
                return slot;
            }
        }
        *cursor++ = c;
        c = *text++;
    }
    *cursor = 0;
    return slot;
}

Slot& Print (int64_t value, Slot& slot, char delimiter) {
    enum {
        kSizeMax = 22,
    };

    char * begin,
         * start = slot.start,
         * cursor = slot.stop,
         * end;
    size_t size;
    //int    num_chars;

    // First we need to check if the ring buffer is wrapped around.
    if (start > cursor) {
        // 1a. Check if there is enough room in the upper chunk first.
        end = slot.end;
        size = end - cursor;
        if (size < kSizeMax) {
            // Add the lower chunk size to the size
            begin = slot.begin;
            end = slot.end;
            size += (start - begin);
            if (size < kSizeMax) {
                return slot;
            }
            cursor = Print (value, cursor, end + kSizeMax, delimiter);
            // Check if it was an Overflow Wraparound and shift the bytes into
            // the lower chunk if it was.
            cursor = SlotOverflowShift (begin, end, cursor);
        } else {
            end = slot.end;
            cursor = Print (value, cursor, end + kSizeMax, delimiter);
        }
    }
    else {
        // 2b. Check if there enough room in the buffer.
        size = start - cursor;
        if (size < kSizeMax) {
            return slot;
        }
        // 3b. There is no buffer overflow for this branch
        //     so just write the string like any old string.
        end = slot.end;
        cursor = Print (value, cursor, end + kSizeMax, delimiter);
        if (!cursor) {
            return slot;
        }
    }
    // 4. Update the ring buffer stop.
    slot.stop = cursor;
    return slot;
}

Slot& Print (uint64_t value, Slot& slot, char delimiter) {
    enum {
        kSizeMax = 21,
    };

    char * begin,
         * start = slot.start,
         * cursor = slot.stop,
         * end;
    size_t size;
    //int    num_chars;

    // First we need to check if the ring buffer is wrapped around.
    if (start > cursor) {
        // 1a. Check if there is enough room in the upper chunk first.
        end = slot.end;
        size = end - cursor;
        if (size < kSizeMax) {
            // Add the lower chunk size to the size
            begin = slot.begin;
            end = slot.end;
            size += (start - begin);
            if (size < kSizeMax) {
                // @todo Do I need to clear the buffer here in case of attack?
                return slot;
            }
            cursor = Print (value, cursor, end + kSizeMax, delimiter);
            // Check if it was an Overflow Wraparound and shift the bytes into
            // the lower chunk if it was.
            cursor = SlotOverflowShift (begin, end, cursor);
        }
        else {
            cursor = Print (value, cursor, end + kSizeMax, delimiter);
        }
    }
    else {
        // 2b. Check if there enough room in the buffer.
        size = start - cursor;
        if (size < kSizeMax) {
            return slot;
        }
        // 3b. There is no buffer overflow for this branch
        //     so just write the string like any old string.
        end = slot.end;
        cursor = Print (value, cursor, end + kSizeMax, delimiter);
        if (!cursor) {
            return slot;
        }
    }
    // 4. Update the ring buffer stop.
    slot.stop = cursor;
    return slot;
}
/*
inline size_t SlotSpaceFast (Slot& slot, char* start, char*stop) {
    if (start > stop) {
        return size + (start - slot.begin);
    }
    return stop - start;
}

inline char* SlotLastAddress (Slot& slot, char* start, char*stop, char* end, int size) {
    size_t space;
    if (stop < start) {
        if (stop - start < size) {
            return nullptr;
        }
    }
    space = end - stop;
    if (space < size) {
        if (space + (start - slot.begin) < size) {
            return nullptr;
        }
        return nullptr;
    }
    return stop + size;
}*/

Slot& Print (float value, Slot& slot, char delimiter) {
    char * begin,
         * start = slot.start,
         * cursor = slot.stop,
         * end;
    size_t size;
    //int    num_chars;



    // First we need to check if the ring buffer is wrapped around.
    if (start > cursor) {
        // 1a. Check if there is enough room in the upper chunk first.
        end = slot.end;
        size = end - cursor;
        if (size < kkFloat32DigitsMax) {
            // Add the lower chunk size to the size
            begin = slot.begin;
            end = slot.end;
            size += (start - begin);
            if (size < kkFloat32DigitsMax) {
                // @todo Do I need to clear the buffer here in case of attack?
                return slot;
            }
            cursor = Print (value, cursor, end + kkFloat32DigitsMax, delimiter);
            // Check if it was an Overflow Wraparound and shift the bytes into
            // the lower chunk if it was.
            cursor = SlotOverflowShift (begin, end, cursor);
        }
        else {
            end = slot.end;
            cursor = Print (value, cursor, end + kkFloat32DigitsMax, delimiter);
        }
    }
    else {
        // 2b. Check if there enough room in the buffer.
        size = start - cursor;
        if (size < kkFloat32DigitsMax) {
            return slot;
        }
        // 3b. There is no buffer overflow for this branch
        //     so just write the string like any old string
        end = slot.end;
        cursor = Print (value, cursor, end + kkFloat32DigitsMax, delimiter);
        if (!cursor) {
            return slot;
        }
    }
    // 4. Update the ring buffer stop.
    slot.stop = cursor;
    return slot;
}

Slot& Print (double value, Slot& slot, char delimiter) {
    char * begin,
         * start  = slot.start,
         * cursor = slot.stop,
         * end;
    size_t size;
    //int    num_chars;

    // First we need to check if the ring buffer is wrapped around.
    if (start > cursor) {
        // Check if there is enough room in the upper chunk first.
        end = slot.end;
        size = end - cursor;
        if (size < kFloat64DigitsMax) {
            // Add the lower chunk size to the size
            begin = slot.begin;
            end = slot.end;
            size += (start - begin);
            if (size < kFloat64DigitsMax) {
                // @todo Do I need to clear the buffer here in case of attack?
                return slot;
            }
            cursor = Print (value, cursor, end + kFloat64DigitsMax, delimiter);
            // Check if it was an Overflow Wraparound and shift the bytes into
            // the lower chunk if it was.
            cursor = SlotOverflowShift (begin, end, cursor);
        }
        else {
            end = slot.end;
            cursor = Print (value, cursor, end + kFloat64DigitsMax, delimiter);
        }
    }
    else {
        // 2b. Check if there enough room in the buffer.
        size = start - cursor;
        if (size < kFloat64DigitsMax) {
            return slot;
        }
        // 3b. There is no buffer overflow for this branch
        //     so just write the string like any old string.
        end = slot.end;
        cursor = Print (value, cursor, end + kFloat64DigitsMax);
    }
    // 4. Update the ring buffer stop.
    slot.stop = cursor;
    return slot;
}

Slot& PrintRight (const char* token, int num_columns, Slot& slot,
                  char delimiter) {
    char* start  = slot.start,
        * cursor = slot.stop,
        * end    = slot.end;
    uintptr_t size;
    //char      c;
    if (cursor < start) {
        size = start - cursor;
        slot.stop = PrintRight (token, num_columns, cursor, start - 1);
        return slot;
    }
    size = start - cursor;
    if (size < num_columns) {
        size += cursor - slot.begin;
        if (size < num_columns) {
            return slot;
        }
    }
    for (; num_columns > 0; --num_columns) {
        *cursor++ = *token++;
        if (cursor > end) {
            cursor = slot.begin;
        }
    }
    slot.stop = cursor;
    return slot;
}

Slot& PrintLine (char c, int num_columns, Slot& slot,
                 char delimiter) {
    char* start = slot.start,
        *cursor = slot.stop,
        *end = slot.end;
    intptr_t size;
    if (num_columns < 1) {
        return slot;
    }
    if (start > cursor) {
        size = start - cursor;
        if (size < num_columns) {
            size += cursor - slot.begin;
            if (size < num_columns) {
                return slot;
            }
        }
    }
    else if (cursor - start < num_columns) {
        return slot;
    }

    while (num_columns > 0) {
        if (cursor > end) {
            cursor = slot.begin;
        }
        *cursor++ = c;
    }
    *cursor = delimiter;
    slot.stop = cursor;
    return slot;
}

Slot& PrintLine (const char* string, int num_columns, Slot& slot,
                 char delimiter) {
    char* start  = slot.start,
        * cursor = slot.stop,
        * end    = slot.end;
    intptr_t size;
    const char* read = string;
    if (num_columns < 1) {
        return slot;
    }
    if (!string) {
        return slot;
    }
    if (start > cursor) {
        size = start - cursor;
        if (size < num_columns) {
            size += cursor - slot.begin;
            if (size < num_columns) {
                return slot;
            }
        }
    }
    else if (cursor - start < num_columns) {
        return slot;
    }

    while (num_columns > 0) {
        char c = *read++;
        if (!c) {
            read = string;
        }
        if (cursor > end) {
            cursor = slot.begin;
        }
        *cursor++ = c;
    }
    *cursor = delimiter;
    slot.stop = cursor;
    return slot;
}

Slot& PrintLines (int num_rows, Slot& slot, char delimiter) {
    char* cursor = slot.stop + 1,
        *end = slot.end,
        *stop = cursor + num_rows + 1;
    uintptr_t upper_size = end - cursor + 1;
    if (upper_size < num_rows) {
        while (cursor < stop) {

        }
    }
    while (num_rows > 0) {
        *cursor++ = '\n';
    }
    *cursor = delimiter;
    slot.stop = cursor + 1;
    return slot;
}

Slot& PrintHex (char c, Slot& slot, char delimiter) {
    uint16_t chars = TextByteToUpperCaseHex (c);
    return slot << (char)chars << ((char)(chars >> 8)) << ' ';
}

Slot& PrintHex (uintptr_t value, Slot& slot) {
    enum { kHexStringLengthSizeMax = sizeof (void*) * 2 + 3 };

    // @todo Replace this with faster algorithm.
    char    * start = slot.start,
        * cursor = slot.stop + 1,
        * end    = slot.end;
    //* value_ptr;              //< Pointer to a byte in value.
    char      c;                        //< Temp variable.
    uintptr_t size;                     // Current size we care about.

    if (cursor < start) {
        size = start - cursor;
        if (size < kHexStringLengthSizeMax) {
            return slot;
        }
    }
    else {
        size = end - cursor;
        if (size < kHexStringLengthSizeMax) {
            size += (slot.start - slot.begin);
            if (size < kHexStringLengthSizeMax) {
                return slot;
            }
        }
    }

    for (int num_bits_shift = 0; num_bits_shift < sizeof (void*) * 8; 
         num_bits_shift += 8) {
        c = (char)(value >> num_bits_shift);
        c = TextNibbleToUpperCaseHex (c);
        if (++cursor > end) {
            cursor = slot.begin;
        }
    }
    slot.stop = cursor;
    return slot;
}

Slot& PrintMemory (const void* address, const void* address_end, Slot& slot,
                   char delimiter) {
    slot << "\n " << 0;
    //  columns
    char* start = slot.start,
        *cursor = slot.stop,
        *end = slot.end;
    const char* address_ptr     = reinterpret_cast<const char*> (address),
        * address_end_ptr = reinterpret_cast<const char*> (address_end);

    for (int i = 8; i <= 66; i += 8) {
        PrintRight (i, 8, slot);
    }
    slot << "\n|";
    for (int i = 0; i < 65; ++i) {
        *cursor++ = '_';
        if (cursor > end) {
            cursor = slot.begin;
        }
    }

    const char* chars = reinterpret_cast<const char*> (address);
    char temp;
    while (chars < cursor) {
        *cursor++ = '\n';
        if (cursor > end) {
            cursor = slot.begin;
        }
        *cursor++ = '|';
        if (cursor > end) {
            cursor = slot.begin;
        }
        for (int i = 0; i < 64; ++i) {
            temp = *chars++;
            if (chars >= cursor)
                temp = 'x';
            *cursor++ = temp;
            if (cursor > end) {
                cursor = slot.begin;
            }
        }
        *cursor++ = '|';
        if (cursor > end) {
            cursor = slot.begin;
        }
        *cursor++ = ' ';
        if (cursor > end) {
            cursor = slot.begin;
        }
        PrintHex (chars + (address_end_ptr - address_ptr), slot);
    }
    *cursor++ = '\n';
    if (cursor > end) {
        cursor = slot.begin;
    }
    *cursor++ = '|';
    if (cursor > end) {
        cursor = slot.begin;
    }
    for (int i = 0; i < 64; ++i) {
        slot << '_';
    }
    *cursor++ = '|';
    if (cursor > end) {
        cursor = slot.begin;
    }
    *cursor++ = ' ';
    if (cursor > end) {
        cursor = slot.begin;
    }
    return PrintHex (chars + (address_end_ptr - address_ptr), slot);
}

Slot& PrintError (const char* message, const char* end_string, Slot& slot) {
    return slot << "\nError: " << message << end_string;
}

Slot& PrintBsq (const uint_t* params, Slot& slot) {
    if (!params) {
        return slot << "nil";
    }
    if (params < (const uint_t*)256) {
        return slot << reinterpret_cast<uintptr_t> (params) << '\n';
    }
    uint_t num_params = *params++,
        i,
        type,
        value = 0;

    slot << "Param<";
    if (num_params > _::kParamsMax) {
        return slot << "\nInvalid num_params: " << num_params;
    }
    slot << num_params << ": ";
    for (i = 1; i < num_params; ++i) {
        value = *params++;
        type = value & 0x1f;    //< Mask off type.
        value = value >> 5;     //< Shift over array type.
        slot << TypeString (value) << ", ";
        if (type >= STR) {
            if (value) {
                return slot << "\nError: arrays may only be created from POD types.";
            }
            // Print out the max length of the string.
            ++i;
            value = *params++;
            slot << value;
        }
        else if (value > 31) {
            if (value > 127) {      //< It's a multi-dimensional array.
                slot << "Multi-dimensional Array:" << value << ", ";
            }
            // Then it's an array.
            ++i;
            switch (value) {        //< Print out the Array type.
                case 0: {
                    break;
                }
                case 1: {
                    value = *params++;
                    slot << "UI1:" << value << ", ";
                    break;
                }
                case 2: {
                    value = *params++;
                    slot << "UI2:" << value << ", ";
                    break;
                }
                case 3: {
                    value = *params++;
                    slot << "UI4:" << value << ", ";
                    break;
                }
                case 4: {
                    value = *params++;
                    slot << "UI8:" << value << ", ";
                    break;
                }
                case 5: {
                    value = *params++;
                    if (value == 0) {
                        slot << "UI1:[0]";
                        break;
                    }
                    slot << "UI1:[" << value << ": ";
                    for (uint_t i = value; i != 0; --i) {
                        value = *params++;
                        slot << value << ", ";
                    }
                    value = *params++;
                    slot << value << "]";
                    break;
                }
                case 6: {
                    value = *params++;
                    if (value == 0) {
                        slot << "UI2:[0]";
                        break;
                    }
                    slot << "UI2:[" << value << ": ";
                    for (uint_t i = value; i != 0; --i) {
                        value = *params++;
                        slot << value << ", ";
                    }
                    value = *params++;
                    slot << value << "]";
                    break;
                }
                case 7: {
                    value = *params++;
                    if (value == 0) {
                        slot << "UI4:[0]";
                        break;
                    }
                    slot << "UI4:[" << value << ": ";
                    for (uint_t i = value; i != 0; --i) {
                        value = *params++;
                        slot << value << ", ";
                    }
                    value = *params++;
                    slot << value << "]";
                    break;
                }
            }
        }
    }
    // Do the last set without a comma.
    value = *params++;
    slot << TypeString (value) << ", ";
    if (value == STR) {
        ++i;
        value = *params++;
        slot << value;
    }
    else if (value > 31) {
        // Then it's an array.
        type = value & 0x1f;    //< Mask off type.
        value = value >> 5;     //< Shift over array type.
        ++i;
        switch (value) {
            case 0:
            {
                break;
            }
            case 1:
            {
                value = *params++;
                slot << "UI1:" << value << ", ";
                break;
            }
            case 2:
            {
                value = *params++;
                slot << "UI2:" << value << ", ";
                break;
            }
            case 3:
            {
                value = *params++;
                slot << "UI4:" << value << ", ";
                break;
            }
            case 4:
            {
                value = *params++;
                slot << "UI5:" << value << ", ";
                break;
            }
            case 5:
            {
                value = *params++;
                if (value == 0) {
                    slot << "UI1:[0]";
                    break;
                }
                slot << "UI1:[" << value << ": ";
                for (uint_t i = value; i != 0; --i) {
                    value = *params++;
                    slot << value << ", ";
                }
                value = *params++;
                slot << value << "]";
                break;
            }
            case 6:
            {
                value = *params++;
                if (value == 0) {
                    slot << "UI2:[0]";
                    break;
                }
                slot << "UI2:[" << value << ": ";
                for (uint_t i = value; i != 0; --i) {
                    value = *params++;
                    slot << value << ", ";
                }
                value = *params++;
                slot << value << "]";
                break;
            }
            case 7:
            {
                value = *params++;
                if (value == 0) {
                    slot << "UI4:[0]";
                    break;
                }
                slot << "UI4:[" << value << ": ";
                for (uint_t i = value; i != 0; --i) {
                    value = *params++;
                    slot << value << ", ";
                }
                value = *params++;
                slot << value << "]";
                break;
            }
        }
    }
    return slot << '>';
}

/*
Slot& SlotWrite (Slot& slot, const char* text,
                   char delimiter) {
    if (!text) {
        return slot;
    }
    char* cursor = target;
    char s = *text;
    while (s != delimiter) {
        if (!s) {
            *target = 0; //< Replace the nil-term char.
            return slot;
        }
        *cursor = s;
        if (++cursor > target_end) {
            *target = 0; //< Replace the nil-term char.
            return slot;
        }
        ++text;
        s = *text;
    }
    *cursor = s;
    return slot;
}

Slot& SlotWrite (Slot& slot, const char* text,
                   const char* text_end) {
    if (!text) {
        return slot;
    }
    if (text > text_end) {
        return slot;
    }
    char* cursor = target;
    char t = *text;
    while (t) {
        *cursor = t;
        if (++cursor > target_end) {
            *target = 0;
            return slot;
        }
        if (++text > text_end) {
            *target = 0;
            return slot;
        }
        t = *text;
    }
    *cursor = t;
    return slot;
}

Slot& SlotWrite (Slot& slot, const char* text,
                   const char* text_end, char delimiter) {
    if (target > target_end) {
        return slot;
    }
    if (!text) {
        return slot;
    }
    if (text > text_end) {
        return slot;
    }
    char* cursor = target;
    char t = *text;
    while (t != delimiter) {
        if (!t) {
            //std::cout << "\n There was a !t error in SlotWrite.";
            return target;
        }
        *target = t;
        if (++target > target_end) {
            *target = 0;
            return slot;
        }
        if (++text > text_end) {
            *target = 0;
            return slot;
        }
        t = *text;
    }
    *target = t;
    return target;
}
*/
/*
Slot& PrintLine (const char* text, const char* header, int length, Slot& slot) {
    #if MAJOR_SEAM == 1 && MINOR_SEAM == 1
    std::cout << header << "Writing Line with length " << length;
    #endif

    char* begin,
        * start  = slot.start,
        * cursor = slot.stop,
        * end;
    intptr_t size;

    if (cursor < start) {
        size = cursor - start;
        if (size < length + 1) {
            return slot;
        }
        TextLine (cursor, stop, header);
    } else {

    }

    SlotWrite (slot, cursor, end, header);

    if (!cursor) {
        #if MAJOR_SEAM == 1 && MINOR_SEAM == 1
        std::cout << "\n Error writing header!";
        #endif
        return slot;
    }
    #if MAJOR_SEAM == 1 && MINOR_SEAM == 1
    std::cout << "... wrote " << TextLength (cursor) << " chars.";
    #endif
    
    stop = cursor + length;
    if ((cursor + length) > end) {
        stop = end;
    }
    #if MAJOR_SEAM == 1 && MINOR_SEAM == 1
    std::cout << "\n new_stop_length:" << stop - cursor;
    #endif

    while (cursor < stop) {
        *cursor++ = text;
    }
    *cursor = 0;
    #if MAJOR_SEAM == 1 && MINOR_SEAM == 1
    std::cout << "\n - Wrote " << SlotLength (cursor) << " chars: " 
              << cursor;
    #endif
    slot.stop = cursor;
    return slot;
}*/

Slot& PrintOp (const Op* op, Slot& slot) {
    slot << "\n Op:\n" << op->name << "\nparams_in:";

    PrintBsq (op->in, slot);
    slot << "\nparams_out:";
    PrintBsq (op->out, slot);
    slot << "\npop:" << op->pop << " close:" << op->close
        << " default_op:" << op->default_op
        << "\nignore_chars:" << op->ignore_chars
        << "\nallowed_chars:" << op->allowed_chars
        << "\n description :\"" << op->description;
    return slot;
}

}       //< namespace _
#undef PRINTF
#undef PUTCHAR
#endif  //< MAJOR_SEAM == 1 && MINOR_SEAM >= 3

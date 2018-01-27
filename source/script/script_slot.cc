/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/script_slot.cc
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
#include "type.h"
#include "error.h"
#include "memory.h"

namespace _ {

Slot& WriteSet (Slot& slot) {
    static Slot& slot_ = (slot.begin != slot_.begin) ?
                         slot : slot_;
    return slot_;
}

Slot& Out () {
    static uintptr_t buffer;
    static Slot empty_slot = {
        reinterpret_cast<char*> (&buffer),
        reinterpret_cast<char*> (&buffer),
        reinterpret_cast<char*> (&buffer),
        reinterpret_cast<char*> (&buffer) + sizeof (uintptr_t) - 1
    };
    static Slot& slot = WriteSet (empty_slot);
    return slot;
}

Slot& WriteLine () {
    return Out () << '\n';
}

Slot& Dump () {
    Slot& write  = Out ();
    char* begin,
        * cursor = write.start,
        * stop   = write.stop,
        * end    = write.end;
    while (cursor != stop) {
        if (cursor > end) {
            cursor == begin;
        }
        std::cerr << *cursor++;
    }
}

Slot& Line (Slot& slot, char token, const char* header, int length) {
    #if DEBUG_SCRIPT_SLOT
    Write () << header << "Writing Line with length " << length;
    #endif

    char* begin  = slot.begin,
        * start  = slot.start,
        * cursor = slot.stop,
        * end    = slot.end;

    stop = SlotWrite (cursor, end, header);

    if (!stop) {
    #if DEBUG_SCRIPT_SLOT
        Write () << "\n Error writing header!";
    #endif
        return slot;
    }
    #if DEBUG_SCRIPT_SLOT
    Write () << "... wrote " << SlotLength (cursor) << " chars.";
    #endif
    stop = cursor + length;
    if ((cursor + length) > end) {
        stop = end;
    }
    #if DEBUG_SCRIPT_SLOT
    Write () << "\n new_stop_length:" << stop - cursor;
    #endif

    while (cursor < stop) {
        *cursor++ = token;
    }
    *cursor = 0;
    #if DEBUG_SCRIPT_SLOT
    Write () << "\n Wrote " << SlotLength (cursor_) << " chars: " 
              << cursor_ << '\n';
    #endif
    cursor_ = cursor;
    return slot;
}

Slot& LineString (Slot& slot, const char* string, int num_columns) {
    //Line ();
    char* begin     = slot.begin,
        * cursor    = slot.start,
        * last_char = cursor + num_columns + 1, //< +1 for nil-term char.
        * end       = GetEnd ();

    const char* read = string;
    if (num_columns < 1) {
        return slot;
    }
    if (!string) {
        return slot;
    }
    if (cursor == end) {
        return slot;
    }
    if (last_char > end) { // Chop of some of the columns.
        last_char = end;
    }

    while (cursor < last_char) {
        char c = *read++;
        if (!c) {
            *cursor++ = '_';
            read = string;
        }
    }
    *cursor_ = 0;
    cursor_ = cursor;
    return slot;
}

Slot& LineBreak (Slot& slot, const char* message, int top_bottom_margin,
                           char c, int num_columns) {
    Lines (slot, top_bottom_margin);
    slot << "\n " << message;
    return Line (slot, c, "\n", num_columns);
}

Slot& Lines (Slot& slot, int num_rows) {
    char* cursor = slot.stop + 1,
        * end    = slot.end,
        * stop   = cursor + num_rows + 1;
    while (cursor < stop) {
        *cursor++ = '\n';
    }
    *cursor = 0;
    cursor_ = cursor + 1;
    return slot;
}

Slot& Hex (Slot& slot, const void* pointer) {
    // @todo Replace with PrintHex.
    int bytes_written = sprintf_s (cursor_, end_ - cursor_,
                                   "0x%p", pointer);
    cursor_ += bytes_written;
    return slot;
}

Slot& Memory (Slot& slot, const void* address, const void* stop) {
    // Todo: Rewrite with 
    //
    slot << "\n " << 0;
    //  columns
    for (int i = 8; i <= 66; i += 8) {
        Right (slot, i, 8);
    }
    Out () << "\n|";
    for (int i = 0; i < 65; ++i) {
        slot << '_';
    }

    const char* chars = reinterpret_cast<const char*> (address);
    char temp;
    while (chars < stop) {
        Out () << "\n|";
        for (int i = 0; i < 64; ++i) {
            temp = *chars;
            if (chars >= stop)
                temp = 'x';
            putchar (temp);
            ++chars;
        }
        slot << "| " << Hex (slot, chars + MemoryVector (address, stop));
    }
    Out () << "\n|";
    for (int i = 0; i < 64; ++i) {
        Out () << '_';
    }
    return slot << "| " << Hex (slot, chars + MemoryVector (address, stop));
}

Slot& Memory (Slot& slot, const void* address, int size) {
    return Memory (slot, address, reinterpret_cast<const char*> (address) +
                   size);
}

Slot& Hex (Slot& slot, byte c) {
    uint16_t chars = MemoryByteToUpperCaseHex (c);
    return slot << (char)chars << ((char)(chars >> 8)) << ' ';
}

Slot& WriteError (const char* message, const char* end_string) {
    return Out () << "\n Error: " << message << end_string;
}

static const char kDigits0To99[] =
    "0001020304050607080910111213141516171819"
    "2021222324252627282930313233343536373839"
    "4041424344454647484950515253545556575859"
    "6061626364656667686970717273747576777879"
    "8081828384858687888990919293949596979899";

typedef long long int64_t;
typedef unsigned long long uint64_t;

// Formats value in reverse and returns the number of digits.
template<typename UI>
char* FormatUnsiged (UI value) {

    enum {
        // 
        kMinBufferSize = std::numeric_limits<UI>::digits10 + 3
    };

    const char* digit;
    char *buffer_end = buffer_ + kMinBufferSize - 1;

    while (value >= 100) {
        // Algorithm stolen fair and square from Alexandrescu's "Three 
        // Optimization Tips for C++" and optimized by Cale McCollough.

        unsigned index = static_cast<unsigned>((value % 100) << 1);
        // << 1 to * 2
        value /= 100;
        digit = &kDigits0To99[index];
        *(buffer_end - 1) = *(digit + 1);
        *(buffer_end - 2) = *digit;
        buffer_end -= 2;
    }
    if (value < 10) {
        *(buffer_end - 1) = '0' + value;
        return buffer_end;
    }

    unsigned index = static_cast<unsigned>(value * 2);
    digit = &kDigits0To99[index];
    *(buffer_end - 1) = *(digit + 1);
    *(buffer_end - 2) = *digit;
    buffer_end -= 2;
    return buffer_end;
}

template<typename UI, typename SI>
void FormatSigned (SI value) {
    UI abs_value = static_cast<SI>(value);
    bool negative = value < 0;
    if (negative) {
        abs_value = 0 - abs_value;
    }
    str_ = FormatUnsiged (abs_value);
    if (negative) {
        *--str_ = '-';
    }
}

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
    slot->begin = begin;
    slot->start = begin;
    slot->stop  = begin;
    slot->end   = begin + size;
}

inline const Op* SlotError (Slot* slot, Error error,
                            const uint_t* header) {
#if DEBUG_SCRIPT_SLOT
    WriteLine () << ErrorStrings ()[error]);
#endif
    return reinterpret_cast<const Op*> (error);
}

inline const Op* SlotError (Slot* slot, Error error,
                            const uint_t* header,
                            byte offset) {
#if DEBUG_SCRIPT_SLOT
    WriteLine () << ErrorStrings ()[error];
#endif
    return reinterpret_cast<const Op*> (error);
}

inline const Op* SlotError (Slot* slot, Error error,
                            const uint_t* header,
                            uint_t offset,
                            char* address) {
#if DEBUG_SCRIPT_SLOT
    WriteLine () << ErrorStrings ()[error]);
#endif
    return reinterpret_cast<const Op*> (error);
}

inline const Op* SlotError (Slot* slot, Error error) {
#if DEBUG_SCRIPT_SLOT
    WriteLine () << ErrorStrings ()[error]);
#endif
    return reinterpret_cast<const Op*> (error);
}

Slot& SlotWrite (Slot& slot, const char* strand) {
    if (!strand) {
        return slot;
    }
    char* cursor = slot.stop + 1,
        * end    = slot.end;
    char c = *strand;
    ++strand;
    while (c) {
        if (cursor > end) {
            cursor = slot.start;
        }
        *cursor = c;
        ++cursor;
        c = *strand;
        ++strand;
    }
    *cursor = 0;
    return slot;
}

/*
Slot& SlotWrite (Slot& slot, const char* strand,
                   char delimiter) {
    if (!strand) {
        return slot;
    }
    char* cursor = target;
    char s = *strand;
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
        ++strand;
        s = *strand;
    }
    *cursor = s;
    return slot;
}

Slot& SlotWrite (Slot& slot, const char* strand,
                   const char* strand_end) {
    if (!strand) {
        return slot;
    }
    if (strand > strand_end) {
        return slot;
    }
    char* cursor = target;
    char t = *strand;
    while (t) {
        *cursor = t;
        if (++cursor > target_end) {
            *target = 0;
            return slot;
        }
        if (++strand > strand_end) {
            *target = 0;
            return slot;
        }
        t = *strand;
    }
    *cursor = t;
    return slot;
}

Slot& SlotWrite (Slot& slot, const char* strand,
                   const char* strand_end, char delimiter) {
    if (target > target_end) {
        return slot;
    }
    if (!strand) {
        return slot;
    }
    if (strand > strand_end) {
        return slot;
    }
    char* cursor = target;
    char t = *strand;
    while (t != delimiter) {
        if (!t) {
            //Write () << "\n There was a !t error in SlotWrite.";
            return target;
        }
        *target = t;
        if (++target > target_end) {
            *target = 0;
            return slot;
        }
        if (++strand > strand_end) {
            *target = 0;
            return slot;
        }
        t = *strand;
    }
    *target = t;
    return target;
}
*/

Slot& SlotWrite (Slot& slot, int8_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = target_end - target,
        result = sprintf_s (target, buffer_size, "%i", value);
    if (result < 0) {
        return slot;
    }
    return target + result;
}

Slot& SlotWrite (Slot& slot, uint8_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    enum { kStringSizeMax = 4 };

    intptr_t buffer_size = target_end - target,
        result = sprintf_s (target, buffer_size, "%u", value);
    if (result < 0) {
        *target = 0;
        return slot;
    }
    return target + result;
}

Slot& SlotWrite (Slot& slot, int16_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = target_end - target,
        result = sprintf_s (target, buffer_size, "%i", value);
    if (result < 0) {
        *target = 0;
        return slot;
    }
    return target + result;
}

Slot& SlotWrite (Slot& slot, uint16_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = target_end - target,
             result      = sprintf_s (target, buffer_size, "%u", value);
    if (result < 0) {
        *target = 0;
        return slot;
    }
    return target + result;
}

Slot& SlotWrite (Slot& slot, int32_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = target_end - target,
             result = sprintf_s (target, buffer_size, "%i", value);
    if (result < 0) {
        *target = 0;
        return slot;
    }
    return target + result;
}

Slot& SlotWrite (Slot& slot, uint32_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = target_end - target,
             result = sprintf_s (target, buffer_size, "%u", value);
    if (result < 0) {
        *target = 0;
        return slot;
    }
    return target + result;
}

Slot& SlotWrite (Slot& slot, int64_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = target_end - target,
             result = sprintf_s (target, buffer_size, "%lli", value);
    if (result < 0) {
        *target = 0;
        return slot;
    }
    return target + result;
}

Slot& SlotWrite (Slot& slot, uint64_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t result = sprintf_s (target, target_end - target, "%llu", value);
    if (result <= 0) {
        *target = 0;
        return slot;
    }
    Out () << "\n !!! " << target << " result:" << result;
    return target + result;
}

Slot& SlotWrite (Slot& slot, float value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = target_end - target,
             result = sprintf_s (target, buffer_size, "%f", value);
    if (result < 0) {
        *target = 0;
        return slot;
    }
    return target + result;
}

Slot& SlotWrite (Slot& slot, double value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = target_end - target,
             result = sprintf_s (target, buffer_size, "%f", value);
    if (result < 0) {
        *target = 0;
        return slot;
    }
    return target + result;
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
    #if USING_SCRIPT_4_BYTE_TYPES
    uint32_t  ui4;
    #endif
    #if USING_SCRIPT_8_BYTE_TYPES
    uint64_t  ui8;     //< Temp UI8 variable.
    #endif
    char*     ui1_ptr; //< Pointer to a UI1.
    uint16_t* ui2_ptr; //< Pointer to a UI2.
    uint32_t* ui4_ptr; //< Pointer to a UI4.
    uint64_t* ui8_ptr; //< Pointer to a UI8.
    uint_t    type,    //< Current type being read.
              size,    //< Size of the ring buffer.
              length,  //< Length of the data in the buffer.
              index,   //< Index in the escape sequence.
              num_params = *params; //< Number of params.
    uintptr_t offset,  //< Offset to word align the current type.
              count;   //< Argument length.

    if (!num_params) {
        return nullptr;
    }

    #if DEBUG_SCRIPT_SLOT
    Write () << "\n\nReading BIn: ";
    #endif

    char* begin = slot->begin,        //< Beginning of the buffer.
        * end   = slot->end,          //< end of the buffer.
        * start = slot->start,        //< start of the data.
        * stop  = slot->stop;         //< stop of the data.
    const uint_t* param = params + 1; //< current param.

    size = end - begin;

    length = SlotLength (start, stop, size);

    #if DEBUG_SCRIPT_SLOT
    //Text<> text;
    //text << "\n\nReading BIn:"
    //     << BsqPrint (params)
    //     << "\nbegin: " << Pointer (text , begin) 
    //     << " start:"     << MemoryVector (begin, start)
    //     << " stop:"      << MemoryVector (begin, stop )
    //     << " end:"       << MemoryVector (begin, end  )
    //     << " length:"    << length;
    #endif
    // When we scan, we are reading from the beginning of the BIn buffer.

    for (index = 0; index < num_params; ++index) {
        type = (byte)*param;
        ++param;
        #if DEBUG_SCRIPT_SLOT
        Write () << "\nindex " << index << ": " << TypeString (type) 
                 << "  start:" << MemoryVector (begin, start)
                 << ", stop:" << MemoryVector (begin, stop);
        #endif

        switch (type) {
            case NIL:
                return SlotError (slot, kErrorInvalidType);
            case ADR:    //< _R_e_a_d__S_t_r_i_n_g_-_1______________________
            case STR:
                // Load buffered-type argument length and increment the 
                // index.
                count = *param;
                ++param;

                //Write () << "\nReading char with max length " << count;

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
                    #if DEBUG_SCRIPT_SLOT
                    Write () << ui1;
                    #endif

                    ui1 = *start;       // Read byte from ring-buffer.
                    if (++start > end) {
                        start -= size;
                    }
                    *ui1_ptr = ui1;     // Write byte to destination.
                    ++ui1_ptr;
                }
                    
                #if DEBUG_SCRIPT_SLOT
                Write () << " done!\n";
                #endif

                if (type == 0) {
                    *ui1_ptr = ui1;
                }
                break;
            case SI1:    //< _R_e_a_d__1__B_y_t_e__T_y_p_e_s________________
            case UI1:
            case BOL:
                #if USING_SCRIPT_1_BYTE_TYPES
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
                #if USING_SCRIPT_2_BYTE_TYPES
                Read2ByteType:{
                    // Word-align
                    offset = MemoryAlign2 (start);
                    if (length < offset + 2) {
                        return SlotError (slot, kErrorBufferUnderflow,
                                                params, index, start);
                    }
                    length -= offset + 2;
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
                }
                break;
                #else
                return SlotError (slot, kErrorInvalidType);
                #endif
            #if USING_SCRIPT_VARINT2
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
                #if USING_SCRIPT_4_BYTE_TYPES
                Read4ByteType:{
                    // Word-align
                    offset = MemoryAlign4 (start);
                    if (length < offset + 4) {
                        return SlotError (slot, kErrorBufferUnderflow,
                                          params, index, start);
                    }
                    length -= offset + 4;
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
                }
                #else
                return SlotError (slot, kErrorInvalidType);
                #endif
            case SI8:     //< _R_e_a_d__6_4_-_b_i_t__T_y_p_e_s______________
            case UI8:
            case DBL:
            case TMU:
            case SV8:
            case UV8:
                #if USING_SCRIPT_8_BYTE_TYPES
                Read8ByteType:{
                    // Word-align
                    offset = MemoryAlign8 (start);
                    if (length < offset + sizeof (int64_t)) {
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
                }
                #else
                return SlotError (slot, kErrorInvalidType);
                #endif
            default: {
                #if USING_SCRIPT_OBJ
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
            #if DEBUG_SCRIPT_SLOT
            Write () << " |";
            #endif
        }
    }
    #if DEBUG_SCRIPT_SLOT
    Write () << "\nDone reading";
    SlotWipe (slot);
    #endif

    // Convert pointer back to offset
    slot->start = start;

    return 0;
}

const Op* SlotWrite (Slot* slot, const Op& op, void** args) {
    return nullptr;
}

}       //< namespace _

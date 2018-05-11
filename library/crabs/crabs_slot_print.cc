/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/crabs_slot.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017-2018 Cale McCollough <calemccollough@gmail.com>;
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
#include "line.h"

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 5

#include "slot_print.h"

#if MAJOR_SEAM == 1 && MINOR_SEAM == 5

#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PUTCHAR(c) putchar(c);
#define PRINT_BSQ(bsq)\ {\
    enum { kSize = 64*1024 }\
    char bsq_buffer[kSize];\
}
#else
#define PRINTF(x, ...)
#define PUTCHAR(c)
#endif

namespace _ {


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

Slot& Print (int32_t value, Slot& slot, char delimiter) {
    return slot;
}

Slot& Print (uint32_t value, Slot& slot, char delimiter) {
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

Slot& PrintCentered (const char* text, int num_columns, Slot& slot,
                     char delimiter) {
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
    uint16_t chars = HexByteToUpperCase (c);
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
        c = HexNibbleToUpperCase (c);
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
        slot << Hex<const void*> (chars + (address_end_ptr - address_ptr));
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
    slot << Hex<const void*> (chars + (address_end_ptr - address_ptr));
    return slot;
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

void Print (Slot& slot) {
    intptr_t size = SlotLength (slot);
    char* buffer = new char[size + 128];
    Printer print (buffer, buffer + size + 129);
    print << Line ('_', 80)
          << "\nBOut:" << Hex<uintptr_t> (&slot) << " size:" << size
          << " start:" << slot.start << " stop:" << slot.stop
          << " read:"  << size;
    PrintMemory (slot.start, size + 64, slot);
    //< @todo remove the + 64.);
}

Slot& PrintLine (char c, int num_columns, Slot& slot,
                 char delimiter) {
    char   * start = slot.start,
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

Slot& PrintLine (char c, int num_columns, Slot& slot,
                 char delimiter) {
    char* start = slot.start,
        *cursor = slot.stop,
        *end = slot.end;
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

}       //< namespace _
#undef PRINTF
#undef PUTCHAR
#endif  //> #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 5

/** Kabuki Toolkit
    @version 0.x
    @file    ~/libraries/crabs/list.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-8 Cale McCollough <calemccollough@gmail.com>;
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

#pragma once
#include <stdafx.h>

#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 3
#ifndef HEADER_FOR_CRABS_LIST
#define HEADER_FOR_CRABS_LIST

#include "set.h"
#include "align.h"

namespace _ {

/** An ASCII List header.
    Like all ASCII Object Types, the size may only be 16-bit, 32-bit, or 
    64-bit. The unsigned value must be twice the width of the signed value.

    @code
    +==========================+ -----------
    |_______ Buffer            |   ^     ^
    |_______ ...               |   |     |
    |_______ Data N            |  Data   |
    |_______ ...               |   |     |
    |_______ Data 0            |   v     |
    |==========================| -----   |
    |_______ count_max         |   ^     |
    |_______ ...               |   |     |
    |_______ Data Offset N     |   |     |
    |_______ ...               |   |    Size
    |        Data Offset 1     |   |     |
    |==========================| Header  |
    |_______ count_max         |   |     |
    |_______ ...               |   |     |
    |_______ Type byte N       |   |     |
    |_______ ...               |   |     |
    |        Type byte 1       |   |     |   ^ 0x(N+1)+sizeof (AsciiList<UI, SI>)
    |==========================|   |     |   | 
    | AsciiList<UI, SI> Struct |   v     v   ^
    +==========================+ ----------- ^ 0xN
    @endcode
*/
template<typename UI = uint32_t, typename SI = int16_t>
struct AsciiList {
    UI size_bytes;
    SI count_max,
       count;
};

/** Returns the minimum count to align the data struct to a 64-bit boundary. */
template<typename UI = uint32_t, typename SI = int16_t>
SI ListCountMaxMin () {
    return 8 / sizeof (SI);
}

template<typename UI = uint32_t, typename SI = int16_t>
inline UI ListSizeMin (SI count_max) {
    return (UI)sizeof (AsciiList<UI, SI>) +  ((UI)count_max << 1);
    // << 2 to * 2.
}

/** Initializes a AsciiList from preallocated memory.
    count_max must be in multiples of 4. Given there is a fixed size, both the 
    count_max and size will be downsized to a multiple of 4 automatically. */
template<typename UI = uint32_t, typename SI = int16_t>
AsciiList<UI, SI>* ListInit (uintptr_t* buffer, UI size_bytes, SI count_max) {
    if (!buffer) // This may be nullptr if ListNew<UI,SI> (SI, UI) failed.
        return nullptr;

    AsciiList<UI, SI>* list = reinterpret_cast<AsciiList<UI, SI>*> (buffer);
    list->size_bytes = size_bytes;
    list->count      = 0;
    list->count_max  = count_max;
    return list;
}

/** Creates a list from dynamic memory. */
template<typename UI = uint32_t, typename SI = int16_t>
uintptr_t* ListNew (UI size_bytes, SI count_max) {
    count_max = AlignUp8<SI> (count_max);
    if (size_bytes < ListSizeMin<UI, SI> (count_max))
        return nullptr;
    uintptr_t* buffer = new uintptr_t[size_bytes >> kWordBitCount];
    ListInit<UI, SI> (buffer, size_bytes, count_max);
    return buffer;
}

/** Returns the type bytes array. */
template<typename UI = uint32_t, typename SI = int16_t>
type_t* ListTypes (AsciiList<UI, SI>* list) {
    ASSERT (list)
    return reinterpret_cast<type_t*> (list) + sizeof (AsciiList<UI, SI>);
}

/** Gets a pointer to the begging of the data buffer. */
template<typename UI = uint32_t, typename SI = int16_t>
inline char* ListDataBegin (AsciiList<UI, SI>* list) {
    ASSERT (list)
    return reinterpret_cast<char*> (list) + list->count_max * sizeof (SI);
}

/** Gets the base element 0 of the list's offset array. */
template<typename UI = uint32_t, typename SI = int16_t>
inline UI* ListOffsets (AsciiList<UI, SI>* list) {
    char* base = reinterpret_cast<char*> (list);
    return reinterpret_cast<UI*> (base + list->count_max);
}

/** Returns the last byte in the data array. */
template<typename UI = uint32_t, typename SI = int16_t>
inline char* ListDataStop (AsciiList<UI, SI>* list, SI index) {
    ASSERT (list)
    SI count = list->count;
    if (count == 0)
        return nullptr;
    type_t* types = ListTypes<UI, SI> (list);
    type_t  type  = types[index];
    UI* offsets = ListOffsets<UI, SI> (list);
    UI offset = offsets[index];
    char* pointer = reinterpret_cast<char*> (list) + offset;
    return ObjectEnd<UI> (type, pointer);
}

/** Returns the last byte in the data array. */
template<typename UI = uint32_t, typename SI = int16_t>
inline char* ListDataStop (AsciiList<UI, SI>* list) {
    ASSERT (list);
    return ListDataStop<UI, SI> (list, list->count - 1);
}

/** Returns the last byte in the data array. */
template<typename UI = uint32_t, typename SI = int16_t>
inline char* ListDataEnd (AsciiList<UI, SI>* list) {
    ASSERT (list)
    return reinterpret_cast<char*> (list) + list->size_bytes - 1;
}

/** Returns the last byte in the data array. */
template<typename UI = uint32_t, typename SI = int16_t>
inline char* ListDataEnd (AsciiList<UI, SI>* list, SI index) {
    ASSERT (list)
    if (index < 0 || index >= index->count)
        return nullptr;
    return reinterpret_cast<char*> (list) + list->size_bytes - 1;
}

/** Returns a pointer to the begging of the data buffer. */
template<typename UI = uint32_t, typename SI = int16_t>
Socket ListDataVector (AsciiList<UI, SI>* list) {
    return Socket (ListDataBegin<UI, SI> (list), ListDataEnd<UI, SI> (list));
}

/** Insets the given type-value tuple. */
template<typename UI = uint32_t, typename SI = int16_t>
SI ListInsert (AsciiList<UI, SI>* list, type_t type, const void* value, SI index) {
    ASSERT (list)
    if (value == nullptr)
        return -1;
    SI count = list->count;
    if (count >= list->count_max || TypeIsValid (type))
        return -1;

    type_t* types = ListTypes<UI, SI> (list);

    // 1. Check if the count is zero and do add at the begin.
    if (count == 0) {
        types[index] = type;
        Write (ListDataBegin<UI, SI> (list), ListDataEnd<UI, SI> (list), type, 
               value);
        return 0;
    }

    types += index;

    // 2. Shift up the types.
    type_t* types_cursor = types + count - 1;
    while (types_cursor > types)
        *types_cursor-- = *types_cursor;

    // Add the type byte
    *types = type;

    // Shift up the offsets.
    UI* offsets_begin  = ListOffsets<UI, SI> (list),
      * offsets_cursor = offsets_begin + count;

    while (offsets_cursor > offsets_begin)
        *offsets_cursor-- = *offsets_cursor;

    char* aligned_begin  = AlignUpPointer8<> (ListDataStop<UI, SI> (list) + 1);

    if (!Write (aligned_begin, ListDataEnd<UI, SI> (list), type, value))
        return -1;
    list->count = ++count;
    return count;
}

/** Adds a type-value to the end of the list. */
template<typename UI = uint32_t, typename SI = int16_t>
inline SI ListPush (AsciiList<UI, SI>* list, type_t type, const void* value) {
    return ListInsert<UI, SI> (list, type, value, list->count - 1);
}

/** Returns the max count an array can handle. */
template<typename UI = uint32_t, typename SI = int16_t>
SI ListCountMax () {
    enum {
        kMaxIndexes = sizeof (SI) == 1 ? 120 :
                      sizeof (SI) == 2 ? 8 * 1024 : 
                      sizeof (SI) == 4 ? 512 * 1024 * 1024 : 0,
    };
    return kMaxIndexes;
}

/** Deletes the list contents without wiping the contents. */
template<typename UI = uint32_t, typename SI = int16_t>
void ListClear (AsciiList<UI, SI>* list) {
    ASSERT (list)
    list->count = 0;
}

/** Deletes the list contents by overwriting it with zeros. */
template<typename UI = uint32_t, typename SI = int16_t>
void ListWipe (AsciiList<UI, SI>* list) {
    ASSERT (list)
    list->count = 0;
    UI size_bytes = list->size_bytes - sizeof (AsciiList<UI, SI>);
    memset (reinterpret_cast<char*> (list) + sizeof (AsciiList<UI, SI>), 0, size_bytes);
}

/** Returns true if this expr contains only the given address.
    @warning This function assumes that the member you're checking for came 
             from Kabuki Toolkit. If it's you're own code calling this, you are
             required to ensure the value came from a ASCII List.
    @return  True if the data lies in the list's memory socket. */
template<typename UI = uint32_t, typename SI = int16_t>
bool ListContains (AsciiList<UI, SI>* list, void* address) {
    ASSERT (list)
    if (reinterpret_cast<char*> (address) < reinterpret_cast<char*> (list))
        return false;
    if (reinterpret_cast<char*> (address) > ListEndByte())
        return false;
    return true;
}

/** Removes the item at the given address from the list. */
template<typename UI = uint32_t, typename SI = int16_t>
bool ListRemove (AsciiList<UI, SI>* list, SI index) {
    ASSERT (list)
    SI count = list->count;
    if (index < 0 || index >= count) {
        return false;
    }
    
    type_t* cursor = ListTypes<UI, SI> (list),
           * end    = cursor + (count - index);
    type_t type_to_remove = *cursor;
    for (; cursor < end; ++cursor)
        *cursor++ = cursor;
    cursor = ListTypes<UI, SI> (list);
    UI* data_cursor = ListOffsets<UI, SI> (list);
    return false;
}

/** Finds a tuple that contains the given pointer. */
template<typename UI = uint32_t, typename SI = int16_t>
bool ListFind (AsciiList<UI, SI>* list, void* adress) {
    ASSERT (list)
    UI* offsets = ListOffsets<UI, SI> (list),
      * offset_end = offsets +  list->count;
    while (offsets < offset_end) {
        char* begin = reinterpret_cast<char*> (list) + *offsets++,
            * end   = ListDataStop<UI, SI> (list, index);
        if (reinterpret_cast<char*> (address) >= begin &&
            reinterpret_cast<char*> (address) <= end)
            return true;
    }
    return false;
}

/** Removes the item at the given address from the list. */
template<typename UI = uint32_t, typename SI = int16_t>
bool ListRemove (AsciiList<UI, SI>* list, void* adress) {
    return ListRemove<UI, SI> (list, ListFind (list, address));
}

/** Prints the given AsciiList to the console. */
template<typename UI = uint32_t, typename SI = int16_t>
Printer ListPrint (AsciiList<UI, SI>* list, Printer out_) {
    ASSERT (list)
    
    SI count = list->count;
    out_ << "\n\nList:\ncount:" << count << " count_max:" << count_max;
    for (SI index = 0; index < count; ++index) {
        type_t type = ListTypes<UI, SI> (list)[index];
        out_ << "\n" << index << ".) type:" << TypeString (type)
              << " value:" << TypePrint (type, ListValue<UI, SI> (list, count), 
                                         out_);
    }
    return out_;
}

/** ASCII List that uses dynamic memory. */
template<typename UI = uint32_t, typename SI = int16_t>
class List {
    public:

    /** constructs a List with the given size_bytes and count_max. 
        size_bytes and count_max both get rounded down to a multiple of 64 
        before allocating the buffer. If the count_max is not enough for the 
        buffer then the size_bytes will be increased to the minimum size to
        make a valid ASCII List. */
    List (UI size_bytes = 2048, SI count_max = 0) {
        if (count_max == 0)
            count_max = ListCountMaxMin<UI, SI> ();
        buffer_ = ListNew<UI, SI> (size_bytes, count_max);
    }

    /** Deletes the dynamically allocated buffer. */
    ~List () {
        delete buffer_;
    }

    inline SI Push (type_t type, const void* data) {
        return ListPush<UI, SI> (This (), type, data);
    }

    /** Inserts the given type-value tuple in the list at the given index. */
    inline SI Insert (byte type, void* data, SI index) {
        return ListInsert<UI, SI> (This (), type, data, index);
    }

    /** Returns the maximum count of the give list in the current memory 
        profile. */
    inline SI CountMax () {
        return ListCountMax<UI, SI> ();
    }

    void Clear (AsciiList<UI, SI>* list) {
        ListClear<UI, SI> (This ());
    }

    /** Deletes the list contents by overwriting it with zeros. */
    inline void Wipe () {
        ListWipe<UI, SI> (This ());
    }

    inline bool Contains (AsciiList<UI, SI>* list) {
        return ListContains<UI, SI> (This ());
    }

    /** Returns true if this expr contains only the given address.
        @warning This function assumes that the member you're checking for came
                 from Kabuki Toolkit. If it's you're own code calling this, you 
                 are required to ensure the value came from a ASCII List.
        @return  True if the data lies in the list's memory socket. */
    inline bool Contains (void* data) {
        return ListContains<UI, SI> (This (), data);
    }

    /** Removes the item at the given address from the list. */
    inline bool Remove (void* adress) {
        return ListRemove<UI, SI> (This (), adress);
    }

    /** Prints the given AsciiList to the console. */
    inline Printer& Print (Printer& printer) {
        return ListPrint<UI, SI> (printer, This ());
    }

    private:

    uintptr_t* buffer_; //< Dynamically allocated word-aligned buffer.

    inline AsciiList<UI, SI>* This () {
        return reinterpret_cast<AsciiList<UI, SI>*> (buffer_);
    }
};

}       //< namespace _
#endif  //< HEADER_FOR_CRABS_LIST
#endif  //< MAJOR_SEAM >= 1 && MINOR_SEAM >= 3

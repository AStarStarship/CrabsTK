/** Kabuki Toolkit
    @version 0.x
    @file    ~/libraries/crabs/list.h
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

#pragma once
#include <stdafx.h>

#ifndef HEADER_FOR_CRABS_LIST
#define HEADER_FOR_CRABS_LIST

#include "set.h"
#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 3

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
    |        Type byte 1       |   |     |
    |==========================|   |     |    +
    |   TList<UI, SI> Struct   |   v     v    |
    +==========================+ ----------- 0xN
    @endcode
*/
template<typename UI = uint32_t, typename SI = int16_t>
struct TList {
    UI size;
    SI count_max,
       count;
};
    
/** Initializes a TList.
    @post    Users might want to call the IsValid () function after construction
             to verify the integrity of the object.
    @warning The reservedNumOperands must be aligned to a 32-bit value, and it
             will get rounded up to the next higher multiple of 4. */
template<typename UI = uint32_t, typename SI = int16_t>
TList<UI, SI>* ListInit (uintptr_t* buffer, UI max_size, uint16_t table_size, 
                         uint16_t size) {
    assert (buffer);

    if (table_size >= size)
        return nullptr;
    if (table_size < sizeof (TList) + max_size *
        (ListOverheadPerIndex<UI, SI> () + 2))
        return nullptr;

    List<UI, SI>* list = reinterpret_cast<TList<UI, SI>*> (buffer);
    list->size = table_size;
    list->table_size = table_size;
    list->; = 0;
    list->max_items = max_size;
    list->pile_size = 1;
    return list;
}

template<typename UI = uint32_t, typename SI = int16_t>
char* ListTypes (TList<UI, SI>* list) {
    assert (list);
    return reinterpret_cast<char*> (list) + sizeof (List<UI, SI>);
}

/** Adds a tuple to the end of the list. */
template<typename UI = uint32_t, typename SI = int16_t>
SI ListAdd (TList<UI, SI>* list, char type, void* value) {
    assert (list);
    assert (value);
    SI count = list->count;
    if (count >= list->count_max || TypeIsValid (type))
        return -1;

    char* types = ListTypes (list);

    if (count == 0) {
        types[count] = type;
        TypeWrite (type, value);
    }

    // Add the type byte
    types[count] = type;
    char last_type = types[count - 1];
    if ()
}

/** Insets the given tuple.
*/
template<typename UI = uint32_t, typename SI = int16_t>
SI ListInsert (TList<UI, SI>* list, byte type,
               const char* key, void* data, SI index) {
    assert (list);
    return ~0;
}

template<typename UI = uint32_t, typename SI = int16_t>
SI ListCountMax () {
    enum {
        kMaxIndexes = sizeof (SI) == 1 ? 120 :
                      sizeof (SI) == 2 ? 8 * 1024 : 
                      sizeof (SI) == 4 ? 512 * 1024 * 1024 : 0
    };
    return kMaxIndexes;
}

/** Adds a key-value pair to the end of the list. */
//byte Add2 (List2* list, const char* key, byte data) {
//    return ListAdd<byte, uint16_t, uint16_t> (list, key, UI1, &data);
//}

/** Deletes the list contents without wiping the contents. */
template<typename UI = uint32_t, typename SI = int16_t>
void ListClear (TList<UI, SI>* list) {
    assert (list);
    list->count = 0;
}

/** Deletes the list contents by overwriting it with zeros. */
template<typename UI = uint32_t, typename SI = int16_t>
void ListWipe (TList<UI, SI>* list) {
    assert (list);
    list->count = 0;
    UI size = list->size - sizeof (TList<UI, SI>);
    memset (reinterpret_cast<char*> (list) + sizeof (TList<UI, SI>), 0, size);
}

template<typename UI = uint32_t, typename SI = int16_t>
bool ListContains (TList<UI, SI>* list) {
    assert (list);
    return false;
}

/** Returns true if this expr contains only the given address. */
template<typename UI = uint32_t, typename SI = int16_t>
bool ListContains (TList<UI, SI>* list, void* data) {
    assert (list);
    if (reinterpret_cast<char*> (data) < reinterpret_cast<char*> (list))
        return false;
    if (reinterpret_cast<char*> (data) > ListEndByte())
        return false;
    return true;
}

/** Removes that object from the list and copies it to the destination. */
template<typename UI = uint32_t, typename SI = int16_t>
bool ListRemoveCopy (TList<UI, SI>* list, void* destination,
                 size_t buffer_size, void* data) {
    assert (list);

    return false;
}

/** Removes the item at the given address from the list. */
template<typename UI = uint32_t, typename SI = int16_t>
bool ListRemove (TList<UI, SI>* list, void* adress) {
    assert (list);

    return false;
}

/** Removes all but the given list from the list. */
template<typename UI = uint32_t, typename SI = int16_t>
bool ListRetain (TList<UI, SI>* list, SI index) {
    assert (list);

    return false;
}

/** Creates a list from dynamic memory. */
template<typename UI = uint32_t, typename SI = int16_t>
TList<UI, SI>* ListCreate (SI buffered_indexes, UI table_size, UI size) {
    assert (list);
    TList<UI, SI>* list = New<TList, uint_t> ();
    return list;
}

/** Prints the given TList to the console. */
template<typename UI = uint32_t, typename SI = int16_t>
Printer& ListPrint (TList<UI, SI>* list, Printer& print) {
    assert (list);
    return print;
}

/** C++ Wrapper class for TList that uses dynamic memory. */
template<typename UI = uint32_t, typename SI = int16_t>
class List {
    public:

    List () {

    }

    ~List () {
        delete buffer_;
    }

    private:

    uintptr_t* buffer_; //< Dynamically allocated word-aligned buffer.
};

}       //< namespace _
#endif  //< MAJOR_SEAM >= 1 && MINOR_SEAM >= 3
#endif  //< HEADER_FOR_CRABS_LIST

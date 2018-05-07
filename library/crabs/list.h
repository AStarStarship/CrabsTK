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

#ifndef CRABS_TLIST_H
#define CRABS_TLIST_H

#include "set.h"
#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 3

namespace _ {

template<typename TIndex, typename TOffset, typename TData>
constexpr uint_t ListOverheadPerIndex () {
        return sizeof (2 * sizeof (TIndex) + sizeof (TOffset) + sizeof (TData) +
        		       3);
};

template<typename TIndex, typename TOffset, typename TData>
constexpr TData MinSizeList (TIndex num_items) {
    return num_items * sizeof (2 * sizeof (TIndex) + sizeof (TOffset) +
    		                   sizeof (TData) + 3);
};

enum {
    kMaxNumPagesList2 = 255,                //< The number of pages in a List2.
    kMaxNumPagesList4 = 8 * 1024,           //< The number of pages in a List4.
    kMaxNumPagesList8 = 256 * 1024 * 1024,  //< The number of pages in a List8.
    kOverheadPerList2Index = ListOverheadPerIndex<byte, uint16_t, uint16_t> (),
    kOverheadPerList4Index = ListOverheadPerIndex<byte, uint16_t, uint32_t> (),
    kOverheadPerList8Index = ListOverheadPerIndex<byte, uint16_t, uint64_t> (),
};
    
/** Initializes a TList.
    @post    Users might want to call the IsValid () function after construction
             to verify the integrity of the object.
    @warning The reservedNumOperands must be aligned to a 32-bit value, and it
             will get rounded up to the next higher multiple of 4.
static TList* ListInit (char* buffer, byte max_size, uint16_t table_size, uint16_t size)
{
    if (buffer == nullptr)
        return nullptr;
    if (table_size >= size)
        return nullptr;
    if (table_size < sizeof (TList) + max_size *
        (ListOverheadPerIndex<byte, uint16_t, uint16_t> () + 2))
        return nullptr;

    List2* collection = reinterpret_cast<TList*> (buffer);
    collection->size = table_size;
    collection->table_size = table_size;
    collection->; = 0;
    collection->max_items = max_size;
    collection->pile_size = 1;
    return collection;
}
*/

/** Insets the given key-value pair.
*/
template<typename TIndex, typename TOffset, typename TData>
TIndex ListInsert (TList<TIndex, TOffset, TData, THash>* collection, byte type,
               const char* key, void* data, TIndex index) {
    if (collection == nullptr) return 0;
    return ~0;
}

template<typename TIndex>
TIndex ListIndexMax () {
    enum {
        kMaxIndexes = sizeof (TIndex) == 1 ? 255 : sizeof (TIndex) == 2 ?
                       8 * 1024 : sizeof (TIndex) == 4 ? 512 * 1024 * 1024 : 0
    };
    return kMaxIndexes;
}

/** Adds a key-value pair to the end of the collection. */
template<typename TIndex, typename TOffset, typename TData>
TIndex ListAdd (TList<TIndex, TOffset, TData, THash>* collection, const char* key,
                TType type, void* data) {


    return ;;
}

/** Adds a key-value pair to the end of the collection. */
//byte Add2 (List2* collection, const char* key, byte data) {
//    return ListAdd<byte, uint16_t, uint16_t> (collection, key, UI1, &data);
//}

/** Returns  the given query char in the hash table. */
template<typename TIndex, typename TOffset, typename TData>
TIndex ListFind (TList<TIndex, TOffset, TData, THash>* collection, const char* key) {
    if (collection == nullptr)
        return 0;

    return ~((TIndex)0);
}

/** Deletes the collection contents without wiping the contents. */
template<typename TIndex, typename TOffset, typename TData>
void ListClear (TList<TIndex, TOffset, TData, THash>* collection) {
    if (collection == nullptr) return;
    collection->num_items = 0;
}

/** Deletes the collection contents by overwriting it with zeros. */
template<typename TIndex, typename TOffset, typename TData>
void ListWipe (TList<TIndex, TOffset, TData, THash>* collection) {
    if (collection == nullptr) return;
    TData size = collection->size;
    memset (collection, 0, size);
}

/** Returns true if this expr contains only the given address. */
template<typename TIndex, typename TOffset, typename TData>
bool ListContains (TList<TIndex, TOffset, TData, THash>* collection, void* data) {
    if (collection == nullptr) return false;
    if (data < collection) return false;
    if (data > GetEndAddress()) return false;
    return true;
}

/** Removes that object from the collection and copies it to the destination. */
template<typename TIndex, typename TOffset, typename TData>
bool ListRemoveCopy (TList<TIndex, TOffset, TData, THash>* collection, void* destination,
                 size_t buffer_size, void* data) {
    if (collection == nullptr) return false;

    return false;
}

/** Removes the item at the given address from the collection. */
template<typename TIndex, typename TOffset, typename TData>
bool ListRemove (TList<TIndex, TOffset, TData, THash>* collection, void* adress) {
    if (collection == nullptr) return false;

    return false;
}

/** Removes all but the given collection from the collection. */
template<typename TIndex, typename TOffset, typename TData>
bool ListRetain (TList<TIndex, TOffset, TData, THash>* collection) {
    if (collection == nullptr) return false;

    return false;
}

/** Creates a collection from dynamic memory. */
template<typename TIndex, typename TOffset, typename TData>
TList<TIndex, TOffset, TData, THash>* ListCreate (TIndex buffered_indexes,
                                                        TData table_size,
                                                        TData size) {
    TList<TIndex, TOffset, TData, THash>* collection = New<TList, uint_t> ();
    return collection;
}

/** Prints the given TList to the console. */
template<typename TIndex, typename TOffset, typename TData>
void ListPrint (TList<TIndex, TOffset, TData, THash>* collection) {

}

}       //< namespace _
#endif  //< MAJOR_SEAM >= 1 && MINOR_SEAM >= 6
#endif  //< CRABS_TLIST_H

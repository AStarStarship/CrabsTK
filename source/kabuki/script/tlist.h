/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/include/tlist.h
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

#pragma once
#include <stdafx.h>

#ifndef SCRIPT_TLIST_H
#define SCRIPT_TLIST_H

#include "set.h"
#if USING_SCRIPT_LIST

namespace _ {

template<typename TIndex, typename TKey, typename TData, typename THash>
constexpr uint_t SetOverheadPerIndex () {
        return sizeof (2 * sizeof (TIndex) + sizeof (TKey) + sizeof (TData) +
        		       3);
};

template<typename TIndex, typename TKey, typename TData, typename THash>
constexpr TData MinSizeSet (TIndex num_items) {
    return num_items * sizeof (2 * sizeof (TIndex) + sizeof (TKey) +
    		                   sizeof (TData) + 3);
};

enum {
    kMaxNumPagesSet2 = 255,                //< The number of pages in a Set2.
    kMaxNumPagesSet4 = 8 * 1024,           //< The number of pages in a Set4.
    kMaxNumPagesSet8 = 256 * 1024 * 1024,  //< The number of pages in a Set8.
    kOverheadPerSet2Index = SetOverheadPerIndex<byte, uint16_t, uint16_t, hash16_t> (),
    kOverheadPerSet4Index = SetOverheadPerIndex<byte, uint16_t, uint16_t, hash16_t> (),
    kOverheadPerSet8Index = SetOverheadPerIndex<byte, uint16_t, uint16_t, hash16_t> (),
};
    
/** Initializes a Set.
    @post    Users might want to call the IsValid () function after construction
             to verify the integrity of the object.
    @warning The reservedNumOperands must be aligned to a 32-bit value, and it
             will get rounded up to the next higher multiple of 4.
static Set* Init2 (byte* buffer, byte max_size, uint16_t table_size, uint16_t size)
{
    if (buffer == nullptr)
        return nullptr;
    if (table_size >= size)
        return nullptr;
    if (table_size < sizeof (Set) + max_size *
        (SetOverheadPerIndex<byte, uint16_t, uint16_t, hash16_t> () + 2))
        return nullptr;

    Set2* collection = reinterpret_cast<Set*> (buffer);
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
template<typename TIndex, typename TKey, typename TData, typename THash>
TIndex SetInsert (Set<TIndex, TKey, TData, THash>* collection, byte type,
               const byte* key, void* data, TIndex index) {
    if (collection == nullptr) return 0;
    return ~0;
}

template<typename TIndex>
TIndex MaxSetIndexes () {
    enum {
        kMaxIndexes = sizeof (TIndex) == 1 ? 255 : sizeof (TIndex) == 2 ?
                       8 * 1024 : sizeof (TIndex) == 4 ? 512 * 1024 * 1024 : 0
    };
    return kMaxIndexes;
}

/** Adds a key-value pair to the end of the collection. */
template<typename TIndex, typename TKey, typename TData, typename THash>
TIndex SetAdd (Set<TIndex, TKey, TData, THash>* collection, const char* key,
                TType type, void* data) {


    return ;;
}

/** Adds a key-value pair to the end of the collection. */
//byte Add2 (Set2* collection, const char* key, byte data) {
//    return SetAdd<byte, uint16_t, uint16_t, hash16_t> (collection, key, UI1, &data);
//}

/** Returns  the given query char in the hash table. */
template<typename TIndex, typename TKey, typename TData, typename THash>
TIndex SetFind (Set<TIndex, TKey, TData, THash>* collection, const char* key) {
    if (collection == nullptr)
        return 0;

    return ~((TIndex)0);
}

//static byte Find2 (Set2* collection, const char* key) {
//    return SetFind<byte, uint16_t, uint16_t, hash16_t> (collection, key);
//}

/** Prints this object out to the console. */
template<typename TIndex, typename TKey, typename TData, typename THash>
void SetPrint (const Set<TIndex, TKey, TData, THash>* collection) {
    if (collection == nullptr) {
    	return;
    }

}

/** Deletes the collection contents without wiping the contents. */
template<typename TIndex, typename TKey, typename TData, typename THash>
void Clear (Set<TIndex, TKey, TData, THash>* collection) {
    if (collection == nullptr) return;
    collection->num_items = 0;
}

/** Deletes the collection contents by overwriting it with zeros. */
template<typename TIndex, typename TKey, typename TData, typename THash>
void Wipe (Set<TIndex, TKey, TData, THash>* collection) {
    if (collection == nullptr) return;
    TData size = collection->size;
    memset (collection, 0, size);
}

/** Returns true if this expr contains only the given address. */
template<typename TIndex, typename TKey, typename TData, typename THash>
bool Contains (Set<TIndex, TKey, TData, THash>* collection, void* data) {
    if (collection == nullptr) return false;
    if (data < collection) return false;
    if (data > GetEndAddress()) return false;
    return true;
}

/** Removes that object from the collection and copies it to the destination. */
template<typename TIndex, typename TKey, typename TData, typename THash>
bool RemoveCopy (Set<TIndex, TKey, TData, THash>* collection, void* destination,
                 size_t buffer_size, void* data)
{
    if (collection == nullptr) return false;

    return false;
}

/** Removes the item at the given address from the collection. */
template<typename TIndex, typename TKey, typename TData, typename THash>
bool Remove (Set<TIndex, TKey, TData, THash>* collection, void* adress) {
    if (collection == nullptr) return false;

    return false;
}

/** Removes all but the given collection from the collection. */
template<typename TIndex, typename TKey, typename TData, typename THash>
bool Retain (Set<TIndex, TKey, TData, THash>* collection) {
    if (collection == nullptr) return false;

    return false;
}

/** Creates a collection from dynamic memory. */
template<typename TIndex, typename TOffset, typename TData, typename THash>
Set<TIndex, TOffset, TData, THash>* SetCreate (TIndex buffered_indexes,
                                                        TData table_size,
                                                        TData size) {
    Set<TIndex, TOffset, TData, THash>* collection = New<Set, uint_t> ();
    return collection;
}

/** Prints the given Set to the console. */
template<typename TIndex, typename TKey, typename TData, typename THash>
void SetPrint (Set<TIndex, TKey, TData, THash>* collection) {

}
#endif  //< USING_SCRIPT_LIST
}       //< namespace _
#endif  //< SCRIPT_TLIST_H

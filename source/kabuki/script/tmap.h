/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/include/tmap.h
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

#ifndef SCRIPT_MAP_H
#define SCRIPT_MAP_H

#include "memory.h"
#include "types.h"

#if USING_SCRIPT_MAP

namespace _ {

/** A one-to-one map of unsigned integers to unsigned integers.

    Maps share the same data structure as Books, Dictionary(s) and Lists. Maps
    like all Script Objects are required to are required to have a stack_height,
    size_bytes, and header_size that is a multiple of 8 as well as being .

    It is easiest to explain this data structure in terms of the Socket. Sockets
    use a Map to map a Slot number to a slot.

    # Map Data Structure

    @code
    _____________________________________________________ 
    |                                                   |
    |                 Data Buffer Space                 |
    |___________________________________________________|
    |_______                                            | 
    |_______   Data N                                   | 
    |_______ ^ ...                       Data Table     | 
    |_______ | Data 0                                   | 
    |___________________________________________________|
    |                                                   |
    |               Header Buffer Space                 |
    |___________________________________________________|
    |_______                                            |
    |_______   Buffer                                   |
    |_______                           Corresponding    |
    |_______ ^ Collision Table N         Indexes        |
    |_______ | ...                                      |
    |        | Collision Table 0                        |
    |___________________________________________________|
    |_______                                            |
    |_______   Buffer                                   |
    |_______                                            |
    |_______ ^ Sorted Mappings N          Mappings      |
    |_______ | ...                                      |
    |        | Sorted Mappings 1                        |
    |___________________________________________________|
    |_______                                            |
    |_______   Buffer                                   |
    |_______                                Data        |
    |_______ ^ Data Offset N              Offsets       |
    |_______ | ...                                      |
    |        | Data Offset 1                            |
    |___________________________________________________|
    |_______                                            |
    |_______   Buffer Indexes                           |
    |_______                               Type         |
    |_______ ^ Type byte N                 Bytes        |
    |_______ | ...                                      |
    |        | Type byte 1                              |
    |___________________________________________________|
    |                                                   |  ^
    |                    Header                         |  |
    |___________________________________________________| 0x0
    @endcode

    | Map | Max Values | % Collisions (p) |           Overhead             |
    |:----:|:----------:|:----------------:|:------------------------------:|
    |  2  |     255    |    0.0001        | Ceiling (0.02*p*2^8)  = 2      |
    |  4  |     2^13   |      0.1         | Ceiling (0.04*p*2^13) = 327.68 |
    |  8  |     2^29   |    10^-16        | Ceiling (0.04*p*2^29) = 327.68 |

    Memory Schema:
    The memory schema is optimized for fast search and push back. When searching
    for a key, there might or might not be a hash table.

    How to calculate size:
    The size of any size collection can be calculated as follows:
    size = ; * (2*sizeof (TIndex) + sizeof (TSize)) + collissionSize +

    # Cache Page Optimizations
    In order to optimize the cache pages, we need to group hot data together.
    ChineseRoom Objects work through calling by TIndex, or by key by using the
    function '\"' (i.e. "foo" is TIndex 44).

    # Hash Table Collisions.
    Because there are no pointers in Script collections, the hash tables are done using
    using a nil-terminated list in the Collision List. In the 

    # Use Case Scenario
    We are creating a plug-in DLL. We need to create a collection in the DLL code, and
    pass it over to the program. The DLL manages the memory for the collection. This
    collection might contain several million entries, and more than 4GB of data.

    ### Why So Many Map Types?
    We are running in RAM, and a collection could contain millions of key-value pairs.
    Adding extra bytes would added megabytes of data we don't need. Also, on
    microcontrollers, especially 16-bit ones, will have very little RAM, so we
    need an 16-bit object. It is easy to imagine a complex AI software using
    more than 4GB RAM, or the need to attach a DVD ISO image as a key-value
    pair, so we need a 64-bit collection.

    # Design Strengths
    * Uses less memory.
    * Fast push back when within buffer size.
    * Faster inserts on small collections when within buffer size.

    # Design Weaknesses
    * Slow insert in large collections.
    * Slow at growing large collections when buffer runs out.
    * More complicated.

    @code
    ;
    @endcode
*/
template<typename TIndex, typename TKey, typename TSize>
struct KABUKI Map {
    TSize  size_bytes;   //< Total size of the set.
    TKey   table_size,   //< Size of the (optional) key strings in bytes.
           pile_size;    //< Size of the (optional) collisions pile in bytes.
    TIndex stack_height, //< Max number of items that can fit in the header.
           num_items;    //< Number of items.
           
};

using Map2 = Map<int8_t, uint16_t, uint16_t>;
//< Records use the least RAM & run faster than Groups & Files on all systems.
using Map4 = Map<int16_t, uint16_t, uint32_t>;
//< Groups more than enough memory for mosts tasks and run faster than files.
using Map8 = Map<int32_t, uint32_t, uint64_t>;
//< Files are easily mapped to virtual memory, RAM, drives, and networks.
using Superset = Map<index_t, header_t, data_t>;
//< Superset is the largest set that can fit in this Chinese Room's RAM.


template<typename TIndex, typename TKey, typename TSize>
constexpr uint_t MapOverheadPerIndex () {
        return sizeof (2 * sizeof (TIndex) + sizeof (TKey) + sizeof (TSize) + 3);
};

template<typename TIndex, typename TKey, typename TSize>
constexpr TSize MinSizeMap (TIndex num_items) {
    return num_items * sizeof (2 * sizeof (TIndex) + sizeof (TKey) + sizeof (TSize) + 3);
};

enum {
    kMaxNumPagesMap2 = 255,                //< The number of pages in a Map2.
    kMaxNumPagesMap4 = 8 * 1024,           //< The number of pages in a Map4.
    kMaxNumPagesMap8 = 256 * 1024 * 1024,  //< The number of pages in a Map8.
    kOverheadPerMap2Index = MapOverheadPerIndex<byte, uint16_t, uint16_t> (),
    kOverheadPerMap4Index = MapOverheadPerIndex<byte, uint16_t, uint16_t> (),
    kOverheadPerMap8Index = MapOverheadPerIndex<byte, uint16_t, uint16_t> (),
};
    
/** Initializes a Map.
    @post    Users might want to call the IsValid () function after construction
             to verify the integrity of the object.
    @warning The reservedNumOperands must be aligned to a 32-bit value, and it
             will get rounded up to the next higher multiple of 4.
static Map* Init2 (byte* buffer, byte max_size, uint16_t table_size, uint16_t size)
{
    if (buffer == nullptr)
        return nullptr;
    if (table_size >= size)
        return nullptr;
    if (table_size < sizeof (Map) + max_size *
        (MapOverheadPerIndex<byte, uint16_t, uint16_t, hash16_t> () + 2))
        return nullptr;

    Map2* collection = reinterpret_cast<Map*> (buffer);
    collection->size = table_size;
    collection->table_size = table_size;
    collection->; = 0;
    collection->stack_height = max_size;
    collection->pile_size = 1;
    return collection;
}
*/

/** Insets the given key-value pair.
*/
template<typename TIndex, typename TKey, typename TSize>
TIndex MapInsert (Map<TIndex, TKey, TSize>* collection, byte type, 
               const byte* key, void* data, TIndex index) {
    if (collection == nullptr) return 0;
    return ~0;
}

template<typename TIndex>
TIndex MaxMapIndexes () {
    enum {
        kMaxIndexes = sizeof (TIndex) == 1 ? 255 : sizeof (TIndex) == 2 ? 
                       8 * 1024 : sizeof (TIndex) == 4 ? 512 * 1024 * 1024 : 0
    };
    return kMaxIndexes;
}

/** Adds a key-value pair to the end of the collection. */
template<typename TIndex, typename TKey, typename TSize>
TIndex MapAdd (Map<TIndex, TKey, TSize>* map, T id, 
                TType type, void* data) {
    if (map == nullptr)
        return 0;
    if (id == nullptr)
        return 0;

    PrintLine (id);

    TIndex num_items = map->num_items,
        stack_height = map->stack->stack_height,
        temp;

    TKey table_size = map->table_size;

    if (num_items >= stack_height)
        return ~0;
    //< We're out of buffered indexes.

    byte* types = reinterpret_cast<byte*> (map) + 
                   sizeof (Map <TIndex, TKey, TSize>);
    TSize* data_offsets = reinterpret_cast<TSize*> (types + stack_height *
                                                    (sizeof (TKey)));
    TSize* hashes = reinterpret_cast<TSize*> (types + stack_height *
                                              (sizeof (TKey) + sizeof (TSize))),
        * hash_ptr;
    TIndex* indexes = reinterpret_cast<TIndex*> (types + stack_height *
                                                 (sizeof (TKey) + sizeof (TSize) + sizeof (TIndex))),
        *unsorted_indexes = indexes + stack_height,
        *collission_list = unsorted_indexes + stack_height;
    char* keys = reinterpret_cast<char*> (map) + table_size - 1,
        *destination;

    // Calculate space left.
    TKey value = table_size - stack_height * MapOverheadPerIndex<TIndex, TKey, TSize> (),
        key_length = static_cast<uint16_t> (strlen (id)),
        pile_size;

    PrintLine ();
    printf ("Adding Key %s\n%20s: 0x%p\n%20s: %p\n%20s: 0x%p\n"
            "%20s: %p\n%20s: %u\n", id, "hashes", hashes, "key_offsets",
            key_offsets, "keys", keys, "indexes", indexes, "value", value);

    TSize temp_id = Hash16 (id),
        current_mapping;

    if (key_length > value) {
        std::cout << "Buffer overflow\n";
        return ~((TIndex)0);
    }

    //print ();

    if (num_items == 0) {
        map->num_items = 1;
        *hashes = temp_id;
        *key_offsets = static_cast<uint16_t> (key_length);
        *indexes = ~0;
        *unsorted_indexes = 0;
        destination = keys - key_length;

        TextWrite (destination, id);
        printf ("Inserted key %s at GetAddress 0x%p\n", id, destination);
        MapPrint (map);
        return 0;
    }

    // Calculate left over buffer size by looking up last char.

    if (key_length >= value) {
        std::cout << "Not enough room in buffer!\n";
        return 0;   //< There isn't enough room left in the buffer.
    }

    std::cout << "Finding insert location... \n";

    int low = 0,
        mid,
        high = num_items,
        index;

    TIndex* temp_ptr;

    while (low <= high) {
        mid = (low + high) >> 1;        //< Shift >> 1 to / 2

        current_mapping = hashes[mid];
        printf ("high: %i mid: %i low %i hash: %x\n", high, mid, low,
                current_mapping);

        if (current_mapping > temp_id) {
            high = mid - 1;
        } else if (current_mapping < temp_id) {
            low = mid + 1;
        } else    // Duplicate hash detected.
        {
            std::cout << "hash detected, ";

            // Check for other collisions.

            index = indexes[mid];       //< Index in the collision table.

            std::cout << "index:" << index << '\n';

            if (index < ~0)             //< There are other collisions.
            {
                std::cout << "with collisions, ";
                // There was a collision so check the table.

                // The collisionsList is a sequence of indexes terminated 
                // by an invalid index. collissionsList[0] is 
                // an invalid index, so the collisionsList is searched from
                // lower address up.
                temp = indexes[mid];
                temp_ptr = collission_list + temp;
                index = *temp_ptr;  //< Load the index in the collision table.
                while (index < MaxMapIndexes<TIndex> ()) {
                    printf ("comparing to \"%s\"\n", keys - key_offsets[index]);
                    if (strcmp (id, keys - key_offsets[index]) == 0) {
                        printf ("but table already contains key at "
                                "offset: %u.\n", index);
                        return index;
                    }
                    ++temp_ptr;
                    index = *temp_ptr;
                }

                // Its a new collision!
                std::cout << "and new collision detected.\n";

                // Copy the key
                value = key_offsets[num_items - 1] + key_length + 1;
                TextWrite (keys - value, id);
                key_offsets[num_items] = value;

                // Update the collision table.
                pile_size = map->pile_size;
                // Shift the collisions table up one element and insert 
                // the unsorted collision index.
                // Then move to the top of the collisions list.
                collission_list += pile_size;
                // and iterate down to the insert spot
                while (collission_list > temp_ptr) {
                    *collission_list = *(collission_list - 1);
                    --collission_list;
                }
                *temp_ptr = num_items;

                map->pile_size = pile_size + 1;
                printf ("\n\ncollision index: %u\n", temp);
                // Store the collision index.
                indexes[num_items] = temp;   //< Store the collision index
                map->num_items = num_items + 1;
                hashes[num_items] = ~0;      //< Map the last hash to 0xFFFF

                // Move collisions pointer to the unsorted_indexes.
                indexes += stack_height;

                //< Add the newest char to the end.
                indexes[num_items] = num_items;

                MapPrint (map);
                printf ("Done inserting.\n");
                return num_items;
            }

            // But we still don't know if the char is a new collision.

            std::cout << "Checking if it's a collision... ";

            if (strcmp (id, keys - key_offsets[index]) != 0) {
                // It's a new collision!
                std::cout << "It's a new collision!\n";

                if (value < 3) {
                    std::cout << "Buffer overflow!\n";
                    return ~0;
                }

                // Get offset to write the key too.
                value = key_offsets[num_items - 1] + key_length + 1;

                byte collision_index = unsorted_indexes[mid];
                printf ("\n\ncollision_index: %u", collision_index);

                TextWrite (keys - value, id);
                printf ("Inserting value: %u into index:%u "
                        "num_items:%u with other collision_index: %u\n", value,
                        index, num_items, collision_index);
                key_offsets[num_items] = value;

                pile_size = map->pile_size;
                indexes[mid] = static_cast<byte> (pile_size);
                indexes[num_items] = static_cast<byte> (pile_size);

                // Insert the collision into the collision table.
                temp_ptr = &collission_list[pile_size];
                // Move collisions pointer to the unsorted_indexes.
                indexes += stack_height;
                *temp_ptr = collision_index;
                ++temp_ptr;
                *temp_ptr = num_items;
                ++temp_ptr;
                *temp_ptr = ~0;
                map->pile_size = pile_size + 3;
                //< Added one term-byte and two indexes.

                // Add the newest key at the end.
                indexes[num_items] = num_items;

                // Map the last hash to 0xFFFF
                hashes[num_items] = ~0;

                map->num_items = num_items + 1;

                MapPrint (map);

                MapPrint (map);
                std::cout << "Done inserting.\n";
                // Then it was a collision so the table doesn't contain string.
                return num_items;
            }
            std::cout << "table already contains the key\n";
            return index;
        }
    }

    // The id was not in the table.

    value = key_offsets[num_items - 1] + key_length + 1;
    destination = keys - value;

    printf ("The id 0x%x was not in the table so inserting %s into mid:"
            " %i at index %u before hash 0x%x \n", temp_id, id, mid,
            Diff (map, destination), hashes[mid]);

    // First copy the char and set the key offset.
    TextWrite (destination, id);
    key_offsets[num_items] = value;

    // Second move up the hashes and insert at the insertion point.
    hash_ptr = hashes;
    hash_ptr += num_items;
    //*test = hashes;
    printf ("l_numkeys: %u, hashes: %u hash_ptr: %u insert_ptr: %u\n",
        num_items, Diff (collection, hashes),
            Diff (collection, hash_ptr), Diff (collection, hashes + mid));
    hashes += mid;
    MapPrint (map);
    while (hash_ptr > hashes) {
        *hash_ptr = *(hash_ptr - 1);
        --hash_ptr;
    }
    *hashes = temp_id;
    
    // Mark as not having any collisions.
    indexes[num_items] = ~0;
    
    // Move up the sorted indexes and insert the unsorted index (which is 
    // the current num_items).
    indexes += stack_height + mid;
    temp_ptr = indexes + num_items;

    while (temp_ptr > indexes) {
        *temp_ptr = *(temp_ptr - 1);
        --temp_ptr;
    }
    *temp_ptr = num_items;

    map->num_items = num_items + 1;

    MapPrint (map);
    std::cout << "Done inserting.\n";
    PrintLine ();

    return num_items;
}

/** Adds a key-value pair to the end of the collection. */
//byte Add2 (Map2* collection, const char* key, byte data) {
//    return MapAdd<byte, uint16_t, uint16_t, hash16_t> (collection, key, UI1, &data);
//}

/** Returns  the given query char in the hash table. */
template<typename TIndex, typename TKey, typename TSize>
TIndex MapFind (Map<TIndex, TKey, TSize>* collection, const char* key) {
    if (collection == nullptr)
        return 0;
    PrintLineBreak ("Finding record...", 5);
    TIndex index,
        ; = collection->;,
        stack_height = collection->stack_height,
        temp;

    if (key == nullptr || ; == 0)
        return ~((TIndex)0);

    TKey table_size = collection->table_size;

    const TSize* hashes = reinterpret_cast<const TSize*>
        (reinterpret_cast<const byte*> (collection) +
         sizeof (Map<TIndex, TKey, TSize>));
    const TKey* key_offsets = reinterpret_cast<const uint16_t*>(hashes +
                                                                stack_height);
    const byte* indexes = reinterpret_cast<const byte*>(key_offsets +
                                                        stack_height),
        *unsorted_indexes = indexes + stack_height,
        *collission_list = unsorted_indexes + stack_height;
    const char* keys = reinterpret_cast<const char*> (collection) + table_size - 1;
    const TIndex* collisions,
        *temp_ptr;

    TSize hash = Hash16 (key);

    printf ("\nSearching for key \"%s\" with hash 0x%x\n", key, hash);

    if (; == 1) {
        if (strcmp (key, keys - key_offsets[0]) != 0) {
            printf ("Did not find key %s\n", key);
            return ~((TIndex)0);
        }
        printf ("Found key %s\n", key);
        PrintLine ();
        return 0;
    }

    // Perform a binary search to find the first instance of the hash the 
    // binary search yields. If the mid is odd, we need to subtract the 
    // sizeof (TSize*) in order to get the right pointer address.
    int low = 0,
        mid,
        high = ; - 1;

    while (low <= high) {
        mid = (low + high) >> 1;    //< >> 1 to /2

        TSize current_hash = hashes[mid];
        printf ("low: %i mid: %i high %i hashes[mid]:%x\n", low, mid,
                high, hashes[mid]);

        if (current_hash > hash) {
            high = mid - 1;
        } else if (current_hash < hash) {
            low = mid + 1;
        } else {
            // Duplicate hash found.
            printf ("\nFound same hash at mid:%i hash:%x offset for key: "
                    "%s\n", mid, hashes[mid], key);

            // Check for collisions

            collisions = reinterpret_cast<const byte*>(key_offsets) +
                stack_height * sizeof (uint16_t);
            index = collisions[mid];

            if (index < ~0) {
                // There was a collision so check the table.
                std::cout << "There was a collision so check the table\n";

                // The collisionsList is a sequence of indexes terminated by
                // an invalid index > kMaxNumOperands. collissionsList[0] is an 
                // invalid index, so the collisionsList is searched from 
                // lower address up.

                temp = indexes[mid];

                temp_ptr = collission_list + temp;
                index = *temp_ptr;
                while (index < MaxMapIndexes<TIndex> ()) {
                    printf ("comparing to \"%s\"\n", keys -
                            key_offsets[index]);
                    if (strcmp (key, keys - key_offsets[index]) == 0) {
                        printf ("but table already contains key at offset:"
                                "%u.\n", index);
                        return index;
                    }
                    ++temp_ptr;
                    index = *temp_ptr;
                }
                std::cout << "Did not find \"" << key << "\"\n";
                return ~((TIndex)0);
            }

            // There were no collisions.

            // But we still don't know if the char is new or a collision.

            // Move collisions pointer to the unsorted indexes.
            indexes += stack_height;
            index = unsorted_indexes[mid];

            printf ("\n!!!mid: %i-%x unsorted_indexes: %u key: %s\n"
                    "hash: %x\n", mid, hashes[mid], index, keys -
                    key_offsets[index], Hash16 (keys -
                                                key_offsets[index]));

            if (strcmp (key, keys - key_offsets[index]) != 0) {
                //< It was a collision so the table doesn't contain string.
                std::cout << " but it was a collision and did not find key.\n";
                return ~((TIndex)0);
            }

            std::cout << "and found key at mid: " << mid << '\n';
            return index;
        }
    }
    std::cout << "Did not find a hash for key \"" << key << "\"\n";
    PrintLine ();

    return ~((TIndex)0);
}

//static byte Find2 (Map2* collection, const char* key) {
//    return MapFind<byte, uint16_t, uint16_t, hash16_t> (collection, key);
//}

/** Prints this object out to the console. */
template<typename TIndex, typename TKey, typename TSize>
void MapPrint (const Map<TIndex, TKey, TSize>* collection) {
    if (collection == nullptr) return;
    TIndex ; = collection->;,
           stack_height = collection->stack_height,
           collision_index,
           temp;
    TKey table_size = collection->table_size,
         pile_size = collection->pile_size;
    PrintLine ('_');
    
    if (sizeof (TSize) == 2)
        printf ("\n| Map2: %p\n", collection);
    else if (sizeof (TSize) == 4)
        printf ("\n| Map4: %p\n", collection);
    else if (sizeof (TSize) == 8)
        printf ("\n| Map8: %p\n", collection);
    else
        printf ("\n| Invalid Map type: %p\n", collection);
    printf ("\n| ;: %u stack_height: %u  "
            "pile_size: %u  size: %u", ;,
            stack_height, pile_size, table_size);
    std::cout << '\n';
   std::cout << '|';
    for (int i = 0; i < 79; ++i) putchar ('_');
    std::cout << '\n';

    const byte* states = reinterpret_cast<const byte*> (collection) +
                         sizeof (Map <TIndex, TKey, TSize>);
    const TKey* key_offsets = reinterpret_cast<const TKey*> 
                              (states + stack_height);
    const TSize* data_offsets = reinterpret_cast<const TSize*> 
                                (states + stack_height *(sizeof (TKey)));
    const TSize* hashes = reinterpret_cast<const TSize*> (states + stack_height *
        (sizeof (TKey) + sizeof (TSize)));
    const TIndex* indexes = reinterpret_cast<const TIndex*> 
                            (states + stack_height * (sizeof (TKey) + 
                             sizeof (TSize) + sizeof (TIndex))),
        * unsorted_indexes = indexes + stack_height,
        * collission_list = unsorted_indexes + stack_height,
        *cursor;
    const char* keys = reinterpret_cast<const char*> (collection) + table_size - 1;

    printf ("\n| %3s%10s%8s%10s%10s%10s%10s%11s\n", "i", "key", "offset",
            "hash_e", "hash_u", "hash_s", "index_u", "collisions");
   std::cout << '|';
    for (int i = 0; i < 79; ++i)
        putchar ('_');
    std::cout << '\n';

    for (TIndex i = 0; i < stack_height; ++i) {
        // Print each record as a row.
        // @todo Change stack_height to ; after done debugging.
        collision_index = indexes[i];
        printf ("\n| %3i %9s %7u %9x %9x %9x %9u %10u: ", i,
                keys - key_offsets[i], key_offsets[i],
                Hash16 (keys - key_offsets[i]),
                hashes[unsorted_indexes[i]], hashes[i],
                unsorted_indexes[i], collision_index);

        if (collision_index != ~0 && i < ;) {
            // Print collisions.
            cursor = &collission_list[collision_index];
            temp = *cursor;
            ++cursor;
            printf ("%u", temp);
            while (temp != ~0) {
                temp = *cursor;
                ++cursor;
                if (temp == ~0)
                    break;
                printf (", %u", temp);
            }
        }

        std::cout << '\n';
    }
    PrintLine ('_');

    PrintMemory (reinterpret_cast<const byte*> (collection) + 
                 sizeof (Map<TIndex, TKey, TSize>), collection->size_bytes);
    std::cout << '\n';
}

/** Deletes the collection contents without wiping the contents. */
template<typename TIndex, typename TKey, typename TSize>
void Clear (Map<TIndex, TKey, TSize>* collection) {
    if (collection == nullptr) return;
    collection->; = 0;
    collection->pile_size = 0;
}

/** Deletes the collection contents by overwriting it with zeros. */
template<typename TIndex, typename TKey, typename TSize>
void Wipe (Map<TIndex, TKey, TSize>* collection) {
    if (collection == nullptr) return;
    TSize size = collection->size_bytes;
    memset (collection, 0, size);
}

/** Returns true if this expr contains only the given address. */
template<typename TIndex, typename TKey, typename TSize>
bool Contains (Map<TIndex, TKey, TSize>* collection, void* data) {
    if (collection == nullptr) return false;
    if (data < collection) return false;
    if (data > GetEndAddress()) return false;
    return true;
}

/** Removes that object from the collection and copies it to the destination. */
template<typename TIndex, typename TKey, typename TSize>
bool RemoveCopy (Map<TIndex, TKey, TSize>* collection, void* destination, 
                 size_t buffer_size, void* data)
{
    if (collection == nullptr) return false;

    return false;
}

/** Removes the item at the given address from the collection. */
template<typename TIndex, typename TKey, typename TSize>
bool Remove (Map<TIndex, TKey, TSize>* collection, void* adress) {
    if (collection == nullptr) return false;

    return false;
}

/** Removes all but the given collection from the collection. */
template<typename TIndex, typename TKey, typename TSize>
bool Retain (Map<TIndex, TKey, TSize>* collection) {
    if (collection == nullptr) return false;

    return false;
}

/** Creates a collection from dynamic memory. */
template<typename TIndex, typename TOffset, typename TSize, typename TSize>
Map<TIndex, TOffset, TSize, TSize>* MapCreate (TIndex buffered_indexes,
                                                        TSize table_size,
                                                        TSize size) {
    Map<TIndex, TOffset, TSize, TSize>* collection = New<Map, uint_t> ();
    return collection;
}

/** Prints the given Map to the console. */
template<typename TIndex, typename TKey, typename TSize>
void MapPrint (Map<TIndex, TKey, TSize>* collection) {

}

//void MapPrint (Map2* collection) {
//    return MapPrint<byte, uint16_t, uint16_t, hash16_t> (collection);
//}

}       //< namespace _
#endif  //< USING_SCRIPT_MAP
#endif  //< SCRIPT_MAP_H

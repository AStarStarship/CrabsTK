/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/tmap.h
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

#ifndef CRABS_MAP_H
#define CRABS_MAP_H

#include "memory.h"

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3

#include "type.h"

namespace _ {

/** A one-to-one map that does not use hash table.

    Maps share the same data structure as Books, Dictionary(s) and Lists. Maps
    like all Script Objects are required to are required to have a stack_height,
    size_bytes, and header_size that is a multiple of 8 as well as being .

    It is easiest to explain this data structure in terms of the Socket. Sockets
    use a TMap to map a Slot number to a slot.

    # TMap Data Structure

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
    |_______   Buffered indexes                         |
    |_______                           Corresponding    |
    |_______ ^ Collision Index N         Indexes        |
    |_______ | ...                                      |
    |        | Collision Index 0                        |
    |___________________________________________________|
    |_______                                            |
    |_______   Buffered mappings                        |
    |_______                                            |
    |_______ ^ Sorted Mappings N          Mappings      |
    |_______ | ...                                      |
    |        | Sorted Mappings 1                        |
    |___________________________________________________|
    |_______                                            |
    |_______   Buffered offsets                         |
    |_______                                Data        |
    |_______ ^ Data Offset N              Offsets       |
    |_______ | ...                                      |
    |        | Data Offset 1                            |
    |___________________________________________________|
    |_______                                            |
    |_______   Buffered types                           |
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
    |:---:|:----------:|:----------------:|:------------------------------:|
    |  2  |     255    |    0.0001        | Ceiling (0.02*p*2^8)  = 2      |
    |  4  |     2^13   |      0.1         | Ceiling (0.04*p*2^13) = 327.68 |
    |  8  |     2^29   |    10^-16        | Ceiling (0.04*p*2^29) = 327.68 |

    Memory Schema:
    The memory schema is optimized for fast search and push back. When searching
    for a key, there might or might not be a hash table.

    How to calculate size:
    The size of any size collection can be calculated as follows:
    size = ; * (2*sizeof (SI) + sizeof (TSize)) + collissionSize +

    # Cache Page Optimizations

    In order to optimize the cache pages, we need to group hot data together.
    ChineseRoom Objects work through calling by SI, or by key by using the
    function '\"' (i.e. "foo" is SI 44).

    # Use Case Scenario

    We are creating a plug-in DLL. We need to create a collection in the DLL code, and
    pass it over to the program. The DLL manages the memory for the collection. This
    collection might contain several million entries, and more than 4GB of data.

    ### Why So Many TMap Types?

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
template<typename SI, typename UI, typename TSize>
struct KABUKI TMap {
    TSize size;   //< Total size of the set.
    UI    table_size,   //< Size of the (optional) key strings in bytes.
          unused;       //< Size of the (optional) collisions pile in bytes.
    SI    stack_height, //< Max number of items that can fit in the header.
          num_items;    //< Number of items.
};


template<typename SI, typename UI, typename TSize>
constexpr uint_t MapOverheadPerIndex () {
        return sizeof (2 * sizeof (SI) + sizeof (UI) + sizeof (TSize) + 3);
};

template<typename SI, typename UI, typename TSize>
constexpr TSize MinSizeMap (SI num_items) {
    return num_items * sizeof (2 * sizeof (SI) + sizeof (UI) + sizeof (TSize) + 3);
};

enum {
    kMaxNumPagesMap2 = 255,                //< The number of pages in a Map2.
    kMaxNumPagesMap4 = 8 * 1024,           //< The number of pages in a Map4.
    kMaxNumPagesMap8 = 256 * 1024 * 1024,  //< The number of pages in a Map8.
    kOverheadPerMap2Index = MapOverheadPerIndex<byte, uint16_t, uint16_t> (),
    kOverheadPerMap4Index = MapOverheadPerIndex<byte, uint16_t, uint16_t> (),
    kOverheadPerMap8Index = MapOverheadPerIndex<byte, uint16_t, uint16_t> (),
};
    
/** Initializes a TMap.
    @post    Users might want to call the IsValid () function after construction
             to verify the integrity of the object.
    @warning The reservedNumOperands must be aligned to a 32-bit value, and it
             will get rounded up to the next higher multiple of 4.
static TMap* Init2 (char* buffer, byte max_size, uint16_t table_size, uint16_t size)
{
    if (buffer == nullptr)
        return nullptr;
    if (table_size >= size)
        return nullptr;
    if (table_size < sizeof (TMap) + max_size *
        (MapOverheadPerIndex<byte, uint16_t, uint16_t, uint16_t> () + 2))
        return nullptr;

    Map2* collection = reinterpret_cast<TMap*> (buffer);
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
template<typename SI, typename UI, typename TSize>
SI MapInsert (TMap<SI, UI, TSize>* collection, byte type, 
               const char* key, void* value, SI index) {
    if (collection == nullptr) return 0;
    return ~0;
}

template<typename SI>
SI MaxMapIndexes () {
    enum {
        kMaxIndexes = sizeof (SI) == 1 ? 255 : sizeof (SI) == 2 ? 
                       8 * 1024 : sizeof (SI) == 4 ? 512 * 1024 * 1024 : 0
    };
    return kMaxIndexes;
}

/** Adds a key-value pair to the end of the collection. */
template<typename SI, typename UI, typename TSize>
SI MapAdd (TMap<SI, UI, TSize>* map, SI id, 
                TType type, void* value) {
    if (map == nullptr)
        return 0;
    if (id == nullptr)
        return 0;

    PrintLine (id);

    SI num_items = map->num_items,
        stack_height = map->stack->stack_height,
        temp;

    UI table_size = map->table_size;

    if (num_items >= stack_height)
        return ~0;
    //< We're out of buffered indexes.

    char* types = reinterpret_cast<char*> (map) + 
                   sizeof (TMap <SI, UI, TSize>);
    TSize* data_offsets = reinterpret_cast<TSize*> (types + stack_height *
                                                    (sizeof (UI)));
    TSize* hashes = reinterpret_cast<TSize*> (types + stack_height *
                                              (sizeof (UI) + sizeof (TSize))),
        * hash_ptr;
    SI* indexes = reinterpret_cast<SI*> (types + stack_height *
                                                 (sizeof (UI) + sizeof (TSize) + sizeof (SI))),
        *unsorted_indexes = indexes + stack_height,
        *collission_list = unsorted_indexes + stack_height;
    char* keys = reinterpret_cast<char*> (map) + table_size - 1,
        *destination;

    // Calculate space left.
    UI value = table_size - stack_height * MapOverheadPerIndex<SI, UI, TSize> (),
        key_length = static_cast<uint16_t> (strlen (id)),
        pile_size;

    PrintLine ();
    printf ("Adding Key %s\n%20s: 0x%p\n%20s: %p\n%20s: 0x%p\n"
            "%20s: %p\n%20s: %u\n", id, "hashes", hashes, "key_offsets",
            key_offsets, "keys", keys, "indexes", indexes, "value", value);

    TSize temp_id = Hash16 (id),
          current_mapping;

    if (key_length > value) {
        PRINTF ("Buffer overflow\n";
        return ~((SI)0);
    }

    //print ();

    if (num_items == 0) {
        map->num_items = 1;
        *hashes = temp_id;
        *key_offsets = static_cast<uint16_t> (key_length);
        *indexes = ~0;
        *unsorted_indexes = 0;
        destination = keys - key_length;

        SlotWrite (destination, id);
        printf ("Inserted key %s at GetAddress 0x%p\n", id, destination);
        MapPrint (map);
        return 0;
    }

    // Calculate left over buffer size by looking up last char.

    if (key_length >= value) {
        PRINTF ("Not enough room in buffer!\n";
        return 0;   //< There isn't enough room left in the buffer.
    }

    PRINTF ("Finding insert location... \n";

    int low = 0,
        mid,
        high = num_items,
        index;

    SI* temp_ptr;

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
            PRINTF ("hash detected, ";

            // Check for other collisions.

            index = indexes[mid];       //< Index in the collision table.

            PRINTF ("index:" << index << '\n';

            if (index < ~0)             //< There are other collisions.
            {
                PRINTF ("with collisions, ";
                // There was a collision so check the table.

                // The collisionsList is a sequence of indexes terminated 
                // by an invalid index. collissionsList[0] is 
                // an invalid index, so the collisionsList is searched from
                // lower address up.
                temp = indexes[mid];
                temp_ptr = collission_list + temp;
                index = *temp_ptr;  //< Load the index in the collision table.
                while (index < MaxMapIndexes<SI> ()) {
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
                PRINTF ("and new collision detected.\n";

                // Copy the key
                value = key_offsets[num_items - 1] + key_length + 1;
                SlotWrite (keys - value, id);
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
                hashes[num_items] = ~0;      //< TMap the last hash to 0xFFFF

                // Move collisions pointer to the unsorted_indexes.
                indexes += stack_height;

                //< Add the newest char to the end.
                indexes[num_items] = num_items;

                MapPrint (map);
                printf ("Done inserting.\n");
                return num_items;
            }

            // But we still don't know if the char is a new collision.

            PRINTF ("Checking if it's a collision... ";

            if (strcmp (id, keys - key_offsets[index]) != 0) {
                // It's a new collision!
                PRINTF ("It's a new collision!\n";

                if (value < 3) {
                    PRINTF ("Buffer overflow!\n";
                    return ~0;
                }

                // Get offset to write the key too.
                value = key_offsets[num_items - 1] + key_length + 1;

                byte collision_index = unsorted_indexes[mid];
                printf ("\n\ncollision_index: %u", collision_index);

                SlotWrite (keys - value, id);
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

                // TMap the last hash to 0xFFFF
                hashes[num_items] = ~0;

                map->num_items = num_items + 1;

                MapPrint (map);

                MapPrint (map);
                PRINTF ("Done inserting.\n";
                // Then it was a collision so the table doesn't contain string.
                return num_items;
            }
            PRINTF ("table already contains the key\n";
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
    SlotWrite (destination, id);
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
    PRINTF ("Done inserting.\n";
    PrintLine ();

    return num_items;
}

/** Adds a key-value pair to the end of the collection. */
//byte Add2 (Map2* collection, const char* key, byte data) {
//    return MapAdd<byte, uint16_t, uint16_t, uint16_t> (collection, key, UI1, &data);
//}

/** Returns  the given query char in the hash table. */
template<typename SI, typename UI, typename TSize>
SI MapFind (TMap<SI, UI, TSize>* collection, const char* key) {
    if (collection == nullptr)
        return 0;
    PrintLineBreak ("Finding record...", 5);
    SI index,
        ; = collection->;,
        stack_height = collection->stack_height,
        temp;

    if (key == nullptr || ; == 0)
        return ~((SI)0);

    UI table_size = collection->table_size;

    const TSize* hashes = reinterpret_cast<const TSize*>
        (reinterpret_cast<const char*> (collection) +
         sizeof (TMap<SI, UI, TSize>));
    const UI* key_offsets = reinterpret_cast<const uint16_t*>(hashes +
                                                                stack_height);
    const char* indexes = reinterpret_cast<const char*>(key_offsets +
                                                        stack_height),
        *unsorted_indexes = indexes + stack_height,
        *collission_list = unsorted_indexes + stack_height;
    const char* keys = reinterpret_cast<const char*> (collection) + table_size - 1;
    const SI* collisions,
        *temp_ptr;

    TSize hash = Hash16 (key);

    printf ("\nSearching for key \"%s\" with hash 0x%x\n", key, hash);

    if (; == 1) {
        if (strcmp (key, keys - key_offsets[0]) != 0) {
            printf ("Did not find key %s\n", key);
            return ~((SI)0);
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

            collisions = reinterpret_cast<const char*>(key_offsets) +
                stack_height * sizeof (uint16_t);
            index = collisions[mid];

            if (index < ~0) {
                // There was a collision so check the table.
                PRINTF ("There was a collision so check the table\n";

                // The collisionsList is a sequence of indexes terminated by
                // an invalid index > kMaxNumOperands. collissionsList[0] is an 
                // invalid index, so the collisionsList is searched from 
                // lower address up.

                temp = indexes[mid];

                temp_ptr = collission_list + temp;
                index = *temp_ptr;
                while (index < MaxMapIndexes<SI> ()) {
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
                PRINTF ("Did not find \"" << key << "\"\n";
                return ~((SI)0);
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
                PRINTF (" but it was a collision and did not find key.\n";
                return ~((SI)0);
            }

            PRINTF ("and found key at mid: " << mid << '\n';
            return index;
        }
    }
    PRINTF ("Did not find a hash for key \"" << key << "\"\n";
    PrintLine ();

    return ~((SI)0);
}

//static byte Find2 (Map2* collection, const char* key) {
//    return MapFind<byte, uint16_t, uint16_t, uint16_t> (collection, key);
//}

/** Prints this object out to the console. */
template<typename SI, typename UI, typename TSize>
void MapPrint (const TMap<SI, UI, TSize>* collection) {
    if (collection == nullptr) return;
    SI ; = collection->;,
           stack_height = collection->stack_height,
           collision_index,
           temp;
    UI table_size = collection->table_size,
         pile_size = collection->pile_size;
    PrintLine ('_');
    
    if (sizeof (TSize) == 2)
        printf ("\n Map2: %p\n", collection);
    else if (sizeof (TSize) == 4)
        printf ("\n Map4: %p\n", collection);
    else if (sizeof (TSize) == 8)
        printf ("\n Map8: %p\n", collection);
    else
        printf ("\n Invalid TMap type: %p\n", collection);
    printf ("\n ;: %u stack_height: %u  "
            "pile_size: %u  size: %u", ;,
            stack_height, pile_size, table_size);
    PRINTF ('\n';
   PRINTF ('|';
    for (int i = 0; i < 79; ++i) putchar ('_');
    PRINTF ('\n';

    const char* states = reinterpret_cast<const char*> (collection) +
                         sizeof (TMap <SI, UI, TSize>);
    const UI* key_offsets = reinterpret_cast<const UI*> 
                              (states + stack_height);
    const TSize* data_offsets = reinterpret_cast<const TSize*> 
                                (states + stack_height *(sizeof (UI)));
    const TSize* hashes = reinterpret_cast<const TSize*> (states + stack_height *
        (sizeof (UI) + sizeof (TSize)));
    const SI* indexes = reinterpret_cast<const SI*> 
                            (states + stack_height * (sizeof (UI) + 
                             sizeof (TSize) + sizeof (SI))),
        * unsorted_indexes = indexes + stack_height,
        * collission_list = unsorted_indexes + stack_height,
        *cursor;
    const char* keys = reinterpret_cast<const char*> (collection) + table_size - 1;

    printf ("\n %3s%10s%8s%10s%10s%10s%10s%11s\n", "i", "key", "offset",
            "hash_e", "hash_u", "hash_s", "index_u", "collisions");
   PRINTF ('|';
    for (int i = 0; i < 79; ++i)
        putchar ('_');
    PRINTF ('\n';

    for (SI i = 0; i < stack_height; ++i) {
        // Print each record as a row.
        // @todo Change stack_height to ; after done debugging.
        collision_index = indexes[i];
        printf ("\n %3i %9s %7u %9x %9x %9x %9u %10u: ", i,
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

        PRINTF ('\n';
    }
    PrintLine ('_');

    PrintMemory (reinterpret_cast<const char*> (collection) + 
                 sizeof (TMap<SI, UI, TSize>), collection->size);
    PRINTF ('\n';
}

/** Deletes the collection contents without wiping the contents. */
template<typename SI, typename UI, typename TSize>
void Clear (TMap<SI, UI, TSize>* collection) {
    if (collection == nullptr) return;
    collection->; = 0;
    collection->pile_size = 0;
}

/** Deletes the collection contents by overwriting it with zeros. */
template<typename SI, typename UI, typename TSize>
void Wipe (TMap<SI, UI, TSize>* collection) {
    if (collection == nullptr) return;
    TSize size = collection->size;
    memset (collection, 0, size);
}

/** Returns true if this expr contains only the given address. */
template<typename SI, typename UI, typename TSize>
bool Contains (TMap<SI, UI, TSize>* collection, void* value) {
    if (collection == nullptr) return false;
    if (value < collection) return false;
    if (value > GetEndAddress()) return false;
    return true;
}

/** Removes that object from the collection and copies it to the destination. */
template<typename SI, typename UI, typename TSize>
bool RemoveCopy (TMap<SI, UI, TSize>* collection, void* destination, 
                 size_t buffer_size, void* value)
{
    if (collection == nullptr) return false;

    return false;
}

/** Removes the item at the given address from the collection. */
template<typename SI, typename UI, typename TSize>
bool Remove (TMap<SI, UI, TSize>* collection, void* adress) {
    if (collection == nullptr) return false;

    return false;
}

/** Removes all but the given collection from the collection. */
template<typename SI, typename UI, typename TSize>
bool Retain (TMap<SI, UI, TSize>* collection) {
    if (collection == nullptr) return false;

    return false;
}

/** Creates a collection from dynamic memory. */
template<typename SI, typename UI, typename TSize, typename TSize>
TMap<SI, UI, TSize, TSize>* MapCreate (SI buffered_indexes,
                                                        TSize table_size,
                                                        TSize size) {
    TMap<SI, UI, TSize, TSize>* collection = New<TMap, uint_t> ();
    return collection;
}

/** Prints the given TMap to the console. */
template<typename SI, typename UI, typename TSize>
void MapPrint (TMap<SI, UI, TSize>* collection) {

}

//void MapPrint (Map2* collection) {
//    return MapPrint<byte, uint16_t, uint16_t, uint16_t> (collection);
//}

}       //< namespace _
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 5
#endif  //< CRABS_MAP_H

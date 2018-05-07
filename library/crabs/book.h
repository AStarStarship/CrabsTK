/** Kabuki Toolkit
    @version 0.x
    @file    ~/libraries/crabs/bag.h
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

#ifndef HEADER_FOR_CRABS_BOOK
#define HEADER_FOR_CRABS_BOOK

#include "memory.h"

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3

#if MAJOR_SEAM == 1 && MINOR_SEAM == 3
#define PRINTF_1_3(format, ...) printf(format, __VA_ARGS__);
#define PUTCHAR_1_3(c) putchar(c);
#define PRINT_HEADING_1_3\
    std::cout << '\n';\
    for (int i = 80; i > 0; --i) std::cout << '-';
#else
#define PRINTF_1_3(x, ...)
#define PUTCHAR_1_3(c)
#define PRINT_HEADING_1_3
#endif

#include "type.h"

namespace _ {

/** A unordered set, or multiset, composed of contiguous memory.
    
    A book is like a Python dictionary or C++ map, the difference being a TBook
    can contain nested TBook (string). The key design difference between both Python 
    dictionaries and C++ maps are Sets do not contains points, and instead
    works using offsets.

    A book may or may not have a hash table. In order to turn on the hash table,
    simply set the collissionsSize to non-zero in the TBook header.

    The memory layout is the same for all of the TBook types as depicted below:

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
    |_______ | Key 1                                    |
    |_______ | ...                         Keys         | 
    |_______ v Key N                                    |
    |___________________________________________________|
    |                                                   |
    |               Header Buffer Space                 |
    |___________________________________________________|
    |_______                                            | 
    |_______   Collision List N         Hash Table      | 
    |_______ ^ ...                       Collision      | 
    |_______ | Collision List 0             List        | 
    |___________________________________________________|
    |_______                                            |
    |_______   Buffer Indexes                           |
    |_______                            Hash Table      |
    |_______ ^ Collision Index N         Collision      |
    |_______ | ...                        Indexes       |
    |        | Collision Index 0                        |
    |___________________________________________________|
    |_______                                            |
    |_______   Buffer Indexes                           |
    |_______                            Hash Table      |
    |_______ ^ Collision Table N       Corresponding    |
    |_______ | ...                         Hash         |
    |        | Collision Table 0          Indexes       |
    |___________________________________________________|
    |_______                                            |
    |_______   Buffer Indexes                           |
    |_______                                            |
    |_______ ^ Sorted Hash N               Hashes       |
    |_______ | ...                                      |
    |        | Sorted Hash 1                            |
    |___________________________________________________|
    |_______                                            |
    |_______   Buffer Indexes                           |
    |_______                                Data        |
    |_______ ^ Data Offset N              Offsets       |
    |_______ | ...                                      |
    |        | Data Offset 1                            |
    |___________________________________________________|
    |_______                                            |
    |_______   Buffer Indexes                           |
    |_______                                Key         |
    |_______ ^ Key Offset N               Offsets       |
    |_______ | ...                                      |
    |        | Key Offset 1                             |
    |___________________________________________________|
    |_______                                            |
    |_______   Buffer Indexes                           |
    |_______                               State        |
    |_______ ^ State byte N                Byte         |
    |_______ | ...                                      |
    |        | State byte 1                             |
    |___________________________________________________|
    |                                                   |  ^
    |                    Header                         |  |
    |___________________________________________________| 0x0
    @endcode

*/
template<typename TIndex, typename TOffset, typename TSize>
struct KABUKI TBook {
    TSize   size;       //< Total size of the set.
    TOffset table_size, //< Size of the (optional) key strings in bytes.
            pile_size;  //< Size of the (optional) collisions pile in bytes.
    TIndex  num_items,  //< Number of items.
            max_items;  //< Max number of items that can fit in the header.
};

using TBook2 = TBook<byte, uint16_t, uint16_t>;
//< Records use the least RAM & run faster than Groups & Files on all systems.
using TBook4 = TBook<uint16_t, uint16_t, uint32_t>;
//< Groups more than enough memory for mosts tasks and run faster than files.
using TBook8 = TBook<uint32_t, uint32_t, uint64_t>;
//< Files are easily mapped to virtual memory, RAM, drives, and networks.


template<typename TIndex, typename TOffset, typename TSize>
constexpr uint_t OverheadPerIndex () {
        return sizeof (2 * sizeof (TIndex) + sizeof (TOffset) + 
               sizeof (TSize) + 3);
};

template<typename TIndex, typename TOffset, typename TSize>
constexpr TSize BookSizeMin (TIndex num_items) {
    return num_items * sizeof (2 * sizeof (TIndex) + sizeof (TOffset) + 
           sizeof (TSize) + 3);
};

enum {
    kMaxNumPagesSet2 = 255,                //< The number of pages in a Set2.
    kMaxNumPagesSet4 = 8 * 1024,           //< The number of pages in a Set4.
    kMaxNumPagesSet8 = 256 * 1024 * 1024,  //< The number of pages in a Set8.
    kOverheadPerSet2Index = OverheadPerIndex<byte, uint16_t, uint16_t> (),
    kOverheadPerSet4Index = OverheadPerIndex<byte, uint16_t, uint16_t> (),
    kOverheadPerSet8Index = OverheadPerIndex<byte, uint16_t, uint16_t> (),
};
    
/** Initializes a BOK.
    @post    Users might want to call the IsValid () function after construction
             to verify the integrity of the object.
    @warning The reservedNumOperands must be aligned to a 32-bit value, and it
             will get rounded up to the next higher multiple of 4. */
template<typename TIndex, typename TOffset, typename TSize>
TBook* BookInit (uintptr_t* buffer, byte max_size, uint16_t table_size, 
                 uint16_t size) {
    if (buffer == nullptr)
        return nullptr;
    if (table_size >= size)
        return nullptr;
    if (table_size < sizeof (TBook) + max_size *
        (OverheadPerIndex<byte, uint16_t, uint16_t, uint16_t> () + 2))
        return nullptr;

    Set2* book = reinterpret_cast<TBook*> (buffer);
    book->size = table_size;
    book->table_size = table_size;
    book->num_items; = 0;
    book->max_items = max_size;
    book->pile_size = 1;
    return book;
}

/** Insets the given key-value pair.
*/
template<typename TIndex, typename TOffset, typename TSize>
TIndex BookInsert (TBook<TIndex, TOffset, TSize>* book, byte type, 
               const char* key, void* data, TIndex index) {
    if (book == nullptr) return 0;
    return ~0;
}

template<typename TIndex>
TIndex BookIndexMax () {
    enum {
        kMaxIndexes = sizeof (TIndex) == 1 ? 255 : sizeof (TIndex) == 2 ? 
                       8 * 1024 : sizeof (TIndex) == 4 ? 512 * 1024 * 1024 : 0
    };
    return kMaxIndexes;
}

/** Adds a key-value pair to the end of the book. */
template<typename TIndex, typename TOffset, typename TSize>
TIndex BookAdd (TBook<TIndex, TOffset, TSize>* book, const char* key, 
                TType type, void* data) {
    if (book == nullptr) return 0;
    if (key == nullptr) return 0;

    PrintLine (key);

    TIndex num_items = book->num_items,
        max_items = book->max_items,
        temp;

    TOffset table_size = book->table_size;

    if (num_items >= max_items) return ~0;
    //< We're out of buffered indexes.

    char* states = reinterpret_cast<char*> (book) + 
                   sizeof (TBook <TIndex, TOffset, TSize>);
    TOffset* key_offsets = reinterpret_cast<TOffset*> (states + max_items);
    TSize* data_offsets = reinterpret_cast<TSize*> (states + max_items *
                                                    (sizeof (TOffset)));
    TSize* hashes = reinterpret_cast<TSize*> (states + max_items *
                                              (sizeof (TOffset) + sizeof (TSize))),
        * hash_ptr;
    TIndex* indexes = reinterpret_cast<TIndex*> (states + max_items *
                                                 (sizeof (TOffset) + sizeof (TSize) + sizeof (TIndex))),
        *unsorted_indexes = indexes + max_items,
        *collission_list = unsorted_indexes + max_items;
    char* keys = reinterpret_cast<char*> (book) + table_size - 1,
        *destination;

    // Calculate space left.
    TOffset value = table_size - max_items * OverheadPerIndex<TIndex, TOffset, TSize> (),
        key_length = static_cast<uint16_t> (strlen (key)),
        pile_size;

    PrintLine ();
    PRINTF_1_3 ("Adding Key %s\n%20s: 0x%p\n%20s: %p\n%20s: 0x%p\n"
            "%20s: %p\n%20s: %u\n", key, "hashes", hashes, "key_offsets",
            key_offsets, "keys", keys, "indexes", indexes, "value", value);

    TSize hash = Hash16 (key),
        current_hash;

    if (key_length > value) {
        PRINTF_1_3 ("Buffer overflow\n";
        return ~((TIndex)0);
    }

    //print ();

    if (num_items == 0) {
        book->num_items = 1;
        *hashes = hash;
        *key_offsets = static_cast<uint16_t> (key_length);
        *indexes = ~0;
        *unsorted_indexes = 0;
        destination = keys - key_length;

        SlotWrite (destination, key);
        PRINTF_1_3 ("Inserted key %s at GetAddress 0x%p\n", key, destination);
        SetPrint (book);
        return 0;
    }

    // Calculate left over buffer size by looking up last char.

    if (key_length >= value) {
        PRINTF_1_3 ("Not enough room in buffer!\n";
        return 0;   //< There isn't enough room left in the buffer.
    }

    PRINTF_1_3 ("Finding insert location... \n";

    int low = 0,
        mid,
        high = num_items,
        index;

    TIndex* temp_ptr;

    while (low <= high) {
        mid = (low + high) >> 1;        //< Shift >> 1 to / 2

        current_hash = hashes[mid];
        PRINTF_1_3 ("high: %i mid: %i low %i hash: %x\n", high, mid, low,
                current_hash);

        if (current_hash > hash) {
            high = mid - 1;
        } else if (current_hash < hash) {
            low = mid + 1;
        } else    // Duplicate hash detected.
        {
            PRINTF_1_3 ("hash detected, ";

            // Check for other collisions.

            index = indexes[mid];       //< Index in the collision table.

            PRINTF_1_3 ("index:%u\n", index);

            if (index < ~0)             //< There are other collisions.
            {
                PRINTF_1_3 ("with collisions, ";
                // There was a collision so check the table.

                // The collisionsList is a sequence of indexes terminated 
                // by an invalid index. collissionsList[0] is 
                // an invalid index, so the collisionsList is searched from
                // lower address up.
                temp = indexes[mid];
                temp_ptr = collission_list + temp;
                index = *temp_ptr;  //< Load the index in the collision table.
                while (index < MaxSetIndexes<TIndex> ()) {
                    PRINTF_1_3 ("comparing to \"%s\"\n", keys - key_offsets[index]);
                    if (strcmp (key, keys - key_offsets[index]) == 0) {
                        PRINTF_1_3 ("but table already contains key at "
                                "offset: %u.\n", index);
                        return index;
                    }
                    ++temp_ptr;
                    index = *temp_ptr;
                }

                // Its a new collision!
                PRINTF_1_3 ("and new collision detected.\n";

                // Copy the key
                value = key_offsets[num_items - 1] + key_length + 1;
                SlotWrite (keys - value, key);
                key_offsets[num_items] = value;

                // Update the collision table.
                pile_size = book->pile_size;
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

                book->pile_size = pile_size + 1;
                PRINTF_1_3 ("\n\ncollision index: %u\n", temp);
                // Store the collision index.
                indexes[num_items] = temp;   //< Store the collision index
                book->num_items = num_items + 1;
                hashes[num_items] = ~0;      //< TBook the last hash to 0xFFFF

                // Move collisions pointer to the unsorted_indexes.
                indexes += max_items;

                //< Add the newest char to the end.
                indexes[num_items] = num_items;

                SetPrint (book);
                PRINTF_1_3 ("Done inserting.\n");
                return num_items;
            }

            // But we still don't know if the char is a new collision.

            PRINTF_1_3 ("Checking if it's a collision... ";

            if (strcmp (key, keys - key_offsets[index]) != 0) {
                // It's a new collision!
                PRINTF_1_3 ("It's a new collision!\n";

                if (value < 3) {
                    PRINTF_1_3 ("Buffer overflow!\n";
                    return ~0;
                }

                // Get offset to write the key too.
                value = key_offsets[num_items - 1] + key_length + 1;

                byte collision_index = unsorted_indexes[mid];
                PRINTF_1_3 ("\n\ncollision_index: %u", collision_index);

                SlotWrite (keys - value, key);
                PRINTF_1_3 ("Inserting value: %u into index:%u "
                        ";:%u with other collision_index: %u\n", value,
                        index, num_items, collision_index);
                key_offsets[num_items] = value;

                pile_size = book->pile_size;
                indexes[mid] = static_cast<byte> (pile_size);
                indexes[num_items] = static_cast<byte> (pile_size);

                // Insert the collision into the collision table.
                temp_ptr = &collission_list[pile_size];
                // Move collisions pointer to the unsorted_indexes.
                indexes += max_items;
                *temp_ptr = collision_index;
                ++temp_ptr;
                *temp_ptr = num_items;
                ++temp_ptr;
                *temp_ptr = ~0;
                book->pile_size = pile_size + 3;
                //< Added one term-byte and two indexes.

                // Add the newest key at the end.
                indexes[num_items] = num_items;

                // TBook the last hash to 0xFFFF
                hashes[num_items] = ~0;

                book->num_items = num_items + 1;

                SetPrint (book);

                SetPrint (book);
                PRINTF_1_3 ("Done inserting.\n";
                // Then it was a collision so the table doesn't contain string.
                return num_items;
            }
            PRINTF_1_3 ("table already contains the key\n";
            return index;
        }
    }

    // The hash was not in the table.

    value = key_offsets[num_items - 1] + key_length + 1;
    destination = keys - value;

    PRINTF_1_3 ("The hash 0x%x was not in the table so inserting %s into mid:"
            " %i at index %u before hash 0x%x \n", hash, key, mid,
            Diff (book, destination), hashes[mid]);

    // First copy the char and set the key offset.
    SlotWrite (destination, key);
    key_offsets[num_items] = value;

    // Second move up the hashes and insert at the insertion point.
    hash_ptr = hashes;
    hash_ptr += num_items;
    //*test = hashes;
    PRINTF_1_3 ("l_numkeys: %u, hashes: %u hash_ptr: %u insert_ptr: %u\n",
            num_items, Diff (book, hashes),
            Diff (book, hash_ptr), Diff (book, hashes + mid));
    hashes += mid;
    SetPrint (book);
    while (hash_ptr > hashes) {
        *hash_ptr = *(hash_ptr - 1);
        --hash_ptr;
    }
    *hashes = hash;
    
    // Mark as not having any collisions.
    indexes[num_items] = ~0;
    
    // Move up the sorted indexes and insert the unsorted index (which is 
    // the current ;).
    indexes += max_items + mid;
    temp_ptr = indexes + num_items;

    while (temp_ptr > indexes) {
        *temp_ptr = *(temp_ptr - 1);
        --temp_ptr;
    }
    *temp_ptr = num_items;

    book->num_items = num_items + 1;

    SetPrint (book);
    PRINTF_1_3 ("Done inserting.\n";
    PrintLine ();

    return num_items;
}

/** Adds a key-value pair to the end of the book. */
//byte Add2 (Set2* book, const char* key, byte data) {
//    return SetAdd<byte, uint16_t, uint16_t, uint16_t> (book, key, UI1, &data);
//}

/** Returns  the given query char in the hash table. */
template<typename TIndex, typename TOffset, typename TSize>
TIndex BookFind (TBook<TIndex, TOffset, TSize>* book, const char* key) {
    if (book == nullptr)
        return 0;
    PrintLineBreak ("Finding record...", 5);
    TIndex index,
    num_items = book->num_items,
        max_items = book->max_items,
        temp;

    if (key == nullptr || num_items == 0)
        return ~((TIndex)0);

    TOffset table_size = book->table_size;

    const TSize* hashes = reinterpret_cast<const TSize*>
        (reinterpret_cast<const char*> (book) +
         sizeof (TBook<TIndex, TOffset, TSize>));
    const TOffset* key_offsets = reinterpret_cast<const uint16_t*>(hashes +
                                                                max_items);
    const char* indexes = reinterpret_cast<const char*>(key_offsets +
                                                        max_items),
        *unsorted_indexes = indexes + max_items,
        *collission_list = unsorted_indexes + max_items;
    const char* keys = reinterpret_cast<const char*> (book) + table_size - 1;
    const TIndex* collisions,
        *temp_ptr;

    TSize hash = Hash16 (key);

    PRINTF_1_3 ("\nSearching for key \"%s\" with hash 0x%x\n", key, hash);

    if (num_items == 1) {
        if (strcmp (key, keys - key_offsets[0]) != 0) {
            PRINTF_1_3 ("Did not find key %s\n", key);
            return ~((TIndex)0);
        }
        PRINTF_1_3 ("Found key %s\n", key);
        PrintLine ();
        return 0;
    }

    // Perform a binary search to find the first instance of the hash the 
    // binary search yields. If the mid is odd, we need to subtract the 
    // sizeof (TSize*) in order to get the right pointer address.
    int low = 0,
        mid,
        high = num_items - 1;

    while (low <= high) {
        mid = (low + high) >> 1;    //< >> 1 to /2

        TSize current_hash = hashes[mid];
        PRINTF_1_3 ("low: %i mid: %i high %i hashes[mid]:%x\n", low, mid,
                high, hashes[mid]);

        if (current_hash > hash) {
            high = mid - 1;
        } else if (current_hash < hash) {
            low = mid + 1;
        } else {
            // Duplicate hash found.
            PRINTF_1_3 ("\nFound same hash at mid:%i hash:%x offset for key: "
                    "%s\n", mid, hashes[mid], key);

            // Check for collisions

            collisions = reinterpret_cast<const char*>(key_offsets) +
                max_items * sizeof (uint16_t);
            index = collisions[mid];

            if (index < ~0) {
                // There was a collision so check the table.
                PRINTF_1_3 ("There was a collision so check the table\n";

                // The collisionsList is a sequence of indexes terminated by
                // an invalid index > kMaxNumOperands. collissionsList[0] is an 
                // invalid index, so the collisionsList is searched from 
                // lower address up.

                temp = indexes[mid];

                temp_ptr = collission_list + temp;
                index = *temp_ptr;
                while (index < MaxSetIndexes<TIndex> ()) {
                    PRINTF_1_3 ("comparing to \"%s\"\n", keys -
                            key_offsets[index]);
                    if (strcmp (key, keys - key_offsets[index]) == 0) {
                        PRINTF_1_3 ("but table already contains key at offset:"
                                "%u.\n", index);
                        return index;
                    }
                    ++temp_ptr;
                    index = *temp_ptr;
                }
                PRINTF_1_3 ("Did not find \"" << key << "\"\n";
                return ~((TIndex)0);
            }

            // There were no collisions.

            // But we still don't know if the char is new or a collision.

            // Move collisions pointer to the unsorted indexes.
            indexes += max_items;
            index = unsorted_indexes[mid];

            PRINTF_1_3 ("\n!!!mid: %i-%x unsorted_indexes: %u key: %s\n"
                    "hash: %x\n", mid, hashes[mid], index, keys -
                    key_offsets[index], Hash16 (keys -
                                                key_offsets[index]));

            if (strcmp (key, keys - key_offsets[index]) != 0) {
                //< It was a collision so the table doesn't contain string.
                PRINTF_1_3 (" but it was a collision and did not find key.\n";
                return ~((TIndex)0);
            }

            PRINTF_1_3 ("and found key at mid: " << mid << '\n';
            return index;
        }
    }
    PRINTF_1_3 ("Did not find a hash for key \"" << key << "\"\n";
    PrintLine ();

    return ~((TIndex)0);
}

//static byte Find2 (Set2* book, const char* key) {
//    return SetFind<byte, uint16_t, uint16_t, uint16_t> (book, key);
//}

/** Prints this object out to the console. */
template<typename TIndex, typename TOffset, typename TSize>
void BookPrint (const TBook<TIndex, TOffset, TSize>* book) {
    if (book == nullptr) return;
    TIndex num_items = book->num_items,
           max_items = book->max_items,
           collision_index,
           temp;
    TOffset table_size = book->table_size,
         pile_size = book->pile_size;
    PrintLine ('_');
    
    if (sizeof (TSize) == 2)
        PRINTF_1_3 ("\nSet2: %p\n", book);
    else if (sizeof (TSize) == 4)
        PRINTF_1_3 ("\nSet4: %p\n", book);
    else if (sizeof (TSize) == 8)
        PRINTF_1_3 ("\nSet8: %p\n", book);
    else
        PRINTF_1_3 ("\nInvalid TBook type: %p\n", book);
    PRINTF_1_3 ("\n;: %u max_items: %u  "
            "pile_size: %u  size: %u", num_items,
            max_items, pile_size, table_size);
    PRINTF_1_3 ('\n';
    PRINTF_1_3 ('|';
    for (int i = 0; i < 79; ++i) putchar ('_');
    PRINTF_1_3 ('\n';

    const char* states = reinterpret_cast<const char*> (book) +
                         sizeof (TBook <TIndex, TOffset, TSize>);
    const TOffset* key_offsets = reinterpret_cast<const TOffset*> 
                              (states + max_items);
    const TSize* data_offsets = reinterpret_cast<const TSize*> 
                                (states + max_items *(sizeof (TOffset)));
    const TSize* hashes = reinterpret_cast<const TSize*> (states + max_items *
        (sizeof (TOffset) + sizeof (TSize)));
    const TIndex* indexes = reinterpret_cast<const TIndex*> 
                            (states + max_items * (sizeof (TOffset) + 
                             sizeof (TSize) + sizeof (TIndex))),
        * unsorted_indexes = indexes + max_items,
        * collission_list = unsorted_indexes + max_items,
        *cursor;
    const char* keys = reinterpret_cast<const char*> (book) + table_size - 1;

    PRINTF_1_3 ("\n%3s%10s%8s%10s%10s%10s%10s%11s\n", "i", "key", "offset",
            "hash_e", "hash_u", "hash_s", "index_u", "collisions");
   PRINTF_1_3 ('|';
    for (int i = 0; i < 79; ++i)
        putchar ('_');
    PRINTF_1_3 ('\n';

    for (TIndex i = 0; i < max_items; ++i) {
        // Print each record as a row.
        // @todo Change max_items to ; after done debugging.
        collision_index = indexes[i];
        PRINTF_1_3 ("\n%3i %9s %7u %9x %9x %9x %9u %10u: ", i,
                keys - key_offsets[i], key_offsets[i],
                Hash16 (keys - key_offsets[i]),
                hashes[unsorted_indexes[i]], hashes[i],
                unsorted_indexes[i], collision_index);

        if (collision_index != ~0 && i < num_items) {
            // Print collisions.
            cursor = &collission_list[collision_index];
            temp = *cursor;
            ++cursor;
            PRINTF_1_3 ("%u", temp);
            while (temp != ~0) {
                temp = *cursor;
                ++cursor;
                if (temp == ~0)
                    break;
                PRINTF_1_3 (", %u", temp);
            }
        }

        PRINTF_1_3 ('\n';
    }
    PrintLine ('_');

    PrintMemory (reinterpret_cast<const char*> (book) + 
                 sizeof (TBook<TIndex, TOffset, TSize>), book->size);
    PRINTF_1_3 ('\n';
}

/** Deletes the book contents without wiping the contents. */
template<typename TIndex, typename TOffset, typename TSize>
void BookClear (TBook<TIndex, TOffset, TSize>* book) {
    if (book == nullptr) return;
    book->num_items = 0;
    book->pile_size = 0;
}

/** Deletes the book contents by overwriting it with zeros. */
template<typename TIndex, typename TOffset, typename TSize>
void BookWipe (TBook<TIndex, TOffset, TSize>* book) {
    if (book == nullptr) return;
    TSize size = book->size;
    memset (book, 0, size);
}

/** Returns true if this expr contains only the given address. */
template<typename TIndex, typename TOffset, typename TSize>
bool BookContains (TBook<TIndex, TOffset, TSize>* book, void* data) {
    if (book == nullptr) return false;
    if (data < book) return false;
    if (data > GetEndAddress()) return false;
    return true;
}

/** Removes that object from the book and copies it to the destination. */
template<typename TIndex, typename TOffset, typename TSize>
bool BookRemoveCopy (TBook<TIndex, TOffset, TSize>* book, void* destination,
                 size_t buffer_size, void* data)
{
    if (book == nullptr) return false;

    return false;
}

/** Removes the item at the given address from the book. */
template<typename TIndex, typename TOffset, typename TSize>
bool BookRemove (TBook<TIndex, TOffset, TSize>* book, void* adress) {
    if (book == nullptr) return false;

    return false;
}

/** Removes all but the given book from the book. */
template<typename TIndex, typename TOffset, typename TSize>
bool BookRetain (TBook<TIndex, TOffset, TSize>* book) {
    if (book == nullptr) return false;

    return false;
}

/** Creates a book from dynamic memory. */
template<typename TIndex, typename TOffset, typename TSize>
TBook<TIndex, TOffset, TSize>* BookCreate (TIndex buffered_indexes,
                                           TSize table_size,
                                           TSize size) {
    TBook<TIndex, TOffset, TSize>* book = New<TBook, uint_t> ();
    return book;
}

/** Prints the given TBook to the console. */
template<typename TIndex, typename TOffset, typename TSize>
void BookPrint (TBook<TIndex, TOffset, TSize>* book) {

}

template<typename TSize>
TSize BookSomethingOrOther () {
    return 0;
}


template<typename TIndex, typename TOffset, typename TSize>
class Book {
    public:

    Book (TSize size) {
        BookInit<TSize>
    }

    ~Book () {
        delete buffer_;
    }

    constexpr uint_t OverheadPerIndex () {
        return OverheadPerIndex<TIndex, TOffset, TSize> ();
    };

    constexpr TSize SizeMin (TIndex num_items) {
        return BookSizeMin<TIndex, TOffset, TSize> (num_items);
    };

    /** Insets the given key-value pair.
    */
    inline TIndex Insert (byte type, const char* key, void* data, TIndex index) {
        auto book = reinterpret_cast<Book<TIndex, TOffset, TSize>> (buffer_);
        return BookInsert<TIndex, TOffset, TSize> (book, type, key, data, 
                                                   index);
    }

    inline TIndex IndexMax () {
        return BookIndexMax<TIndex, TOffset, TSize> ();
    }

    inline TIndex Add (const char* key, TType type, void* data) {
        auto book = reinterpret_cast<Book<TIndex, TOffset, TSize>> (buffer_);
        return BookAdd<TIndex, TOffset, TSize> (book, key, type, data);
    }

    /** Returns  the given query char in the hash table. */
    inline TIndex Find (const char* key) {
        auto book = reinterpret_cast<Book<TIndex, TOffset, TSize>> (buffer_);
        return BookFind (book, key);
    }

    /** Deletes the book contents without wiping the contents. */
    inline void Clear () {
        auto book = reinterpret_cast<Book<TIndex, TOffset, TSize>> (buffer_);
        return BookClear<TIndex, TOffset, TSize> (book);
    }

    /** Deletes the book contents by overwriting it with zeros. */
    inline void Wipe () {
        auto book = reinterpret_cast<Book<TIndex, TOffset, TSize>> (buffer_);
        return Wipe<TIndex, TOffset, TSize> (book);
    }

    /** Returns true if this expr contains only the given address. */
    inline bool Contains (void* data) {
        auto book = reinterpret_cast<Book<TIndex, TOffset, TSize>> (buffer_);
        return BookContains<TIndex, TOffset, TSize> (book, void* data);
    }

    /** Removes that object from the book and copies it to the destination. */
    inline bool BookRemoveCopy (void* destination, size_t buffer_size, void* data) {
        auto book = reinterpret_cast<Book<TIndex, TOffset, TSize>> (buffer_);
        return;
    }

    /** Removes the item at the given address from the book. */
    inline bool BookRemove (void* adress) {
        auto book = reinterpret_cast<Book<TIndex, TOffset, TSize>> (buffer_);
        return BookRemove<TIndex, TOffset, TSize> (book);
    }

    /** Removes all but the given book from the book. */
    inline bool Retain () {
        auto book = reinterpret_cast<Book<TIndex, TOffset, TSize>> (buffer_);
        return BookRetain<TIndex, TOffset, TSize> (book);
    }

    /** Prints the given TBook to the console. */
    inline void Print () {
        auto book = reinterpret_cast<Book<TIndex, TOffset, TSize>> (buffer_);
        BookPrint<TIndex, TOffset, TSize> (book);
    }

    private:

    uintptr_t* buffer_;
};

using Book2 = TBook<byte, uint16_t, uint16_t>;
//< Records use the least RAM & run faster than Groups & Files on all systems.
using Book4 = TBook<uint16_t, uint16_t, uint32_t>;
//< Groups more than enough memory for mosts tasks and run faster than files.
using Book8 = TBook<uint32_t, uint32_t, uint64_t>;
//< Files are easily mapped to virtual memory, RAM, drives, and networks.

}       //< namespace _
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3
#endif  //< HEADER_FOR_CRABS_BOOK

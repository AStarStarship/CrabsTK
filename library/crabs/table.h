/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/table.h
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
#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 4
#ifndef HEADER_FOR_CRABS_TABLE
#define HEADER_FOR_CRABS_TABLE
// Dependencies:
#include "memory.h"
#include "operand.h"
// End dependencies.
#if MAJOR_SEAM == 1 && MINOR_SEAM == 4
#ifndef PRINTF
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PUTCHAR(c) putchar(c);
#define PRINT_HEADING {\
    std::cout << '\n';\
    for (int i = 80; i > 0; --i) std::cout << '-';\
}
#define PRINT_TYPE(type, value)\
    Console<> ().Out () << TypeValue (type, value);
#define WIPE MapWipe<UI, SI> (map);
#define PRINT_LINE(token) {\
    for (int cout_123 = 80; count > 0; --count) std::cout << (char)token;\
}
#endif
#define PRINT_TABLE #if MAJOR_SEAM == 1 && MINOR_SEAM == 4

#else
#define PRINTF(x, ...)
#define PUTCHAR(c)
#define PRINT_HEADING(heading)
#define PRINT_TYPE(type, value)
#define WIPE(buffer, size)
#define PRINT_LINE(token)
#define PRINT_TABLE
#endif

namespace _ {

/** A one-to-one key-index map where indexes are natural numbers.

    # Collision Table

    Collision table works by using the maximum key value (i.e. 255 for a byte, 
    2^15-1 for a int16_t, etc). The collisions list is a sequence of indexes
    terminated by an invalid index that is greater than kMaxNumOps. 
    collissionsList[0] is an invalid index, so the collisionsList is searched 
    from lower address up.
    
    # Memory Layout

    @code
       _____________________________________________________
       |_______ | Key 1                                    |
       |_______ | ...                          Keys        |
       |_______ v Key N                                    |
       |___________________________________________________|
       |                                                   |
       |               Header Buffer Space                 |
       |___________________________________________________|
       |_______                          UI Size = 2^N     |
       |_______                                            |
       |_______   Collision List N          Hash Table     |
       |_______ ^ ...                        Collision     |
       |_______ | Collision List 0              List       |
       |___________________________________________________|
       |_______                          UI Size = 2^N     |
       |_______   Buffer Indexes                           |
       |_______                             Hash Table     |
       |_______ ^ Collision Table N          Unsorted      |
       |_______ | ...                         Indexes      |
       |        | Collision table 0                        |
       |___________________________________________________|
       |_______                          UI Size = 2^N     |
       |_______   Buffer Indexes                           |
       |_______                             Hash Table     |
       |_______ ^ Collision SI N          Collision     |
       |_______ | ...                         Indexes      |
       |        | Collision SI 0                        |
       |___________________________________________________|
       |_______                          UI Size = 2^(N+1) |
       |_______   Buffer Indexes                           |
       |_______                                UI          |
       |_______ ^ Key Offset N                 key         |
       |_______ | ...                        Offsets       |
       |        | Key Offset 1                             |
       |___________________________________________________|
       |_______                          UI Size = 2^(N+1) |
       |_______   Buffer Indexes                           |
       |_______                               UI           |
       |_______ ^ Sorted Hash N              Hashes        |
       |_______ | ...                                      |
       |        | Sorted Hash 1                            |
       |___________________________________________________|
     ^ |                                                   |
     | |                 Header (8 bytes)                  |
    0x0|___________________________________________________|
    @endcode
*/
template<typename UI, typename SI>
struct KABUKI Table {
    UI    size,       //< Size of this object in bytes.
          pile_size;  //< Size of the collision table pile.
    SI first_char, //< The first char of the Table.
          num_keys,   //< Number of keys.
          max_keys;   //< Number of buffered indexes.
};

enum {
    kMinTableSize     = 64,   //< Min size of a Table
    kOverheadPerIndex = sizeof (uint16_t) + sizeof (uint16_t) + 
                        sizeof (byte)     + sizeof (byte),
};

//void Print (Table* rt);

/** Creates a streamable hash table with enough buffer space for the 
    max_keys.
    @post    Users might want to call the isValid () function after 
             construction to verify the integrity of the object.
    @warning The reservedNumOps must be aligned to a 32-bit value, and 
             it will get rounded up to the next higher multiple of 4. */
template<typename UI, typename SI>
Table<UI, SI>* TableInit (uintptr_t* buffer, SI max_keys, UI size_bytes) {
    if (!buffer)
        return nullptr;
    Table* table = reinterpret_cast<Table<UI, SI>*>(buffer);

    uint_t min_required_size = sizeof (Table<UI, SI>) + max_keys *
        (kOverheadPerIndex + 2);
    if (set_size < min_required_size)
        return nullptr;

    table->num_keys = 0;
    table->max_keys = max_keys;
    table->pile_size = 0;
    table->size = set_size;
    return table;
}
    
/** Adds the given key and returns the index 64-255 of the index of the 
    op.
    @return Returns an index 64-255 */
template<typename UI, typename SI>
byte TableAdd (Table<UI, SI>* table, const char* key) {
    if (table == nullptr) return 0;
    if (key == nullptr) return 0;

    //PrintLine (key);

    byte num_keys = table->num_keys,
        max_keys = table->max_keys,
        temp;

    uint16_t size = table->size;

    if (num_keys >= max_keys) return kInvalidIndex;
    //< We're out of buffered indexes.

    uint16_t* hashes = reinterpret_cast<uint16_t*> (reinterpret_cast<char*> (table) +
                                                    sizeof (Table<UI, SI>));
    uint16_t* key_offsets = reinterpret_cast<uint16_t*> (hashes +
                                                         max_keys);
    char* indexes = reinterpret_cast<char*> (key_offsets +
                                             max_keys),
        *unsorted_indexes = indexes + max_keys,
        *collission_list = unsorted_indexes + max_keys;
    char* keys = reinterpret_cast<char*> (table) + size - 1,
        *destination;

    // Calculate space left.
    uint16_t value = size - max_keys * kOverheadPerIndex,
        pile_size,
        key_length = static_cast<uint16_t> (SlotLength (key));

    PRINTF ("\nAdding key \"%s\":%u\n%20s:%x\n%20s:%x\n%20s:%x\n%20s:%x"
            "\n%20s:%x\n%20s:%x\n%20s:%x\n%20s:%x\n%20s:%x", key, 
            (uint)key_length, "hashes", hashes, "key_offsets", key_offsets, 
            "keys", keys, "indexes", indexes, "value", value, "hashes", hashes,
            "key_offsets", key_offsets, "keys", keys, "indexes", indexes,
            "value", value)

    uint16_t hash = Hash16 (key),
        current_hash;

    if (key_length > value) {
    PRINTF ("\nBuffer overflow!")
        return ~(byte)0;
    }

    PRINT_TABLE

    if (num_keys == 0) {
        table->num_keys = 1;
        *hashes = hash;
        *key_offsets = static_cast<uint16_t> (key_length);
        *indexes = kInvalidIndex;
        *unsorted_indexes = 0;
        destination = keys - key_length;

        SlotWrite (destination, key);
        PRINTF ("\nInserted key \"%s\" at GetAddress 0x%p", key, destination);
        PRINT_TABLE
        return 0;
    }

    // Calculate left over buffer size by looking up last char.

    if (key_length >= value) {
        PRINTF ("\nNot enough room in buffer!\n")
        return 0;   //< There isn't enough room left in the buffer.
    }

    PRINTF ("\nFinding insert location...")

    SI low = 0,
       mid,
       high = num_keys,
       index;

    char* temp_ptr;

    while (low <= high) {
        mid = (low + high) >> 1;        //< Shift >> 1 to / 2

        current_hash = hashes[mid];
        PRINTF ("high:%i mid:%i low:%i hash:0x%x", high, mid, low, current_hash)

        if (current_hash > hash) {
            high = mid - 1;
        }
        else if (current_hash < hash) {
            low = mid + 1;
        }
        else    // Duplicate hash detected.
        {
            PRINTF ("hash detected, ")

            // Check for other collisions.

            index = indexes[mid];       //< SI in the collision table.

            PRINTF ("index:%i", (int)index)

            if (index != kInvalidIndex) { //< There are other collisions.
                PRINTF ("with collisions, ")
                // There was a collision so check the table.

                // The collisionsList is a sequence of indexes terminated 
                // by an invalid index. collissionsList[0] is 
                // an invalid index, so the collisionsList is searched from
                // lower address up.
                temp = indexes[mid];
                temp_ptr = collission_list + temp;
                index = *temp_ptr;  //< Load the index in the collision table.
                while (index < kInvalidIndex) {
                    PRINTF ("comparing to \"%s\" ", keys - key_offsets[index])
                    if (SlotEquals (key, keys - key_offsets[index])) {
                        PRINTF ("but table already contains key at "
                              "offset:%i", (int)index)
                        return index;
                    }
                    ++temp_ptr;
                    index = *temp_ptr;
                }

                PRINTF ("\nNew collision detected.\n")

                // Copy the key
                value = key_offsets[num_keys - 1] + key_length + 1;
                SlotWrite (keys - value, key);
                key_offsets[num_keys] = value;

                // Update the collision table.
                pile_size = table->pile_size;
                // Shift the collisions table up one element and insert 
                // the unsorted collision index.
                // Then move to the top of the collisions list.
                collission_list += pile_size;
                // and iterate down to the insert spot
                while (collission_list > temp_ptr) {
                    *collission_list = *(collission_list - 1);
                    --collission_list;
                }
                *temp_ptr = num_keys;

                table->pile_size = pile_size + 1;
                PRINTF ("\ncollision index:%i", (int)temp)
                // Store the collision index.
                indexes[num_keys] = temp;   //< Store the collision index
                table->num_keys = num_keys + 1;
                hashes[num_keys] = ~0;      //< Set the last hash to 0xFFFF

                                            // Move collisions pointer to the unsorted_indexes.
                indexes += max_keys;

                //< Add the newest char to the end.
                indexes[num_keys] = num_keys;

                PRINT_TABLE
                PRINTF ("Done inserting.\n")
                return num_keys;
            }

            // But we still don't know if the char is a new collision.

            index = unsorted_indexes[mid];

            PRINTF ("\nChecking if %i is a collision...", (int)index)
            if (!SlotEquals (key, keys - key_offsets[index])) {
                // It's a new collision!
                PRINTF ("\nIt's a new collision!")

                if (value < 3) {
                    PRINTF ("\nBuffer overflow!")
                    return kInvalidIndex;
                }

                // Get offset to write the key too.
                value = key_offsets[num_keys - 1] + key_length + 1;

                byte collision_index = unsorted_indexes[mid];
                SlotWrite (keys - value, key);
                PRINTF ("Inserting value: into index:%i num_keys:%u with "
                        "other collision_index:%i", value, 
                        index, num_keys, collision_index)
                key_offsets[num_keys] = value;

                pile_size = table->pile_size;
                indexes[mid] = static_cast<byte> (pile_size);
                indexes[num_keys] = static_cast<byte> (pile_size);

                // Insert the collision into the collision table.
                temp_ptr = &collission_list[pile_size];
                // Move collisions pointer to the unsorted_indexes.
                indexes += max_keys;
                *temp_ptr = collision_index;
                ++temp_ptr;
                *temp_ptr = num_keys;
                ++temp_ptr;
                *temp_ptr = ~0;
                table->pile_size = pile_size + 3;
                //< Added one term-byte and two indexes.

                // Add the newest key at the end.
                indexes[num_keys] = num_keys;

                // Set the last hash to 0xFFFF
                hashes[num_keys] = ~0;

                table->num_keys = num_keys + 1;

                PRINT_TABLE
                PRINTF ("\nDone inserting.")
                // Then it was a collision so the table doesn't contain string.
                return num_keys;
            }
            PRINTF ("\nTable already contains the key")
            return index;
        }
    }

    // The hash was not in the table.

    value = key_offsets[num_keys - 1] + key_length + 1;
    destination = keys - value;

    PRINTF ("\nThe hash 0x%x was not in the table so inserting \"%s\""
            "into mid:%i at index 0x%p before hash 0x%x", hash, key, (int)mid, 
            destination - reinterpret_cast<char*> (table), hashes[mid])

    // First copy the char and set the key offset.
    SlotWrite (destination, key);
    key_offsets[num_keys] = value;

    // Second move up the hashes and insert at the insertion point.
    uint16_t* hash_ptr = hashes + num_keys;
    //*test = hashes;
    PRINTF ("l_numkeys: %u, hashes: %u hash_ptr: %u insert_ptr: %u\n",
            num_keys, hashes - reinterpret_cast<uint16_t*> (table),
            hash_ptr - reinterpret_cast<uint16_t*> (table), hashes + mid -
            reinterpret_cast<uint16_t*> (table));
    hashes += mid;
    PRINT_TABLE
    while (hash_ptr > hashes) {
        *hash_ptr = *(hash_ptr - 1);
        --hash_ptr;
    }
    *hashes = hash;

    // Mark as not having any collisions.
    indexes[num_keys] = kInvalidIndex;

    // Move up the sorted indexes and insert the unsorted index (which is 
    // the current num_keys).
    indexes += max_keys + mid;
    temp_ptr = indexes + num_keys;

    while (temp_ptr > indexes) {
        *temp_ptr = *(temp_ptr - 1);
        --temp_ptr;
    }
    *temp_ptr = num_keys;      //unsorted_indexes[mid] = num_keys;

    table->num_keys = num_keys + 1;

    PRINT_TABLE
    PRINTF ("Done inserting.\n")
    PRINT_LINE ('-')

    return num_keys;
}

/** Attempts to find the given key.
    @return Returns 0 upon failure, and valid index upon success. */
template<typename UI, typename SI>
KABUKI byte TableFind (const Table<UI, SI>* table, const char* key) {
    ASSERT (table)
    SI index,
          num_keys = table->num_keys,
          max_keys = table->max_keys,
          temp;

    PRINT_HEADING ("Finding record...")

    if (key == nullptr || num_keys == 0)
        return kInvalidIndex;

    uint16_t size = table->size;

    const uint16_t* hashes = reinterpret_cast<const uint16_t*>
        (reinterpret_cast<const char*> (table) +
         sizeof (Table<UI, SI>));
    const uint16_t* key_offsets = reinterpret_cast<const uint16_t*>(hashes +
                                                                    max_keys);
    const char* indexes = reinterpret_cast<const char*>(key_offsets +
                                                        max_keys),
        *unsorted_indexes = indexes + max_keys,
        *collission_list = unsorted_indexes + max_keys;
    const char* keys = reinterpret_cast<const char*> (table) + size - 1;
    const char* collisions,
        *temp_ptr;

    uint16_t hash = Hash16 (key);

    PRINTF ("\nSearching for key \"%s\" with hash 0x%x\n", key, hash)

    if (num_keys == 1) {
        PRINTF ("Comparing keys - key_offsets[0] - this %u\n%s\n", 
                (keys - key_offsets[0]) - reinterpret_cast<const char*> (table),
                keys - key_offsets[0])
        if (!SlotEquals (key, keys - key_offsets[0])) {
            PRINTF ("Did not find key %s\n", key)
            return kInvalidIndex;
        }
        PRINTF ("Found key %s\n", key)
        PRINT_LINE('-')
        return 0;
    }

    // Perform a binary search to find the first instance of the hash the 
    // binary search yields. If the mid is odd, we need to subtract the 
    // sizeof (uint16_t*) in order to get the right pointer address.
    int low = 0,
        mid,
        high = num_keys - 1;

    while (low <= high) {
        mid = (low + high) >> 1;    //< >> 1 to /2

        uint16_t current_hash = hashes[mid];
        PRINTF ("low: %i mid: %i high %i hashes[mid]:%x\n", low, mid,
                high, hashes[mid])

        if (current_hash > hash) {
            high = mid - 1;
        }
        else if (current_hash < hash) {
            low = mid + 1;
        }
        else {
            // Duplicate hash found.
            PRINTF ("\nFound same hash at mid:%i hash:%x offset for key: "
                    "%s\n", mid, hashes[mid], key)

            // Check for collisions

            collisions = reinterpret_cast<const char*>(key_offsets) +
            max_keys * sizeof (uint16_t);
            index = collisions[mid];

            if (index != kInvalidIndex) {
                // There was a collision so check the table.
                //PRINTF ("There was a collision so check the table\n");

                // The collisionsList is a sequence of indexes terminated by
                // an invalid index > kMaxNumOps. collissionsList[0] is an 
                // invalid index, so the collisionsList is searched from 
                // lower address up.

                temp = indexes[mid];

                temp_ptr = collission_list + temp;
                index = *temp_ptr;
                while (index != kInvalidIndex) {
                    PRINTF ("comparing to \"%s\"", keys - key_offsets[index])
                    if (SlotEquals (key, keys - key_offsets[index])) {
                        PRINTF ("Table already contains key at offset:"
                                "%u.\n", index)
                        return index;
                    }
                    ++temp_ptr;
                    index = *temp_ptr;
                }
                PRINTF ("\nDid not find \"%s\"", key)
                return kInvalidIndex;
            }

            // There were no collisions.

            // But we still don't know if the char is new or a collision.

            // Move collisions pointer to the unsorted indexes.
            indexes += max_keys;
            index = unsorted_indexes[mid];

            PRINTF ("\nmid:%i-%u unsorted_indexes:%SI key:\"%s\" hash:0x%x", (int)mid, 
                    (uint)hashes[mid], index, keys - key_offsets[index],
                    Hash16 (keys - key_offsets[index]))

            if (!SlotEquals (key, keys - key_offsets[index])) {
                //< It was a collision so the table doesn't contain string.
                PRINTF (" but it was a collision and did not find key.")
                return kInvalidIndex;
            }

            PRINTF ("\nFound key at mid:%i", mid)
            return index;
        }
    }
    PRINTF ("\nDidn't find a hash for key \"%s\"", key)
    PRINT_LINE('-')

    return kInvalidIndex;
}

#if USING_PRINTER
/** Prints this object out to the console. */
template<typename UI, typename SI>
Printer& TablePrint (Printer& print, Table<UI, SI>* table) {
    if (table == nullptr)
        return;
    SI num_keys  = table->num_keys,
        max_keys = table->max_keys,
        collision_index,
        temp;
    UI size       = table->size,
        pile_size = table->pile_size;

    print << Line ('_')
          << "\nTable:" << Hex<> (table) << "\nnum_keys:" 
          << num_keys << " max_keys:" << max_keys << " pile_size:" << pile_size 
          << " size:" << size << "\n|";
    for (int i = 0; i < 79; ++i)
        print << '_';
    print << '\n';

    uint16_t* hashes = reinterpret_cast<uint16_t*>
        (reinterpret_cast<char*>(table) +
         sizeof (Table<UI, SI>));
    uint16_t* key_offsets = reinterpret_cast<uint16_t*>(hashes + max_keys);
    char* indexes = reinterpret_cast<char*> (key_offsets + max_keys),
        *unsorted_indexes = indexes + max_keys,
        *collission_list = unsorted_indexes + max_keys,
        *cursor;
    char* keys = reinterpret_cast<char*> (table) + size - 1;

    print << '\n' << Right<SI> ("i", 3) << Right<> ("key"    , 10)
          << Right<> ("offset" ,  8)    << Right<> ("hash_e" , 10) 
          << Right<> ("hash_u" , 10)    << Right<> ("index_u", 10)
          << Right<> ("collisions", 11) << '|';
    for (int i = 0; i < 79; ++i)
        print << '_';
    print << '\n';

    for (int i = 0; i < num_keys; ++i) {
        // Print each record as a row.
        // @todo Change max_keys to num_keys after done debugging.
        collision_index = indexes[i];
        print << Right<int> (i, 3) << Right<> (keys - key_offsets[i], 10)
              << Right<UI> (key_offsets[i],  8)
              << Right<Hex<UI>> (Hash16 (keys - key_offsets[i]), 10)
              << Right<Hex<UI>> (hashes[unsorted_indexes[i]], 10)
              << Right<Hex<UI>> (hashes[i], 10)
              << Right<UI> (unsorted_indexes[i], 10)
              << Right<UI> (collision_index, 11);

        if ((collision_index != kInvalidIndex) && (i < num_keys)) {
            // Print collisions.
            cursor = &collission_list[collision_index];
            temp = *cursor;
            ++cursor;
            print << temp;
            while (temp != kInvalidIndex) {
                temp = *cursor;
                ++cursor;
                if (temp != kInvalidIndex)
                    print << ", " << temp;

            }
        }

        print << '\n';
    }
    print << '|';
    for (int i = 0; i < 79; ++i)
        print << '_';
    print << '\n' << Memory (table, table->size) << '\n';
}
#endif

}       //< namespace _
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 5
#endif  //< HEADER_FOR_CRABS_TABLE

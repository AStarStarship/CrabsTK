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

#ifndef HEADER_FOR_CRABS_TABLE
#define HEADER_FOR_CRABS_TABLE

#include "memory.h"

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 4
#include "operand.h"

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
       |_______ ^ Collision Index N          Collision     |
       |_______ | ...                         Indexes      |
       |        | Collision Index 0                        |
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
template<typename Index, typename UI>
struct KABUKI Table {
    Index first_char; //< The first char of the Table.
          num_keys,   //< Number of keys.
          max_keys;   //< Number of buffered indexes.
    UI    pile_size,  //< Size of the collision table pile.
          size;       //< Size of this object in bytes.
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
template<typename Index, typename UI>
KABUKI Table* TableInit (uintptr_t* buffer, Index max_keys, UI size_bytes) {
    if (!buffer)
        return nullptr;
    Table* table = reinterpret_cast<Table*>(buffer);

    uint_t min_required_size = sizeof (Table) + max_keys *
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
template<typename Index, typename UI>
KABUKI byte TableAdd (Table* table, const char* key) {
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
                                                    sizeof (Table));
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

    #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
    PRINTF (PrintLine () << "Adding Key " << key << ':' << key_length
        << '\n' << PrintRight ("hashes"     , 20) << ':' << PrintHex (hashes)
        << '\n' << PrintRight ("key_offsets", 20) << ':' << PrintHex (key_offsets)
        << '\n' << PrintRight ("keys"       , 20) << ':' << PrintHex (keys)
        << '\n' << PrintRight ("indexes"    , 20) << ':' << PrintHex (indexes)
        << '\n' << PrintRight ("value"      , 20) << ':' << value;
    #endif

    uint16_t hash = Hash16 (key),
        current_hash;

    if (key_length > value) {
    #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
        Write ("Buffer overflow\n");
    #endif
        return ~(byte)0;
    }

#if MAJOR_SEAM == 1 && MINOR_SEAM == 4
    TablePrint (table);
#endif

    if (num_keys == 0) {
        table->num_keys = 1;
        *hashes = hash;
        *key_offsets = static_cast<uint16_t> (key_length);
        *indexes = kInvalidIndex;
        *unsorted_indexes = 0;
        destination = keys - key_length;

        SlotWrite (destination, key);
    #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
        PRINTF ("\nInserted key " << key << " at GetAddress "
                 << out.Pointer (destination));
        TablePrint (table, out);
    #endif
        return 0;
    }

    // Calculate left over buffer size by looking up last char.

    if (key_length >= value) {
    #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
        Print ("Not enough room in buffer!\n");
    #endif
        return 0;   //< There isn't enough room left in the buffer.
    }

#if MAJOR_SEAM == 1 && MINOR_SEAM == 4
    Write ("Finding insert location... \n");
#endif

    int low = 0,
        mid,
        high = num_keys,
        index;

    char* temp_ptr;

    while (low <= high) {
        mid = (low + high) >> 1;        //< Shift >> 1 to / 2

        current_hash = hashes[mid];
    #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
        Write ("high: " << high << " mid: " << mid < " low " << low << " hash: "
              << current_hash);
    #endif

        if (current_hash > hash) {
            high = mid - 1;
        }
        else if (current_hash < hash) {
            low = mid + 1;
        }
        else    // Duplicate hash detected.
        {
        #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
            Write ("hash detected, ");
        #endif

            // Check for other collisions.

            index = indexes[mid];       //< Index in the collision table.

        #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
            PRINTF ("index:" << index);
        #endif

            if (index != kInvalidIndex) { //< There are other collisions.
            #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
                Write ("with collisions, ");
            #endif
                // There was a collision so check the table.

                // The collisionsList is a sequence of indexes terminated 
                // by an invalid index. collissionsList[0] is 
                // an invalid index, so the collisionsList is searched from
                // lower address up.
                temp = indexes[mid];
                temp_ptr = collission_list + temp;
                index = *temp_ptr;  //< Load the index in the collision table.
                while (index < kInvalidIndex) {
                #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
                    Write ("comparing to \"" << keys - key_offsets[index] << "\" ");
                #endif
                    if (SlotEquals (key, keys - key_offsets[index])) {
                    #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
                        PRINTF ("but table already contains key at "
                              "offset:" << index);
                    #endif
                        return index;
                    }
                    ++temp_ptr;
                    index = *temp_ptr;
                }

                // Its a new collision!
            #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
                Write ("and new collision detected.\n");
            #endif

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
            #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
                Write ("\ncollision index: " << temp);
            #endif
                // Store the collision index.
                indexes[num_keys] = temp;   //< Store the collision index
                table->num_keys = num_keys + 1;
                hashes[num_keys] = ~0;      //< Set the last hash to 0xFFFF

                                            // Move collisions pointer to the unsorted_indexes.
                indexes += max_keys;

                //< Add the newest char to the end.
                indexes[num_keys] = num_keys;

            #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
                TablePrint (table);
                Write ("Done inserting.\n";
            #endif
                return num_keys;
            }

            // But we still don't know if the char is a new collision.

            index = unsorted_indexes[mid];

        #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
            Write ("Checking if " << index << " is a collision...";
        #endif
            if (!SlotEquals (key, keys - key_offsets[index])) {
                // It's a new collision!
            #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
                Write ("It's a new collision!\n";
            #endif

                if (value < 3) {
                #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
                    Write ("Buffer overflow!\n";
                #endif
                    return kInvalidIndex;
                }

                // Get offset to write the key too.
                value = key_offsets[num_keys - 1] + key_length + 1;

                byte collision_index = unsorted_indexes[mid];
                SlotWrite (keys - value, key);
            #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
                PRINTF ("Inserting value: " << value << " into index:" <<
                      index << "num_keys:" << num_keys <<
                      " with other collision_index:" << collision_index);
            #endif
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

            #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
                TablePrint (table);
                Write ("Done inserting.\n";
            #endif
                // Then it was a collision so the table doesn't contain string.
                return num_keys;
            }
        #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
            Write ("table already contains the key";
        #endif
            return index;
        }
    }

    // The hash was not in the table.

    value = key_offsets[num_keys - 1] + key_length + 1;
    destination = keys - value;

#if MAJOR_SEAM == 1 && MINOR_SEAM == 4
    Write ("\nThe hash " << PrintHex (hash) << " was not in the table so inserting "
          << key << "into mid:" << mid <<
          " at index " << destination - reinterpret_cast<char*> (table) <<
          " before hash " << PrintHex (hashes[mid]));
#endif

    // First copy the char and set the key offset.
    SlotWrite (destination, key);
    key_offsets[num_keys] = value;

    // Second move up the hashes and insert at the insertion point.
    uint16_t* hash_ptr = hashes + num_keys;
    //*test = hashes;
#if MAJOR_SEAM == 1 && MINOR_SEAM == 4
    Write ("l_numkeys: %u, hashes: %u hash_ptr: %u insert_ptr: %u\n",
          num_keys, hashes - reinterpret_cast<uint16_t*> (table),
          hash_ptr - reinterpret_cast<uint16_t*> (table), hashes + mid -
          reinterpret_cast<uint16_t*> (table));
#endif
    hashes += mid;
#if MAJOR_SEAM == 1 && MINOR_SEAM == 4
    TablePrint (table);
#endif
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

#if MAJOR_SEAM == 1 && MINOR_SEAM == 4
    TablePrint (table);
    Write ("Done inserting.\n");
    PrintLine ();
#endif

    return num_keys;
}

/** Attempts to find the given key.
    @return Returns 0 upon failure, and valid index upon success. */
template<typename Index, typename UI>
KABUKI byte TableFind (const Table* table, const char* key) {
    if (table == nullptr)
        return 0;
    #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
    PrintLineBreak ("Finding record...", 5);
    #endif
    int index,
        num_keys = table->num_keys,
        max_keys = table->max_keys,
        temp;

    if (key == nullptr || num_keys == 0)
        return kInvalidIndex;

    uint16_t size = table->size;

    const uint16_t* hashes = reinterpret_cast<const uint16_t*>
        (reinterpret_cast<const char*> (table) +
         sizeof (Table));
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

    #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
    Write ("\nSearching for key \"%s\" with hash 0x%x\n", key, hash);
    #endif

    if (num_keys == 1) {
        #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
        Write ("Comparing keys - key_offsets[0] - this %u\n%s\n", (keys -
              key_offsets[0]) - reinterpret_cast<const char*> (table), keys -
              key_offsets[0]);
        #endif
        if (!SlotEquals (key, keys - key_offsets[0])) {
            #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
            Write ("Did not find key %s\n", key);
            #endif
            return kInvalidIndex;
        }
        #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
        Write ("Found key %s\n", key);
        PrintLine ();
        #endif
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
        #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
        Write ("low: %i mid: %i high %i hashes[mid]:%x\n", low, mid,
              high, hashes[mid]);
        #endif

        if (current_hash > hash) {
            high = mid - 1;
        }
        else if (current_hash < hash) {
            low = mid + 1;
        }
        else {
            // Duplicate hash found.
            //Write ("\nFound same hash at mid:%i hash:%x offset for key: "
            //        "%s\n", mid, hashes[mid], key);

            // Check for collisions

            collisions = reinterpret_cast<const char*>(key_offsets) +
                max_keys * sizeof (uint16_t);
            index = collisions[mid];

            if (index != kInvalidIndex) {
                // There was a collision so check the table.
                //Write ("There was a collision so check the table\n");

                // The collisionsList is a sequence of indexes terminated by
                // an invalid index > kMaxNumOps. collissionsList[0] is an 
                // invalid index, so the collisionsList is searched from 
                // lower address up.

                temp = indexes[mid];

                temp_ptr = collission_list + temp;
                index = *temp_ptr;
                while (index != kInvalidIndex) {
                    #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
                    Write ("comparing to \"%s\"\n", keys -
                          key_offsets[index]);
                    #endif
                    if (SlotEquals (key, keys - key_offsets[index])) {
                    #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
                        Write ("Table already contains key at offset:"
                              "%u.\n", index);
                    #endif
                        return index;
                    }
                    ++temp_ptr;
                    index = *temp_ptr;
                }
                #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
                Write ("Did not find " << key << '\n';
                #endif
                return kInvalidIndex;
            }

            // There were no collisions.

            // But we still don't know if the char is new or a collision.

            // Move collisions pointer to the unsorted indexes.
            indexes += max_keys;
            index = unsorted_indexes[mid];

            #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
            PRINTF ("\nmid:" << mid << "-" << hashes[mid] << 
                  " unsorted_indexes: " << index << " key:\"" << 
                  keys - key_offsets[index] << "\" hash:" << 
                  PrintHex (Hash16 (keys - key_offsets[index])));
            #endif

            if (!SlotEquals (key, keys - key_offsets[index])) {
                //< It was a collision so the table doesn't contain string.
                #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
                Write (" but it was a collision and did not find key.");
                #endif
                return kInvalidIndex;
            }

        #if MAJOR_SEAM == 1 && MINOR_SEAM == 4
            Write ("; found key at mid: " << mid << '\n';
        #endif
            return index;
        }
    }
#if MAJOR_SEAM == 1 && MINOR_SEAM == 4
    Write ("; didn't find a hash for key " << key << '\n';
#endif
    PrintLine ();

    return kInvalidIndex;
}

#if USING_PRINTER
/** Prints this object out to the console. */
KABUKI void TablePrint (Table* table) {
    if (table == nullptr)
        return;
    byte num_keys = table->num_keys,
        max_keys = table->max_keys,
        collision_index,
        temp;
    uint16_t size = table->size,
        pile_size = table->pile_size;
    PrintLine ('_');
    Write ("\nTable:0x%p\nnum_keys:%u max_keys:%u  "
          "pile_size:%u  size:%u", table, num_keys,
          max_keys, pile_size, size);
    Write ('\n';
    Write ('|';
    for (int i = 0; i < 79; ++i)
        Write ('_';
    Write ('\n';

    uint16_t* hashes = reinterpret_cast<uint16_t*>
        (reinterpret_cast<char*>(table) +
         sizeof (Table));
    uint16_t* key_offsets = reinterpret_cast<uint16_t*>(hashes + max_keys);
    char* indexes = reinterpret_cast<char*> (key_offsets + max_keys),
        *unsorted_indexes = indexes + max_keys,
        *collission_list = unsorted_indexes + max_keys,
        *cursor;
    char* keys = reinterpret_cast<char*> (table) + size - 1;

    Write ("\n%3s%10s%8s%10s%10s%10s%10s%11s\n", "i", "key", "offset",
          "hash_e", "hash_u", "hash_s", "index_u", "collisions");
    Write ('|';
    for (int i = 0; i < 79; ++i)
        Write ('_';
    Write ('\n';

    for (int i = 0; i < num_keys; ++i) {
        // Print each record as a row.
        // @todo Change max_keys to num_keys after done debugging.
        collision_index = indexes[i];
        Write ("\n%3i %9s %7u %9x %9x %9x %9u %10u: ", i,
              keys - key_offsets[i], key_offsets[i],
              Hash16 (keys - key_offsets[i]),
              hashes[unsorted_indexes[i]], hashes[i],
              unsorted_indexes[i], collision_index);

        if ((collision_index != kInvalidIndex) && (i < num_keys)) {
            // Print collisions.
            cursor = &collission_list[collision_index];
            temp = *cursor;
            ++cursor;
            Write (temp;
            while (temp != kInvalidIndex) {
                temp = *cursor;
                ++cursor;
                if (temp != kInvalidIndex)
                    Write (", %u", temp);

            }
        }

        Write ('\n';
    }
    Write ('|';
    for (int i = 0; i < 79; ++i)
        Write ('_';
    Write ('\n';

    PrintMemory (table, table->size);
    Write ('\n';
}
#endif

}       //< namespace _
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 5
#endif  //< HEADER_FOR_CRABS_TABLE

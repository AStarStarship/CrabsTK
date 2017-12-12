/** kabuki::script
#include <script_utils.h>
#include <script_table.h>
    @version 0.x
    @file    ~/source/kabuki/script/impl/script_table.cc
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

#include <stdafx.h>
#include "table.h"
#include "text.h"

#if USING_TABLE

namespace _ {

Table* TableInit (uintptr_t* buffer, byte max_keys, uint16_t set_size) {
    if (buffer == nullptr)
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

byte TableAdd (Table* table, const char* key) {
    if (table == nullptr) return 0;
    if (key == nullptr) return 0;

    //PrintLine (key);

    byte num_keys = table->num_keys,
        max_keys = table->max_keys,
        temp;

    uint16_t size = table->size;

    if (num_keys >= max_keys) return kInvalidIndex;
    //< We're out of buffered indexes.

    hash16_t* hashes = reinterpret_cast<hash16_t*> (reinterpret_cast<byte*> (table) +
                                                    sizeof (Table));
    uint16_t* key_offsets = reinterpret_cast<uint16_t*> (hashes +
                                                         max_keys);
    byte* indexes = reinterpret_cast<byte*> (key_offsets +
                                             max_keys),
        *unsorted_indexes = indexes + max_keys,
        *collission_list = unsorted_indexes + max_keys;
    char* keys = reinterpret_cast<char*> (table) + size - 1,
        *destination;

    // Calculate space left.
    uint16_t value = size - max_keys * kOverheadPerIndex,
        pile_size,
        key_length = static_cast<uint16_t> (StrandLength (key));

#if SCRIPT_DEBUG
    PrintLine ();
    printf ("Adding Key %s:%u \n%20s: 0x%p\n%20s: %p\n%20s: 0x%p\n"
            "%20s: %p\n%20s:%u\n", key, key_length, "hashes", hashes, "key_offsets",
            key_offsets, "keys", keys, "indexes", indexes, "value", value);
#endif  //< SCRIPT_DEBUG

    hash16_t hash = Hash16 (key),
        current_hash;

    if (key_length > value) {
#if SCRIPT_DEBUG
        printf ("Buffer overflow\n");
#endif  //< SCRIPT_DEBUG
        return ~(byte)0;
    }

#if SCRIPT_DEBUG
    TablePrint (table);
#endif  //< SCRIPT_DEBUG

    if (num_keys == 0) {
        table->num_keys = 1;
        *hashes = hash;
        *key_offsets = static_cast<uint16_t> (key_length);
        *indexes = kInvalidIndex;
        *unsorted_indexes = 0;
        destination = keys - key_length;

        TextWrite (destination, key);
#if SCRIPT_DEBUG
        printf ("Inserted key %s at GetAddress 0x%p\n", key, destination);
        TablePrint (table);
#endif  //< SCRIPT_DEBUG
        return 0;
    }

    // Calculate left over buffer size by looking up last char.

    if (key_length >= value) {
#if SCRIPT_DEBUG
        printf ("Not enough room in buffer!\n");
#endif  //< SCRIPT_DEBUG
        return 0;   //< There isn't enough room left in the buffer.
    }

#if SCRIPT_DEBUG
    std::cout << "Finding insert location... \n";
#endif  //< SCRIPT_DEBUG

    int low = 0,
        mid,
        high = num_keys,
        index;

    byte* temp_ptr;

    while (low <= high) {
        mid = (low + high) >> 1;        //< Shift >> 1 to / 2

        current_hash = hashes[mid];
#if SCRIPT_DEBUG
        printf ("high: %i mid: %i low %i hash: %x\n", high, mid, low,
                current_hash);
#endif  //< SCRIPT_DEBUG

        if (current_hash > hash) {
            high = mid - 1;
        } else if (current_hash < hash) {
            low = mid + 1;
        } else    // Duplicate hash detected.
        {
#if SCRIPT_DEBUG
            std::cout << "hash detected, ";
#endif  //< SCRIPT_DEBUG

            // Check for other collisions.

            index = indexes[mid];       //< Index in the collision table.

#if SCRIPT_DEBUG
            printf ("index:%u\n", index);
#endif  //< SCRIPT_DEBUG

            if (index != kInvalidIndex) { //< There are other collisions.
#if SCRIPT_DEBUG
                std::cout << "with collisions, ";
#endif  //< SCRIPT_DEBUG
                // There was a collision so check the table.

                // The collisionsList is a sequence of indexes terminated 
                // by an invalid index. collissionsList[0] is 
                // an invalid index, so the collisionsList is searched from
                // lower address up.
                temp = indexes[mid];
                temp_ptr = collission_list + temp;
                index = *temp_ptr;  //< Load the index in the collision table.
                while (index < kInvalidIndex) {
#if SCRIPT_DEBUG
                    printf ("comparing to \"%s\"\n", keys - key_offsets[index]);
#endif  //< SCRIPT_DEBUG
                    if (StrandEquals (key, keys - key_offsets[index])) {
#if SCRIPT_DEBUG
                        printf ("but table already contains key at "
                                "offset: %u.\n", index);
#endif  //< SCRIPT_DEBUG
                        return index;
                    }
                    ++temp_ptr;
                    index = *temp_ptr;
                }

                // Its a new collision!
#if SCRIPT_DEBUG
                std::cout << "and new collision detected.\n";
#endif  //< SCRIPT_DEBUG

                // Copy the key
                value = key_offsets[num_keys - 1] + key_length + 1;
                TextWrite (keys - value, key);
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
#if SCRIPT_DEBUG
                printf ("\n\ncollision index: %u\n", temp);
#endif  //< SCRIPT_DEBUG
                // Store the collision index.
                indexes[num_keys] = temp;   //< Store the collision index
                table->num_keys = num_keys + 1;
                hashes[num_keys] = ~0;      //< Set the last hash to 0xFFFF

                // Move collisions pointer to the unsorted_indexes.
                indexes += max_keys;

                //< Add the newest char to the end.
                indexes[num_keys] = num_keys;

#if SCRIPT_DEBUG
                TablePrint (table);
                std::cout << "Done inserting.\n";
#endif  //< SCRIPT_DEBUG
                return num_keys;
            }

            // But we still don't know if the char is a new collision.

            index = unsorted_indexes[mid];

#if SCRIPT_DEBUG
            std::cout << "Checking if " << index << " is a collision...";
#endif  //< SCRIPT_DEBUG
            if (!StrandEquals (key, keys - key_offsets[index])) {
                // It's a new collision!
#if SCRIPT_DEBUG
                std::cout << "It's a new collision!\n";
#endif  //< SCRIPT_DEBUG

                if (value < 3) {
#if SCRIPT_DEBUG
                    std::cout << "Buffer overflow!\n";
#endif  //< SCRIPT_DEBUG
                    return kInvalidIndex;
                }

                // Get offset to write the key too.
                value = key_offsets[num_keys - 1] + key_length + 1;

                byte collision_index = unsorted_indexes[mid];
                TextWrite (keys - value, key);
#if SCRIPT_DEBUG
                printf ("Inserting value: %u into index:%u "
                        "num_keys:%u with other collision_index: %u\n", value,
                        index, num_keys, collision_index);
#endif  //< SCRIPT_DEBUG
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

#if SCRIPT_DEBUG
                TablePrint (table);
                std::cout << "Done inserting.\n";
#endif  //< SCRIPT_DEBUG
                // Then it was a collision so the table doesn't contain string.
                return num_keys;
            }
#if SCRIPT_DEBUG
            std::cout << "table already contains the key\n";
#endif  //< SCRIPT_DEBUG
            return index;
        }
    }

    // The hash was not in the table.

    value = key_offsets[num_keys - 1] + key_length + 1;
    destination = keys - value;

#if SCRIPT_DEBUG
    printf ("The hash 0x%x was not in the table so inserting %s into mid:"
            " %i at index %u before hash 0x%x \n", hash, key, mid,
            destination - reinterpret_cast<char*> (table), hashes[mid]);
#endif  //< SCRIPT_DEBUG

    // First copy the char and set the key offset.
    TextWrite (destination, key);
    key_offsets[num_keys] = value;

    // Second move up the hashes and insert at the insertion point.
    hash16_t* hash_ptr = hashes + num_keys;
    //*test = hashes;
#if SCRIPT_DEBUG
    printf ("l_numkeys: %u, hashes: %u hash_ptr: %u insert_ptr: %u\n",
            num_keys, hashes - reinterpret_cast<hash16_t*> (table),
            hash_ptr - reinterpret_cast<hash16_t*> (table), hashes + mid -
            reinterpret_cast<hash16_t*> (table));
#endif  //< SCRIPT_DEBUG
    hashes += mid;
#if SCRIPT_DEBUG
    TablePrint (table);
#endif  //< SCRIPT_DEBUG
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

#if SCRIPT_DEBUG
    TablePrint (table);
    std::cout << "Done inserting.\n";
    PrintLine ();
#endif  //< SCRIPT_DEBUG

    return num_keys;
}

byte TableFind (const Table* table, const char* key) {
    if (table == nullptr)
        return 0;
#if SCRIPT_DEBUG
    PrintLineBreak ("Finding record...", 5);
#endif  //< SCRIPT_DEBUG
    int index,
        num_keys = table->num_keys,
        max_keys = table->max_keys,
        temp;

    if (key == nullptr || num_keys == 0)
        return kInvalidIndex;

    uint16_t size = table->size;

    const hash16_t* hashes = reinterpret_cast<const hash16_t*>
        (reinterpret_cast<const byte*> (table) +
         sizeof (Table));
    const uint16_t* key_offsets = reinterpret_cast<const uint16_t*>(hashes +
                                                                    max_keys);
    const byte* indexes = reinterpret_cast<const byte*>(key_offsets +
                                                        max_keys),
        *unsorted_indexes = indexes + max_keys,
        *collission_list = unsorted_indexes + max_keys;
    const char* keys = reinterpret_cast<const char*> (table) + size - 1;
    const byte* collisions,
        *temp_ptr;

    hash16_t hash = Hash16 (key);

#if SCRIPT_DEBUG
    printf ("\nSearching for key \"%s\" with hash 0x%x\n", key, hash);
#endif  //< SCRIPT_DEBUG

    if (num_keys == 1) {
#if SCRIPT_DEBUG
        printf ("Comparing keys - key_offsets[0] - this %u\n%s\n", (keys - 
                key_offsets[0]) - reinterpret_cast<const char*> (table), keys - 
                key_offsets[0]);
#endif  //< SCRIPT_DEBUG
        if (!StrandEquals (key, keys - key_offsets[0])) {
#if SCRIPT_DEBUG
            printf ("Did not find key %s\n", key);
#endif  //< SCRIPT_DEBUG
            return kInvalidIndex;
        }
#if SCRIPT_DEBUG
        printf ("Found key %s\n", key);
        PrintLine ();
#endif  //< SCRIPT_DEBUG
        return 0;
    }

    // Perform a binary search to find the first instance of the hash the 
    // binary search yields. If the mid is odd, we need to subtract the 
    // sizeof (hash16_t*) in order to get the right pointer address.
    int low = 0,
        mid,
        high = num_keys - 1;

    while (low <= high) {
        mid = (low + high) >> 1;    //< >> 1 to /2

        hash16_t current_hash = hashes[mid];
#if SCRIPT_DEBUG
        printf ("low: %i mid: %i high %i hashes[mid]:%x\n", low, mid, 
                 high, hashes[mid]);
#endif  //< SCRIPT_DEBUG

        if (current_hash > hash) {
            high = mid - 1;
        } else if (current_hash < hash) {
            low = mid + 1;
        } else {
            // Duplicate hash found.
            //printf ("\nFound same hash at mid:%i hash:%x offset for key: "
            //        "%s\n", mid, hashes[mid], key);

            // Check for collisions

            collisions = reinterpret_cast<const byte*>(key_offsets) +
                max_keys * sizeof (uint16_t);
            index = collisions[mid];

            if (index != kInvalidIndex) {
                // There was a collision so check the table.
                //printf ("There was a collision so check the table\n");

                // The collisionsList is a sequence of indexes terminated by
                // an invalid index > kMaxNumOperations. collissionsList[0] is an 
                // invalid index, so the collisionsList is searched from 
                // lower address up.

                temp = indexes[mid];

                temp_ptr = collission_list + temp;
                index = *temp_ptr;
                while (index != kInvalidIndex) {
#if SCRIPT_DEBUG
                    printf ("comparing to \"%s\"\n", keys - 
                            key_offsets[index]);
#endif  //< SCRIPT_DEBUG
                    if (StrandEquals (key, keys - key_offsets[index])) {
#if SCRIPT_DEBUG
                        printf ("Table already contains key at offset:"
                                "%u.\n", index);
#endif  //< SCRIPT_DEBUG
                        return index;
                    }
                    ++temp_ptr;
                    index = *temp_ptr;
                }
#if SCRIPT_DEBUG
                std::cout << "Did not find "<< key << '\n';
#endif  //< SCRIPT_DEBUG
                return kInvalidIndex;
            }

            // There were no collisions.

            // But we still don't know if the char is new or a collision.

            // Move collisions pointer to the unsorted indexes.
            indexes += max_keys;
            index = unsorted_indexes[mid];

#if SCRIPT_DEBUG
            printf ("\n!!!mid: %i-%x unsorted_indexes: %u key: %s\n"
                    "hash: %x\n", mid, hashes[mid], index, keys - 
                    key_offsets[index], Hash16 (keys - 
                    key_offsets[index]));
#endif  //< SCRIPT_DEBUG

            if (!StrandEquals (key, keys - key_offsets[index])) {
                //< It was a collision so the table doesn't contain string.
#if SCRIPT_DEBUG
                printf (" but it was a collision and did not find key.\n");
#endif  //< SCRIPT_DEBUG
                return kInvalidIndex;
            }

#if SCRIPT_DEBUG
            std::cout << "; found key at mid: %i " << mid << '\n';
#endif  //< SCRIPT_DEBUG
            return index;
        }
    }
#if SCRIPT_DEBUG
    std::cout << "; didn't find a hash for key " << key << '\n';
#endif  //< SCRIPT_DEBUG
    PrintLine ();

    return kInvalidIndex;
}

#if USE_MORE_ROM
void TablePrint (Table* table) {
    if (table == nullptr)
        return;
    byte num_keys = table->num_keys,
        max_keys = table->max_keys,
        collision_index,
        temp;
    uint16_t size = table->size,
        pile_size = table->pile_size;
    PrintLine ('_');
    printf ("\n| Table:0x%p\n| num_keys:%u max_keys:%u  "
            "pile_size:%u  size:%u", table, num_keys,
            max_keys, pile_size, size);
    std::cout << '\n';
    std::cout << '|';
    for (int i = 0; i < 79; ++i)
        std::cout << '_';
    std::cout << '\n';

    hash16_t* hashes = reinterpret_cast<hash16_t*>
        (reinterpret_cast<byte*>(table) +
         sizeof (Table));
    uint16_t* key_offsets = reinterpret_cast<uint16_t*>(hashes + max_keys);
    byte* indexes = reinterpret_cast<byte*> (key_offsets + max_keys),
        * unsorted_indexes = indexes + max_keys,
        * collission_list = unsorted_indexes + max_keys,
        * cursor;
    char* keys = reinterpret_cast<char*> (table) + size - 1;

    printf ("\n| %3s%10s%8s%10s%10s%10s%10s%11s\n", "i", "key", "offset",
            "hash_e", "hash_u", "hash_s", "index_u", "collisions");
    std::cout << '|';
    for (int i = 0; i < 79; ++i)
        std::cout << '_';
    std::cout << '\n';

    for (int i = 0; i < num_keys; ++i) {
        // Print each record as a row.
        // @todo Change max_keys to num_keys after done debugging.
        collision_index = indexes[i];
        printf ("\n| %3i %9s %7u %9x %9x %9x %9u %10u: ", i,
                keys - key_offsets[i], key_offsets[i],
                Hash16 (keys - key_offsets[i]),
                hashes[unsorted_indexes[i]], hashes[i],
                unsorted_indexes[i], collision_index);

        if ((collision_index != kInvalidIndex) && (i < num_keys)) {
            // Print collisions.
            cursor = &collission_list[collision_index];
            temp = *cursor;
            ++cursor;
            std::cout << temp;
            while (temp != kInvalidIndex) {
                temp = *cursor;
                ++cursor;
                if (temp != kInvalidIndex)
                    printf (", %u", temp);
                
            }
        }

        std::cout << '\n';
    }
    std::cout << '|';
    for (int i = 0; i < 79; ++i)
        std::cout << '_';
    std::cout << '\n';

    PrintMemory (table, table->size);
    std::cout << '\n';
}
#endif  //< USE_MORE_ROM
}       //< namespace _
#endif  //< USING_TABLE

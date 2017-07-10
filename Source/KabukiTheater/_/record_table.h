/** The Chinese Room
    @version 0.x
    @file    /.../record_table.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough] (calemccollough.github.io)

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#ifndef CHINESEROOM_RECORDTABLE_H
#define CHINESEROOM_RECORDTABLE_H

#include "utils.h"
#include "print.h"
#include "error.h"

namespace _ {

/** A dictionary designed to work with Device (s) with less than 192 members.
    
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
       |_______                                            |
       |_______   Collision List N          Hash Table     |
       |_______ ^ ...                        Collision     |
       |_______ | Collision List 0              List       |
       |___________________________________________________|
       |_______                          Element Size = 1  |
       |_______   Buffer Indexes                           |
       |_______                             Hash Table     |
       |_______ ^ Collision Table N          Unsorted      |
       |_______ | ...                         Indexes      |
       |        | Collision table 0                        |
       |___________________________________________________|
       |_______                          Element Size = 1  |
       |_______   Buffer Indexes                           |
       |_______                             Hash Table     |
       |_______ ^ Collision Index N          Collision     |
       |_______ | ...                         Indexes      |
       |        | Collision Index 0                        |
       |___________________________________________________|
       |_______                         Element Size = 2   |
       |_______   Buffer Indexes                           |
       |_______                              16-bit        |
       |_______ ^ Key Offset N                 key         |
       |_______ | ...                        Offsets       |
       |        | Key Offset 1                             |
       |___________________________________________________|
       |_______                         Element Size = 2   |
       |_______   Buffer Indexes                           |
       |_______                              32-bit        |
       |_______ ^ Sorted Hash N              Hashes        |
       |_______ | ...                                      |
       |        | Sorted Hash 1                            |
       |___________________________________________________|
     ^ |                                                   |
     | |                 Header (8 bytes)                  |
    0x0|___________________________________________________|
    @endcode
*/
struct RecordTable {
    byte num_keys,             //< Number of Device members.
        reserved_indexes;      //< The number of buffered indexes.
    uint16_t collisions_size,  //< The size of the collision table.
        size,                  //< The size of this object in bytes.
        reserved_;              //< Reserved for memory alignment.
};

enum {
    kInvalidRecord      = 255,
    kMaxI2PMembers      = 192,
    kNoCollidingRecords = 255, 
    kRecordOverflow     = 254,
    kRecordTableFull    = 253,
    kOverheadPerRecord  = sizeof (hash16_t) + sizeof (uint16_t) + sizeof (byte) + 
        sizeof (byte),
};

static RecordTable* InitRecordTable (RecordTable* rt, byte reserved_indexes, uint16_t total_size)
/** Constructs a streamable hash table with enough buffer space for the 
    reserved_indexes.
    @post Users might want to call the isValid () function after 
        construction to verify the integrity of the object.
    @warning The reservedNumMembers must be aligned to a 32-bit value, and 
        it will get rounded up to the next higher multiple of 4.
*/
{
    uint_t min_required_size = sizeof (RecordTable) + reserved_indexes * 
                                (kOverheadPerRecord + 2);
    if (total_size < min_required_size)
        return nullptr;

    rt->num_keys = 0;
    reserved_indexes = reserved_indexes;
    rt->collisions_size = 0;
    rt->size = total_size;
    return rt;
}

/** Prints this object out to the console. */
static void Print (RecordTable* rt) {
    if (rt == nullptr) return;
    byte num_keys = rt->num_keys,
        reserved_indexes = rt->reserved_indexes,
        collisionIndex,
        temp;
    uint16_t size = rt->size,
        collisions_size = rt->collisions_size;
    PrintLine ('_');
    printf ("| RecordTable: %p\n| num_keys: %u reserved_indexes: %u  "
            "collisions_size: %u  size: %u", rt, num_keys,
            reserved_indexes, collisions_size, size);
    putchar ('\n');
    putchar ('|');
    for (int i = 0; i < 79; ++i) putchar ('_');
    putchar ('\n');

    hash16_t* hashes = reinterpret_cast<hash16_t*>(reinterpret_cast<byte*>(rt) +
                                                   sizeof (RecordTable));
    uint16_t* key_offsets = reinterpret_cast<uint16_t*>(hashes +
                                                        reserved_indexes);
    byte* indexes = reinterpret_cast<byte*> (key_offsets +
                                             reserved_indexes), *ununsorted_indexes = indexes + reserved_indexes,
        *collission_list = ununsorted_indexes + reserved_indexes,
        *currentCollision;
    char*     keys = reinterpret_cast<char*> (rt) + size - 1;

    printf ("| %3s%10s%8s%10s%10s%10s%10s%11s\n", "i", "key", "offset",
            "hash_e", "hash_u", "hash_s", "index_u", "collisions");
    putchar ('|');
    for (int i = 0; i < 79; ++i)
        putchar ('_');
    putchar ('\n');

    for (int i = 0; i < reserved_indexes; ++i) {
        collisionIndex = indexes[i];
        /*
        //printf ("| %i: \"%s\":%u hash: %x:%x sorted:%u_%x ",
        //    i, keys - key_offsets[i], key_offsets[i],
        //    PrimeHash<hash16_t> (keys - key_offsets[i]),
        //    hashes[ununsorted_indexes[i]], ununsorted_indexes[i],
        //    hashes[i], collisionIndex);*/
        printf ("| %3i%10s%8u%10x%10x%10x%10u%11u: ", i, keys -
                key_offsets[i], key_offsets[i], PrimeHash<hash16_t> (keys -
                                                                     key_offsets[i]), hashes[ununsorted_indexes[i]], hashes[i],
                ununsorted_indexes[i], collisionIndex);

        if (collisionIndex != kNoCollidingRecords && i < num_keys) {
            currentCollision = &collission_list[collisionIndex];
            temp = *currentCollision;
            ++currentCollision;
            printf ("%u", temp);
            while (temp != kNoCollidingRecords) {
                temp = *currentCollision;
                ++currentCollision;
                if (temp == kNoCollidingRecords)
                    break;
                printf (", %u", temp);
            }
        }

        putchar ('\n');
    }
    putchar ('|');
    for (int i = 0; i < 79; ++i) putchar ('_');
    putchar ('\n');

    PrintMemory (rt, size);
    putchar ('\n');
}
    
/** Adds the given key and returns the index 64-255 of the index of the 
    member.
    @return Returns an index 64-255
*/
static byte Add (RecordTable* rt, const char* key) {
    if (rt == nullptr) return 0;
    if (key == nullptr) return 0;
    
    PrintStringLine (key);

    byte num_keys = rt->num_keys,
        reserved_indexes = rt->reserved_indexes,
        temp;

    uint16_t size = rt->size;

    if (num_keys >= reserved_indexes) return kRecordTableFull;
    //< We're out of buffered indexes.

    hash16_t* hashes = reinterpret_cast<hash16_t*> (reinterpret_cast<byte*> (rt) +
        sizeof (RecordTable));
    uint16_t* key_offsets = reinterpret_cast<uint16_t*> (hashes + 
        reserved_indexes);
    byte* indexes = reinterpret_cast<byte*> (key_offsets + 
                    reserved_indexes),
        *ununsorted_indexes = indexes + reserved_indexes,
        *collission_list = ununsorted_indexes + reserved_indexes;
    char* keys = reinterpret_cast<char*> (rt) + size - 1,
        *destination;

    //printf ("Offsets:\nhashes: %u key_offsets: %u indexes: %u "
    //    ununsorted_indexes: %u collisionList: %u keys: %u", 
    //    reinterpret_cast<byte*> (hashes) - reinterpret_cast<byte*> (this), 
    //    reinterpret_cast<byte*> (key_offsets) -  
    //    reinterpret_cast<byte*> (this), indexes - 
    //    reinterpret_cast<byte*> (this), ununsorted_indexes - 
    //    reinterpret_cast<byte*> (this), collission_list - 
    //    reinterpret_cast<byte*> (this), keys - 
    //    reinterpret_cast<char*> (this));

    // Calculate space left.
    uint16_t value = size - reserved_indexes * kOverheadPerRecord,
        collisions_size,
        key_length = static_cast<uint16_t> (strlen (key));

    PrintLine ();
    //printf ("Adding Key %s\n%20s: 0x%p\n%20s: %p\n%20s: 0x%p\n"
    //    "%20s: %p\n%20s: %u\n", key, "hashes", hashes, "key_offsets", 
    //    key_offsets, "keys", keys, "indexes", indexes, "value", value);

    hash16_t hash = PrimeHash<hash16_t> (key),
        current_hash;

    if (key_length > value)
    {
        //printf ("Buffer overflow\n");
        return BufferOverflowError;
    }

    //print ();

    if (num_keys == 0)
    {
        num_keys = 1;
        *hashes = hash;
        *key_offsets = static_cast<uint16_t> (key_length);
        *indexes = kNoCollidingRecords;
        *ununsorted_indexes = 0;
        destination = keys - key_length;

        CopyString (destination, key);
        //printf ("Inserted key %s at GetAddress 0x%p\n", key, destination);
        Print (rt);
        return 0;
    }

    // Calculate left over buffer size by looking up last string.

    if (key_length >= value)
    {
        //printf ("Not enough room in buffer!\n");
        return 0;   //< There isn't enough room left in the buffer.
    }

    //printf ("Finding insert location... \n");

    int low = 0,
        mid,
        high = num_keys,
        index;

    byte* temp_ptr;

    while (low <= high)
    {
        mid = (low + high) >> 1;        //< Shift >> 1 to / 2

        current_hash = hashes[mid];
        //printf ("high: %i mid: %i low %i hash: %x\n", high, mid, low, 
        //    current_hash);

        if (current_hash > hash)
        {
            high = mid - 1;
        }
        else if (current_hash < hash)
        {
            low = mid + 1;
        }
        else    // Duplicate hash detected.
        {
            //printf ("hash detected, ");

            // Check for other collisions.

            index = indexes[mid];       //< Index in the collision table.

            //printf ("index:%u\n", index);

            if (index < kNoCollidingRecords)   //< There are other collisions.
            {
                //printf ("with collisions, ");
                // There was a collision so check the table.

                // The collisionsList is a sequence of indexes terminated 
                // by an invalid index > kMaxI2PMembers. collissionsList[0] is 
                // an invalid index, so the collisionsList is searched from
                // lower address up.
                temp = indexes[mid];
                temp_ptr = collission_list + temp;
                index = *temp_ptr;
                while (index < kMaxI2PMembers) {
                    //printf ("comparing \"%s\" to \"%s\"\n", key, 
                    //  keys - key_offsets[index]);
                    if (strcmp (key, keys - key_offsets[index]) == 0) {
                        //printf ("but table already contains key at "
                        //  "offset: %u.\n", index);
                        return index;
                    }
                    ++temp_ptr;
                    index = *temp_ptr;
                }

                // Its a new collision!
                //printf ("and new collision detected.\n");

                value = key_offsets[num_keys - 1] + key_length + 1;
                CopyString (keys - value, key);
                key_offsets[num_keys] = value;

                collisions_size = rt->collisions_size;
                // Shift the collisions table up one element and insert 
                // the unsorted collision index.
                // Then move to the top of the collisions list.
                collission_list += collisions_size;
                // and iterate down to the insert spot
                while (collission_list > temp_ptr)
                {
                    *collission_list = * (collission_list - 1);
                    --collission_list;
                }
                *temp_ptr = num_keys;

                rt->collisions_size = collisions_size + 1;

                // Store the collision index.
                indexes[num_keys] = temp;   //< Temp is storing the 
                num_keys = num_keys + 1;
                hashes[num_keys] = ~0;      //< Set the last hash to 0xff..f

                // Move collisions pointer to the ununsorted_indexes.
                indexes += reserved_indexes;

                //< Add the newest string to the end.
                indexes[num_keys] = num_keys;

                Print (rt);
                return num_keys;
            }

            // But we still don't know if the string is a new collision.

            //printf ("Checking if it's a collision... ");

            if (strcmp (key, keys - key_offsets[index]) != 0) {
                // It's a new collision!
                //printf ("It's a new collision!\n");

                if (value < 3)
                {
                    //printf ("Buffer overflow!\n");
                    return kRecordOverflow;
                }

                value = key_offsets[num_keys - 1] + key_length + 1;

                CopyString (keys - value, key);
                //printf ("$$$ Inserting value: %u into index:%u "
                //    "num_keys:%u\n", value, index, num_keys);
                key_offsets[num_keys] = value;

                collisions_size = rt->collisions_size;

                indexes[mid] = static_cast<byte> (collisions_size);
                indexes[num_keys] = static_cast<byte> (collisions_size);

                // Insert the collision into the collision table.
                temp_ptr = &collission_list[collisions_size];
                *temp_ptr = num_keys;
                ++temp_ptr;
                *temp_ptr = mid;
                ++temp_ptr;
                *temp_ptr = ~0;
                rt->collisions_size = collisions_size + 3;
                //< Added one term-char and two indexes.

                // Move collisions pointer to the ununsorted_indexes.
                indexes += reserved_indexes;
                // Add the newest string at the end.
                indexes[num_keys] = num_keys;

                // Set the last hash to 0xff..f
                hashes[num_keys] = ~0;

                num_keys = num_keys + 1;

                Print (rt);

                // Then it was a collision so the table doesn't contain s.
                return num_keys;
            }
            //printf ("table already contains the key\n");
            return index;
        }
    }

    // The hash was not in the table.

    value = key_offsets[num_keys - 1] + key_length + 1;
    destination = keys - value;

    //printf ("The hash 0x%x was not in the table so inserting %s into mid:"
    //    " %i at index %u before hash 0x%x \n", hash, key, mid, 
    //    destination - reinterpret_cast<char*> (this), hashes[mid]);

    // First copy the string and set the key offset.
    CopyString (destination, key);
    key_offsets[num_keys] = value;

    // Second move up the hashes and insert at the insertion point.

    hash16_t* hash_ptr = hashes + num_keys;
    //*test = hashes;
    //printf ("l_numkeys: %u, hashes: %u hash_ptr: %u insert_ptr: %u\n", 
    //    num_keys, hashes - reinterpret_cast<hash16_t*> (this), 
    //    hash_ptr - reinterpret_cast<hash16_t*> (this), hashes + mid - 
    //    reinterpret_cast<hash16_t*> (this));
    hashes += mid;
    //print ();
    while (hash_ptr > hashes) {
        *hash_ptr = * (hash_ptr - 1);
        --hash_ptr;
    }
    *hashes = hash;

    // There were no collisions so set collisionIndex to zero.
    indexes[num_keys] = kNoCollidingRecords;

    // Move up the sorted indexes and insert the unsorted index (which is 
    // the current num_keys).
    indexes += reserved_indexes + mid;
    temp_ptr = indexes + num_keys;

    while (temp_ptr > indexes) {
        *temp_ptr = * (temp_ptr - 1);
        --temp_ptr;
    }
    *temp_ptr = num_keys;      //ununsorted_indexes[mid] = num_keys;

    num_keys = num_keys + 1;

    /*
    // The table did not contain the has so insert it in the low spot.


    // Now move up the corresponding indexes.

    byte* index_ptr = indexes + num_keys * sizeof (byte*);

    while (index_ptr != indexes)
    {
    *index_ptr = * (index_ptr - 1);
    --index_ptr;
    }*/

    //printf ("\nAfter...");
    Print (rt);
    //printf ("Done inserting.\n");
    PrintLine ();

    return num_keys;
}

/** Attempts to find the given key.
    @return Returns 0 upon failure, and valid index upon success. */
static byte Find (const RecordTable* rt, const char* key) {
    if (rt == nullptr)
        return 0;
    PrintLine ();
    byte index,
        num_keys = rt->num_keys,
        reserved_indexes = rt->reserved_indexes,
        temp;

    if (key == nullptr || num_keys == 0)
        return kInvalidRecord;

    uint16_t size = rt->size;

    const hash16_t* hashes = reinterpret_cast<const hash16_t*> (reinterpret_cast<const byte*> (rt)
                    + sizeof (RecordTable));
    const uint16_t* key_offsets = reinterpret_cast<const uint16_t*>(hashes +
                            reserved_indexes);
    const byte* indexes = reinterpret_cast<const byte*>(key_offsets +
                        reserved_indexes),
        *ununsorted_indexes = indexes + reserved_indexes,
        *collission_list = ununsorted_indexes + reserved_indexes;
    const char* keys = reinterpret_cast<const char*> (rt) + size - 1;
    const byte* collisions,
        *temp_ptr;

    hash16_t hash = PrimeHash<hash16_t> (key);

    //printf ("\nSearching for key \"%s\" with hash 0x%x\n", key, hash);

    if (num_keys == 1)
    {
        //printf ("Comparing keys - key_offsets[0] - this %u\n%s\n", keys - 
        //    key_offsets[0] - reinterpret_cast<char*> (this), keys - 
        //    key_offsets[0]);
        if (strcmp (key, keys - key_offsets[0]) != 0) {
            //printf ("Did not find key %s\n", key);
            return kInvalidRecord;
        }
        //printf ("Found key %s\n", key);
        PrintLine ();
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
        //printf ("low: %i mid: %i high %i hashes[mid]:%x\n", low, mid, 
        //    high, hashes[mid]);

        if (current_hash > hash) {
            high = mid - 1;
        }
        else if (current_hash < hash) {
            low = mid + 1;
        } else {
            // Duplicate hash found.
            //printf ("\nFound same hash at mid:%i hash:%x offset for key: "
            //    "%s\n", mid, hashes[mid], key);

            // Check for collisions

            collisions = reinterpret_cast<const byte*>(key_offsets) +
                reserved_indexes * sizeof (uint16_t);
            index = collisions[mid];

            if (index < kNoCollidingRecords)
            {
                // There was a collision so check the table.
                //printf ("There was a collision so check the table\n");

                // The collisionsList is a sequence of indexes terminated by
                // an invalid index > kMaxI2PMembers. collissionsList[0] is an 
                // invalid index, so the collisionsList is searched from 
                // lower address up.
                temp = indexes[mid];
                temp_ptr = collission_list + temp;
                index = *temp_ptr;
                while (index < kMaxI2PMembers) {
                    //printf ("comparing \"%s\" to \"%s\"\n", key, keys - 
                    //    key_offsets[index]);
                    if (strcmp (key, keys - key_offsets[index]) == 0) {
                        //printf ("but table already contains key at offset:"
                        //    "%u.\n", index);
                        return index;
                    }
                    ++temp_ptr;
                    index = *temp_ptr;
                }
                //printf ("Did not find %s.\n", key);
                return kInvalidRecord;
            }

            // There were no collisions.

            // But we still don't know if the string is new or a collision.

            // Move collisions pointer to the unsorted indexes.
            indexes += reserved_indexes;
            index = ununsorted_indexes[mid];

            //printf ("\n!!!mid: %i-%x ununsorted_indexes: %u key: \"%s\" "
            //    hash: %x\n", mid, hashes[mid], index, keys - 
            //    key_offsets[index], PrimeHash<hash16_t> (keys - 
            //    key_offsets[index]));

            if (strcmp (key, keys - key_offsets[index]) != 0) {
                //< It was a collision so the table doesn't contain s.
                //printf (" but it was a collision and did not find key.\n");
                return kInvalidRecord;
            }

            //printf ("and found key at mid: %i\n", mid);
            return index;
        }
    }
    //printf ("Did not find a hash for key %s\n", key);
    PrintLine ();

    return kInvalidRecord;
}

enum {
    kMinRecordTableSize = 64,
};


/** Creates a RecordTable from new dynamic memory. */
static RecordTable* CreateRecordTable (byte reserved_indexes, uint16_t total_size) {
    RecordTable* rt = New<RecordTable, uint16_t> (total_size, kMinRecordTableSize);
    if (rt == nullptr) return nullptr;
    if (InitRecordTable (rt, reserved_indexes, total_size))
        Destroy (rt);
    return rt;
}

}       //< namespace _

#endif  //< CHINESEROOM_RECORDTABLE_H

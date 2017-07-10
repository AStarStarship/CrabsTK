/** The Chinese Room
    @version 0.x
    @file    /.../Book.h
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

#ifndef CHINESEROOM_BOOK_H
#define CHINESEROOM_BOOK_H

#include "device.h"
#include "record_table.h"

namespace _ {

enum
{
    LockedBitMask      = 1 << 7,    //< Mask for the locked bit.
    ReadLockedBitMask  = 1 << 6,    //< Mask for the read-locked bit.
    WriteLockedBitMask = 1 << 5,    //< Mask for the write-locked bit.
};

/** A collection of key-value dictionaries that might or might not have a hash
    table.
    A book is like a Python dictionary or C++ map, the difference being a Book
    can contain nested Book (s). The key design difference between both Python 
    dictionaries and C++ maps are Books do not contains points, and instead
    works using offsets.

    A book may or may not have a hash table. In order to turn on the hash table,
    simply set the collissionsSize to non-zero in the Book header.

    The memory layout is the same for all of the Book types as depicted below:

    @code
    _____________________________________________________ 
    |_______                                            | 
    |_______   Data N                                   | 
    |_______ ^ ...                       Data Table     | 
    |_______ | Data 0                                   | 
    |___________________________________________________|
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
    |_______                                            |
    |_______   Buffer Indexes                           |
    |_______                             Hash Table     |
    |_______ ^ Collision Index N          Collision     |
    |_______ | ...                         Indexes      |
    |        | Collision Index 0                        |
    |___________________________________________________|
    |_______                                            |
    |_______   Buffer Indexes                           |
    |_______                             Hash Table     |
    |_______ ^ Collision Table N        Corresponding   |
    |_______ | ...                          Hash        |
    |        | Collision table 0           Indexes      |
    |___________________________________________________|
    |_______                                            |
    |_______   Buffer Indexes                           |
    |_______                                            |
    |_______ ^ Sorted Hash N                Hashes      |
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
    |                                                   | ^
    |                    Header                         | |
    |___________________________________________________|0x0
    @endcode

    | Dict | Max Values | % Collisions (p)  |          Overhead             |
    |:----:|:----------:|:----------------:|:-----------------------------:|
    |   2  |     255    |    0.0001        | Ceiling (0.02*p*2^8)  = 2      |
    |   4  |     8192   |      0.1         | Ceiling (0.04*p*2^16) = 327.68 |
    |   8  |    2^32/8  |    10^-16        | Ceiling (0.04*p*2^32) = 327.68 |

    Memory Schema:
    The memory schema is optimized for fast search and push back. When searching
    for a key, there might or might not be a hash table.

    How to calculate size:
    The size of any size book can be calculated as follows:
    size = num_keys_ * (2*sizeof (TIndex) + sizeof (TData)) + collissionSize +

    # State Format
    @code
    ____________________________________________________
    |                State byte                        |
    |__________________________________________________|
    | b7: Loaded | b6: Used | b5: Hidden | b4-b0: Type |
    |____________|__________|____________|_____________|
    @endcode

    # Cache Page Optimizations
    In order to optimize the cache pages, we need to group hot data together.
    ChineseRoom Objects work through calling by TIndex, or by key by using the
    function '\"' (i.e. "foo" is TIndex 44).

    # Hash Table Collisions.
    Because there are no pointers in I2P books, the hash tables are done using
    using a nil-terminated list in the Collision List. In the 

    # Use Case Scenario
    We are creating a plug-in DLL. We need to create a book in the DLL code, and
    pass it over to the program. The DLL manages the memory for the book. This
    book might contain several million entries, and more than 4GB of data.

    ### Why So Many Book Types?
    We are running in RAM, and a book could contain millions of key-value pairs.
    Adding extra bytes would added megabytes of data we don't need. Also, on
    microcontrollers, especially 16-bit ones, will have very little RAM, so we
    need an 16-bit object. It is easy to imagine a complex AI software using
    more than 4GB RAM, or the need to attach a DVD ISO image as a key-value
    pair, so we need a 64-bit book.

    # Design Strengths
    * Uses less memory.
    * Fast push back when within buffer size.
    * Faster inserts on small books when within buffer size.

    # Design Weaknesses
    * Slow insert in large books.
    * Slow at growing large books when buffer runs out.
    * More complicated.

    @code
    ;
    @endcode
*/
template<typename TIndex, typename THeader, typename TData, typename THash>
class Book {
    public:

    enum {
        OverheadPerIndex = sizeof (2*TIndex + THeader + TData + 3)
    };
    
    /** Initializes a Book.
        @post       Users might want to call the isValid () function after
            construction to verify the integrity of the object.
        @warning    The reservedNumMembers must be aligned to a 32-bit value,
            and it will get rounded up to the next higher multiple of 4.
    */
    Book<TIndex, THeader, TData, THash>* init (TIndex numBufferedIndexes,
            uint16_t totalSize)  {
        if (totalSize < sizeof (Book) + numBufferedIndexes *
            (OverheadPerIndex + 2))
            return nullptr;

        num_keys_ = 0;
        buffered_indexes_ = numBufferedIndexes;
        collisions_size_ = 0;
        size = totalSize;
        return this;
    }

    /** Insets the given key-value pair.
    */
    TIndex Insert (byte type, const char* key, void* data, TIndex index) {
        return 0;
    }

    /** Adds a key-value pair. */
    TIndex Add (byte type, const char* key, void* data) {
        PrintStringLine (key);

        if (key == nullptr) return 0;

        TIndex num_keys = num_keys_,
            buffered_indexes = buffered_indexes_,
            TIndex,
            temp;

        THeader l_headerSize = size;
        TData l_dataSize = data_size_;

        if (num_keys >= buffered_indexes) return TableFull;   //< We're out of buffered TIndexes.

        THash* hashes = reinterpret_cast<THash*> (reinterpret_cast<byte*> (this) + sizeof (Book));
        uint16_t* keyOffsets = reinterpret_cast<uint16_t*> (hashes + buffered_indexes);
        byte*     TIndexes = reinterpret_cast<byte*> (keyOffsets + buffered_indexes),
            *ununsorted_indexes = TIndexes + buffered_indexes,
            *collission_list = ununsorted_indexes + buffered_indexes;
        char*     keys = reinterpret_cast<char*> (this) + l_headerSize - 1,
            *destination;

        //printf ("Offsets:\nhashes: %u keyOffsets: %u TIndexes: %u ununsorted_indexes: %u collisionList: %u keys: %u",
        //  reinterpret_cast<byte*> (hashes) - reinterpret_cast<byte*> (this), reinterpret_cast<byte*> (keyOffsets) - reinterpret_cast<byte*> (this),
        //  TIndexes - reinterpret_cast<byte*> (this), ununsorted_indexes - reinterpret_cast<byte*> (this),
        //  collission_list - reinterpret_cast<byte*> (this), keys - reinterpret_cast<char*> (this));

        // Calculate space left.
        THeader value = l_headerSize - buffered_indexes * OverheadPerTIndex,
            l_collisionsSize,
            key_length = static_cast<uint16_t> (strlen (key));

        PrintLine ();
        //printf ("Adding Key %s\n%20s: 0x%p\n%20s: %p\n%20s: 0x%p\n%20s: %p\n%20s: %u\n", key, "hashes", hashes, "keyOffsets", keyOffsets,
        //  "keys", keys, "TIndexes", TIndexes, "value", value);

        PrimeHash hash = PrimeHash<hash32_t> (key),
            current_hash;

        if (key_length > value)
        {
            //printf ("Buffer overflow\n");
            return BufferOverflowError;
        }

        //print ();

        if (num_keys == 0)
        {
            num_keys_ = 1;
            *hashes = hash;
            *keyOffsets = static_cast<uint16_t> (key_length);
            *TIndexes = NoCollisions;
            *ununsorted_indexes = 0;
            destination = keys - key_length;

            copyString (destination, key);
            //printf ("Inserted key %s at GetAddress 0x%p\n", key, destination);
            Print ();
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
            high = num_keys;

        byte* temp_ptr;

        while (low <= high)
        {
            mid = (low + high) >> 1;        //< Shift >> 1 to / 2

            current_hash = hashes[mid];
            //printf ("high: %i mid: %i low %i hash: %x\n", high, mid, low,
            //  current_hash);

            if (current_hash > hash)
            {
                high = mid - 1;
            }
            else if (current_hash < hash)
            {
                low = mid + 1;
            }
            else
            {
                // Duplicate hash detected.

                //printf ("hash detected, ");

                // Check for other collisions.

                TIndex = TIndexes[mid];
                //< This is the TIndex in the collision table.

                                                //printf ("TIndex:%u\n", TIndex);

                if (TIndex < NoCollisions)        // There are other collisions.
                {
                    //printf ("with collisions, ");
                    // There was a collision so check the table.

                    // The collisionsList is a sequence of TIndexes terminated by an invalid TIndex > MaxNumKeys.
                    // collissionsList[0] is an invalid TIndex, so the collisionsList is searched from lower GetAddresses up.
                    temp = TIndexes[mid];
                    temp_ptr = collission_list + temp;
                    TIndex = *temp_ptr;
                    while (TIndex < MaxNumKeys)
                    {
                        //printf ("comparing \"%s\" to \"%s\"\n", key, keys - keyOffsets[TIndex]);
                        if (strcmp (key, keys - keyOffsets[TIndex]) == 0)
                        {
                            //printf ("but table already contains key at offset: %u.\n", TIndex);
                            return TIndex;
                        }
                        ++temp_ptr;
                        TIndex = *temp_ptr;
                    }

                    // Its a new collision!
                    //printf ("and new collision detected.\n");

                    value = keyOffsets[num_keys - 1] + key_length + 1;
                    copyString (keys - value, key);
                    keyOffsets[num_keys] = value;

                    l_collisionsSize = collisions_size_;
                    // Shift the collisions table up one element and insert the unsorted collision TIndex.
                    collission_list += l_collisionsSize;     //< Move to the top of the collisions list.
                    while (collission_list > temp_ptr)        //< And iterate down to the insert spot
                    {
                        *collission_list = * (collission_list - 1);
                        --collission_list;
                    }
                    *temp_ptr = num_keys;

                    collisions_size_ = l_collisionsSize + 1;

                    // Store the collision TIndex.
                    TIndexes[num_keys] = temp;              //< Temp is storing the
                    num_keys_ = num_keys + 1;
                    hashes[num_keys] = ~0;                 //< Set the last hash to 0xff..f

                    TIndexes += buffered_indexes;           //< Move collisions pointer to the ununsorted_indexes.
                    TIndexes[num_keys] = num_keys;         //< The newest string added is at the end.

                    Print ();
                    return num_keys;
                }

                // But we still don't know if the string is a new collision.

                //printf ("Checking if it's a collision... ");

                if (strcmp (key, keys - keyOffsets[TIndex]) != 0)  // It's a new collision!
                {
                    //printf ("It's a new collision!\n");

                    if (value < 3)
                    {
                        //printf ("Buffer overflow!\n");
                        return BufferOverflow;
                    }

                    value = keyOffsets[num_keys - 1] + key_length + 1;

                    copyString (keys - value, key);
                    //printf ("$$$ Inserting value: %u into TIndex:%u num_keys:%u\n", value, TIndex, num_keys);
                    keyOffsets[num_keys] = value;

                    l_collisionsSize = collisions_size_;

                    TIndexes[mid] = static_cast<byte> (l_collisionsSize);
                    TIndexes[num_keys] = static_cast<byte> (l_collisionsSize);

                    // Insert the collision into the collision table.
                    temp_ptr = &collission_list[l_collisionsSize];
                    *temp_ptr = num_keys;
                    ++temp_ptr;
                    *temp_ptr = mid;
                    ++temp_ptr;
                    *temp_ptr = ~0;
                    collisions_size_ = l_collisionsSize + 3;  //< added one term-char and two TIndexes.

                    TIndexes += buffered_indexes;        //< Move collisions pointer to the ununsorted_indexes.
                    TIndexes[num_keys] = num_keys;         //< The newest string added is at the end.

                    hashes[num_keys] = ~0;                 //< Set the last hash to 0xff..f

                    num_keys_ = num_keys + 1;

                    Print ();

                    return num_keys;  //< Then it was a collision so the table doesn't contain s.
                }
                //printf ("table already contains the key\n");
                return TIndex;
            }
        }

        // The hash was not in the table.

        value = keyOffsets[num_keys - 1] + key_length + 1;
        destination = keys - value;

        //printf ("The hash 0x%x was not in the table so inserting %s into mid: %i at TIndex %u before hash 0x%x \n", hash, key, mid,
        //  destination - reinterpret_cast<char*> (this), hashes[mid]);

        // First copy the string and set the key offset.
        copyString (destination, key);
        keyOffsets[num_keys] = value;

        // Second move up the hashes and insert at the insertion point.

        PrimeHash* hash_ptr = hashes + num_keys;
        //*test = hashes;
        //printf ("l_numkeys: %u, hashes: %u hash_ptr: %u insert_ptr: %u\n", num_keys, hashes - reinterpret_cast<THash*> (this),
        //  hash_ptr - reinterpret_cast<THash*> (this), hashes + mid - reinterpret_cast<THash*> (this));
        hashes += mid;
        //print ();
        while (hash_ptr > hashes)
        {
            *hash_ptr = * (hash_ptr - 1);
            --hash_ptr;
        }
        *hashes = hash;

        // There were no collisions so set collisionTIndex to zero.
        TIndexes[num_keys] = NoCollisions;

        // Move up the sorted TIndexes and insert the unsorted TIndex (which is the current num_keys_).
        TIndexes += buffered_indexes + mid;
        temp_ptr = TIndexes + num_keys;

        while (temp_ptr > TIndexes)
        {
            *temp_ptr = * (temp_ptr - 1);
            --temp_ptr;
        }
        *temp_ptr = num_keys;      //ununsorted_indexes[mid] = num_keys;

        num_keys_ = num_keys + 1;

        /*
        // The table did not contain the has so insert it in the low spot.


        // Now move up the corresponding TIndexes.

        byte* TIndex_ptr = TIndexes + num_keys * sizeof (byte*);

        while (TIndex_ptr != TIndexes)
        {
        *TIndex_ptr = * (TIndex_ptr - 1);
        --TIndex_ptr;
        }*/

        //printf ("\nAfter...");
        Print ();
        //printf ("Done inserting.\n");
        PrintLine ();

        return num_keys;
    }

    /** Returns  the given query string in the hash table. */
    TIndex Find (const char* s) {
        PrintLine ();
        TIndex TIndex,
            num_keys = num_keys_,
            buffered_indexes = buffered_indexes_,
            temp;

        if (key == nullptr || num_keys == 0)
            return InvalidTIndex;

        THeader l_headerSize = size;

        THash* hashes = reinterpret_cast<THash*> (reinterpret_cast<byte*> (this) + sizeof (Book));
        uint16_t* keyOffsets = reinterpret_cast<uint16_t*> (hashes + buffered_indexes);
        byte*     TIndexes = reinterpret_cast<byte*> (keyOffsets + buffered_indexes),
            *ununsorted_indexes = TIndexes + buffered_indexes,
            *collission_list = ununsorted_indexes + buffered_indexes;
        char*     keys = reinterpret_cast<char*> (this) + l_headerSize - 1;
        byte*     collisions,
            *temp_ptr;

        hash32_t hash = PrimeHash<hash32_t> (key);

        //printf ("\nSearching for key \"%s\" with hash 0x%x\n", key, hash);

        if (num_keys == 1)
        {
            //printf ("Comparing keys - keyOffsets[0] - this %u\n%s\n", keys - keyOffsets[0] - reinterpret_cast<char*> (this), keys - keyOffsets[0]);
            if (strcmp (key, keys - keyOffsets[0]) != 0)
            {
                //printf ("Did not find key %s\n", key);
                return InvalidTIndex;
            }
            //printf ("Found key %s\n", key);
            PrintLine ();
            return 0;
        }

        // Perform a binary search to find the first instance of the THash the binary search yields.
        int low = 0,
            mid,
            high = num_keys - 1;
        //< If the mid is odd, we need to subtract the sizeof (THash*) in order to get the right pointer GetAddress.

        while (low <= high)
        {
            mid = (low + high) >> 1;    //< >> 1 to /2

            PrimeHash current_hash = hashes[mid];
            //printf ("low: %i mid: %i high %i hashes[mid]:%x\n", low, mid, high, hashes[mid]);

            if (current_hash > hash)
            {
                high = mid - 1;
            }
            else if (current_hash < hash)
            {
                low = mid + 1;
            }
            else
            {
                // Duplicate hash found.
                //printf ("\nFound same hash at mid:%i hash:%x offset for key: %s\n", mid, hashes[mid], key);

                // Check for collisions

                collisions = reinterpret_cast<byte*> (keyOffsets) + buffered_indexes * sizeof (uint16_t);
                TIndex = collisions[mid];

                if (TIndex < NoCollisions)   // There was a collision so check the table.
                {
                    //printf ("There was a collision so check the table\n");

                    // The collisionsList is a sequence of TIndexes terminated by an invalid TIndex > MaxNumKeys.
                    // collissionsList[0] is an invalid TIndex, so the collisionsList is searched from lower GetAddresses up.
                    temp = TIndexes[mid];
                    temp_ptr = collission_list + temp;
                    TIndex = *temp_ptr;
                    while (TIndex < MaxNumKeys)
                    {
                        //printf ("comparing \"%s\" to \"%s\"\n", key, keys - keyOffsets[TIndex]);
                        if (strcmp (key, keys - keyOffsets[TIndex]) == 0)
                        {
                            //printf ("but table already contains key at offset: %u.\n", TIndex);
                            return TIndex;
                        }
                        ++temp_ptr;
                        TIndex = *temp_ptr;
                    }
                    //printf ("Did not find %s.\n", key);
                    return InvalidTIndex;
                }

                // There were no collisions.

                // But we still don't know if the string is new or a collision.
                TIndexes += buffered_indexes; //< Move collisions pointer to the unsorted TIndexes.
                TIndex = ununsorted_indexes[mid];

                //printf ("\n!!!mid: %i-%x ununsorted_indexes: %u key: \"%s\" hash: %x\n", mid, hashes[mid], TIndex,
                //  keys - keyOffsets[TIndex], PrimeHash<hash32_t> (keys - keyOffsets[TIndex]));

                if (strcmp (key, keys - keyOffsets[TIndex]) != 0)
                {
                    //printf (" but it was a collision and did not find key.\n");
                    return InvalidTIndex;  //< Then it was a collision so the table doesn't contain s.
                }

                //printf ("and found key at mid: %i\n", mid);
                return TIndex;
            }
        }
        //printf ("Did not find a hash for key %s\n", key);
        PrintLine ();

        return InvalidTIndex;
    }

    /** Gets the size_t of the object being stored. */
    TIndex GetCount () {
        return num_keys_;
    }

    /** Gets the size of the object being stored. */
    TIndex GetBufferCount () {
        return buffered_indexes_;
    }

    /** Prints this object out to the console. */
    void Print () {
        byte num_keys = num_keys_,
            buffered_indexes = buffered_indexes_,
            collisionTIndex,
            temp;
        uint16_t l_headerSize = size;
        PrintLine ('_');
        printf ("| Book: %p\n| num_keys_: %u buffered_indexes_: %u  collisions_size_: %u  size: %u",
            this, num_keys_, buffered_indexes, collisions_size_, size);
        putchar ('\n');
        putchar ('|');
        for (int i = 0; i < 79; ++i) putchar ('_');
        putchar ('\n');

        THash* hashes = reinterpret_cast<THash*> (reinterpret_cast<byte*> (this) + sizeof (Book));
        THeader* keyOffsets = reinterpret_cast<THeader*> (hashes + buffered_indexes);
        TIndex*  TIndexes = reinterpret_cast<TIndex*> (keyOffsets + buffered_indexes),
            *ununsorted_indexes = TIndexes + buffered_indexes,
            *collission_list = ununsorted_indexes + buffered_indexes,
            *currentCollision;
        char*     keys = reinterpret_cast<char*> (this) + l_headerSize - 1;

        printf ("| %3s%10s%8s%10s%10s%10s%10s%11s\n", "i", "key", "offset", "hash_e", "hash_u", "hash_s", "TIndex_u", "collisions");
        putchar ('|');
        for (int i = 0; i < 79; ++i) putchar ('_');
        putchar ('\n');

        for (int i = 0; i < buffered_indexes; ++i)
        {
            collisionTIndex = TIndexes[i];
            /*
            //printf ("| %i: \"%s\":%u hash: %x:%x sorted:%u_%x ",
            //  i, keys - keyOffsets[i], keyOffsets[i], PrimeHash<hash32_t> (keys - keyOffsets[i]),
            //  hashes[ununsorted_indexes[i]], ununsorted_indexes[i], hashes[i], collisionTIndex);*/
            printf ("| %3i%10s%8u%10x%10x%10x%10u%11u: ", i, keys - keyOffsets[i], keyOffsets[i], PrimeHash<hash32_t> (keys - keyOffsets[i]),
                hashes[ununsorted_indexes[i]], hashes[i], ununsorted_indexes[i], collisionTIndex);

            if (collisionTIndex != NoCollisions && i < num_keys)
            {
                currentCollision = &collission_list[collisionTIndex];
                temp = *currentCollision;
                ++currentCollision;
                printf ("%u", temp);
                while (temp != NoCollisions)
                {
                    temp = *currentCollision;
                    ++currentCollision;
                    if (temp == NoCollisions) break;
                    printf (", %u", temp);
                }
            }

            putchar ('\n');
        }
        putchar ('|');
        for (int i = 0; i < 79; ++i) putchar ('_');
        putchar ('\n');

        PrintMemory (this, size);
        putchar ('\n');
    }

    /** Deletes the book contents by overwriting it with zeros. */
    void Wipe () {
    }

    /** Deletes the book contents without wiping the contents. */
    void Clear () {
    }

    /** Returns true if this dictionary contains only the given address. */
    bool Contains (void* data) {
        if (data < this) return false;
        if (data > GetEndAddress()) return false;
        return true;
    }

    /** Returns true of this Collection is empty. */
    bool IsEmpty () {
        return GetCount () == 0;
    }

    /** Removes that object from the book. */
    bool RemoveCopy (void* data) {
        return false;
    }

    /** Removes the object at the given GetAddress from the book. */
    bool Remove (void* adress) {
        return false;
    }

    /** Removes all but the given collection from the book. */
    bool Retain () {
        return false;
    }

    static const char* GetVersion () { return kChineseRoomVersion; }

    private:

    TIndex num_keys_,           //< Number of Device members.
        num_indexes_;           //< The number of buffered items.
    THeader collisions_size_,   //< The size of the (optional) collisions table.
        header_size_;           //< The size of the key strings in bytes.
    TData data_size_;           //< The total size of the folder.
};

using Dictionary = Book<TBookIndex, TBookHeader, TBookDataOffset, TBookHash>;


class Book2 : public Device {
    public:

    Book2 (Book<byte, uint16_t, uint16_t, hash32_t>* book)
    :   book_ (book)
    {

    }

    virtual Member* Op (Rx* rx, Tx& tx, char index) {
        if (book_ == nullptr) return nullptr;

        return nullptr;
    }

    private:

    Book<byte, uint16_t, uint16_t, hash32_t>* book_;
};
using Book4 = Book<uint16_t, uint16_t, uint32_t, hash32_t>;
using Book8 = Book<uint32_t, uint32_t, uint64_t, hash64_t>;



enum
{
    MaxNumPagesBook2 = 255,
    MaxNumPagesBook4 = 8 * 1024,
    MaxNumPagesBook8 = 32 * 1024 * 1024,
};

/** Creates a book from dynamic memory. */
template<typename TIndex, typename TOffset, typename TData, typename THash>
inline Book<TIndex, TOffset, TData, THash>* CreateBook (TIndex buffered_indexes, 
                                                        TData header_size, 
                                                        TData data_size_) {
    Book<TIndex, TOffset, TData, THash>* book = New<Book, uint_t> ()
}

}       //< namespace _
#endif  //< CHINESEROOM_BOOK_H

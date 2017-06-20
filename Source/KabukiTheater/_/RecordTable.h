/** The Chinese Room
    @version 0.x
    @file    /.../RecordTable.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright(C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved(R).

        Licensed under the Apache License, Version 2.0(the "License"); you may
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

#include "IDevice.h"

namespace _ {

template<typename THash>
inline hash32_t hashChar (char c, THash hash)
/*< Hashes a single char. */
{
    return hash * (101 + c);
}

template<typename THash>
inline hash32_t hashString32(const char* s, THash hash = HashSeed)
/*< Hashes the given string using the hashChar function. */
{
    char c = *s;
    while (c)
    {
        hash = hashChar<THash> (c, hash);
        ++s;
        c = *s;
    }
    return hash;
}

template<typename THash>
class RecordTable
/** A dictionary designed to work with Device(s) with less than 192 members.
    
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
     | |                  Header(8 bytes)                  |
    0x0|___________________________________________________|
    @endcode
*/
{
    NONCOPYABLE(RecordTable<THash>)

    public:
    
    typedef enum {
        InvalidIndex        = 255,
        MaxNumKeys          = 192,
        NoCollisions        = 255,
        BufferOverflow      = 254,
        TableFull           = 253,
        OverheadPerIndex    = sizeof(THash) + sizeof(uint16_t) + sizeof(byte) + sizeof(byte),
    } State;
    
    RecordTable<1> ()
    /*< Default constructor. */
    {

    }

    RecordTable* init(byte reserveNumMembers, uint16_t totalSize)
    /*< Constructs a streamable hash table with enough buffer space for the 
        reserveNumMembers.
        @post Users might want to call the isValid() function after 
            construction to verify the integrity of the object.
        @warning The reservedNumMembers must be aligned to a 32-bit value, and it will get rounded 
            up to the next higher multiple of 4.
    */
    {
        if (totalSize < sizeof (RecordTable) + reservedIndexes * (OverheadPerIndex + 2))
            return nullptr;

        numKeys = 0;
        reservedIndexes = reservedIndexes;
        collisionsSize = 0;
        size = totalSize;
        return this;
    }

    byte add(const char* key)
    /*< Adds the given key and returns the index 64-255 of the index of the member.
        @return Returns an index 64-255
    */
    {
        printStringLine (key);

        if (key == nullptr) return 0;

        byte l_numKeys = numKeys,
            l_reservedIndexes = reservedIndexes,
            temp;

        uint16_t l_size = size;

        if (l_numKeys >= l_reservedIndexes) return TableFull;   //< We're out of buffered indexes.

        THash* hashes = reinterpret_cast<THash*>(reinterpret_cast<byte*>(this) + sizeof (RecordTable));
        uint16_t* keyOffsets = reinterpret_cast<uint16_t*>(hashes + l_reservedIndexes);
        byte*     indexes = reinterpret_cast<byte*>(keyOffsets + l_reservedIndexes),
            *ununsortedIndexes = indexes + l_reservedIndexes,
            *collissionList = ununsortedIndexes + l_reservedIndexes;
        char*     keys = reinterpret_cast<char*>(this) + l_size - 1,
            *destination;

        //printf("Offsets:\nhashes: %u keyOffsets: %u indexes: %u ununsortedIndexes: %u collisionList: %u keys: %u", 
        //    reinterpret_cast<byte*>(hashes) - reinterpret_cast<byte*>(this), reinterpret_cast<byte*>(keyOffsets) - reinterpret_cast<byte*>(this), 
        //    indexes - reinterpret_cast<byte*>(this), ununsortedIndexes - reinterpret_cast<byte*>(this), 
        //    collissionList - reinterpret_cast<byte*>(this), keys - reinterpret_cast<char*>(this));

        //! Calculate space left.
        uint16_t value = l_size - l_reservedIndexes * OverheadPerIndex,
            l_collisionsSize,
            keyLength = static_cast<uint16_t>(strlen (key));

        printLine ();
        //printf("Adding Key %s\n%20s: 0x%p\n%20s: %p\n%20s: 0x%p\n%20s: %p\n%20s: %u\n", key, "hashes", hashes, "keyOffsets", keyOffsets, 
        //    "keys", keys, "indexes", indexes, "value", value);

        THash hash = hashString32 (key),
            currentHash;

        if (keyLength > value)
        {
            //printf("Buffer overflow\n");
            return BufferOverflowError;
        }

        //print();

        if (l_numKeys == 0)
        {
            numKeys = 1;
            *hashes = hash;
            *keyOffsets = static_cast<uint16_t>(keyLength);
            *indexes = NoCollisions;
            *ununsortedIndexes = 0;
            destination = keys - keyLength;

            copyString (destination, key);
            //printf("Inserted key %s at address 0x%p\n", key, destination);
            print ();
            return 0;
        }

        //! Calculate left over buffer size by looking up last string.

        if (keyLength >= value)
        {
            //printf("Not enough room in buffer!\n");
            return 0;   //< There isn't enough room left in the buffer.
        }

        //printf("Finding insert location... \n");

        int low = 0,
            mid,
            high = l_numKeys,
            index;

        byte* temp_ptr;

        while (low <= high)
        {
            mid = (low + high) >> 1;        //< Shift >> 1 to / 2

            currentHash = hashes[mid];
            //printf("high: %i mid: %i low %i hash: %x\n", high, mid, low, currentHash);

            if (currentHash > hash)
            {
                high = mid - 1;
            }
            else if (currentHash < hash)
            {
                low = mid + 1;
            }
            else    //! Duplicate hash detected.
            {
                //printf("hash detected, ");

                //! Check for other collisions.

                index = indexes[mid];   //< This is the index in the collision table.

                //printf("index:%u\n", index);

                if (index < NoCollisions)        //! There are other collisions.
                {
                    //printf("with collisions, ");
                    //! There was a collision so check the table.

                    //! The collisionsList is a sequence of indexes terminated 
                    //! by an invalid index > MaxNumKeys. collissionsList[0] is 
                    //! an invalid index, so the collisionsList is searched from
                    //! lower addresses up.
                    temp = indexes[mid];
                    temp_ptr = collissionList + temp;
                    index = *temp_ptr;
                    while (index < MaxNumKeys)
                    {
                        //printf("comparing \"%s\" to \"%s\"\n", key, 
                        //    keys - keyOffsets[index]);
                        if (strcmp (key, keys - keyOffsets[index]) == 0)
                        {
                            //printf("but table already contains key at offset:"
                            //    " %u.\n", index);
                            return index;
                        }
                        ++temp_ptr;
                        index = *temp_ptr;
                    }

                    //! Its a new collision!
                    //printf("and new collision detected.\n");

                    value = keyOffsets[l_numKeys - 1] + keyLength + 1;
                    copyString (keys - value, key);
                    keyOffsets[l_numKeys] = value;

                    l_collisionsSize = collisionsSize;
                    //! Shift the collisions table up one element and insert the unsorted collision index.
                    collissionList += l_collisionsSize;   //< Move to the top of the collisions list.
                    while (collissionList > temp_ptr)   //< And iterate down to the insert spot
                    {
                        *collissionList = *(collissionList - 1);
                        --collissionList;
                    }
                    *temp_ptr = l_numKeys;

                    collisionsSize = l_collisionsSize + 1;

                    //! Store the collision index.
                    indexes[l_numKeys] = temp;              //< Temp is storing the 
                    numKeys = l_numKeys + 1;
                    hashes[l_numKeys] = ~0;                 //< Set the last hash to 0xff..f

                    indexes += l_reservedIndexes;        //< Move collisions pointer to the ununsortedIndexes.
                    indexes[l_numKeys] = l_numKeys;         //< The newest string added is at the end.

                    print ();
                    return l_numKeys;
                }

                //! But we still don't know if the string is a new collision.

                //printf("Checking if it's a collision... ");

                if (strcmp (key, keys - keyOffsets[index]) != 0)  //! It's a new collision!
                {
                    //printf("It's a new collision!\n");

                    if (value < 3)
                    {
                        //printf("Buffer overflow!\n");
                        return BufferOverflow;
                    }

                    value = keyOffsets[l_numKeys - 1] + keyLength + 1;

                    copyString (keys - value, key);
                    //printf("$$$ Inserting value: %u into index:%u l_numKeys:%u\n", value, index, l_numKeys);
                    keyOffsets[l_numKeys] = value;

                    l_collisionsSize = collisionsSize;

                    indexes[mid] = static_cast<byte>(l_collisionsSize);
                    indexes[l_numKeys] = static_cast<byte>(l_collisionsSize);

                    //! Insert the collision into the collision table.
                    temp_ptr = &collissionList[l_collisionsSize];
                    *temp_ptr = l_numKeys;
                    ++temp_ptr;
                    *temp_ptr = mid;
                    ++temp_ptr;
                    *temp_ptr = ~0;
                    collisionsSize = l_collisionsSize + 3;  //< added one term-char and two indexes.

                    indexes += l_reservedIndexes;        //< Move collisions pointer to the ununsortedIndexes.
                    indexes[l_numKeys] = l_numKeys;         //< The newest string added is at the end.

                    hashes[l_numKeys] = ~0;                 //< Set the last hash to 0xff..f

                    numKeys = l_numKeys + 1;

                    print ();

                    return l_numKeys;  //< Then it was a collision so the table doesn't contain s.
                }
                //printf("table already contains the key\n");
                return index;
            }
        }

        //! The hash was not in the table.

        value = keyOffsets[l_numKeys - 1] + keyLength + 1;
        destination = keys - value;

        //printf("The hash 0x%x was not in the table so inserting %s into mid: %i at index %u before hash 0x%x \n", hash, key, mid, 
        //    destination - reinterpret_cast<char*>(this), hashes[mid]);

        //! First copy the string and set the key offset.
        copyString (destination, key);
        keyOffsets[l_numKeys] = value;

        //! Second move up the hashes and insert at the insertion point.

        THash* hash_ptr = hashes + l_numKeys;
        //*test = hashes;
        //printf("l_numkeys: %u, hashes: %u hash_ptr: %u insert_ptr: %u\n", l_numKeys, hashes - reinterpret_cast<THash*>(this), 
        //    hash_ptr - reinterpret_cast<THash*>(this), hashes + mid - reinterpret_cast<THash*>(this));
        hashes += mid;
        //print();
        while (hash_ptr > hashes)
        {
            *hash_ptr = *(hash_ptr - 1);
            --hash_ptr;
        }
        *hashes = hash;

        //! There were no collisions so set collisionIndex to zero.
        indexes[l_numKeys] = NoCollisions;

        //! Move up the sorted indexes and insert the unsorted index(which is the current numKeys).
        indexes += l_reservedIndexes + mid;
        temp_ptr = indexes + l_numKeys;

        while (temp_ptr > indexes)
        {
            *temp_ptr = *(temp_ptr - 1);
            --temp_ptr;
        }
        *temp_ptr = l_numKeys;      //ununsortedIndexes[mid] = l_numKeys;

        numKeys = l_numKeys + 1;

        /*
        //! The table did not contain the has so insert it in the low spot.


        //! Now move up the corresponding indexes.

        byte* index_ptr = indexes + l_numKeys * sizeof(byte*);

        while(index_ptr != indexes)
        {
        *index_ptr = *(index_ptr - 1);
        --index_ptr;
        }*/

        //printf("\nAfter...");
        print ();
        //printf("Done inserting.\n");
        printLine ();

        return l_numKeys;
    }

    byte find(const char* key)
    /*< Returns  the given query string in the hash table. */
    {
        printLine ();
        byte index,
            l_numKeys = numKeys,
            l_reservedIndexes = reservedIndexes,
            temp;

        if (key == nullptr || l_numKeys == 0)
            return InvalidIndex;

        uint16_t l_size = size;

        THash* hashes = reinterpret_cast<THash*>(reinterpret_cast<byte*>(this) + sizeof (RecordTable));
        uint16_t* keyOffsets = reinterpret_cast<uint16_t*>(hashes + l_reservedIndexes);
        byte*     indexes = reinterpret_cast<byte*>(keyOffsets + l_reservedIndexes),
            *ununsortedIndexes = indexes + l_reservedIndexes,
            *collissionList = ununsortedIndexes + l_reservedIndexes;
        char*     keys = reinterpret_cast<char*>(this) + l_size - 1;
        byte*     collisions,
            *temp_ptr;

        THash hash = hashString32 (key);

        //printf("\nSearching for key \"%s\" with hash 0x%x\n", key, hash);

        if (l_numKeys == 1)
        {
            //printf("Comparing keys - keyOffsets[0] - this %u\n%s\n", keys - keyOffsets[0] - reinterpret_cast<char*>(this), keys - keyOffsets[0]);
            if (strcmp (key, keys - keyOffsets[0]) != 0)
            {
                //printf("Did not find key %s\n", key);
                return InvalidIndex;
            }
            //printf("Found key %s\n", key);
            printLine ();
            return 0;
        }

        //! Perform a binary search to find the first instance of the hash the binary search yields.
        int low = 0,
            mid,
            high = l_numKeys - 1;
        //< If the mid is odd, we need to subtract the sizeof(THash*) in order to get the right pointer address.

        while (low <= high)
        {
            mid = (low + high) >> 1;    //< >> 1 to /2

            THash currentHash = hashes[mid];
            //printf("low: %i mid: %i high %i hashes[mid]:%x\n", low, mid, high, hashes[mid]);

            if (currentHash > hash)
            {
                high = mid - 1;
            }
            else if (currentHash < hash)
            {
                low = mid + 1;
            }
            else
            {
                //! Duplicate hash found.
                //printf("\nFound same hash at mid:%i hash:%x offset for key: %s\n", mid, hashes[mid], key);

                //! Check for collisions

                collisions = reinterpret_cast<byte*>(keyOffsets) + l_reservedIndexes * sizeof (uint16_t);
                index = collisions[mid];

                if (index < NoCollisions)   //! There was a collision so check the table.
                {
                    //printf("There was a collision so check the table\n");

                    //! The collisionsList is a sequence of indexes terminated by an invalid index > MaxNumKeys.
                    //! collissionsList[0] is an invalid index, so the collisionsList is searched from lower addresses up.
                    temp = indexes[mid];
                    temp_ptr = collissionList + temp;
                    index = *temp_ptr;
                    while (index < MaxNumKeys)
                    {
                        //printf("comparing \"%s\" to \"%s\"\n", key, keys - keyOffsets[index]);
                        if (strcmp (key, keys - keyOffsets[index]) == 0)
                        {
                            //printf("but table already contains key at offset: %u.\n", index);
                            return index;
                        }
                        ++temp_ptr;
                        index = *temp_ptr;
                    }
                    //printf("Did not find %s.\n", key);
                    return InvalidIndex;
                }

                //! There were no collisions.

                //! But we still don't know if the string is new or a collision.
                indexes += l_reservedIndexes; //< Move collisions pointer to the unsorted indexes.
                index = ununsortedIndexes[mid];

                //printf("\n!!!mid: %i-%x ununsortedIndexes: %u key: \"%s\" hash: %x\n", mid, hashes[mid], index, 
                //    keys - keyOffsets[index], hashString32(keys - keyOffsets[index]));

                if (strcmp (key, keys - keyOffsets[index]) != 0)
                {
                    //printf(" but it was a collision and did not find key.\n");
                    return InvalidIndex;  //< Then it was a collision so the table doesn't contain s.
                }

                //printf("and found key at mid: %i\n", mid);
                return index;
            }
        }
        //printf("Did not find a hash for key %s\n", key);
        printLine ();

        return InvalidIndex;
    }

    byte getCount()
    /*< Gets the number of key-value pairs. */
    {
        return numKeys;
    }

    byte getBufferCount()
    /*< Gets the number of buffered key-value pairs. */
    {
        return reservedIndexes;
    }

    void print()
    /*< Prints this object out to the console. */
    {
        byte l_numKeys = numKeys,
            l_reservedIndexes = reservedIndexes,
            collisionIndex,
            temp;
        uint16_t l_size = size;
        printLine ('_');
        printf ("| RecordTable: %p\n| numKeys: %u reservedIndexes: %u  collisionsSize: %u  size: %u",
            this, numKeys, l_reservedIndexes, collisionsSize, size);
        putchar ('\n');
        putchar ('|');
        for (int i = 0; i < 79; ++i) putchar ('_');
        putchar ('\n');

        THash* hashes = reinterpret_cast<THash*>(reinterpret_cast<byte*>(this) + sizeof (RecordTable));
        uint16_t* keyOffsets = reinterpret_cast<uint16_t*>(hashes + l_reservedIndexes);
        byte*     indexes = reinterpret_cast<byte*>(keyOffsets + l_reservedIndexes),
            *ununsortedIndexes = indexes + l_reservedIndexes,
            *collissionList = ununsortedIndexes + l_reservedIndexes,
            *currentCollision;
        char*     keys = reinterpret_cast<char*>(this) + l_size - 1;

        printf ("| %3s%10s%8s%10s%10s%10s%10s%11s\n", "i", "key", "offset", "hash_e", "hash_u", "hash_s", "index_u", "collisions");
        putchar ('|');
        for (int i = 0; i < 79; ++i) putchar ('_');
        putchar ('\n');

        for (int i = 0; i < l_reservedIndexes; ++i)
        {
            collisionIndex = indexes[i];
            /*
            //printf("| %i: \"%s\":%u hash: %x:%x sorted:%u_%x ",
            //    i, keys - keyOffsets[i], keyOffsets[i], hashString32(keys - keyOffsets[i]),
            //    hashes[ununsortedIndexes[i]], ununsortedIndexes[i], hashes[i], collisionIndex);*/
            printf ("| %3i%10s%8u%10x%10x%10x%10u%11u: ", i, keys - keyOffsets[i], keyOffsets[i], hashString32 (keys - keyOffsets[i]),
                hashes[ununsortedIndexes[i]], hashes[i], ununsortedIndexes[i], collisionIndex);

            if (collisionIndex != NoCollisions && i < l_numKeys)
            {
                currentCollision = &collissionList[collisionIndex];
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

        printMemory (this, size);
        putchar ('\n');
    }

    private:

    byte numKeys,               //< Number of IDevice members.
        reservedIndexes;        //< The number of buffered indexes.
    uint16_t collisionsSize,    //< The size of the collision table.
        size,                   //< The size of this object in bytes.
        resesrved;              //< Reserved for memory alignment.

};


template<typename THash>
RecordTable<THash>* createRecordTable(byte reservedIndexes, uint16_t totalSize)
/*< Creates a RecordTable from new dynamic memory. */
{
    byte* buffer = nullptr;
    try
    {
        buffer = new byte[totalSize];
        #if DEBUG_CHINESEROOM
        memset (buffer, 0, totalSize);
        #endif
        RecordTable<THash>* t = new(buffer) RecordTable<THash> ();
        if (!t->init (reservedIndexes, totalSize))
        {
            delete[] buffer;
            return nullptr;
        }
        return t;
    }
    catch (...)
    {
        if (buffer == nullptr) delete[] buffer;
        return nullptr;
    }
    return nullptr;
}

template<typename THash>
inline void destroy(RecordTable<THash>* t)
/*< Destroys the given object. */
{
    if (t == nullptr) return;
    delete[] reinterpret_cast<byte*>(t);
}

}       //< namespace _

#endif  //< CHINESEROOM_RECORDTABLE_H

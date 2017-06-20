/** The Chinese Room
    @version 0.x
    @file    /.../Book.h
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

#ifndef CHINESEROOM_BOOK_H
#define CHINESEROOM_BOOK_H

#include "Config.h"

#include "IDevice.h"
#include "RecordTable.h"

namespace _ {

enum
{
    LockedBitMask      = 1 << 7,    //< Mask for the locked bit.
    ReadLockedBitMask  = 1 << 6,    //< Mask for the read-locked bit.
    WriteLockedBitMask = 1 << 5,    //< Mask for the write-locked bit.
};

inline hash64_t hashString64(const char* s, hash64_t seed = 3)
/*< Hashes the given string. */
{
    char asciiValue = *s;
    while (asciiValue)
    {
        seed *= 101 + asciiValue;
        asciiValue = *(++s);
    }

    return seed;
}

template<typename TIndex, typename THeader, typename TData,
    typename THash>
class Book
/*< A collection of key-value dictionaries that might or might not have a hash
    table.
    A book is like a Python dictionary or C++ map, the difference being a Book
    can contain nested Book(s). The key design difference between both Python 
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
     ^ |                                                   |
     | |                    Header                         |
    0x0|___________________________________________________|
    @endcode

    | Dict | Max Values | % Collisions(p)  |          Overhead             |
    |:----:|:----------:|:----------------:|:-----------------------------:|
    |   2  |     255    |    0.0001        | Ceiling(0.02*p*2^8)  = 2      |
    |   4  |     8192   |      0.1         | Ceiling(0.04*p*2^16) = 327.68 |
    |   8  |    2^32/8  |    10^-16        | Ceiling(0.04*p*2^32) = 327.68 |

    Memory Schema:
    The memory schema is optimized for fast search and push back. When searching
    for a key, there might or might not be a hash table.

    How to calculate size:
    The size of any size book can be calculated as follows:
    size = numKeys *(2*sizeof(TIndex) + sizeof(TData)) + collissionSize +

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
    function '\"'(i.e. "foo" is TIndex 44).

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
{
    public:

    enum {
        OverheadPerIndex = sizeof (2*TIndex + THeader + TData + 3)
    };

    Book<TIndex, THeader, TData, THash>* init(byte numBufferedIndexes,
            uint16_t totalSize)
    /*< Initializes a Book.
        @post       Users might want to call the isValid() function after
            construction to verify the integrity of the object.
        @warning    The reservedNumMembers must be aligned to a 32-bit value,
            and it will get rounded up to the next higher multiple of 4.
    */
    {
        if (totalSize < sizeof (Book) + numBufferedIndexes *
                (OverheadPerIndex + 2))
            return nullptr;

        numKeys = 0;
        bufferedIndexes = numBufferedIndexes;
        collisionsSize = 0;
        size = totalSize;
        return this;
    }

    TIndex insert(byte type, const char* key, void* data, TIndex index)
    /*< Insets the given key-value pair.
    */

    TIndex add(byte type, const char* key, void* data)
    /*< Adds a key-value pair. */
    {
        printStringLine (key);

        if (key == nullptr) return 0;

        TIndex l_numKeys = numKeys,
            l_reservedTIndexes = bufferedIndexes,
            TIndex,
            temp;

        THeader l_headerSize = size;
        TData l_dataSize = dataSize;

        if (l_numKeys >= l_reservedTIndexes) return TableFull;   //< We're out of buffered TIndexes.

        hash32_t* hashes = reinterpret_cast<hash32_t*>(reinterpret_cast<byte*>(this) + sizeof (Book));
        uint16_t* keyOffsets = reinterpret_cast<uint16_t*>(hashes + l_reservedTIndexes);
        byte*     TIndexes = reinterpret_cast<byte*>(keyOffsets + l_reservedTIndexes),
            *ununsortedTIndexes = TIndexes + l_reservedTIndexes,
            *collissionList = ununsortedTIndexes + l_reservedTIndexes;
        char*     keys = reinterpret_cast<char*>(this) + l_headerSize - 1,
            *destination;

        //printf("Offsets:\nhashes: %u keyOffsets: %u TIndexes: %u ununsortedTIndexes: %u collisionList: %u keys: %u",
        //    reinterpret_cast<byte*>(hashes) - reinterpret_cast<byte*>(this), reinterpret_cast<byte*>(keyOffsets) - reinterpret_cast<byte*>(this),
        //    TIndexes - reinterpret_cast<byte*>(this), ununsortedTIndexes - reinterpret_cast<byte*>(this),
        //    collissionList - reinterpret_cast<byte*>(this), keys - reinterpret_cast<char*>(this));

        //! Calculate space left.
        THeader value = l_headerSize - l_reservedTIndexes * OverheadPerTIndex,
            l_collisionsSize,
            keyLength = static_cast<uint16_t>(strlen (key));

        printLine ();
        //printf("Adding Key %s\n%20s: 0x%p\n%20s: %p\n%20s: 0x%p\n%20s: %p\n%20s: %u\n", key, "hashes", hashes, "keyOffsets", keyOffsets,
        //    "keys", keys, "TIndexes", TIndexes, "value", value);

        hash32_t hash = hashString32 (key),
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
            *TIndexes = NoCollisions;
            *ununsortedTIndexes = 0;
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
            high = l_numKeys;

        byte* temp_ptr;

        while (low <= high)
        {
            mid = (low + high) >> 1;        //< Shift >> 1 to / 2

            currentHash = hashes[mid];
            //printf("high: %i mid: %i low %i hash: %x\n", high, mid, low,
            //    currentHash);

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
                //! Duplicate hash detected.

                //printf("hash detected, ");

                //! Check for other collisions.

                TIndex = TIndexes[mid];
                //< This is the TIndex in the collision table.

                                                //printf("TIndex:%u\n", TIndex);

                if (TIndex < NoCollisions)        //! There are other collisions.
                {
                    //printf("with collisions, ");
                    //! There was a collision so check the table.

                    //! The collisionsList is a sequence of TIndexes terminated by an invalid TIndex > MaxNumKeys.
                    //! collissionsList[0] is an invalid TIndex, so the collisionsList is searched from lower addresses up.
                    temp = TIndexes[mid];
                    temp_ptr = collissionList + temp;
                    TIndex = *temp_ptr;
                    while (TIndex < MaxNumKeys)
                    {
                        //printf("comparing \"%s\" to \"%s\"\n", key, keys - keyOffsets[TIndex]);
                        if (strcmp (key, keys - keyOffsets[TIndex]) == 0)
                        {
                            //printf("but table already contains key at offset: %u.\n", TIndex);
                            return TIndex;
                        }
                        ++temp_ptr;
                        TIndex = *temp_ptr;
                    }

                    //! Its a new collision!
                    //printf("and new collision detected.\n");

                    value = keyOffsets[l_numKeys - 1] + keyLength + 1;
                    copyString (keys - value, key);
                    keyOffsets[l_numKeys] = value;

                    l_collisionsSize = collisionsSize;
                    //! Shift the collisions table up one element and insert the unsorted collision TIndex.
                    collissionList += l_collisionsSize;     //< Move to the top of the collisions list.
                    while (collissionList > temp_ptr)        //< And iterate down to the insert spot
                    {
                        *collissionList = *(collissionList - 1);
                        --collissionList;
                    }
                    *temp_ptr = l_numKeys;

                    collisionsSize = l_collisionsSize + 1;

                    //! Store the collision TIndex.
                    TIndexes[l_numKeys] = temp;              //< Temp is storing the
                    numKeys = l_numKeys + 1;
                    hashes[l_numKeys] = ~0;                 //< Set the last hash to 0xff..f

                    TIndexes += l_reservedTIndexes;           //< Move collisions pointer to the ununsortedTIndexes.
                    TIndexes[l_numKeys] = l_numKeys;         //< The newest string added is at the end.

                    print ();
                    return l_numKeys;
                }

                //! But we still don't know if the string is a new collision.

                //printf("Checking if it's a collision... ");

                if (strcmp (key, keys - keyOffsets[TIndex]) != 0)  //! It's a new collision!
                {
                    //printf("It's a new collision!\n");

                    if (value < 3)
                    {
                        //printf("Buffer overflow!\n");
                        return BufferOverflow;
                    }

                    value = keyOffsets[l_numKeys - 1] + keyLength + 1;

                    copyString (keys - value, key);
                    //printf("$$$ Inserting value: %u into TIndex:%u l_numKeys:%u\n", value, TIndex, l_numKeys);
                    keyOffsets[l_numKeys] = value;

                    l_collisionsSize = collisionsSize;

                    TIndexes[mid] = static_cast<byte>(l_collisionsSize);
                    TIndexes[l_numKeys] = static_cast<byte>(l_collisionsSize);

                    //! Insert the collision into the collision table.
                    temp_ptr = &collissionList[l_collisionsSize];
                    *temp_ptr = l_numKeys;
                    ++temp_ptr;
                    *temp_ptr = mid;
                    ++temp_ptr;
                    *temp_ptr = ~0;
                    collisionsSize = l_collisionsSize + 3;  //< added one term-char and two TIndexes.

                    TIndexes += l_reservedTIndexes;        //< Move collisions pointer to the ununsortedTIndexes.
                    TIndexes[l_numKeys] = l_numKeys;         //< The newest string added is at the end.

                    hashes[l_numKeys] = ~0;                 //< Set the last hash to 0xff..f

                    numKeys = l_numKeys + 1;

                    print ();

                    return l_numKeys;  //< Then it was a collision so the table doesn't contain s.
                }
                //printf("table already contains the key\n");
                return TIndex;
            }
        }

        //! The hash was not in the table.

        value = keyOffsets[l_numKeys - 1] + keyLength + 1;
        destination = keys - value;

        //printf("The hash 0x%x was not in the table so inserting %s into mid: %i at TIndex %u before hash 0x%x \n", hash, key, mid,
        //    destination - reinterpret_cast<char*>(this), hashes[mid]);

        //! First copy the string and set the key offset.
        copyString (destination, key);
        keyOffsets[l_numKeys] = value;

        //! Second move up the hashes and insert at the insertion point.

        hash32_t* hash_ptr = hashes + l_numKeys;
        //*test = hashes;
        //printf("l_numkeys: %u, hashes: %u hash_ptr: %u insert_ptr: %u\n", l_numKeys, hashes - reinterpret_cast<hash32_t*>(this),
        //    hash_ptr - reinterpret_cast<hash32_t*>(this), hashes + mid - reinterpret_cast<hash32_t*>(this));
        hashes += mid;
        //print();
        while (hash_ptr > hashes)
        {
            *hash_ptr = *(hash_ptr - 1);
            --hash_ptr;
        }
        *hashes = hash;

        //! There were no collisions so set collisionTIndex to zero.
        TIndexes[l_numKeys] = NoCollisions;

        //! Move up the sorted TIndexes and insert the unsorted TIndex(which is the current numKeys).
        TIndexes += l_reservedTIndexes + mid;
        temp_ptr = TIndexes + l_numKeys;

        while (temp_ptr > TIndexes)
        {
            *temp_ptr = *(temp_ptr - 1);
            --temp_ptr;
        }
        *temp_ptr = l_numKeys;      //ununsortedTIndexes[mid] = l_numKeys;

        numKeys = l_numKeys + 1;

        /*
        //! The table did not contain the has so insert it in the low spot.


        //! Now move up the corresponding TIndexes.

        byte* TIndex_ptr = TIndexes + l_numKeys * sizeof(byte*);

        while(TIndex_ptr != TIndexes)
        {
        *TIndex_ptr = *(TIndex_ptr - 1);
        --TIndex_ptr;
        }*/

        //printf("\nAfter...");
        print ();
        //printf("Done inserting.\n");
        printLine ();

        return l_numKeys;
    }

    byte find (const char* s)
    /*< Returns  the given query string in the hash table. */
    {
        printLine ();
        TIndex TIndex,
            l_numKeys = numKeys,
            l_reservedTIndexes = bufferedIndexes,
            temp;

        if (key == nullptr || l_numKeys == 0)
            return InvalidTIndex;

        THeader l_headerSize = size;

        hash32_t* hashes = reinterpret_cast<hash32_t*>(reinterpret_cast<byte*>(this) + sizeof (Book));
        uint16_t* keyOffsets = reinterpret_cast<uint16_t*>(hashes + l_reservedTIndexes);
        byte*     TIndexes = reinterpret_cast<byte*>(keyOffsets + l_reservedTIndexes),
            *ununsortedTIndexes = TIndexes + l_reservedTIndexes,
            *collissionList = ununsortedTIndexes + l_reservedTIndexes;
        char*     keys = reinterpret_cast<char*>(this) + l_headerSize - 1;
        byte*     collisions,
            *temp_ptr;

        hash32_t hash = hashString32 (key);

        //printf("\nSearching for key \"%s\" with hash 0x%x\n", key, hash);

        if (l_numKeys == 1)
        {
            //printf("Comparing keys - keyOffsets[0] - this %u\n%s\n", keys - keyOffsets[0] - reinterpret_cast<char*>(this), keys - keyOffsets[0]);
            if (strcmp (key, keys - keyOffsets[0]) != 0)
            {
                //printf("Did not find key %s\n", key);
                return InvalidTIndex;
            }
            //printf("Found key %s\n", key);
            printLine ();
            return 0;
        }

        //! Perform a binary search to find the first instance of the hash32_t the binary search yields.
        int low = 0,
            mid,
            high = l_numKeys - 1;
        //< If the mid is odd, we need to subtract the sizeof(hash32_t*) in order to get the right pointer address.

        while (low <= high)
        {
            mid = (low + high) >> 1;    //< >> 1 to /2

            hash32_t currentHash = hashes[mid];
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

                collisions = reinterpret_cast<byte*>(keyOffsets) + l_reservedTIndexes * sizeof (uint16_t);
                TIndex = collisions[mid];

                if (TIndex < NoCollisions)   //! There was a collision so check the table.
                {
                    //printf("There was a collision so check the table\n");

                    //! The collisionsList is a sequence of TIndexes terminated by an invalid TIndex > MaxNumKeys.
                    //! collissionsList[0] is an invalid TIndex, so the collisionsList is searched from lower addresses up.
                    temp = TIndexes[mid];
                    temp_ptr = collissionList + temp;
                    TIndex = *temp_ptr;
                    while (TIndex < MaxNumKeys)
                    {
                        //printf("comparing \"%s\" to \"%s\"\n", key, keys - keyOffsets[TIndex]);
                        if (strcmp (key, keys - keyOffsets[TIndex]) == 0)
                        {
                            //printf("but table already contains key at offset: %u.\n", TIndex);
                            return TIndex;
                        }
                        ++temp_ptr;
                        TIndex = *temp_ptr;
                    }
                    //printf("Did not find %s.\n", key);
                    return InvalidTIndex;
                }

                //! There were no collisions.

                //! But we still don't know if the string is new or a collision.
                TIndexes += l_reservedTIndexes; //< Move collisions pointer to the unsorted TIndexes.
                TIndex = ununsortedTIndexes[mid];

                //printf("\n!!!mid: %i-%x ununsortedTIndexes: %u key: \"%s\" hash: %x\n", mid, hashes[mid], TIndex,
                //    keys - keyOffsets[TIndex], hashString32(keys - keyOffsets[TIndex]));

                if (strcmp (key, keys - keyOffsets[TIndex]) != 0)
                {
                    //printf(" but it was a collision and did not find key.\n");
                    return InvalidTIndex;  //< Then it was a collision so the table doesn't contain s.
                }

                //printf("and found key at mid: %i\n", mid);
                return TIndex;
            }
        }
        //printf("Did not find a hash for key %s\n", key);
        printLine ();

        return InvalidTIndex;
    }

    TIndex getCount ()
    /*< Gets the size_t of the object being stored. */
    {
        return numKeys;
    }

    size_t getBufferCount()
    /*< Gets the size of the object being stored. */
    {
        return bufferedIndexes;
    }

    void print ()
    /*< Prints this object out to the console. */
    {
        byte l_numKeys = numKeys,
            l_reservedTIndexes = bufferedIndexes,
            collisionTIndex,
            temp;
        uint16_t l_headerSize = size;
        printLine ('_');
        printf ("| Book: %p\n| numKeys: %u bufferedIndexes: %u  collisionsSize: %u  size: %u",
            this, numKeys, l_reservedTIndexes, collisionsSize, size);
        putchar ('\n');
        putchar ('|');
        for (int i = 0; i < 79; ++i) putchar ('_');
        putchar ('\n');

        hash32_t* hashes = reinterpret_cast<hash32_t*>(reinterpret_cast<byte*>(this) + sizeof (Book));
        uint16_t* keyOffsets = reinterpret_cast<uint16_t*>(hashes + l_reservedTIndexes);
        byte*     TIndexes = reinterpret_cast<byte*>(keyOffsets + l_reservedTIndexes),
            *ununsortedTIndexes = TIndexes + l_reservedTIndexes,
            *collissionList = ununsortedTIndexes + l_reservedTIndexes,
            *currentCollision;
        char*     keys = reinterpret_cast<char*>(this) + l_headerSize - 1;

        printf ("| %3s%10s%8s%10s%10s%10s%10s%11s\n", "i", "key", "offset", "hash_e", "hash_u", "hash_s", "TIndex_u", "collisions");
        putchar ('|');
        for (int i = 0; i < 79; ++i) putchar ('_');
        putchar ('\n');

        for (int i = 0; i < l_reservedTIndexes; ++i)
        {
            collisionTIndex = TIndexes[i];
            /*
            //printf("| %i: \"%s\":%u hash: %x:%x sorted:%u_%x ",
            //    i, keys - keyOffsets[i], keyOffsets[i], hashString32(keys - keyOffsets[i]),
            //    hashes[ununsortedTIndexes[i]], ununsortedTIndexes[i], hashes[i], collisionTIndex);*/
            printf ("| %3i%10s%8u%10x%10x%10x%10u%11u: ", i, keys - keyOffsets[i], keyOffsets[i], hashString32 (keys - keyOffsets[i]),
                hashes[ununsortedTIndexes[i]], hashes[i], ununsortedTIndexes[i], collisionTIndex);

            if (collisionTIndex != NoCollisions && i < l_numKeys)
            {
                currentCollision = &collissionList[collisionTIndex];
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

    void wipe ()
    /*< Deletes the book contents by overwriting it with zeros. */
    {
    }

    void clear ()
    /*< Deletes the book contents without wiping the contents. */
    {
    }

    bool contains (byte type, void* data)
    /*< Returns true if this book contains thatCollection. */
    {
        return false;
    }

    bool contains (void* data)
    /*< Returns true if this dictionary contains only the given data. */
    {
        return false;
    }

    long getHash ()
    /*< Generates a hash for this Collection. */
    {
        return 0;
    }

    bool isEmpty ()
    /*< Returns true of this Collection is empty. */
    {
        return getCount () == 0;
    }

    bool removeCopy (void* data)
    /*< Removes that object from the collection. */
    {
        return false;
    }

    bool remove (void* address)
    /*< Removes the object at the given address from the collection. */
    {
        return false;
    }

    bool retain ()
    /*< Removes all but the given collection from this collection. */
    {
        return false;
    }

    IIterator* getIterator ()
    /*< Gets an iterator for this collection. */
    {
        return nullptr;
    }

    private:

    TIndex numKeys,            //< Number of IDevice members.
        numIndexes;         //< The number of buffered items.
    THeader collisionsSize, //< The size of the (optional) collisions table.
        headerSize;         //< The size of the key strings in bytes.
    TData dataSize;         //< The total size of the folder.
};

using BookX = Book<BookIndexType, BookHeaderType, BookDataOffsetType,
    BookHashType>;
using Book2 = Book<byte, uint16_t, uint16_t, hash32_t>;
using Book4 = Book<uint16_t, uint16_t, uint32_t, hash32_t>;
using Book8 = Book<uint32_t, uint32_t, uint64_t, hash64_t>;

enum
{
    MaxNumPagesBook2 = 255,
    MaxNumPagesBook4 = 8 * 1024,
    MaxNumPagesBook8 = 32 * 1024 * 1024,
};

template<typename TIndex, typename TOffset, typename TData, typename THash>
inline Book* createBook (TIndex bufferedIndexes, TData headerSize,
    TData dataSize)
{
    byte* buffer = nullptr;
    try
    {
        buffer = new byte[totalSize];
        #if DEBUG_CHINESEROOM
        memset (buffer, 0, totalSize);
        #endif
        Book* t = new(buffer) Book ();
        if (!t->init (bufferedIndexes, totalSize))
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

template<typename TIndex, typename TOffset, typename TData, typename THash>
inline void destroy (Book<TIndex, TOffset, TData, THash>* b)
{
    if (b == nullptr) return;
    delete[] reinterpret_cast<byte*>(b);
}

}       //< namespace _

#endif  //< CHINESEROOM_BOOK_H

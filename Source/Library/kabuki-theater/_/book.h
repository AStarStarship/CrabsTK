/** The Chinese Room
    @version 0.x
    @file    ~/Book.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough] (calemccollough.github.io)
                            All right reserved (R).
             Licensed under the Apache License, Version 2.0 (the "License"); 
             you may not use this file except in compliance with the License. 
             You may obtain a copy of the License at
                        http://www.apache.org/licenses/LICENSE-2.0
             Unless required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#ifndef CHINESEROOM_BOOK_H
#define CHINESEROOM_BOOK_H

#include "utils.h"
#include "types.h"

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

    | Book | Max Values | % Collisions (p) |           Overhead             |
    |:----:|:----------:|:----------------:|:------------------------------:|
    |   2  |     255    |    0.0001        | Ceiling (0.02*p*2^8)  = 2      |
    |   4  |     2^13   |      0.1         | Ceiling (0.04*p*2^13) = 327.68 |
    |   8  |     2^29   |    10^-16        | Ceiling (0.04*p*2^29) = 327.68 |

    Memory Schema:
    The memory schema is optimized for fast search and push back. When searching
    for a key, there might or might not be a hash table.

    How to calculate size:
    The size of any size book can be calculated as follows:
    size = num_items * (2*sizeof (TIndex) + sizeof (TData)) + collissionSize +

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
template<typename TIndex, typename TKey, typename TData, typename THash>
struct Book {
    TData size;         //< The total size of the Book.
    TKey table_size,    //< The size of the key strings in bytes.
        pile_size;      //< The size of the (optional) collision table pile.
    TIndex num_items,   //< Number of items.
        max_items;      //< The max number of buffered items.
};

using Book2 = Book<byte, uint16_t, uint16_t, hash16_t>;
using Book4 = Book<uint16_t, uint16_t, uint32_t, hash32_t>;
using Book8 = Book<uint32_t, uint32_t, uint64_t, hash64_t>;
using BookX = Book<TBookIndex, TBookHeader, TBookDataOffset, TBookHash>;


template<typename TIndex, typename TKey, typename TData, typename THash>
constexpr uint_t OverheadPerBookIndex () {
        return sizeof (2 * sizeof (TIndex) + sizeof (TKey) + sizeof (TData) + 3);
};

template<typename TIndex, typename TKey, typename TData, typename THash>
constexpr TData MinSizeBook (TIndex num_items) {
    return sizeof (2 * sizeof (TIndex) + sizeof (TKey) + sizeof (TData) + 3);
};

enum {
    kMaxNumPagesBook2 = 255,                //< The number of pages in a Book2.
    kMaxNumPagesBook4 = 8 * 1024,           //< The number of pages in a Book4.
    kMaxNumPagesBook8 = 256 * 1024 * 1024,  //< The number of pages in a Book8.
    kOverheadPerBook2Index = OverheadPerBookIndex<byte, uint16_t, uint16_t, hash16_t> (),
    kOverheadPerBook4Index = OverheadPerBookIndex<byte, uint16_t, uint16_t, hash16_t> (),
    kOverheadPerBook8Index = OverheadPerBookIndex<byte, uint16_t, uint16_t, hash16_t> (),
};
    
/** Initializes a Book.
    @post Users might want to call the isValid () function after
        construction to verify the integrity of the object.
    @warning The reservedNumMembers must be aligned to a 32-bit value,
        and it will get rounded up to the next higher multiple of 4.
*/
static Book2* Init2 (byte* buffer, byte max_size, uint16_t table_size, uint16_t size)
{
    if (buffer == nullptr)
        return nullptr;
    if (table_size >= size)
        return nullptr;
    if (table_size < sizeof (Book2) + max_size *
        (OverheadPerBookIndex<byte, uint16_t, uint16_t, hash16_t> () + 2))
        return nullptr;

    Book2* book = reinterpret_cast<Book2*> (buffer);
    book->size = table_size;
    book->table_size = table_size;
    book->num_items = 0;
    book->max_items = max_size;
    book->pile_size = 1;
    return book;
}

/** Insets the given key-value pair.
*/
template<typename TIndex, typename TKey, typename TData, typename THash>
TIndex BookInsert (Book<TIndex, TKey, TData, THash>* book, byte type, 
               const byte* key, void* data, TIndex index) {
    if (book == nullptr) return 0;
    return ~0;
}

template<typename TIndex>
TIndex MaxBookIndexes () {
    enum {
        kMaxIndexes = sizeof (TIndex) == 1 ? 255 : sizeof (TIndex) == 2 ? 
                       8 * 1024 : sizeof (TIndex) == 4 ? 512 * 1024 * 1024 : 0
    };
    return kMaxIndexes;
}

/** Adds a key-value pair to the end of the book. */
template<typename TIndex, typename TKey, typename TData, typename THash>
TIndex BookAdd (Book<TIndex, TKey, TData, THash>* book, const char* key, 
                TType type, void* data) {
    if (book == nullptr) return 0;
    if (key == nullptr) return 0;

    PrintStringLine (key);

    TIndex num_items = book->num_items,
        max_items = book->max_items,
        temp;

    TKey table_size = book->table_size;

    if (num_items >= max_items) return ~0;
    //< We're out of buffered indexes.

    byte* states = reinterpret_cast<byte*> (book) + 
                   sizeof (Book <TIndex, TKey, TData, THash>);
    TKey* key_offsets = reinterpret_cast<TKey*> (states + max_items);
    TData* data_offsets = reinterpret_cast<TData*> (states + max_items *
                                                    (sizeof (TKey)));
    THash* hashes = reinterpret_cast<THash*> (states + max_items *
                                              (sizeof (TKey) + sizeof (TData))),
        * hash_ptr;
    TIndex* indexes = reinterpret_cast<TIndex*> (states + max_items *
                                                 (sizeof (TKey) + sizeof (TData) + sizeof (TIndex))),
        *unsorted_indexes = indexes + max_items,
        *collission_list = unsorted_indexes + max_items;
    char* keys = reinterpret_cast<char*> (book) + table_size - 1,
        *destination;

    // Calculate space left.
    TKey value = table_size - max_items * OverheadPerBookIndex<TIndex, TKey, TData, THash> (),
        key_length = static_cast<uint16_t> (strlen (key)),
        pile_size;

    PrintLine ();
    printf ("Adding Key %s\n%20s: 0x%p\n%20s: %p\n%20s: 0x%p\n"
            "%20s: %p\n%20s: %u\n", key, "hashes", hashes, "key_offsets",
            key_offsets, "keys", keys, "indexes", indexes, "value", value);

    THash hash = Hash16 (key),
        current_hash;

    if (key_length > value) {
        std::cout << "Buffer overflow\n";
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

        CopyString (destination, key);
        printf ("Inserted key %s at GetAddress 0x%p\n", key, destination);
        BookPrint (book);
        return 0;
    }

    // Calculate left over buffer size by looking up last string.

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

        current_hash = hashes[mid];
        printf ("high: %i mid: %i low %i hash: %x\n", high, mid, low,
                current_hash);

        if (current_hash > hash) {
            high = mid - 1;
        } else if (current_hash < hash) {
            low = mid + 1;
        } else    // Duplicate hash detected.
        {
            std::cout << "hash detected, ";

            // Check for other collisions.

            index = indexes[mid];       //< Index in the collision table.

            printf ("index:%u\n", index);

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
                while (index < MaxBookIndexes<TIndex> ()) {
                    printf ("comparing to \"%s\"\n", keys - key_offsets[index]);
                    if (strcmp (key, keys - key_offsets[index]) == 0) {
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
                CopyString (keys - value, key);
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
                printf ("\n\ncollision index: %u\n", temp);
                // Store the collision index.
                indexes[num_items] = temp;   //< Store the collision index
                book->num_items = num_items + 1;
                hashes[num_items] = ~0;      //< Set the last hash to 0xFFFF

                                            // Move collisions pointer to the unsorted_indexes.
                indexes += max_items;

                //< Add the newest string to the end.
                indexes[num_items] = num_items;

                BookPrint (book);
                printf ("Done inserting.\n");
                return num_items;
            }

            // But we still don't know if the string is a new collision.

            std::cout << "Checking if it's a collision... ";

            if (strcmp (key, keys - key_offsets[index]) != 0) {
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

                CopyString (keys - value, key);
                printf ("Inserting value: %u into index:%u "
                        "num_items:%u with other collision_index: %u\n", value,
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

                // Set the last hash to 0xFFFF
                hashes[num_items] = ~0;

                book->num_items = num_items + 1;

                BookPrint (book);

                BookPrint (book);
                std::cout << "Done inserting.\n";
                // Then it was a collision so the table doesn't contain s.
                return num_items;
            }
            std::cout << "table already contains the key\n";
            return index;
        }
    }

    // The hash was not in the table.

    value = key_offsets[num_items - 1] + key_length + 1;
    destination = keys - value;

    printf ("The hash 0x%x was not in the table so inserting %s into mid:"
            " %i at index %u before hash 0x%x \n", hash, key, mid,
            Diff (book, destination), hashes[mid]);

    // First copy the string and set the key offset.
    CopyString (destination, key);
    key_offsets[num_items] = value;

    // Second move up the hashes and insert at the insertion point.
    hash_ptr = hashes;
    hash_ptr += num_items;
    //*test = hashes;
    printf ("l_numkeys: %u, hashes: %u hash_ptr: %u insert_ptr: %u\n",
            num_items, Diff (book, hashes),
            Diff (book, hash_ptr), Diff (book, hashes + mid));
    hashes += mid;
    BookPrint (book);
    while (hash_ptr > hashes) {
        *hash_ptr = *(hash_ptr - 1);
        --hash_ptr;
    }
    *hashes = hash;
    
    // Mark as not having any collisions.
    indexes[num_items] = ~0;
    
    // Move up the sorted indexes and insert the unsorted index (which is 
    // the current num_items).
    indexes += max_items + mid;
    temp_ptr = indexes + num_items;

    while (temp_ptr > indexes) {
        *temp_ptr = *(temp_ptr - 1);
        --temp_ptr;
    }
    *temp_ptr = num_items;

    book->num_items = num_items + 1;

    BookPrint (book);
    std::cout << "Done inserting.\n";
    PrintLine ();

    return num_items;
}

/** Adds a key-value pair to the end of the book. */
inline byte Add2 (Book2* book, const char* key, byte data) {
    return BookAdd<byte, uint16_t, uint16_t, hash16_t> (book, key, UI1, &data);
}

/** Returns  the given query string in the hash table. */
template<typename TIndex, typename TKey, typename TData, typename THash>
TIndex BookFind (Book<TIndex, TKey, TData, THash>* book, const char* key) {
    if (book == nullptr)
        return 0;
    PrintLineBreak ("Finding record...", 5);
    TIndex index,
        num_items = book->num_items,
        max_items = book->max_items,
        temp;

    if (key == nullptr || num_items == 0)
        return ~((TIndex)0);

    TKey table_size = book->table_size;

    const THash* hashes = reinterpret_cast<const THash*>
        (reinterpret_cast<const byte*> (book) +
         sizeof (Book<TIndex, TKey, TData, THash>));
    const TKey* key_offsets = reinterpret_cast<const uint16_t*>(hashes +
                                                                max_items);
    const byte* indexes = reinterpret_cast<const byte*>(key_offsets +
                                                        max_items),
        *unsorted_indexes = indexes + max_items,
        *collission_list = unsorted_indexes + max_items;
    const char* keys = reinterpret_cast<const char*> (book) + table_size - 1;
    const TIndex* collisions,
        *temp_ptr;

    THash hash = Hash16 (key);

    printf ("\nSearching for key \"%s\" with hash 0x%x\n", key, hash);

    if (num_items == 1) {
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
    // sizeof (THash*) in order to get the right pointer address.
    int low = 0,
        mid,
        high = num_items - 1;

    while (low <= high) {
        mid = (low + high) >> 1;    //< >> 1 to /2

        THash current_hash = hashes[mid];
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
                max_items * sizeof (uint16_t);
            index = collisions[mid];

            if (index < ~0) {
                // There was a collision so check the table.
                std::cout << "There was a collision so check the table\n";

                // The collisionsList is a sequence of indexes terminated by
                // an invalid index > kMaxI2PMembers. collissionsList[0] is an 
                // invalid index, so the collisionsList is searched from 
                // lower address up.

                temp = indexes[mid];

                temp_ptr = collission_list + temp;
                index = *temp_ptr;
                while (index < MaxBookIndexes<TIndex> ()) {
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

            // But we still don't know if the string is new or a collision.

            // Move collisions pointer to the unsorted indexes.
            indexes += max_items;
            index = unsorted_indexes[mid];

            printf ("\n!!!mid: %i-%x unsorted_indexes: %u key: %s\n"
                    "hash: %x\n", mid, hashes[mid], index, keys -
                    key_offsets[index], Hash16 (keys -
                                                key_offsets[index]));

            if (strcmp (key, keys - key_offsets[index]) != 0) {
                //< It was a collision so the table doesn't contain s.
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

static byte Find2 (Book2* book, const char* key) {
    return BookFind<byte, uint16_t, uint16_t, hash16_t> (book, key);
}

/** Prints this object out to the console. */
template<typename TIndex, typename TKey, typename TData, typename THash>
void BookPrint (const Book<TIndex, TKey, TData, THash>* book) {
    if (book == nullptr) return;
    TIndex num_items = book->num_items,
           max_items = book->max_items,
           collision_index,
           temp;
    TKey table_size = book->table_size,
         pile_size = book->pile_size;
    PrintLine ('_');
    
    if (sizeof (TData) == 2)
        printf ("| Book2: %p\n", book);
    else if (sizeof (TData) == 4)
        printf ("| Book4: %p\n", book);
    else if (sizeof (TData) == 8)
        printf ("| Book8: %p\n", book);
    else
        printf ("| Invalid Book type: %p\n", book);
    printf ("| num_items: %u max_items: %u  "
            "pile_size: %u  size: %u", num_items,
            max_items, pile_size, table_size);
    std::cout << '\n';
   std::cout << '|';
    for (int i = 0; i < 79; ++i) putchar ('_');
    std::cout << '\n';

    const byte* states = reinterpret_cast<const byte*> (book) +
                         sizeof (Book <TIndex, TKey, TData, THash>);
    const TKey* key_offsets = reinterpret_cast<const TKey*> 
                              (states + max_items);
    const TData* data_offsets = reinterpret_cast<const TData*> 
                                (states + max_items *(sizeof (TKey)));
    const THash* hashes = reinterpret_cast<const THash*> (states + max_items *
        (sizeof (TKey) + sizeof (TData)));
    const TIndex* indexes = reinterpret_cast<const TIndex*> 
                            (states + max_items * (sizeof (TKey) + 
                             sizeof (TData) + sizeof (TIndex))),
        * unsorted_indexes = indexes + max_items,
        * collission_list = unsorted_indexes + max_items,
        *cursor;
    const char* keys = reinterpret_cast<const char*> (book) + table_size - 1;

    printf ("| %3s%10s%8s%10s%10s%10s%10s%11s\n", "i", "key", "offset",
            "hash_e", "hash_u", "hash_s", "index_u", "collisions");
   std::cout << '|';
    for (int i = 0; i < 79; ++i)
        putchar ('_');
    std::cout << '\n';

    for (TIndex i = 0; i < max_items; ++i) {
        // Print each record as a row.
        // @todo Change max_items to num_items after done debugging.
        collision_index = indexes[i];
        printf ("| %3i %9s %7u %9x %9x %9x %9u %10u: ", i,
                keys - key_offsets[i], key_offsets[i],
                Hash16 (keys - key_offsets[i]),
                hashes[unsorted_indexes[i]], hashes[i],
                unsorted_indexes[i], collision_index);

        if (collision_index != ~0 && i < num_items) {
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
    PrintLine ("|", '_');

    PrintMemory (reinterpret_cast<const byte*> (book) + 
                 sizeof (Book<TIndex, TKey, TData, THash>), book->size);
    std::cout << '\n';
}

/** Deletes the book contents by overwriting it with zeros. */
template<typename TIndex, typename TKey, typename TData, typename THash>
void Wipe (Book<TIndex, TKey, TData, THash>* book) {
    if (book == nullptr) return;
    TData size = book->size;
    memset (book, 0, size);
}

/** Deletes the book contents without wiping the contents. */
template<typename TIndex, typename TKey, typename TData, typename THash>
void Clear (Book<TIndex, TKey, TData, THash>* book) {
    if (book == nullptr) return;
    book->num_items = 0;
    book->pile_size = 0;
}

/** Returns true if this dictionary contains only the given address. */
template<typename TIndex, typename TKey, typename TData, typename THash>
bool Contains (Book<TIndex, TKey, TData, THash>* book, void* data) {
    if (book == nullptr) return false;
    if (data < book) return false;
    if (data > GetEndAddress()) return false;
    return true;
}

/** Removes that object from the book and copies it to the destination. */
template<typename TIndex, typename TKey, typename TData, typename THash>
bool RemoveCopy (Book<TIndex, TKey, TData, THash>* book, void* destination, 
                 size_t buffer_size, void* data)
{
    if (book == nullptr) return false;

    return false;
}

/** Removes the item at the given address from the book. */
template<typename TIndex, typename TKey, typename TData, typename THash>
bool Remove (Book<TIndex, TKey, TData, THash>* book, void* adress) {
    if (book == nullptr) return false;

    return false;
}

/** Removes all but the given collection from the book. */
template<typename TIndex, typename TKey, typename TData, typename THash>
bool Retain (Book<TIndex, TKey, TData, THash>* book) {
    if (book == nullptr) return false;

    return false;
}

/** Creates a book from dynamic memory. */
template<typename TIndex, typename TOffset, typename TData, typename THash>
inline Book<TIndex, TOffset, TData, THash>* BookCreate (TIndex buffered_indexes,
                                                        TData table_size,
                                                        TData size) {
    Book<TIndex, TOffset, TData, THash>* book = New<Book, uint_t> ();
    return book;
}

/** Prints the given Book to the console. */
template<typename TIndex, typename TKey, typename TData, typename THash>
inline void Print (Book<TIndex, TKey, TData, THash>* book) {

}

inline void Print (Book2* book) {
    return Print<byte, uint16_t, uint16_t, hash16_t> (book);
}

}       //< namespace _
#endif  //< CHINESEROOM_BOOK_H

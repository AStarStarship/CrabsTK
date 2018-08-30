/* Kabuki Toolkit
@version 0.x
@file    ~/libraries/crabs/bag.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <stdafx.h>
#if SEAM >= SEAM_0_0_2
#ifndef INCLUDED_CRABS_BOOK
#define INCLUDED_CRABS_BOOK
// Dependencies:
#include "ascii_data_types.h"
#include "socket.h"
// End dependencies.
#if SEAM_MAJOR == 0 && SEAM_MINOR == 3
#ifndef PRINTF
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT(c) Print(c)
#define PRINT_HEADING \
  Print('\n');        \
  for (int i = 80; i > 0; --i) std::cout << '-'
#endif
#else
#define PRINTF(x, ...)
#define PRINT(c)
#define PRINT_HEADING
#endif

namespace _ {

/* A multismap created from contiguous memory.
    @ingroup Book
    A multimap is like a Python dictionary or C++ map, the difference being a
   TMultimap can contain nested TMultimap (string). The key design difference
   between both Python dictionaries and C++ maps are Sets do not contains
   points, and instead works using offsets.

    A multimap may or may not have a hash table. In order to turn on the hash
   table, simply set the collissionsSize to non-zero in the TMultimap header.

    The memory layout is the same for all of the TMultimap types as depicted
   below:

    @code
    +==========================+ -----------
    |_______ Buffer            |   ^     ^
    |_______ ...               |   |     |
    |_______ Data N            |  Data   |
    |_______ ...               |   |     |
    |_______ Data 0            |   v     |
    |==========================| -----   |
    |        Key 1             |   ^     |
    |        ...               |   |     |
    |        Key N             |   |     |
    |vvvvvvvvvvvvvvvvvvvvvvvvvv|   |     |
    |        buffer            |   |     |
    |==========================|   |     |
    |_______ count_max         |   |     |
    |_______ ...               |   |    Size
    |_______ Key Offset N      |   |     |
    |_______ ...               |   |     |
    |        Key Offset 1      |   |     |
    |==========================| Header  |
    |_______ count_max         |   |     |
    |_______ ...               |   |     |
    |_______ Data Offset N     |   |     |
    |_______ ...               |   |     |
    |        Data Offset 1     |   |     |
    |==========================|   |     |
    |_______ count_max         |   |     |
    |_______ ...               |   |     |
    |_______ Type byte N       |   |     |
    |_______ ...               |   |     |
    |        Type byte 1       |   |     |   ^ Up in addresses
    |==========================|   |     |   |
    |  TMapKey<UI, SI> Struct  |   v     v   ^
    +==========================+ ----------- ^ 0xN
    @endcode

    # Memory Overhead

    | #Bytes | I | SI | UI | Total | Overhead Per index |
    |:------:|:-:|:--:|:--:|:-----:|:-------------------|
    |    2   | 1 |  2 |  2 |   8   |  8 + 3 per index + buffer.|
    |    4   | 2 |  4 |  4 |   16  | 16 + 5 per index + buffer.|
    |    8   | 4 |  8 |  8 |   32  | 24 + 9 per index + buffer.|

    * Sizes shown in bytes.
*/

using TMultimap2 = TMap<uint16_t, int16_t, byte>;
using TMultimap4 = TMap<uint32_t, int32_t, int16_t>;
using TMultimap8 = TMap<uint64_t, int32_t, int32_t>;

/* The overhead per index for a multimap. */
template <typename UI, typename SI, typename I>
constexpr uint_t MultimapOverheadPerIndex() {
  return sizeof(2 * sizeof(I) + sizeof(SI) + sizeof(UI) + 3);
};

/* The min size a Multimap with the given UI, SI, I sizes. */
template <typename UI, typename SI, typename I>
constexpr UI MultimapSizeMin(I item_count) {
  return item_count * sizeof(2 * sizeof(I) + sizeof(SI) + sizeof(UI) + 3);
};

enum {
  kMaxNumPagesSet2 = 120,                //< The number of pages in a Set2.
  kMaxNumPagesSet4 = 8 * 1024,           //< The number of pages in a Set4.
  kMaxNumPagesSet8 = 256 * 1024 * 1024,  //< The number of pages in a Set8.
  kOverheadPerSet2Index = MultimapOverheadPerIndex<byte, uint16_t, uint16_t>(),
  kOverheadPerSet4Index = MultimapOverheadPerIndex<byte, uint16_t, uint16_t>(),
  kOverheadPerSet8Index = MultimapOverheadPerIndex<byte, uint16_t, uint16_t>(),
};

/* Initializes a Multimap with the given .
    @post    Users might want to call the IsValid () function after construction
             to verify the integrity of the object.
    @warning The reservedNumOperands must be aligned to a 32-bit value, and it
             will get rounded up to the next higher multiple of 4. */
template <typename UI, typename SI, typename I>
uintptr_t* MultimapInit(uintptr_t* buffer, byte count_max, uint16_t size) {
  ASSERT(buffer);
  if (table_size <
      sizeof(TMap) +
          count_max *
              (MultimapOverheadPerIndex<byte, uint16_t, uint16_t, uint16_t>() +
               2))
    return nullptr;

  Multimap<>* multimap = reinterpret_cast<TMap*>(buffer);
  multimap->size = table_size;
  multimap->table_size = table_size;
  multimap->item_count;
  = 0;
  multimap->count_max = count_max;
  multimap->size_pile = 1;
  return buffer;
}

/* Insets the given key-value pair.
 */
template <typename UI, typename SI, typename I>
I MultimapInsert(TMap<SI, I>* multimap, byte type, const char* key, void* data,
                 I index) {
  if (multimap == nullptr) return 0;
  return ~0;
}

template <typename I>
I MultimapIndexMax() {
  enum {
    kMaxIndexes =
        sizeof(I) == 1
            ? 255
            : sizeof(I) == 2 ? 8 * 1024 : sizeof(I) == 4 ? 512 * 1024 * 1024 : 0
  };
  return kMaxIndexes;
}

/* Adds a key-value pair to the end of the multimap. */
template <typename UI, typename SI, typename I>
I MultimapAdd(TMap<SI, I>* multimap, const char* key, TType type, void* data) {
  if (multimap == nullptr) return 0;
  if (key == nullptr) return 0;

  PrintLine(key);

  I item_count = multimap->item_count, count_max = multimap->count_max, temp;

  SI table_size = multimap->table_size;

  if (item_count >= count_max) return ~0;
  //< We're out of buffered indexes.

  char* states = reinterpret_cast<char*>(multimap) + sizeof(TMap<SI, I>);
  SI* key_offsets = reinterpret_cast<SI*>(states + count_max);
  UI* data_offsets = reinterpret_cast<UI*>(states + count_max * (sizeof(SI)));
  UI *hashes =
         reinterpret_cast<UI*>(states + count_max * (sizeof(SI) + sizeof(UI))),
     *hash_ptr;
  I *indexes = reinterpret_cast<I*>(
        states + count_max * (sizeof(SI) + sizeof(UI) + sizeof(I))),
    *unsorted_indexes = indexes + count_max,
    *collission_list = unsorted_indexes + count_max;
  char *keys = reinterpret_cast<char*>(multimap) + table_size - 1, *destination;

  // Calculate space left.
  SI value = table_size - count_max * MultimapOverheadPerIndex<SI, I>(),
     key_length = static_cast<uint16_t>(strlen(key)), size_pile;

  PrintLine();
  PRINTF(
      "Adding Key %s\n%20s: 0x%p\n%20s: %p\n%20s: 0x%p\n"
      "%20s: %p\n%20s: %u\n",
      key, "hashes", hashes, "key_offsets", key_offsets, "keys", keys,
      "indexes", indexes, "value", value);

  UI hash = Hash16(key), current_hash;

  if (key_length > value) {
        PRINTF ("Buffer overflow\n";
        return ~((I)0);
  }

  // print ();

  if (item_count == 0) {
    multimap->item_count = 1;
    *hashes = hash;
    *key_offsets = static_cast<uint16_t>(key_length);
    *indexes = ~0;
    *unsorted_indexes = 0;
    destination = keys - key_length;

    SlotWrite(destination, key);
    PRINTF("Inserted key %s at GetAddress 0x%p\n", key, destination);
    SetPrint(multimap);
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
        high = item_count,
        index;

    I* temp_ptr;

    while (low <= high) {
    mid = (low + high) >> 1;  //< Shift >> 1 to / 2

    current_hash = hashes[mid];
    PRINTF("high: %i mid: %i low %i hash: %x\n", high, mid, low, current_hash);

    if (current_hash > hash) {
      high = mid - 1;
    } else if (current_hash < hash) {
      low = mid + 1;
    } else  // Duplicate hash detected.
    {
            PRINTF ("hash detected, ";

            // Check for other collisions.

            index = indexes[mid];       //< Index in the collision table.

            PRINTF ("index:%u\n", index);

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
                while (index < MaxSetIndexes<I> ()) {
          PRINTF("comparing to \"%s\"\n", keys - key_offsets[index]);
          if (strcmp(key, keys - key_offsets[index]) == 0) {
            PRINTF(
                "but table already contains key at "
                "offset: %u.\n",
                index);
            return index;
          }
          ++temp_ptr;
          index = *temp_ptr;
                }

                // Its a new collision!
                PRINTF ("and new collision detected.\n";

                // Copy the key
                value = key_offsets[item_count - 1] + key_length + 1;
                SlotWrite (keys - value, key);
                key_offsets[item_count] = value;

                // Update the collision table.
                size_pile = multimap->size_pile;
                // Shift the collisions table up one element and insert 
                // the unsorted collision index.
                // Then move to the top of the collisions list.
                collission_list += size_pile;
                // and iterate down to the insert spot
                while (collission_list > temp_ptr) {
          *collission_list = *(collission_list - 1);
          --collission_list;
                }
                *temp_ptr = item_count;

                multimap->size_pile = size_pile + 1;
                PRINTF ("\n\ncollision index: %u\n", temp);
                // Store the collision index.
                indexes[item_count] = temp;   //< Store the collision index
                multimap->item_count = item_count + 1;
                hashes[item_count] = ~0;      //< TMultimap the last hash to 0xFFFF

                // Move collisions pointer to the unsorted_indexes.
                indexes += count_max;

                //< Add the newest char to the end.
                indexes[item_count] = item_count;

                SetPrint (multimap);
                PRINTF ("Done inserting.\n");
                return item_count;
            }

            // But we still don't know if the char is a new collision.

            PRINTF ("Checking if it's a collision... ";

            if (strcmp (key, keys - key_offsets[index]) != 0) {
        // It's a new collision!
                PRINTF ("It's a new collision!\n";

                if (value < 3) {
                    PRINTF ("Buffer overflow!\n";
                    return ~0;
                }

                // Get offset to write the key too.
                value = key_offsets[item_count - 1] + key_length + 1;

                byte collision_index = unsorted_indexes[mid];
                PRINTF ("\n\ncollision_index: %u", collision_index);

                SlotWrite (keys - value, key);
                PRINTF ("Inserting value: %u into index:%u "
                        ";:%u with other collision_index: %u\n", value,
                        index, item_count, collision_index);
                key_offsets[item_count] = value;

                size_pile = multimap->size_pile;
                indexes[mid] = static_cast<byte> (size_pile);
                indexes[item_count] = static_cast<byte> (size_pile);

                // Insert the collision into the collision table.
                temp_ptr = &collission_list[size_pile];
                // Move collisions pointer to the unsorted_indexes.
                indexes += count_max;
                *temp_ptr = collision_index;
                ++temp_ptr;
                *temp_ptr = item_count;
                ++temp_ptr;
                *temp_ptr = ~0;
                multimap->size_pile = size_pile + 3;
                //< Added one term-byte and two indexes.

                // Add the newest key at the end.
                indexes[item_count] = item_count;

                // TMultimap the last hash to 0xFFFF
                hashes[item_count] = ~0;

                multimap->item_count = item_count + 1;

                SetPrint (multimap);

                SetPrint (multimap);
                PRINTF ("Done inserting.\n";
                // Then it was a collision so the table doesn't contain string.
                return item_count;
            }
            PRINTF ("table already contains the key\n";
            return index;
    }
    }

    // The hash was not in the table.

    value = key_offsets[item_count - 1] + key_length + 1;
    destination = keys - value;

    PRINTF ("The hash 0x%x was not in the table so inserting %s into mid:"
            " %i at index %u before hash 0x%x \n", hash, key, mid,
            Diff (multimap, destination), hashes[mid]);

    // First copy the char and set the key offset.
    SlotWrite (destination, key);
    key_offsets[item_count] = value;

    // Second move up the hashes and insert at the insertion point.
    hash_ptr = hashes;
    hash_ptr += item_count;
    //*test = hashes;
    PRINTF ("l_numkeys: %u, hashes: %u hash_ptr: %u insert_ptr: %u\n",
            item_count, Diff (multimap, hashes),
            Diff (multimap, hash_ptr), Diff (multimap, hashes + mid));
    hashes += mid;
    SetPrint (multimap);
    while (hash_ptr > hashes) {
    *hash_ptr = *(hash_ptr - 1);
    --hash_ptr;
    }
    *hashes = hash;
    
    // Mark as not having any collisions.
    indexes[item_count] = ~0;
    
    // Move up the sorted indexes and insert the unsorted index (which is 
    // the current ;).
    indexes += count_max + mid;
    temp_ptr = indexes + item_count;

    while (temp_ptr > indexes) {
    *temp_ptr = *(temp_ptr - 1);
    --temp_ptr;
    }
    *temp_ptr = item_count;

    multimap->item_count = item_count + 1;

    SetPrint (multimap);
    PRINTF ("Done inserting.\n";
    PrintLine ();

    return item_count;
}

/* Adds a key-value pair to the end of the multimap. */
// byte Add2 (Set2* multimap, const char* key, byte data) {
//    return SetAdd<byte, uint16_t, uint16_t, uint16_t> (multimap, key, UI1,
//    &data);
//}

/* Returns  the given query char in the hash table. */
template <typename UI, typename SI, typename I>
I MultimapFind(TMap<SI, I>* multimap, const char* key) {
  if (multimap == nullptr) return 0;
  PrintLineBreak("Finding record...", 5);
  I index, item_count = multimap->item_count, count_max = multimap->count_max,
           temp;

  if (key == nullptr || item_count == 0) return ~((I)0);

  SI table_size = multimap->table_size;

  const UI* hashes = reinterpret_cast<const UI*>(
      reinterpret_cast<const char*>(multimap) + sizeof(TMap<SI, I>));
  const SI* key_offsets = reinterpret_cast<const uint16_t*>(hashes + count_max);
  const char *indexes = reinterpret_cast<const char*>(key_offsets + count_max),
             *unsorted_indexes = indexes + count_max,
             *collission_list = unsorted_indexes + count_max;
  const char* keys = reinterpret_cast<const char*>(multimap) + table_size - 1;
  const I *collisions, *temp_ptr;

  UI hash = Hash16(key);

  PRINTF("\nSearching for key \"%s\" with hash 0x%x\n", key, hash);

  if (item_count == 1) {
    if (strcmp(key, keys - key_offsets[0]) != 0) {
      PRINTF("Did not find key %s\n", key);
      return ~((I)0);
    }
    PRINTF("Found key %s\n", key);
    PrintLine();
    return 0;
  }

  // Perform a binary search to find the first instance of the hash the
  // binary search yields. If the mid is odd, we need to subtract the
  // sizeof (UI*) in order to get the right pointer address.
  int low = 0, mid, high = item_count - 1;

  while (low <= high) {
    mid = (low + high) >> 1;  //< >> 1 to /2

    UI current_hash = hashes[mid];
    PRINTF("low: %i mid: %i high %i hashes[mid]:%x\n", low, mid, high,
           hashes[mid]);

    if (current_hash > hash) {
      high = mid - 1;
    } else if (current_hash < hash) {
      low = mid + 1;
    } else {
      // Duplicate hash found.
      PRINTF(
          "\nFound same hash at mid:%i hash:%x offset for key: "
          "%s\n",
          mid, hashes[mid], key);

      // Check for collisions

      collisions = reinterpret_cast<const char*>(key_offsets) +
                   count_max * sizeof(uint16_t);
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
                while (index < MaxSetIndexes<I> ()) {
          PRINTF("comparing to \"%s\"\n", keys - key_offsets[index]);
          if (strcmp(key, keys - key_offsets[index]) == 0) {
            PRINTF(
                "but table already contains key at offset:"
                "%u.\n",
                index);
            return index;
          }
          ++temp_ptr;
          index = *temp_ptr;
                }
                PRINTF ("Did not find \"" << key << "\"\n";
                return ~((I)0);
      }

      // There were no collisions.

      // But we still don't know if the char is new or a collision.

      // Move collisions pointer to the unsorted indexes.
      indexes += count_max;
      index = unsorted_indexes[mid];

      PRINTF(
          "\n!!!mid: %i-%x unsorted_indexes: %u key: %s\n"
          "hash: %x\n",
          mid, hashes[mid], index, keys - key_offsets[index],
          Hash16(keys - key_offsets[index]));

      if (strcmp(key, keys - key_offsets[index]) != 0) {
        //< It was a collision so the table doesn't contain string.
                PRINTF (" but it was a collision and did not find key.\n";
                return ~((I)0);
      }

            PRINTF ("and found key at mid: " << mid << '\n';
            return index;
    }
  }
    PRINTF ("Did not find a hash for key \"" << key << "\"\n";
    PrintLine ();

    return ~((I)0);
}

// static byte Find2 (Set2* multimap, const char* key) {
//    return SetFind<byte, uint16_t, uint16_t, uint16_t> (multimap, key);
//}

/* Prints this object out to the console. */
template <typename UI, typename SI, typename I>
void MultimapPrint(const TMap<SI, I>* multimap) {
  if (multimap == nullptr) return;
  I item_count = multimap->item_count, count_max = multimap->count_max,
    collision_index, temp;
  SI table_size = multimap->table_size, size_pile = multimap->size_pile;
  PrintLine('_');

  if (sizeof(UI) == 2)
    PRINTF("\nSet2: %p\n", multimap);
  else if (sizeof(UI) == 4)
    PRINTF("\nSet4: %p\n", multimap);
  else if (sizeof(UI) == 8)
    PRINTF("\nSet8: %p\n", multimap);
  else
    PRINTF("\nInvalid TMultimap type: %p\n", multimap);
  PRINTF(
      "\n;: %u count_max: %u  "
      "size_pile: %u  size: %u",
      item_count, count_max, size_pile, table_size);
    PRINTF ('\n';
    PRINTF ('|';
    for (int i = 0; i < 79; ++i) Print ('_');
    PRINTF ('\n';

    const char* states = reinterpret_cast<const char*> (multimap) +
                         sizeof (TMap <SI, I>);
    const SI* key_offsets = reinterpret_cast<const SI*> 
                              (states + count_max);
    const UI* data_offsets = reinterpret_cast<const UI*> 
                                (states + count_max *(sizeof (SI)));
    const UI* hashes = reinterpret_cast<const UI*> (states + count_max *
        (sizeof (SI) + sizeof (UI)));
    const I* indexes = reinterpret_cast<const I*> 
                            (states + count_max * (sizeof (SI) + 
                             sizeof (UI) + sizeof (I))),
        * unsorted_indexes = indexes + count_max,
        * collission_list = unsorted_indexes + count_max,
        *begin;
    const char* keys = reinterpret_cast<const char*> (multimap) + table_size - 1;

    PRINTF ("\n%3s%10s%8s%10s%10s%10s%10s%11s\n", "i", "key", "offset",
            "hash_e", "hash_u", "hash_s", "index_u", "collisions");
   PRINTF ('|';
    for (int i = 0; i < 79; ++i)
        Print ('_');
    PRINTF ('\n';

    for (I i = 0; i < count_max; ++i) {
    // Print each record as a row.
    // @todo Change count_max to ; after done debugging.
    collision_index = indexes[i];
    PRINTF("\n%3i %9s %7u %9x %9x %9x %9u %10u: ", i, keys - key_offsets[i],
           key_offsets[i], Hash16(keys - key_offsets[i]),
           hashes[unsorted_indexes[i]], hashes[i], unsorted_indexes[i],
           collision_index);

    if (collision_index != ~0 && i < item_count) {
      // Print collisions.
      begin = &collission_list[collision_index];
      temp = *begin;
      ++begin;
      PRINTF("%u", temp);
      while (temp != ~0) {
        temp = *begin;
        ++begin;
        if (temp == ~0) break;
        PRINTF(", %u", temp);
      }
    }

        PRINTF ('\n';
    }
    PrintLine ('_');

    PrintMemory (reinterpret_cast<const char*> (multimap) + 
                 sizeof (TMap<SI, I>), multimap->size);
    PRINTF ('\n';
}

/* Deletes the multimap contents without wiping the contents. */
template <typename UI, typename SI, typename I>
void MultimapClear(TMap<SI, I>* multimap) {
  if (multimap == nullptr) return;
  multimap->item_count = 0;
  multimap->size_pile = 0;
}

/* Deletes the multimap contents by overwriting it with zeros. */
template <typename UI, typename SI, typename I>
void MultimapWipe(TMap<SI, I>* multimap) {
  if (multimap == nullptr) return;
  UI size = multimap->size;
  memset(multimap, 0, size);
}

/* Returns true if this expr contains only the given address. */
template <typename UI, typename SI, typename I>
bool MultimapContains(TMap<SI, I>* multimap, void* data) {
  if (multimap == nullptr) return false;
  if (data < multimap) return false;
  if (data > GetEndAddress()) return false;
  return true;
}

/* Removes that object from the multimap and copies it to the destination. */
template <typename UI, typename SI, typename I>
bool MultimapRemoveCopy(TMap<SI, I>* multimap, void* destination,
                        size_t buffer_size, void* data) {
  if (multimap == nullptr) return false;

  return false;
}

/* Removes the item at the given address from the multimap. */
template <typename UI, typename SI, typename I>
bool MultimapRemove(TMap<SI, I>* multimap, void* adress) {
  if (multimap == nullptr) return false;

  return false;
}

/* Removes all but the given multimap from the multimap. */
template <typename UI, typename SI, typename I>
bool MultimapRetain(TMap<SI, I>* multimap) {
  if (multimap == nullptr) return false;

  return false;
}

/* Creates a multimap from dynamic memory. */
template <typename UI, typename SI, typename I>
TMap<SI, I>* MultimapCreate(I buffered_indexes, UI table_size, UI size) {
  TMap<SI, I>* multimap = New<TMap, uint_t>();
  return multimap;
}

/* Prints the given TMultimap to the console. */
template <typename UI, typename SI, typename I>
Utf8& MultimapPrint(Utf8& print, TMap<SI, I>* multimap) {}

/* C++ Wrapper class for an ASCII Multimap that uses dynamic memory and can
    auto-grow. */
template <typename UI, typename SI, typename I>
class Multimap {
 public:
  Multimap(UI size) { MultimapInit<UI, SI, I> }

  /* Deletes the multimap and it's dynamic memory. */
  ~Multimap() { delete begin; }

  constexpr uint_t MultimapOverheadPerIndex() {
    return MultimapOverheadPerIndex<SI, I>();
  };

  constexpr I SizeMin(I item_count) {
    return MultimapSizeMin<SI, I>(item_count);
  };

  /* Insets the given key-value pair.
   */
  inline I Insert(byte type, const char* key, void* data, I index) {
    auto multimap = reinterpret_cast<Multimap<SI, I>>(begin);
    return MultimapInsert<SI, I>(multimap, type, key, data, index);
  }

  inline I IndexMax() { return MultimapIndexMax<SI, I>(); }

  inline I Add(const char* key, TType type, void* data) {
    auto multimap = reinterpret_cast<Multimap<SI, I>>(begin);
    return MultimapAdd<SI, I>(multimap, key, type, data);
  }

  /* Returns  the given query char in the hash table. */
  inline I Find(const char* key) {
    auto multimap = reinterpret_cast<Multimap<SI, I>>(begin);
    return MultimapFind(multimap, key);
  }

  /* Deletes the multimap contents without wiping the contents. */
  inline void Clear() {
    auto multimap = reinterpret_cast<Multimap<SI, I>>(begin);
    return MultimapClear<SI, I>(multimap);
  }

  /* Deletes the multimap contents by overwriting it with zeros. */
  inline void Wipe() {
    auto multimap = reinterpret_cast<Multimap<SI, I>>(begin);
    return Wipe<SI, I>(multimap);
  }

  /* Returns true if this expr contains only the given address. */
  inline bool Contains(void* data) {
    auto multimap = reinterpret_cast<Multimap<SI, I>>(begin);
    return MultimapContains<SI, I>(multimap, void* data);
  }

  /* Removes that object from the multimap and copies it to the destination. */
  inline bool MultimapRemoveCopy(void* destination, size_t buffer_size,
                                 void* data) {
    auto multimap = reinterpret_cast<Multimap<SI, I>>(begin);
    return;
  }

  /* Removes the item at the given address from the multimap. */
  inline bool MultimapRemove(void* adress) {
    auto multimap = reinterpret_cast<Multimap<SI, I>>(begin);
    return MultimapRemove<SI, I>(multimap);
  }

  /* Removes all but the given multimap from the multimap. */
  inline bool Retain() {
    auto multimap = reinterpret_cast<Multimap<SI, I>>(begin);
    return MultimapRetain<SI, I>(multimap);
  }

  /* Prints the given TMultimap to the console. */
  inline Utf8& Print(Utf8& printer) {
    auto multimap = reinterpret_cast<Multimap<SI, I>>(begin);
    return MultimapPrint<SI, I>(multimap);
  }

 private:
  uintptr_t* begin;  //< Dynamic memory buffer.

  TMap
};

using Multimap2 = TMap<byte, uint16_t, uint16_t>;
using Multimap4 = TMap<uint16_t, uint16_t, uint32_t>;
using Multimap8 = TMap<uint32_t, uint32_t, uint64_t>;

}  // namespace _

#undef PRINTF
#undef PRINT
#undef PRINT_HEADING
#endif  //< INCLUDED_CRABS_BOOK
#endif  //< #if SEAM >= SEAM_0_0_2

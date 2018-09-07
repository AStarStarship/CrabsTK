/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/libraries/crabs/dictionary.h
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
#define PRINTF(format, ...) printf(format, __VA_ARGS__)
#define PRINT(c) putchar(c)
#define PRINT_HEADING \
  putchar('\n');      \
  for (int i = 80; i > 0; --i) std::cout << '-'
#endif
#else
#define PRINTF(x, ...)
#define PRINT(c)
#define PRINT_HEADING
#endif

namespace _ {

/* A one-to-one map of Key-{Type-Value} tuples.
  In mathematics, a tuple is a finite ordered list (sequence) of elements. A
  Dictionary is a hash-table that uses contiguous memory. The memory layout is
  the same for all of the Dictionary types as depicted below:

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
  |^^^^^^^^^^^^^^^^^^^^^^^^^^|   |     |
  |_______ Collision N       |   |     |
  |_______ ...               |   |     |
  |        Collision 1       |   |     |
  |==========================|   |     |
  |_______ count_max         |   |     |
  |_______ ...               |   |     |
  |_______ Collision Index N |   |     |
  |_______ ...               |   |     |
  |        Collision Index 1 |   |     |
  |==========================|   |    Size
  |_______ count_max         |   |     |
  |_______ ...               |   |     |
  |_______ Collision Table N |   |     |
  |_______ ...               |   |     |
  |        Collision Table 1 |   |     |
  |==========================| Header  |
  |_______ count_max         |   |     |
  |_______ ...               |   |     |
  |_______ Key Offset N      |   |     |
  |_______ ...               |   |     |
  |        Key Offset 1      |   |     |
  |==========================|   |     |
  |_______ count_max         |   |     |
  |_______ ...               |   |     |
  |_______ Sorted Mappings N |   |     |
  |_______ ...               |   |     |
  |        Sorted Mappings 1 |   |     |
  |==========================|   |     |
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

  | Dictionary | Max Values | % Collisions (p) |           Overhead |
  |:----:|:----------:|:----------------:|:------------------------------:|
  |  2   |     255    |    0.0001        | Ceiling (0.02*p*2^8)  = 2      |
  |  4   |     2^13   |      0.1         | Ceiling (0.04*p*2^13) = 327.68 |
  |  8   |     2^29   |    10^-16        | Ceiling (0.04*p*2^29) = 327.68 |

  Memory Schema:
  The memory schema is optimized for fast search and push back. When searching
  for a key, there might or might not be a hash table.

  How to calculate size:
  The size of any size dictionary can be calculated as follows:
  size = ; * (2*sizeof (I) + sizeof (UI)) + collissionSize +

  # Cache Page Optimizations
  In order to optimize the cache pages, we need to group hot data together.
  ChineseRoom Objs work through calling by I, or by key by using the
  function '\"' (i.e. "foo" is I 44).

  # Hash Table Collisions.
  Because there are no pointers in Script collections, the hash tables are
  done using using a nil-terminated list in the Collision List. In the

  # Use Case Scenario
  We are creating a plug-in API. We need to create a dictionary in the API
  code, and pass it over to the program. The API manages the memory for the
  dictionary. This dictionary might contain several million entries, and more
  than 4GB of data.

  ### Why So Many Dictionary Types?
  We are running in RAM, and a dictionary could contain millions of key-value
  pairs. Adding extra bytes would added megabytes of data we don't need. Also,
  on microcontrollers, especially 16-bit ones, will have very little RAM, so we
  need an 16-bit object. It is easy to imagine a complex AI software using
  more than 4GB RAM, or the need to attach a DVD ISO image as a key-value
  pair, so we need a 64-bit dictionary.

  # Design Strengths
  * Uses less memory.
  * Fast push back when within buffer size.
  * Faster inserts on small collections when within buffer size.

  # Design Weaknesses
  * Slow insert in large collections.
  * Slow at growing large collections when buffer runs out.
  * More complicated.

  @code
  ;
  @endcode
*/
template <typename UI, typename SI, typename I>
struct KABUKI TKeyTuple {
  UI size;        //< Total size of the set.
  SI table_size,  //< Size of the (optional) key strings in bytes.
      size_pile;  //< Size of the (optional) collisions pile in bytes.
  I item_count,   //< Number of items.
      count;      //< Max number of items that can fit in the header.
};

using Dic2 = Dictionary<int8_t, uint16_t, uint16_t>;
using Dic4 = Dictionary<int16_t, uint16_t, uint32_t>;
using Dic8 = Dictionary<int32_t, uint32_t, uint64_t>;

template <typename UI, typename SI, typename I>
constexpr uint_t DicOverheadPerIndex() {
  return sizeof(2 * sizeof(I) + sizeof(SI) + sizeof(UI) + 3);
};

template <typename UI, typename SI, typename I>
constexpr UI MinSizeDic(I item_count) {
  return item_count * sizeof(2 * sizeof(I) + sizeof(SI) + sizeof(UI) + 3);
};

enum {
  kMaxNumMappingsDic2 = 255,                //< The number of pages in a Dic2.
  kMaxNumMappingsDic4 = 8 * 1024,           //< The number of pages in a Dic4.
  kMaxNumMappingsDic8 = 256 * 1024 * 1024,  //< The number of pages in a Dic8.
  kOverheadPerDic2Index = DicOverheadPerIndex<byte, uint16_t, uint16_t>(),
  kOverheadPerDic4Index = DicOverheadPerIndex<uint16_t, uint16_t, uint32_t>(),
  kOverheadPerDic8Index = DicOverheadPerIndex<uint32_t, uint32_t, uint64_t>(),
};

/* Initializes a Dictionary.
    @post    Users might want to call the IsValid () function after construction
             to verify the integrity of the object.
    @warning The reservedNumOperands must be aligned to a 32-bit value, and it
             will get rounded up to the next higher multiple of 4. */
template <typename UI, typename SI, typename I>
Dictionary<UI, SI, I>* DictionaryInit(uintptr_t* buffer, byte max_size,
                                      uint16_t table_size, uint16_t size) {
  ASSERT(buffer);
  if (table_size >= (size - sizeof(Dictionary<UI, SI, I>))) return nullptr;
  if (table_size <
      sizeof(Dictionary<UI, SI, I>) +
          max_size * (DicOverheadPerIndex<byte, uint16_t, uint16_t>() + 2))
    return nullptr;

  Dictionary<UI, SI, I>* dictionary =
      reinterpret_cast<Dictionary<UI, SI, I>*>(buffer);
  dictionary->size = table_size;
  dictionary->table_size = table_size;
  dictionary->item_count = 0;
  dictionary->count = max_size;
  dictionary->size_pile = 1;
  return dictionary;
}

Dictionary<uint16_t, uint16_t, int8_t>* DictionaryInit(uintptr_t* buffer,
                                                       byte max_size,
                                                       uint16_t table_size,
                                                       uint16_t size) {
  return DictionaryInit<uint16_t, uint16_t, int8_t>(buffer, max_size,
                                                    table_size, size);
}

/* Insets the given key-value pair.
 */
template <typename UI, typename SI, typename I, typename T, TType kType>
I DictionaryInsert(Dictionary<UI, SI, I>* dictionary, const char* key, T value,
                   I index) {
  if (dictionary == nullptr) return 0;
  return ~0;
}

template <typename I>
I DictionaryCountUpperBounds() {
  // return sizeof (I) == 1 ? 255 : sizeof (I) == 2 ? 8 * 1024 :
  //                                    sizeof (I) == 4 ? 512 * 1024 * 1024
  //                            : 0;
  return ~(I)0;
}

/* Adds a key-value pair to the end of the dictionary. */
template <typename UI, typename SI, typename I, typename T, TType type>
I DictionaryAdd(Dictionary<UI, SI, I>* dictionary, const char* key, T data) {
  if (dictionary == nullptr) return 0;
  if (key == nullptr) return 0;

  PrintLine(key);

  I item_count = dictionary->item_count, count = dictionary->count, temp;

  SI table_size = dictionary->table_size;

  if (item_count >= count) return ~0;
  //< We're out of buffered indexes.

  char* states =
      reinterpret_cast<char*>(dictionary) + sizeof(Dictionary<UI, SI, I>);
  SI* key_offsets = reinterpret_cast<SI*>(states + count);
  UI* data_offsets = reinterpret_cast<UI*>(states + count * (sizeof(SI)));
  UI *hashes =
         reinterpret_cast<UI*>(states + count * (sizeof(SI) + sizeof(UI))),
     *hash_ptr;
  I *indexes = reinterpret_cast<I*>(
        states + count * (sizeof(SI) + sizeof(UI) + sizeof(I))),
    *unsorted_indexes = indexes + count,
    *collission_list = unsorted_indexes + count;
  char *keys = reinterpret_cast<char*>(dictionary) + table_size - 1,
       *destination;

  // Calculate space left.
  SI value = table_size - count * DicOverheadPerIndex<I, SI, UI>(),
     key_length = static_cast<uint16_t>(SlotLength(key)), size_pile;

  PrintLine();
  PRINTF(
      "Adding Key %s\n%20s: 0x%p\n%20s: %p\n%20s: 0x%p\n"
      "%20s: %p\n%20s: %u\n",
      key, "hashes", hashes, "key_offsets", key_offsets, "keys", keys,
      "indexes", indexes, "value", value)

  UI hash = Hash16(key), current_hash;

  if (key_length > value) {
    PRINTF("Buffer overflow\n")
    return ~((I)0);
  }

  // print ();

  if (item_count == 0) {
    dictionary->item_count = 1;
    *hashes = hash;
    *key_offsets = static_cast<uint16_t>(key_length);
    *indexes = ~0;
    *unsorted_indexes = 0;
    destination = keys - key_length;

    SlotWrite(destination, key);
    PRINTF("\nInserted key %s at GetAddress 0x%p", key, destination)
    DicPrint(dictionary);
    return 0;
  }

  // Calculate left over buffer size by looking up last char.

  if (key_length >= value) {
    PRINTF("\nNot enough room in buffer!")
    return 0;  //< There isn't enough room left in the buffer.
  }

  PRINTF("\nFinding insert location...")

  int low = 0, mid, high = item_count, index;

  I* temp_ptr;

  while (low <= high) {
    mid = (low + high) >> 1;  //< Shift >> 1 to / 2

    current_hash = hashes[mid];
    PRINTF("high: %i mid: %i low %i hash: %x\n", high, mid, low, current_hash)

    if (current_hash > hash) {
      high = mid - 1;
    } else if (current_hash < hash) {
      low = mid + 1;
    } else {  // Duplicate hash detected.
      PRINTF("hash detected, ")

      // Check for other collisions.

      index = indexes[mid];  //< Index in the collision table.

      PRINTF("index:%u", index)

      if (index < ~0) {  //< There are other collisions.
        PRINTF("with collisions, ")
        // There was a collision so check the table.

        // The collisionsList is a sequence of indexes terminated
        // by an invalid index. collissionsList[0] is
        // an invalid index, so the collisionsList is searched from
        // lower address up.
        temp = indexes[mid];
        temp_ptr = collission_list + temp;
        index = *temp_ptr;  //< Load the index in the collision table.
        while (index < MaxDicIndexes<I>()) {
          PRINTF("comparing to \"%s\"", keys - key_offsets[index])
          if (strcmp(key, keys - key_offsets[index]) == 0) {
            PRINTF(" but table already contains key at offset: %u.\n", index)
            return index;
          }
          ++temp_ptr;
          index = *temp_ptr;
        }

        // Its a new collision!
        PRINTF("and new collision detected.")

        // Copy the key
        value = key_offsets[item_count - 1] + key_length + 1;
        SlotWrite(keys - value, key);
        key_offsets[item_count] = value;

        // Update the collision table.
        size_pile = dictionary->size_pile;
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

        dictionary->size_pile = size_pile + 1;
        PRINTF("\ncollision index: %u", temp)
        // Store the collision index.
        indexes[item_count] = temp;  //< Store the collision index
        dictionary->item_count = item_count + 1;
        hashes[item_count] = ~0;  //< Dictionary the last hash to 0xFFFF

        // Move collisions pointer to the unsorted_indexes.
        indexes += count;

        //< Add the newest char to the end.
        indexes[item_count] = item_count;

        DicPrint(dictionary);
        PRINTF("Done inserting.\n")
        return item_count;
      }

      // But we still don't know if the char is a new collision.

      PRINTF("Checking if it's a collision... ")

      if (strcmp(key, keys - key_offsets[index]) != 0) {
        // It's a new collision!
        PRINTF("It's a new collision!\n")

        if (value < 3) {
          PRINTF("Buffer overflow!\n")
          return ~0;
        }

        // Get offset to write the key too.
        value = key_offsets[item_count - 1] + key_length + 1;

        byte collision_index = unsorted_indexes[mid];
        PRINTF("\n\ncollision_index: %u", collision_index)

        SlotWrite(keys - value, key);
        PRINTF(
            "\nInserting value: %u into index:%u item_count:%u with other "
            "collision_index: %u",
            value, index, item_count, collision_index)
        key_offsets[item_count] = value;

        size_pile = dictionary->size_pile;
        indexes[mid] = static_cast<byte>(size_pile);
        indexes[item_count] = static_cast<byte>(size_pile);

        // Insert the collision into the collision table.
        temp_ptr = &collission_list[size_pile];
        // Move collisions pointer to the unsorted_indexes.
        indexes += count;
        *temp_ptr = collision_index;
        ++temp_ptr;
        *temp_ptr = item_count;
        ++temp_ptr;
        *temp_ptr = ~0;
        dictionary->size_pile = size_pile + 3;
        //< Added one term-byte and two indexes.

        // Add the newest key at the end.
        indexes[item_count] = item_count;

        // Dictionary the last hash to 0xFFFF
        hashes[item_count] = ~0;

        dictionary->item_count = item_count + 1;

        DicPrint(dictionary);

        DicPrint(dictionary);
        PRINTF("\nDone inserting.")
        // Then it was a collision so the table doesn't contain string.
        return item_count;
      }
      PRINTF("\nTable already contains the key")
      return index;
    }
  }

  // The hash was not in the table.

  value = key_offsets[item_count - 1] + key_length + 1;
  destination = keys - value;

  PRINTF(
      "The hash 0x%x was not in the table so inserting %s into mid: %i at "
      "index %u before hash 0x%x \n",
      hash, key, mid, Diff(dictionary, destination), hashes[mid]);

  // First copy the char and set the key offset.
  SlotWrite(destination, key);
  key_offsets[item_count] = value;

  // Second move up the hashes and insert at the insertion point.
  hash_ptr = hashes;
  hash_ptr += item_count;
  //*test = hashes;
  PRINTF("l_numkeys: %u, hashes: %u hash_ptr: %u insert_ptr: %u\n", item_count,
         Diff(dictionary, hashes), Diff(dictionary, hash_ptr),
         Diff(dictionary, hashes + mid))
  hashes += mid;
  DicPrint(dictionary);
  while (hash_ptr > hashes) {
    *hash_ptr = *(hash_ptr - 1);
    --hash_ptr;
  }
  *hashes = hash;

  // Mark as not having any collisions.
  indexes[item_count] = ~0;

  // Move up the sorted indexes and insert the unsorted index (which is
  // the current ;).
  indexes += count + mid;
  temp_ptr = indexes + item_count;

  while (temp_ptr > indexes) {
    *temp_ptr = *(temp_ptr - 1);
    --temp_ptr;
  }
  *temp_ptr = item_count;

  dictionary->item_count = item_count + 1;

  DicPrint(dictionary);
  PRINTF("Done inserting.\n")
  PrintLine();

  return item_count;
}

/* Adds a key-value pair to the end of the dictionary. */
// byte Add2 (Dic2* dictionary, const char* key, byte data) {
//    return DicAdd<byte, uint16_t, uint16_t, uint16_t> (dictionary, key, UI1,
//    &data);
//}

/* Returns  the given query char in the hash table. */
template <typename UI, typename SI, typename I>
I DictionaryFind(Dictionary<UI, SI, I>* dictionary, const char* key) {
  if (dictionary == nullptr) return 0;
  PRINT_HEADING("Finding record...")
  I index, item_count = dictionary->item_count, count = dictionary->count, temp;

  if (key == nullptr || item_count == 0) return ~((I)0);

  SI table_size = dictionary->table_size;

  const UI* hashes =
      reinterpret_cast<const UI*>(reinterpret_cast<const char*>(dictionary) +
                                  sizeof(Dictionary<UI, SI, I>));
  const SI* key_offsets = reinterpret_cast<const uint16_t*>(hashes + count);
  const I *indexes = reinterpret_cast<const I*>(key_offsets + count),
          *unsorted_indexes = indexes + count,
          *collission_list = unsorted_indexes + count;
  const char* keys = reinterpret_cast<const char*>(dictionary) + table_size - 1;
  const I *collisions, *temp_ptr;

  UI hash = Hash16(key);

  PRINTF("\nSearching for key \"%s\" with hash 0x%x", key, hash)

  if (item_count == 1) {
    if (!SlotEquals(key, keys - key_offsets[0])) {
      PRINTF("\nDid not find key %s", key)
      return ~((I)0);
    }
    PRINTF("\nFound key \"%s\"", key)
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
    PRINTF("\nlow: %i mid: %i high %i hashes[mid]:%x", low, mid, high,
           hashes[mid])

    if (current_hash > hash) {
      high = mid - 1;
    } else if (current_hash < hash) {
      low = mid + 1;
    } else {
      // Duplicate hash found.
      PRINTF(
          "\nFound same hash at mid:%i hash:%x offset for key: "
          "%s",
          mid, hashes[mid], key)

      // Check for collisions
      collisions = reinterpret_cast<const I*>(key_offsets) + count * sizeof(SI);

      index = collisions[mid];

      if (index < ~0) {
        // There was a collision so check the table.
        PRINTF("\nThere was a collision so check the table")

        // The collisionsList is a sequence of indexes terminated by
        // an invalid index > kMaxNumOperands. collissionsList[0] is an
        // invalid index, so the collisionsList is searched from
        // lower address up.

        temp = indexes[mid];

        temp_ptr = collission_list + temp;
        index = *temp_ptr;
        while (index < MaxDicIndexes<I>()) {
          PRINTF("\nComparing to \"%s\"", keys - key_offsets[index]);
          if (!SlotEquals(key, keys - key_offsets[index])) {
            PRINTF("but table already contains key at offset:%u.", index)
            return index;
          }
          ++temp_ptr;
          index = *temp_ptr;
        }
        PRINTF("Did not find \"" << key << "\"\n")
        return ~((I)0);
      }

      // There were no collisions.

      // But we still don't know if the char is new or a collision.

      // Move collisions pointer to the unsorted indexes.
      indexes += count;
      index = unsorted_indexes[mid];

      PRINTF("\n!!!mid: %i-%x unsorted_indexes: %u key: %s\n hash: %x\n", mid,
             hashes[mid], index, keys - key_offsets[index],
             Hash16(keys - key_offsets[index]))

      if (!SlotEquals(key, keys - key_offsets[index]) != 0) {
        //< It was a collision so the table doesn't contain string.
        PRINTF(" but it was a collision and did not find key.\n")
        return ~((I)0);
      }

      PRINTF("and found key at mid: %i", (int)mid)
      return index;
    }
  }
  PRINTF("\nDid not find a hash for key \"%s\"", key)
  PrintLine();

  return ~((I)0);
}

/* Prints this object out to the console. */
template <typename UI, typename SI, typename I>
Utf8& DicPrint(Utf8& print, const Dictionary<UI, SI, I>* dictionary) {
  ASSERT(dictionary)

  I item_count = dictionary->item_count, count = dictionary->count,
    collision_index, temp;
  SI table_size = dictionary->table_size, size_pile = dictionary->size_pile;

  PRINT_LINE('_');

  PRINTF((sizeof(UI) == 2)
             ? "\nDic2:\n"
             : (sizeof(UI) == 4)
                   ? "\nDic4:0x%p\n"
                   : (sizeof(UI) == 8) ? "\nDic8:0x%p\n" : "\nError:")
  PRINTF("\n0x%p %u stack_height: %u size_pile: %u  size: %u\n|", dictionary,
         item_count, stack_height, size_pile, table_size);
  PRINT_LINE('_')
  PRINTF('\n')

  const char* states =
      reinterpret_cast<const char*>(dictionary) + sizeof(Dictionary<UI, SI, I>);
  const SI* key_offsets = reinterpret_cast<const SI*>(states + count);
  // const UI* data_offsets = reinterpret_cast<const UI*>
  //                            (states + stack_height *(sizeof (SI)));
  const UI* hashes =
      reinterpret_cast<const UI*>(states + count * (sizeof(SI) + sizeof(UI)));
  const I *indexes = reinterpret_cast<const I*>(
              states + count * (sizeof(SI) + sizeof(UI) + sizeof(I))),
          *unsorted_indexes = indexes + count,
          *collission_list = unsorted_indexes + count, *begin;
  const char* keys = reinterpret_cast<const char*>(dictionary) + table_size - 1;

  PRINTF("\n%3s%10s%8s%10s%10s%10s%10s%11s\n", "i", "key", "offset", "hash_e",
         "hash_u", "hash_s", "index_u", "collisions");
    PRINTF ('|';
    for (int i = 0; i < 79; ++i)
        putchar ('_');
    PRINT ('\n')

    for (I i = 0; i < count; ++i) {
    // Print each record as a row.
    // @todo Change stack_height to ; after done debugging.
    collision_index = indexes[i];
    PRINTF("\n%3i %9s %7u %9x %9x %9x %9u %10u: ", i, keys - key_offsets[i],
           key_offsets[i], Hash16(keys - key_offsets[i]),
           hashes[unsorted_indexes[i]], hashes[i], unsorted_indexes[i],
           collision_index)

    if (collision_index != ~0 && i < item_count) {
      // Print collisions.
      begin = &collission_list[collision_index];
      temp = *begin;
      ++begin;
      PRINTF("%u@", temp)
      while (temp != ~0) {
        temp = *begin;
        ++begin;
        if (temp == ~0) break;
        PRINTF(", %u$", temp)
      }
    }

        PRINTF ('\n';
    }
    PrintLine ('_');

    PrintMemory (reinterpret_cast<const char*> (dictionary) + 
                 sizeof (Dictionary<UI, SI, I>), dictionary->size);
    PRINT ('\n')
}

/* Deletes the dictionary contents without wiping the contents. */
template <typename UI, typename SI, typename I>
void DicClear(Dictionary<UI, SI, I>* dictionary) {
  ASSERT(dictionary)

  dictionary->item_count = 0;
  dictionary->size_pile = 0;
}

/* Deletes the dictionary contents by overwriting it with zeros. */
template <typename UI, typename SI, typename I>
void DictionaryWipe(Dictionary<UI, SI, I>* dictionary) {
  ASSERT(dictionary)

  UI size = dictionary->size;
  memset(dictionary, 0, size);
}

/* Returns true if this expr contains only the given address. */
template <typename UI, typename SI, typename I>
void* DicContains(Dictionary<UI, SI, I>* dictionary, void* data) {
  ASSERT(dictionary)

  if (data < dictionary) return false;
  char* base = reinterpret_cast<char*>(dictionary);
  if (data < base) return nullptr;
  if (data > base + dictionary->size_bytes) return nullptr;
  return data;
}

/* Removes that object from the dictionary and copies it to the destination. */
template <typename UI, typename SI, typename I>
bool DicRemoveCopy(Dictionary<UI, SI, I>* dictionary, void* destination,
                   size_t buffer_size, void* data) {
  ASSERT(dictionary)

  return false;
}

/* Removes the item at the given address from the dictionary. */
template <typename UI, typename SI, typename I>
bool DicRemove(Dictionary<UI, SI, I>* dictionary, void* adress) {
  ASSERT(dictionary)

  return false;
}

/* Prints the given Dictionary to the console. */
template <typename UI, typename SI, typename I>
Utf8& DicPrint(Utf8& print, Dictionary<UI, SI, I>* dictionary) {
  return print;
}

}  // namespace _
#endif  //< #if SEAM >= SEAM_0_0_3
#endif  //< INCLUDED_CRABS_BOOK

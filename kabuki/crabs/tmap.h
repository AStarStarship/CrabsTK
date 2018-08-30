/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit
@file    ~/kabuki/crabs/map.h
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
#ifndef INCLUDED_CRAPS_MAP
#define INCLUDED_CRAPS_MAP
// Dependencies:
#include "ascii_data_types.h"
#include "socket.h"
// End dependencies.
#if SEAM == SEAM_0_0_2
#ifndef PRINTF
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT(c) Print(c)
#define PRINT_HEADING                             \
  {                                               \
    Print('\n');                                  \
    for (int i = 80; i > 0; --i) std::cout << '-' \
  }
#define PRINT_TYPE(type, value) Console<>().Out() << TypeValue(type, value);
#define WIPE MapWipe<UI, SI>(map);
#define PRINT_LINE(token)                                                 \
  {                                                                       \
    for (int cout_123 = 80; count > 0; --count) std::cout << (char)token; \
  }
#endif
#else
#define PRINTF(x, ...)
#define PRINT(c)
#define PRINT_HEADING
#define PRINT_TYPE(type, value)
#define WIPE(buffer, size)
#define PRINT_LINE(token)
#endif

namespace _ {

/* A one-to-one map that does not use hash table.

    Maps share the same data structure as Multimaps, Dictionary(s) and Lists.
   Maps like all Script Objs are required to are required to have a
   stack_height, size_bytes, and header_size that is a multiple of 8 as well as
   being .

    It is easiest to explain this data structure in terms of the Socket. Sockets
    use a TMap to map a Slot number to a slot.

    # TMap Data Structure

    @code
    +==========================+ -----------
    |_______ Buffer            |   ^     ^
    |_______ ...               |   |     |
    |_______ Data N            |  Data   |
    |_______ ...               |   |     |
    |_______ Data 0            |   v     |
    |==========================| -----   |
    |_______ count_max         |   ^     |
    |_______ ...               |   |     |
    |_______ Sorted Mappings N |   |     |
    |_______ ...               |   |     |
    |        Sorted Mappings 1 |   |     |
    |==========================|   |     |
    |_______ count_max         |   |    Size
    |_______ ...               |   |     |
    |_______ Data Offset N     |   |     |
    |_______ ...               | Header  |
    |        Data Offset 1     |   |     |
    |==========================|   |     |
    |_______ count_max         |   |     |
    |_______ ...               |   |     |
    |_______ Type byte N       |   |     |
    |_______ ...               |   |     |
    |        Type byte 1       |   |     |   ^ Up in addresses
    |==========================|   |     |   |
    |  TSparseArray<UI, SI> Struct |   v     v   ^
    +==========================+ ----------- ^ 0xN
    @endcode

    | Map | Max Values | % Collisions (p) |           Overhead             |
    |:---:|:----------:|:----------------:|:------------------------------:|
    |  2  |     255    |    0.0001        | Ceiling (0.02*p*2^8)  = 2      |
    |  4  |     2^13   |      0.1         | Ceiling (0.04*p*2^13) = 327.68 |
    |  8  |     2^29   |    10^-16        | Ceiling (0.04*p*2^29) = 327.68 |

    Memory Schema:
    The memory schema is optimized for fast search and push back. When searching
    for a key, there might or might not be a hash table.

    How to calculate size:
    The size of any size map can be calculated as follows:
    size = ; * (2*sizeof (SI) + sizeof (UI)) + collissionSize +

    # Cache Page Optimizations

    In order to optimize the cache pages, we need to group hot data together.
    ChineseRoom Objs work through calling by SI, or by key by using the
    function '\"' (i.e. "foo" is SI 44).

    # Use Case Scenario

    We are creating a plug-in DLL. We need to create a map in the DLL code, and
    pass it over to the program. The DLL manages the memory for the map. This
    map might contain several million entries, and more than 4GB of data.

    ### Why So Many TMap Types?

    We are running in RAM, and a map could contain millions of key-value pairs.
    Adding extra bytes would added megabytes of data we don't need. Also, on
    microcontrollers, especially 16-bit ones, will have very little RAM, so we
    need an 16-bit object. It is easy to imagine a complex AI software using
    more than 4GB RAM, or the need to attach a DVD ISO image as a key-value
    pair, so we need a 64-bit map.

    # Design Strengths

    * Uses less memory.
    * Fast push back when within buffer size.
    * Faster inserts on small collections when within buffer size.
    * 64-bit memory alignment ensures highest performace on Intel.
    * Mandating zeros in buffered memory space can serve help sanatize input.

    # Design Weaknesses

    * Slow insert in large collections.
    * Slow at growing large collections when buffer runs out.
    * Applications with billions of items may have too much memory overhead.
    * More complicated.

    @code
    .
    .
    .
    @endcode
*/
template <typename UI = uint32_t, typename SI = int32_t, typename I = int16_t>
struct TMap {
  UI size;        //< ASCII OBJ size.
  SI table_size,  /*< Size of the key strings in bytes.
                      Set to 0 for ASCII Map. */
      size_pile;  /*< Size of the collisions pile in bytes.
                      Set to 0 for ASCII Map and ASCII Multimap. */
  I count,        //< Element count.
      count_max;  //< Max count of items that can fit in the socket.
};

template <typename UI = uint32_t, typename SI = int32_t, typename I = int16_t>
constexpr uint_t MapOverheadPerIndex() {
  return sizeof(2 * sizeof(SI) + sizeof(UI) + sizeof(UI) + 3);
};

template <typename UI = uint32_t, typename SI = int32_t, typename I = int16_t>
constexpr UI MapSizeRequired(SI count) {
  return count * sizeof(2(sizeof(SI) + sizeof(UI)) + 3);
};

enum {
  kMaxNumPagesMap2 = 255,                //< The number of pages in a Map2.
  kMaxNumPagesMap4 = 8 * 1024,           //< The number of pages in a Map4.
  kMaxNumPagesMap8 = 256 * 1024 * 1024,  //< The number of pages in a Map8.
  kOverheadPerMap2Index = MapOverheadPerIndex<uint16_t, int16_t, int8_t>(),
  kOverheadPerMap4Index = MapOverheadPerIndex<uint32_t, int32_t, int16_t>(),
  kOverheadPerMap8Index = MapOverheadPerIndex<uint64_t, int64_t, int32_t>(),
};

/* Initializes a TMap from a word-aligned buffer.
    @post    Users might want to call the IsValid () function after construction
             to verify the integrity of the object.
    @warning The reservedNumOperands must be aligned to a 32-bit value, and it
             will get rounded up to the next higher multiple of 4.
*/
template <typename UI = uint32_t, typename SI = int32_t, typename I = int16_t>
TMap<UI, SI, I>* MapInit(uintptr_t* buffer, UI size, I count_max) {
  ASSERT(buffer)

  if (table_size >= size) return nullptr;
  UI kSizeMin = sizeof(TMap<UI, SI, I>) +
                count_max * (MapOverheadPerIndex<UI, SI, I>() + 2);
  if (table_size < kSizeMin) return nullptr;

  Map<UI, SI, I>* map = reinterpret_cast<TMap*>(buffer);
  map->size = table_size;
  map->table_size = table_size;
  map->size_pile = 1;
  map->count_max = count_max;
  map->count = 0;
  return map;
}
template <typename UI = uint32_t, typename SI = int32_t, typename I = int16_t>
inline UI MapSizeBoundsLower() {
  return 64;
}
template <typename UI = uint32_t, typename SI = int32_t, typename I = int16_t>
inline UI MapSizeBoundsUpper() {
  UI bounds_upper = 0;
  bounds_upper = ~bounds_upper;
  return bounds_upper - 7;
}

template <typename UI = uint32_t, typename SI = int32_t, typename I = int16_t>
inline SI MapCountBoundsLower() {
  return 8 / sizeof(UI);
}

/* Creates a map from dynamic memory. */
template <typename UI = uint32_t, typename SI = int32_t, typename I = int16_t>
uintptr_t* MapNew(UI size = 0, I count_max = 0) {
  size = AlignDown<int64_t, UI>(size);
  count_max = AlignDown<int64_t, SI>(count_max);
  UI size_min = MapSizeBoundsLower<UI, SI, I>();
  SI count_min = MapCountBoundsLower<UI, SI, I>();
  if (size < size_min || count_max < count_min) {
    size = size_min;
    count_max = size_min;
  }

  uintptr_t* buffer = new uintptr_t[size >> kWordBitCount];
  TMap<UI, SI, I>* map = reinterpret_cast<TMap<UI, SI, I>*>(buffer);
  map->size = size;
  map->table_size = 0;
  map->size_pile = 0;
  map->count = 0;
  map->count_max = count_max;
  return buffer;
}

template <typename SI>
SI MapCountUpperBounds() {
  enum {
    kCountUpperBounds = sizeof(SI) == 1
                            ? 120
                            : sizeof(SI) == 2
                                  ? 8 * 1024
                                  : sizeof(SI) == 4 ? 512 * 1024 * 1024 : 0
  };
  return CountUpperBounds;
}

/* Insets the given key-type-value typle.
 */
template <typename UI = uint32_t, typename SI = int32_t, typename I = int16_t>
SI MapInsert(TMap<UI, SI, I>* map, void* value, type_t type, SI index) {
  ASSERT(map)
  ASSERT(value)

  PRINT_LINE(id);

  SI count = map->count, count = map->stack->count, temp;

  UI table_size = map->table_size;

  if (count >= count) return ~0;
  //< We're out of buffered indexes.

  char* types = reinterpret_cast<char*>(map) + sizeof(TMap<UI, SI, I>);
  UI* data_offsets = reinterpret_cast<UI*>(types + count * (sizeof(UI)));
  UI *hashes = reinterpret_cast<UI*>(types + count * (sizeof(UI) + sizeof(UI))),
     *hash_ptr;
  SI *indexes =
         reinterpret_cast<SI*>(types + count * (sizeof(UI) + 2 * sizeof(SI))),
     *unsorted_indexes = indexes + count,
     *collission_list = unsorted_indexes + count;
  char *keys = reinterpret_cast<char*>(map) + table_size - 1, *destination;

  // Calculate space left.
  UI value = table_size - count * MapOverheadPerIndex<UI, SI, I>(),
     key_length = static_cast<uint16_t>(strlen(id)), size_pile;

  PRINT_LINE('-');
  PRINTF(
      "\nAdding Key %s\n%20s: 0x%p\n%20s: %p\n%20s: 0x%p"
      "%20s: %p\n%20s: %u\n",
      id, "hashes", hashes, "key_offsets", key_offsets, "keys", keys, "indexes",
      indexes, "value", value)

  UI temp_id = Hash16(id), current_mapping;

  if (key_length > value) {
    PRINTF("\nBuffer overflow!")
    return ~((SI)0);
  }

  if (count == 0) {
    map->count = 1;
    *hashes = temp_id;
    *key_offsets = static_cast<uint16_t>(key_length);
    *indexes = ~0;
    *unsorted_indexes = 0;
    destination = keys - key_length;

    SlotWrite(destination, id);
    PRINTF("\nInserted key %s at GetAddress 0x%p", id, destination)
    MapPrint(map);
    return 0;
  }

  // Calculate left over buffer size by looking up last char.

  if (key_length >= value) {
    PRINTF("\nNot enough room in buffer!")
    return 0;  //< There isn't enough room left in the buffer.
  }

  PRINTF("\nFinding insert location...")

  SI low = 0, mid, high = count, index;

  SI* temp_ptr;

  while (low <= high) {
    mid = (low + high) >> 1;  //< Shift >> 1 to / 2

    current_mapping = hashes[mid];
    PRINTF("\nhigh: %i mid: %i low %i hash: %x", high, mid, low,
           current_mapping)

    if (current_mapping > temp_id) {
      high = mid - 1;
    } else if (current_mapping < temp_id) {
      low = mid + 1;
    } else {
      PRINTF(" duplicate hash detected, ")

      // Check for other collisions.

      index = indexes[mid];  //< Index in the collision table.

      PRINTF("index:%i", (int)index)

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
        while (index < MapCountUpperBounds<SI>()) {
          PRINTF("\ncomparing to \"%s\"", keys - key_offsets[index]);
          if (strcmp(id, keys - key_offsets[index]) == 0) {
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
        PRINTF(" new collision detected.")

        // Copy the key
        value = key_offsets[count - 1] + key_length + 1;
        SlotWrite(keys - value, id);
        key_offsets[count] = value;

        // Update the collision table.
        size_pile = map->size_pile;
        // Shift the collisions table up one element and insert
        // the unsorted collision index.
        // Then move to the top of the collisions list.
        collission_list += size_pile;
        // and iterate down to the insert spot
        while (collission_list > temp_ptr) {
          *collission_list = *(collission_list - 1);
          --collission_list;
        }
        *temp_ptr = count;

        map->size_pile = size_pile + 1;
        PRINTF("\ncollision index: %u", temp)
        // Store the collision index.
        indexes[count] = temp;  //< Store the collision index
        map->count = count + 1;
        hashes[count] = ~0;  //< TMap the last hash to 0xFFFF

        // Move collisions pointer to the unsorted_indexes.
        indexes += count;

        //< Add the newest char to the end.
        indexes[count] = count;

        MapPrint(map);
        PRINTF("\nDone inserting.")
        return count;
      }

      // But we still don't know if the char is a new collision.

      PRINTF("\nChecking if it's a collision... ")

      if (strcmp(id, keys - key_offsets[index]) != 0) {
        // It's a new collision!
        PRINTF("It's a new collision!")

        if (value < 3) {
          PRINTF("Buffer overflow!")
          return ~0;
        }

        // Get offset to write the key too.
        value = key_offsets[count - 1] + key_length + 1;

        type_t collision_index = unsorted_indexes[mid];
        PRINTF("\n\ncollision_index: %u", collision_index)

        SlotWrite(keys - value, id);
        PRINTF(
            "\nInserting value: %u into index:%u "
            "count:%u with other collision_index: %u",
            value, index, count, collision_index)
        key_offsets[count] = value;

        size_pile = map->size_pile;
        indexes[mid] = static_cast<type_t>(size_pile);
        indexes[count] = static_cast<type_t>(size_pile);

        // Insert the collision into the collision table.
        temp_ptr = &collission_list[size_pile];
        // Move collisions pointer to the unsorted_indexes.
        indexes += count;
        *temp_ptr = collision_index;
        ++temp_ptr;
        *temp_ptr = count;
        ++temp_ptr;
        *temp_ptr = ~0;
        map->size_pile = size_pile + 3;
        //< Added one term-type_t and two indexes.

        // Add the newest key at the end.
        indexes[count] = count;

        // TMap the last hash to 0xFFFF
        hashes[count] = ~0;

        map->count = count + 1;

        MapPrint(map);

        MapPrint(map);
        PRINTF("\nDone inserting.")
        // Then it was a collision so the table doesn't contain string.
        return count;
      }
      PRINTF("\nTable already contains the key")
      return index;
    }
  }

  // The id was not in the table.

  value = key_offsets[count - 1] + key_length + 1;
  destination = keys - value;

  PRINTF(
      "\nThe id 0x%x was not in the table so inserting %s into mid:"
      " %i at index %u before hash 0x%x",
      temp_id, id, mid, Diff(map, destination), hashes[mid])

  // First copy the char and set the key offset.
  SlotWrite(destination, id);
  key_offsets[count] = value;

  // Second move up the hashes and insert at the insertion point.
  hash_ptr = hashes;
  hash_ptr += count;
  //*test = hashes;
  PRINTF("\nl_numkeys: %u, hashes: %u hash_ptr: %u insert_ptr: %u", count,
         Diff(map, hashes), Diff(map, hash_ptr), Diff(map, hashes + mid))
  hashes += mid;
  MapPrint(map);
  while (hash_ptr > hashes) {
    *hash_ptr = *(hash_ptr - 1);
    --hash_ptr;
  }
  *hashes = temp_id;

  // Mark as not having any collisions.
  indexes[count] = ~0;

  // Move up the sorted indexes and insert the unsorted index (which is
  // the current count).
  indexes += count + mid;
  temp_ptr = indexes + count;

  while (temp_ptr > indexes) {
    *temp_ptr = *(temp_ptr - 1);
    --temp_ptr;
  }
  *temp_ptr = count;

  map->count = count + 1;

  MapPrint(map);
  PRINTF("\nDone inserting.")
  PRINT_LINE('-')

  return count;
}

/* Deletes the map contents without wiping the contents. */
template <typename UI = uint32_t, typename SI = int32_t, typename I = int16_t>
void MapClear(TMap<UI, SI, I>* map) {
  ASSERT(map)

  map->count = 0;
  map->size_pile = 0;
}

/* Deletes the map contents by overwriting it with zeros. */
template <typename UI = uint32_t, typename SI = int32_t, typename I = int16_t>
void MapWipe(TMap<UI, SI, I>* map) {
  ASSERT(map)

  UI size = map->size;
  MemoryWipe(reinterpret_cast<char*>(map) + sizeof(TMap<UI, SI, I>), size);
}

/* Returns true if this expr contains only the given address. */
template <typename UI = uint32_t, typename SI = int32_t, typename I = int16_t>
bool MapContains(TMap<UI, SI, I>* map, void* value) {
  ASSERT(map)

  if (value < map) return false;
  if (value > GetEndAddress()) return false;
  return true;
}

/* Removes the item at the given address from the map. */
template <typename UI = uint32_t, typename SI = int32_t, typename I = int16_t>
bool MapRemove(TMap<UI, SI, I>* map, void* adress) {
  ASSERT(map)

  return false;
}

/* Prints this object out to the console. */
template <typename UI = uint32_t, typename SI = int32_t, typename I = int16_t>
Utf8& MapPrint(Utf8& print, const TMap<UI, SI, I>* map) {
  ASSERT(map)

  SI count = map->count, collision_index, temp;
  UI table_size = map->table_size, size_pile = map->size_pile;

  print << '_';

  print << "\nMap" << sizeof(UI) << ": " << Hex<>(map) << " count:" << count
        << "size_pile:" << size_pile << " size:" << map->size;
  print << "\n|";
  for (int i = 0; i < 79; ++i) print << '_';
  print << '\n';

  const char* states =
      reinterpret_cast<const char*>(map) + sizeof(TMap<UI, SI, I>);
  const UI* key_offsets = reinterpret_cast<const UI*>(states + count);
  const UI* data_offsets =
      reinterpret_cast<const UI*>(states + count * sizeof(UI));
  const UI* hashes =
      reinterpret_cast<const UI*>(states + count * (sizeof(UI) + sizeof(UI)));
  const SI *indexes = reinterpret_cast<const SI*>(
               states + count * (sizeof(UI) + sizeof(UI) + sizeof(SI))),
           *unsorted_indexes = indexes + count,
           *collission_list = unsorted_indexes + count, *begin;
  const char* keys = reinterpret_cast<const char*>(map) + table_size - 1;
  print << "\n " << Right<>("i", 3) << Right<>("key", 10)
        << Right<>("offset", 8) << Right<>("hash_e", 10)
        << Right<>("hash_s", 10) << Right<>("index_u", 10)
        << Right<>("collisions", 11);
  print << '|';
  for (int i = 0; i < 79; ++i) print << '_';
  print << '\n';

  for (SI i = 0; i < count; ++i) {
    // Print each record as a row.
    // @todo Change stack_height to ; after done debugging.
    collision_index = indexes[i];
    print << Right<SI>(i, 3) << Right<SI>(keys - key_offsets[i], 10)
          << Right<SI>(key_offsets[i], 8)
          << Right<Hex<UI>>(Hash16(keys - key_offsets[i]), 10)
          << Right<Hex<UI>>(hashes[unsorted_indexes[i]], 10)
          << Right<Hex<UI>>(hashes[i], 10) << Right<UI>(unsorted_indexes[i], 10)
          << Right<UI>(collision_index, 11);

    if (collision_index != ~0 && i < item_count) {
      // Print collisions.
      begin = &collission_list[collision_index];
      temp = *begin;
      ++begin;
      print << temp;
      while (temp != ~0) {
        temp = *begin;
        ++begin;
        if (temp == ~0) break;
        print << ", " << temp;
      }
    }

    print << '_';
  }

  return print << '_' << Socket(reinterpret_cast<const char*>(map), map->size)
               << '\n';
}

/* C++ Wrapper for the AsciiMap that uses dynamic memory and auto-grows.

    @code
    .
    .
    .
    @endcode
*/
template <typename UI = uint32_t, typename SI = int32_t, typename I = int16_t>
class Map {
 public:
  /* Constructs a Map with the given count_max.
      If count_max is less than 0 it will be set to the default value. IF the
      */
  Map(UI size = 0, I count_max = 0)
      : buffer(MapNew<UI, SI, I>(size, count_max)) {
    // Nothing to do here! ({:-)-+=<
  }

  /* Destructs the dynamically allocated buffer. */
  ~Map() { delete buffer; }

  inline bool Remove(void* adress) {
    return MapRemove<UI, SI, I>(Obj(), adress);
  }

  /* Checks if the map contains the given pointer.
      @return True if the pointer lies in this socket. */
  inline bool Contains(void* value) {
    return MapContains<UI, SI, I>(Obj(), value);
  }

  /* Wipes the map by overwriting it with zeros. */
  inline void Wipe() { MapWipe<UI, SI, I>(Obj()); }

  static inline SI CountUpperBounds() {
    return MapCountUpperBounds<UI, SI, I>();
  }

  inline SI Insert(void* value, SI index, type_t type) {
    return MapInsert<UI, SI, I>(Obj(), value, type, index);
  }

  /* Clears the list. */
  inline void Clear() { MapClear<UI, SI, I>(Obj()); }

  /* Prints this object to a printer. */
  inline Utf8& Print(Utf8& printer) {
    return MapPrint<UI, SI, I>(print, Obj());
  }

 private:
  uintptr_t* buffer;

  /* Returns the buffer casted as a TMap<UI, SI, I>*. */
  inline TMap<UI, SI, I>* Obj() {
    return reinterpret_cast<TMap<UI, SI, I>*>(buffer);
  }
};  //< class Map
}  // namespace _
#endif  //< INCLUDED_CRAPS_MAP
#undef PRINTF
#undef PRINT
#undef PRINT_HEADING
#undef PRINT_TYPE
#undef WIPE
#undef PRINT_LINE
#endif  //< #if SEAM >= SEAM_0_0_2

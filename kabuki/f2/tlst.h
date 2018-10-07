/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/f2/list.h
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
#include <pch.h>

#if SEAM >= SEAM_0_0_0__10
#ifndef INCLUDED_KABUKI_F2_TLIST
#define INCLUDED_KABUKI_F2_TLIST

#include "talign.h"
#include "tset.h"
#include "tstk.h"

namespace _ {

/* An ASCII List header.
Like most ASCII OBJ Types, the size may only be 16-bit, 32-bit, or
64-bit. The unsigned value must be twice the width of the signed value.

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
|_______ Data Offset N     |   |     |
|_______ ...               |   |    Size
|        Data Offset 1     |   |     |
|==========================| Header  |
|_______ count_max         |   |     |
|_______ ...               |   |     |
|_______ Type byte N       |   |     |
|_______ ...               |   |     |
|        Type byte 1       |   |     |   ^ Up in addresses
|==========================|   |     |   |
| AsciiList<UI, SI> Struct |   v     v   ^
+==========================+ ----------- ^ 0xN
@endcode
*/
template <typename UI = uint32_t, typename SI = int16_t>
struct CList {
  UI size;
  SI count_max, count;
};

/* Returns the minimum count to align the data struct to a 64-bit boundary. */
template <typename UI = uint32_t, typename SI = int16_t>
SI ListCountMaxBoundsLower() {
  return 8 / sizeof(SI);
}

/* Returns the min size of an ASCII List with the given count_max.
The min size is defined as enough memory to store the given count_max with
the largest_expected_type.
*/
template <typename UI = uint32_t, typename SI = int16_t,
          size_t largest_expected_type = sizeof(intptr_t)>
constexpr UI ListSizeMin(SI count_max) {
  return (UI)sizeof(CList<UI, SI>) +
         (UI)(count_max * (largest_expected_type + sizeof(UI) + 1));
  // << 2 to * 4.
}

/* Deletes the list contents by overwriting it with zeros. */
template <typename UI = uint32_t, typename SI = int16_t>
void ListWipe(CList<UI, SI>* list) {
  ASSERT(list)
  list->count = 0;
  UI size = list->size - sizeof(CList<UI, SI>);
  memset(reinterpret_cast<char*>(list) + sizeof(CList<UI, SI>), 0, size);
}

/* Initializes a AsciiList from preallocated memory.
count_max must be in multiples of 4. Given there is a fixed size, both the
count_max and size will be downsized to a multiple of 4 automatically. */
template <typename UI = uint32_t, typename SI = int16_t>
CList<UI, SI>* ListInit(uintptr_t* buffer, UI size, SI count_max) {
  if (!buffer)  // This may be nullptr if ListNew<UI,SI> (SI, UI) failed.
    return nullptr;
  PRINTF("\n  Initializing List with size_bytes:%u and count_max:%i",
         (uint)size, (int)count_max)
  SI count_max_bounds_lower = ListCountMaxBoundsLower<UI, SI>();
  if (count_max < count_max_bounds_lower) {
    PRINTF("\n count_max == 0 and is now %i", (int)count_max_bounds_lower)
    count_max = count_max_bounds_lower;
  } else {
    PRINTF("\ncount_max was %i ", (int)count_max)
    count_max = AlignUp<SI>(count_max);
    PRINTF("\n and now is %i.", (int)count_max)
  }

  // count_max = AlignUp<SI> (count_max);
  // PRINTF ("\n  Aligning up to count_max:%i", (int)count_max)

  CList<UI, SI>* list = reinterpret_cast<CList<UI, SI>*>(buffer);
  list->size = size;
  list->count = 0;
  list->count_max = count_max;
  // WIPE
  ListWipe<UI, SI>(list);
  return list;
}

/* Creates a list from dynamic memory. */
template <typename UI = uint32_t, typename SI = int16_t>
uintptr_t* ListNew(SI count_max, UI size) {
  count_max = AlignUpUnsigned<uint64_t, UI>(count_max);
  if (size < ListSizeMin<UI, SI>(count_max)) return nullptr;
  uintptr_t* buffer = new uintptr_t[size >> kWordBitCount];

  CList<UI, SI>* list = reinterpret_cast<CList<UI, SI>*>(buffer);
  list->size = size;
  list->count = 0;
  list->count_max = count_max;
  // WIPE
  ListWipe<UI, SI>(list);
  return buffer;
}

/* Creates a list from dynamic memory. */
template <typename UI = uint32_t, typename SI = int16_t,
          size_t largest_expected_type = sizeof(intptr_t)>
inline uintptr_t* ListNew(SI count_max) {
  count_max = AlignUp<SI>(count_max);
  UI size = ListSizeMin<UI, SI, largest_expected_type>(count_max);
  uintptr_t* buffer = new uintptr_t[size >> kWordBitCount];

  CList<UI, SI>* list = reinterpret_cast<CList<UI, SI>*>(buffer);
  list->size = size;
  list->count = 0;
  list->count_max = count_max;
  // WIPE
  ListWipe<UI, SI>(list);
  return buffer;
}

/* Returns the type bytes array. */
template <typename UI = uint32_t, typename SI = int16_t>
type_t* ListTypes(CList<UI, SI>* list) {
  ASSERT(list)
  return reinterpret_cast<type_t*>(list) + sizeof(CList<UI, SI>);
}

/* Gets a pointer to the begging of the data buffer. */
template <typename UI = uint32_t, typename SI = int16_t>
inline char* ListDataBegin(CList<UI, SI>* list) {
  ASSERT(list)
  return reinterpret_cast<char*>(list) + list->count_max * (sizeof(SI) + 1);
}

/* Gets the base element 0 of the list's offset array. */
template <typename UI = uint32_t, typename SI = int16_t>
inline UI* ListOffsets(CList<UI, SI>* list) {
  uintptr_t ptr = reinterpret_cast<uintptr_t>(list) + sizeof(CList<UI, SI>) +
                  list->count_max;
  return reinterpret_cast<UI*>(ptr);
}

/* Returns the last byte in the data array. */
template <typename UI = uint32_t, typename SI = int16_t>
inline char* ListDataEnd(CList<UI, SI>* list) {
  ASSERT(list)
  return reinterpret_cast<char*>(list) + list->size - 1;
}

/* Returns the last byte in the data array. */
template <typename UI = uint32_t, typename SI = int16_t>
inline char* ListDataEnd(CList<UI, SI>* list, SI index) {
  ASSERT(list)
  if (index < 0 || index >= index->count) return nullptr;
  return reinterpret_cast<char*>(list) + list->size - 1;
}

/* Returns a pointer to the begging of the data buffer. */
template <typename UI = uint32_t, typename SI = int16_t>
Socket ListDataVector(CList<UI, SI>* list) {
  return Socket(ListDataBegin<UI, SI>(list), ListDataEnd<UI, SI>(list));
}

/* Returns the last byte in the data array. */
template <typename UI = uint32_t, typename SI = int16_t>
inline char* ListDataStop(CList<UI, SI>* list, SI index = -1) {
  ASSERT(list)
  SI count = list->count;
  if (count == 0) {
    if (index != -1) return nullptr;
    return ListDataBegin<UI, SI>(list);
  }
  type_t type = ListTypes<UI, SI>(list)[index];
  UI offset = ListOffsets<UI, SI>(list)[index];
  PRINTF("!offset %u", offset)
  char* pointer = reinterpret_cast<char*>(list) + offset;
  return ObjEnd<UI>(pointer, type);
}

template <typename UI = uint32_t, typename SI = int16_t>
void ListDataSpaceBelow(CList<UI, SI>* list, SI index, Socket& free_space) {
  ASSERT(list)
  char* data_stop;
  if (index == 0) {
    data_stop = ListDataBegin<UI, SI>(list);
    free_space.begin = free_space.end = data_stop;
    return;
  }
  ASSERT(index >= 0 && index <= list->count)
  if (index == list->count) {
    free_space.begin = ListDataStop<UI, SI>(list);
    free_space.end = ListDataEnd<UI, SI>(list);
    return;
  }
  data_stop = ListDataStop<UI, SI>(list, index);
  UI* offsets = ListOffsets<UI, SI>(list);
  UI offset_low = offsets[index], offset_high = offsets[index + 1];
}

/* Insets the given type-value tuple.
    @return -1 upon failure or the index upon success. */
template <typename UI = uint32_t, typename SI = int16_t>
SI ListInsert(CList<UI, SI>* list, type_t type, const void* value, SI index) {
  ASSERT(list)
  ASSERT(value)
  PRINTF("\nInserting type:")
  PRINT_TYPE(type, value)
  PRINTF(" into index:%i", index)

  SI count = list->count, count_max = list->count_max;
  if (count >= count_max || index > count || !TypeIsValid(type) || index < 0) {
    PRINTF("\nError inserting type:%s into index %i", TypeString(type),
           (int)index);
    return -1;
  }
  PRINTF(" when count is %i", (int)count)

  type_t* types = ListTypes<UI, SI>(list);

  // 1. Check for stack push operation.
  if (index == count) {
    PRINTF("\nPushing element...")
    // Push type onto the top of the type stack.
    types[index] = type;
    //  Push the offset onto the top of the offset stack.
    char* data_stop = ListDataStop<UI, SI>(list, count - 1);
    PRINTF("\n  Aligning data_stop from %i to ", (int)Size(list, data_stop))
    data_stop = TypeAlignUpPointer<char>(data_stop, type);
    PRINTF("%i", (int)Size(list, data_stop))
    UI stop_offset = (UI)(data_stop - reinterpret_cast<char*>(list));
    ListOffsets<UI, SI>(list)[index] = stop_offset;
    // Write the value to the top of the value stack.
    PRINTF(" leaving %i bytes.", (int)(ListDataEnd<UI, SI>(list) - data_stop))
    if (!Write(data_stop, ListDataEnd<UI, SI>(list), type, value)) return -2;
    list->count = count + 1;
    return index;
  }

  // 2. Shift up the types.
  ArrayInsert<type_t, SI>(types, count, type, index);

  // 3. Calculate the offset to insert at.
  char* aligned_begin = ListDataStop<UI, SI>(list, index);
  PRINTF("\nListDataStop<UI, SI> (list) starts as %p then is aligned to ",
         aligned_begin)
  aligned_begin = TypeAlignUpPointer<char>(aligned_begin, type);
  PRINTF("%p", aligned_begin)

  // 4. Insert the offset.
  PRINTF("\nInserting into ")
  ArrayInsert<UI, SI>(ListOffsets<UI, SI>(list), count, type, index);

  UI space_needed = ObjSize<UI>(value, type);

  // 5. Check if there is enough room to insert the value without shifting up
  // any data.
  Socket free_space;
  ListDataSpaceBelow<UI, SI>(list, index, free_space);
  if (Write(free_space.begin, free_space.end, type, value)) return index;

  // 6. Shift up the data enough to fit the new value.

  // 5. Write the value to the data socket.
  if (!Write(aligned_begin, ListDataEnd<UI, SI>(list), type, value)) return -1;
  list->count = ++count;
  return count;
}

/* Adds a type-value to the end of the list. */
template <typename UI = uint32_t, typename SI = int16_t>
inline SI ListPush(CList<UI, SI>* list, type_t type, const void* value) {
  return ListInsert<UI, SI>(list, type, value, list->count);
}

/* Removes a type-value to the end of the list. */
template <typename UI = uint32_t, typename SI = int16_t>
inline SI ListPop(CList<UI, SI>* list) {
  return ListRemove<UI, SI>(list, list->count - 1);
}

/* Returns the max count an array can handle. */
template <typename UI = uint32_t, typename SI = int16_t>
SI ListCountMax() {
  enum {
    kMaxIndexes = sizeof(SI) == 1
                      ? 120
                      : sizeof(SI) == 2
                            ? 8 * 1024
                            : sizeof(SI) == 4 ? 512 * 1024 * 1024 : 0,
  };
  return kMaxIndexes;
}

/* Deletes the list contents without wiping the contents. */
template <typename UI = uint32_t, typename SI = int16_t>
void ListClear(CList<UI, SI>* list) {
  ASSERT(list)
  list->count = 0;
}

/* Checks if this expr contains only the given address.
@return  True if the data lies in the list's memory socket.

@warning This function assumes that the member you're checking for came
from Kabuki Toolkit. If it's you're own code calling this, you are
required to ensure the value came from a ASCII List. */
template <typename UI = uint32_t, typename SI = int16_t>
bool ListContains(CList<UI, SI>* list, void* address) {
  ASSERT(list)
  if (reinterpret_cast<char*>(address) < reinterpret_cast<char*>(list))
    return false;
  if (reinterpret_cast<char*>(address) > ListEndByte()) return false;
  return true;
}

/* Removes the item at the given address from the list. */
template <typename UI = uint32_t, typename SI = int16_t>
SI ListRemove(CList<UI, SI>* list, SI index) {
  SI count = list->count;
  ArrayRemove<UI, SI>(ListOffsets<UI, SI>(list), count, index);
  return ArrayRemove<type_t, SI>(ListTypes<UI, SI>(list), count, index);
}

/* Finds a tuple that contains the given pointer. */
template <typename UI = uint32_t, typename SI = int16_t>
SI ListFind(CList<UI, SI>* list, void* adress) {
  ASSERT(list)
  UI *offsets = ListOffsets<UI, SI>(list), *offset_end = offsets + list->count;
  while (offsets < offset_end) {
    char *begin = reinterpret_cast<char*>(list) + *offsets++,
         *end = ListDataStop<UI, SI>(list, index);
    if (reinterpret_cast<char*>(address) >= begin &&
        reinterpret_cast<char*>(address) <= end)
      return true;
  }
  return -1;
}

/* Removes the item at the given address from the list. */
template <typename UI = uint32_t, typename SI = int16_t>
bool ListRemove(CList<UI, SI>* list, void* adress) {
  return ListRemove<UI, SI>(list, ListFind(list, address));
}

/* Returns the value at the given index.
    @return Returns nil if the index is out of the count range. */
template <typename UI = uint32_t, typename SI = int16_t>
const void* ListValue(CList<UI, SI>* list, SI index) {
  ASSERT(list)
  if (index < 0 || index >= list->count) return nullptr;
  return reinterpret_cast<char*>(list) + ListOffsets<UI, SI>(list)[index];
}

/* Prints the given AsciiList to the console. */
template <typename UI = uint32_t, typename SI = int16_t>
Utf8& PrintList(Utf8& printer, CList<UI, SI>* list) {
  ASSERT(list)

  SI count = list->count;
  printer << "\n\nList: size:" << list->size << " count:" << count
          << " count_max:" << list->count_max;
  for (SI index = 0; index < count; ++index) {
    printer << '\n'
            << index << ".) "
            << TypeValue(ListTypes<UI, SI>(list)[index],
                         ListValue<UI, SI>(list, index));
  }
  return printer;
}

/* ASCII List that uses dynamic memory. */
template <typename UI = uint32_t, typename SI = int16_t>
class List {
 public:
  /* Constructs a list with a given count_max with estimated size_bytes. */
  List(SI count_max = 0) : begin(ListNew<UI, SI>(count_max)) {
    // Nothing to do here! (:-)|==<,
  }

  /* Constructs a List with the given size_bytes and count_max.
  size_bytes and count_max both get rounded down to a multiple of 64
  before allocating the buffer. If the count_max is not enough for the
  buffer then the size_bytes will be increased to the minimum size to
  make a valid ASCII List. */
  List(SI count_max, UI size) : begin(ListNew<UI, SI>(count_max, size)) {
    // Nothing to do here! (:-)+==<
  }

  /* Deletes the dynamically allocated buffer. */
  ~List() { delete[] begin; }

  inline SI Push(type_t type, const void* value) {
    return ListPush<UI, SI>(Obj(), type, value);
  }

  /* Inserts the given type-value tuple in the list at the given index. */
  inline SI Insert(byte type, void* value, SI index) {
    return ListInsert<UI, SI>(Obj(), type, value, index);
  }

  /* Returns the maximum count of the give list in the current memory
      profile. */
  inline SI CountMax() { return ListCountMax<UI, SI>(); }

  /* Clears the list without overwriting the contents. */
  void Clear(CList<UI, SI>* list) { ListClear<UI, SI>(Obj()); }

  /* Deletes the list contents by overwriting it with zeros. */
  inline void Wipe() { ListWipe<UI, SI>(Obj()); }

  /* Returns true if this expr contains only the given address.
      @warning This function assumes that the member you're checking for came
               from Kabuki Toolkit. If it's you're own code calling this, you
               are required to ensure the value came from a ASCII List.
      @return  True if the data lies in the list's memory socket. */
  inline bool Contains(void* value) {
    return ListContains<UI, SI>(Obj(), value);
  }

  /* Removes the item at the given address from the list. */
  inline bool Remove(void* adress) { return ListRemove<UI, SI>(Obj(), adress); }

  /* Removes the item at the given address from the list. */
  inline bool Remove(SI index) { return ListRemove<UI, SI>(Obj(), index); }

  /* Removes the last item from the list. */
  inline SI Pop() { return ListPop<UI, SI>(Obj()); }

  /* Prints the given AsciiList to the console. */
  inline Utf8& Print(Utf8& printer) {
    return PrintList<UI, SI>(printer, Obj());
  }

  /* Returns the contiguous ASCII List buffer_. */
  inline CList<UI, SI>* Obj() {
    return reinterpret_cast<CList<UI, SI>*>(begin);
  }

 private:
  uintptr_t* begin;  //< Dynamically allocated word-aligned buffer.
};

}  // namespace _

/* Overloaded operator<< prints the list. */
template <typename UI = uint32_t, typename SI = int16_t>
inline _::Utf8& operator<<(_::Utf8& printer, _::List<UI, SI>& list) {
  return list.Print(printer);
}

/* Overloaded operator<< prints the list. */
template <typename UI = uint32_t, typename SI = int16_t>
inline _::Utf8& operator<<(_::Utf8& printer, _::CList<UI, SI>* list) {
  return _::PrintList<UI, SI>(printer, list);
}

#endif  //< INCLUDED_KABUKI_F2_TLIST
#endif  //< #if SEAM >= SEAM_0_0_0__10

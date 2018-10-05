/* Kabuki Toolkit
@version 0.x
@file    ~/kabuki/f2/collection.h
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
#if SEAM >= SEAM_0_0_2
#ifndef F2_COLLECTION_H
#define F2_COLLECTION_H

#include "ascii_data.h"
#include "text.h"


namespace _ {
/*
    @code
    ;
    @endcode
*/
template <typename TIndex, typename TKey, typename TData, typename THash>
struct API Set {
  TData size;         //< Total size of the set.
  TKey table_size,    //< Size of the (optional) key strings in bytes.
      size_pile;      //< Size of the (optional) collisions pile in bytes.
  TIndex item_count,  //< Number of items.
      count_max;      //< Max number of items that can fit in the header.
};

/* A Type-Value Tuple. */
struct Tuple2 {
  StdType type;  //< The tuple type.
  void* value;   //< The tuple value.
};

/* A Type-Value Tuple. */
struct Tuple3 {
  StdType type;     //< The tuple type.
  void* value;      //< The tuple value.
  const char* key;  //< The Tuple key.
};

/* Interface for a Script Set.
    Set Types:

    | Name       | Code | Description                                 |
    |-----------:|:----:|:--------------------------------------------|
    |      Array | ARY  | A packed c-style array of POD number types. |
    |      Stack | STK  | A stack of POD number types.                |
    |       List | LST  | A stack of Type-Value tuples.               |
    |        Map | MAP  | A one-to-one map of Id-Value tuples.        |
    |   Multimap | DIC  | A multimap of Key-Value tuples.             |
    | Dictionary | DIC  | A one-to-one map of Key-Value tuples.       |
    |   Observer | STC  | A Observer with Subscriber List.            |
*/
struct Collection {
  /* Clears the Set without wiping the memory. */
  virtual void Clear() = 0;

  /* Clears the Set and wipes the memory. */
  virtual void Wipe() = 0;

  /* Adds the given Tuple2 to this Set. */
  virtual bool Push(StdType type, void* value) = 0;

  /* Adds the given Tuple3 to this Set. */
  virtual bool Push(StdType type, void* value, const char* key) = 0;

  /* Merges the given Set into this one. */
  virtual bool Merge(Collection* collection) = 0;

  /* Removes the given object from this collection. */
  virtual bool Remove(Tuple2* tuple) = 0;

  /* Removes the given index from this collection. */
  virtual bool Remove(uintptr_t) = 0;

  /* Removes the given key from this collection (if applicable.). */
  virtual bool Remove(const char* key) = 0;

  /* Gets the element at the given index. */
  virtual void* Get(uintptr_t index) = 0;

  /* Searches for the data of the given type and returns a pointer to it.
      @return Returns nil if the Set does not contain the given data. */
  virtual void* Get(const char* key) = 0;

  /* Returns true if this Set contains this given key. */
  virtual uintptr_t FindIndex(const char* key) = 0;

  /* Returns true if this Set contains this given Type-Value. */
  virtual uintptr_t FindIndex(StdType type, void* value) = 0;

  /* Gets the size_t of the object being stored. */
  virtual uintptr_t GetSize() = 0;

  /* Gets the size_t of the object being stored. */
  virtual uintptr_t GetSizeWidth() = 0;

  /*  */
  virtual Utf8& Print(Utf8& out_) = 0;
};
}  // namespace _
#endif  //< #if SEAM >= SEAM_0_0_2
#endif  //< F2_COLLECTION_H

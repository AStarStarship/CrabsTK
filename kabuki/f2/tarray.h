/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/f2/f2_console.cc
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

#if SEAM >= _0_0_0__08
#ifndef INCLUDED_KABUKI_F2_ARRAY
#define INCLUDED_KABUKI_F2_ARRAY

#include "tstk.h"

namespace _ {

/* @group Array
@brief A multi-dimensional array.
@desc  ASCII Array uses the same data structure as the ASCII Stack, the
difference being that the size_array of the Stack is set to 0 for the Stack and
the Array has a packed multi-dimensional array after the stack of dimensions. */
template <typename T = intptr_t, typename UI = uint, typename SI = int>
constexpr SI ArrayCountUpperLimit(SI dimension_count, SI element_count) {
  UI header_size = (UI)(sizeof(CArray<T, UI, SI>) +
                        AlignUpSigned<SI>(dimension_count * sizeof(SI)));
  return (SI)(((~(UI)0) - 7) - header_size) / (UI)sizeof(T);
}

/* Returns the required size of the given array. */
template <typename T = intptr_t, typename UI = uint, typename SI = int>
constexpr SI ArrayElementCount(const SI* dimensions) {
  ASSERT(dimensions);
  SI dimension_count = *dimensions++, element_count = *dimensions++;
  if (--dimension_count < 0 || element_count < 0) return -1;
  UI size = dimension_count * sizeof(SI);
  while (dimension_count-- > 0) {
    SI current_dimension = *dimensions++;
    if (current_dimension < 1) return -1;
    element_count *= current_dimension;
  }
  if (element_count > ArrayCountUpperLimit<T, UI, SI>()) return -1;
  return element_count * sizeof(T);
}

/* Returns the required size of the given array. */
template <typename T = intptr_t, typename UI = uint, typename SI = int>
constexpr UI ArraySize(const SI* dimensions) {
  SI dimension_count = *dimensions++, element_count = *dimensions++;
  UI header_size = sizeof(CArray<T, UI, SI>);
  if (--dimension_count < 0) return 0;
  while (dimension_count-- > 0) {
    SI current_dimension = *dimensions++;
    if (current_dimension < 1) return 0;
    element_count *= current_dimension;
  }
  if (element_count > ArrayCountUpperLimit<T, UI, SI>()) return 0;
  return element_count * sizeof(T);
}

/* Initializes an stack of n elements of the given type.
    @param buffer An stack of bytes large enough to fit the stack. */
template <typename T = intptr_t, typename UI = uint, typename SI = int>
CArray<T, UI, SI>* ArrayInit(const SI* dimensions) {
  ASSERT(dimensions);
  SI dimension_count = *dimension;
  if (dimension_count < 0 || dimension_count > kStackCountMax) return nullptr;
  UI size = (UI)sizeof(CArray<T, UI, SI>) + dimension_count * sizeof(T);
  uintptr_t* buffer = new uintptr_t[size >> kWordBitCount];
  CArray<T, UI, SI>* stack = reinterpret_cast<CArray<T, UI, SI>*>(buffer);
  stack->size_array = 0;
  stack->size_stack = size;
  stack->count_max = dimension_count;
  stack->count = 0;
  return stack;
}

template <typename T = intptr_t, typename UI = uint, typename SI = int>
SI ArrayElementCountMax() {
  return (UnsignedMax<UI>() - (UI)sizeof(CArray<T, UI, SI>)) / sizeof(T);
}

template <typename T = intptr_t, typename UI = uint, typename SI = int>
CArray<T, UI, SI>* ArrayNew(const SI* dimensions) {
  ASSERT(dimensions);
  const SI* begin = dimensions;
  SI count = (*begin++) - 1, element_count = *begin++, index = count;
  while (index-- > 0) element_count *= *begin++;
  UI size = ((UI)element_count * (UI)sizeof(T));
}

/* Gets the address of the packed array.
    @param tarray ASCII Array data structure..
    @return Pointer to the first element in the array. */
template <typename T, typename UI = uint, typename SI = int>
T* ArrayElements(CArray<T, UI, SI>* tarray) {
  char* elements = reinterpret_cast<char*>(tarray) + tarray->size_stack;
  return reinterpret_cast<T*>(elements);
}

/* Gets the address of the packed SI dimensions.
    @param tarray ASCII Array data structure..
    @return Pointer to the first element in the array. */
template <typename T, typename UI = uint, typename SI = int>
SI* ArrayDimensions(CArray<T, UI, SI>* tarray) {
  char* elements = reinterpret_cast<char*>(tarray) + sizeof(CArray<T, UI, SI>);
  return reinterpret_cast<SI*>(elements);
}

/* Gets the end address of the packed SI dimensions.
    @param tarray ASCII Array data structure..
    @return Pointer to the first element in the array. */
template <typename T, typename UI = uint, typename SI = int>
SI* ArrayDimensionsEnd(CArray<T, UI, SI>* tarray) {
  ASSERT(tarray)
  return ArrayDimensions<T, UI, SI>(tarray) + tarray->count - 1;
}

/* Prints the CArray to the Utf. */
template <typename T = intptr_t, typename UI = uint, typename SI = int>
Utf8& PrintArray(Utf8& print, CArray<T, UI, SI>* tarray) {
  ASSERT(tarray)
  UI size_array = tarray->size_array;
  SI count = tarray->count;
  if (size_array == 0) {
    return PrintStack<T, UI, SI>(print, tarray);
  }
  if (count <= 0) print << "Array: Error! Dimension count must be positive!";

  print << "\n\nArray: dimension_count: " << count
        << " count_max:" << tarray->count_max
        << " size_stack:" << tarray->size_stack
        << " size_array:" << tarray->size_array << "\nDimensions:\n";

  SI *dimensions = ArrayDimensions<T, UI, SI>(tarray),
     *dimensions_end = dimensions + count - 1;
  SI element_count = *dimensions++;

  while (dimensions < dimensions_end) {
    SI dimension = *dimensions++;
    element_count *= dimension;
    if (element_count > ArrayElementCountMax<T, UI, SI>())
      return print << "Max element count exceeded";
    if (dimensions == dimensions_end)
      print << dimension << '\n';
    else
      print << dimension << ", ";
  }
  return print;
}

/* Creates a immutable array of dimensions. */
template <const int... N>
inline const int* Dimensions() {
  static const int kCount = (int)sizeof...(N), kList[sizeof...(N)] = {N...};
  return &kCount;
}

/* A multi-dimensional array that uses dynamic memory that can auto-grow.
    An ASCII Stack struct is identical to an Array

    @todo This is an older data structure that needs to be replace this with
    Array.
    This class is used to save a little bit of ROM space over the Array.
    To use this class with anything other than POD types the class T must have
    a overloaded operator= and operator==. */
template <typename T = intptr_t, typename UI = uint, typename SI = int>
class Array {
 public:
  /* Initializes an array of n elements of the given type.
      @param max_elements The max number of elements in the array buffer. */
  Array(SI demension_count = 1) : begin(ArrayNew<T, UI, SI>(1)) {}

  /* Initializes an array of n elements of the given type.
      @param max_elements The max number of elements in the array buffer. */
  Array(const Array& other)
      : size_array(other.size_array),
        size_stack(other.size_stack),
        count_max(other.count_max),
        count(other.count) {
    SI *elements_other = StackElements<SI, UI, SI>(other.Obj()),
       *element = StackElements<SI, UI, SI>(Obj()),
       *elements_end = StackElementsEnd<SI, UI, SI>(Obj());
    while (element < elements_end) *element++ = *elements_other++;
  }

  /* Deletes the dynamically allocated Array. */
  ~Array() { delete[] begin; }

  /* Clones the other object; up-sizing the buffer only if required. */
  void Clone(Array<T, UI, SI>& other) {}

  /* Gets the number of dimensions. */
  SI GetDimensionCount() { return Obj()->count; }

  /* Gets the dimensions array. */
  T* Dimension() { return StackElements<T, UI, SI>(Obj()); }

  /* Gets the underlying array. */
  T* Elements() { return ArrayElements<T, UI, SI>(Obj()); }

  /* Operator= overload. */
  inline Array<T, UI, SI>& operator=(Array<T, UI, SI>& other) {
    Clone(other);
    return *this;
  }

  inline CArray<T, UI, SI>* Obj() {
    return reinterpret_cast<CArray<T, UI, SI>*>(begin);
  }

 private:
  uintptr_t* begin;  //< Dynamically word-aligned buffer.
};                   //< class Array
}  // namespace _

template <typename T = intptr_t, typename UI = uint, typename SI = int>
inline _::Utf8& operator<<(_::Utf8& printer, _::Stack<T, UI, SI>* stack) {
  return _::PrintArray<T, UI, SI>(printer, stack->Obj());
}

template <typename T = intptr_t, typename UI = uint, typename SI = int>
inline _::Utf8& operator<<(_::Utf8& printer, _::Stack<T, UI, SI>& stack) {
  return _::PrintArray<T, UI, SI>(printer, stack.Obj());
}

#endif  //< INCLUDED_KABUKI_F2_ARRAY
#endif  //< #if SEAM >= _0_0_0__08

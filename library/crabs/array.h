/** Kabuki Toolkit
    @file    ~/library/crabs/array.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
             All right reserved (R). Licensed under the Apache License, Version
             2.0 (the "License"); you may not use this file except in
             compliance with the License. You may obtain a copy of the License
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
             implied. See the License for the specific language governing
             permissions and limitations under the License.
*/

#pragma once
#include <stdafx.h>
#if      MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3
#ifndef  HEADER_FOR_CRABS_ARRAY
#define  HEADER_FOR_CRABS_ARRAY
#include "stack.h"

namespace _ {

/** @group Array 
    @brief A multi-dimensional array.
    @desc  ASCII array uses the same data structure as the
*/
template<typename T = intptr_t, typename UI = uint, typename SI = int>
constexpr SI ArrayCountUpperLimit (SI dimension_count, SI element_count) {
    UI header_size = sizeof (TArray<T, UI, SI>) + 
                     AlignUp<SI> (dimension_count * sizeof (SI));
    return (SI)(((~(UI)0) - 7) - header_size) / (UI)sizeof (T);
}

/** Returns the required size of the given array. */
template<typename T = intptr_t, typename UI = uint, typename SI = int>
constexpr SI ArrayElementCount (const SI* dimensions) {
    assert (dimensions);
    SI dimension_count = *dimensions++,
       element_count  = *dimensions++;
    if (--dimension_count < 0 || element_count < 0)
        return -1;
    UI size = dimension_count * sizeof (SI);
    while (dimension_count-- > 0) {
        SI current_dimension = *dimensions++;
        if (current_dimension < 1)
            return -1;
        element_count *= current_dimension;
    }
    if (element_count > ArrayCountUpperLimit<T, UI, SI> ())
        return -1;
    return element_count * sizeof (T);
}

/** Returns the required size of the given array. */
template<typename T = intptr_t, typename UI = uint, typename SI = int>
constexpr UI ArraySize (const SI* dimensions) {
    SI dimension_count = *dimensions++,
       element_count   = *dimensions++;
    UI header_size = sizeof (TArray<T, UI, SI>);
    if (--dimension_count < 0)
        return 0;
    while (dimension_count-- > 0) {
        SI current_dimension = *dimensions++;
        if (current_dimension < 1)
            return 0;
        element_count *= current_dimension;
    }
    if (element_count > ArrayCountUpperLimit<T, UI, SI> ())
        return 0;
    return element_count * sizeof (T);
}
 
/** Initializes an stack of n elements of the given type.
    @param buffer An stack of bytes large enough to fit the stack. */
template<typename T = intptr_t, typename UI = uint, typename SI = int>
TArray<T, UI, SI>* ArrayInit (const SI* dimensions) {
    assert (dimensions);
    SI dimension_count = *dimension;
    if (dimension_count < 0 || dimension_count > kStackCountMax)
        return nullptr;
    UI size = (UI)sizeof (TArray<T, UI, SI>) + 
              dimension_count * sizeof (T);
    uintptr_t* buffer = new uintptr_t[size >> kWordBitCount];
    TArray<T, UI, SI>* stack = reinterpret_cast<TArray<T, UI, SI>*> (buffer);
    stack->size_array = 0;
    stack->size_stack = size;
    stack->count_max = dimension_count;
    stack->count = 0;
    return stack;
}

template<typename T = intptr_t, typename UI = uint, typename SI = int>
SI ArrayElementCountMax () {
    SI count_max = UnsignedMax<UI> () - (UI)sizeof (TArray<T, UI, SI>);
}

template<typename T = intptr_t, typename UI = uint, typename SI = int>
TArray<T, UI, SI>* ArrayNew (const SI* dimensions) {
    assert (dimensions);
    const SI* cursor = dimensions;
    SI count         = (*cursor++) - 1,
       element_count = *cursor++,
       index         = count;
    while (index-- > 0) {
        element_count *= *cursor++;
    }
    UI size = ((UI)element_count * (UI)sizeof (T));
    
}

/** Gets the address of the packed array.
    @param tarray ASCII Array data structure..
    @return Pointer to the first element in the array. */
template<typename T, typename SI = int, typename UI = uint>
T* ArrayElements (TArray<T, UI, SI>* tarray) {
    char* elements = reinterpret_cast<char*> (tarray) + tarray->size_stack;
    return reinterpret_cast<T*> (elements);
}

template<typename T, typename SI = int, typename UI = uint>
inline SI ObjectCountRound (SI count) {
    enum {
        kRoundEpochMask = (sizeof (SI) == 8) ? 7 :
                          (sizeof (SI) == 4) ? 3 :
                          (sizeof (SI) == 2) ? 1 : 0,
    };
    AlignUp<SI> (count);
}

/** Creates a immutable array of dimensions. */
template<const int... N>
inline const int* Dimensions () {
    static const int kCount = (int)sizeof... (N),
                     kList[sizeof... (N)] = { N... };
    return &kCount;
}

/** A multi-dimensional array that uses dynamic memory that can auto-grow.

    An ASCII Stack struct is identical to an Array

    @todo This is an older data structure that needs to be replace this with 
    _::Array.
    This class is used to save a little bit of ROM space over the data::Array.
    To use this class with anything other than POD types the class T must have
    a overloaded operator= and operator==. */
template<typename T, typename SI = int, typename UI = uint>
class Array {
    public:

    /** Initializes an array of n elements of the given type.
        @param max_elements The max number of elements in the array buffer. */
    Array (SI demension_count = 1) :
        buffer_ (ArrayNew<T, UI, SI> (1)) {

    }

    Array (const SI* dimensions) {
        assert (dimensions);
    }

    /** Initializes an array of n elements of the given type.
        @param max_elements The max number of elements in the array buffer. */
    Array (const Array& other) {
        for (int i = count_ - 1; i >= 0; --i) {
            elements_[i] = other.elements_[i];
        }
    }

    /** Deletes the dynamically allocated Array. */
    ~Array () {
        delete[] buffer_;
    }

    void Clone (Array<T, UI, SI>& other) {

    }

    /** Gets the number of dimensions. */
    SI GetDimensionCount () {
        return This ()->count;
    }

    /** Gets the dimensions array. */
    T* Dimension () {
        return StackElements<T, UI, SI> (This ());
    }

    /** Gets the underlying array. */
    T* Elements () {
        return ArrayElements<T, UI, SI> (This ());
    }

    /** Operator= overload. */
    inline _::Array<T, UI, SI>& operator= (_::Array<T, UI, SI>& other) {
        Clone (other);
        return *this;
    }

    private:

    uintptr_t* buffer_; //< Dynamically allocted word-aligned buffer.

    inline TArray<T, UI, SI>* This () {
        return reinterpret_cast<TArray<T, UI, SI>*> (buffer_);
    }
};      //< class Array
}       //< namespace _
#endif  //< HEADER_FOR_CRABS_ARRAY
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3

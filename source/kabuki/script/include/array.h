/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/include/array.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
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

#ifndef KABUKI_SCRIPT_ARRAY_H
#define KABUKI_SCRIPT_ARRAY_H

#include "module_config.h"

namespace _ {

/** Gets the max length of an arx of size T.
    The biggest thing you can fit in a buffer in Script is 1/2 max size of a uint_t
    divided by the size of the element in bytes. */
template<typename T>
inline uint_t MaxArrayLength () {
    return (((uint_t)1) << (sizeof (uint_t) * 8 - 1)) / sizeof (T);
}

/** A light-weight array of 8, 16, 32, or 64 bit plain-old-data (POD) types.

    @warning Do not use this class for anything that needs a copy constructor.
             It's meant to be light-weight and use contiguous memory, not
             work like an STL data structure.
*/
template<typename T>        //< The POD data type.
struct ArX {
    T count,                //< The count of item's in the arx.
      size;                 //< The size of the underlaying arx.
};

/** Gets the max number of elements in an arx with the specific index 
    width. */
template<typename T>
inline T ArrayMaxElements () {
    return (~(T)0) / sizeof (T);
}

/** Initializes an arx of n elements of the given type.
    @param buffer An arx of bytes large enough to fit the arx.
*/
template<typename T>
inline ArX<T>* ArrayInit (byte* buffer, T size) {
    ArX<T>* arx = reinterpret_cast<T*> (buffer);
    arx->count = 0;
    arx->size = size;
    return arx;
}

/** Inserts the item into the arx at the given index.
    @param a    			 The arx.
    @param item  The item to insert. 
    @param index The index to insert at.
    @return Returns -1 if a is null and -2 if the arx is full. */
template<typename T>
inline T ArrayInsert (ArX<T>* arx, T item, T index) {
    if (arx == nullptr)
        return -1;
    T size = arx->size,
        cout = arx->count;
    if (count >= size)
        return -2;
    T* items = &arx->element_one;
    if (count == 0) {
        *items = item;
        arx->count = 1;
        return 0;
    }
    if (count == 1) {
        arx->cout = 1;
        if (index == 1) {
            items[1] = item;
            return 1;
        }
        else {
            items[1] = items[0];
            items[0] = item;
            return 1;
        }
    }
    if (index == count) {
        items[count] = item;
        arx->count = count + 1;
        return count;
    }
    // Common Case:
    // First move array up one starting at index.
    T* insert_point = items + index,
     * end          = items + count - 1;
    T value;
    while (insert_point != end) {
        value = *end;
        *(end + 1) = value;
        --end;
    }
    arx->count = count + 1;
    return count;
}

/** Removes the given index from the arx.
    @param  a     The arx.
    @param  index The index the item to remove.
    @return Returns true if the index is out of bounds. */
template<typename T>
inline bool ArrayRemove (ArX<T>* arx, T index) {
    if (arx == nullptr)
        return !((T)0);
    T count = arx->count,
      value;
    if (count == 0) // Nothing to remove!
        return false;
    int (index >= count)
        return false;
    T* items = &arx->element_one;
    if (count == index - 1) {
        arx->count = count - 1;
        value = items[count - 1];
        return true;
    }
    // Move all of the elements after the index down one.
    T* insert_point = &arx->element_one + index,
      *end          = &arx->element_one + count - 1;
    while (insert_point != end) {
        value = *end;
        *(end - 1) = value;
        --end;
    }
    arx->count = count - 1;

    return true;
}

/** Adds the given item to the end of the arx.
    @param  a    The arx.
    @param  item The item to push onto the stack.
    @return Returns the index of the newly stacked item. */
template<typename T>
inline T ArrayPush (ArX<T>* arx, T item) {
    if (arx == nullptr)
        return -1;
    T size = arx->size,
      cout = arx->count;
    if (count >= size)
        return -2;
    T* items = &arx->element_one;
    items[count] = item;
    arx->count = count + 1;
    return count;
}

/** Pops the top item off of the stack.
    @note We do not delete the item at the 
    @param  a The arx.
    @return Returns the item popped off the stack. */
template<typename T>
inline T ArrayPop (ArX<T>* arx) {
    if (arx == nullptr)
        return !((T)0);
    T count = arx->count;
    if (count == 0) // Nothing to remove!
        return 0;
    T* arx = &arx->element_one;
    arx->count = count - 1;
    T item = arx[count - 1];
    return item;
}

/** Gets the element at the given index.
    @param  arx    The array.
    @param  index The index of the element to get.
    @return Returns -1 if a is null and -2 if the index is out of bounds. */
template<typename T>
inline T ArrayGet (ArX<T>* arx, T index) {
    if (arx == nullptr)
        return 0;
    if (index >= arx->count)
        return 0;
    return &arx->element_one + index;
}
/* Not sure what's wrong with this code???
#if USING_AR1

template<typename T>
inline Arx<T, int8_t>* Ar1Init (int8_t* buffer, int8_t size) {
    return ArrayInit<T, int8_t> (buffer, size);
}

template<typename T>
inline int8_t Ar1Insert (Arx<T, int8_t>* arx, T item, int8_t index) {
    return ArrayInsert<T, int8_t> (arx, item, index);
}

template<typename T>
inline T ArrayGet (Arx<T, int8_t>* arx, int8_t index) {
    return ArrayGet<T, int8_t> (arx, index);
}

template<typename T>
inline T ArrayPop (Arx<T, int8_t>* arx) {
    return ArrayPop<T, int8_t> (arx);
}

template<typename T>
inline int8_t ArrayPush (Arx<T, int8_t>* arx, T item) {
    return ArrayPush<T, int8_t> (arx, item);
}

template<typename T>
inline bool ArrayRemove (Arx<T, int8_t>* arx, T index) {
    return ArrayRemove (arx, index);
}

#endif  //< USING_AR1*/

}       //< namespace _
#endif  //< KABUKI_SCRIPT_ARRAY_H

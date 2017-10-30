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

/** Gets the max length of an array of size T.
    The biggest thing you can fit in a buffer in Script is 1/2 max size of a uint_t
    divided by the size of the element in bytes. */
template<typename T, typename I = int>
inline uint_t MaxArrayLength () {
    return (((uint_t)1) << (sizeof (uint_t) * 8 - 1)) / sizeof (T);
}

/** A stack of 8, 16, 32, or 64 bit plain-old-data (POD) types.

    Stack Memory Layout

    @code
        |================|
        |  Packed Array  |  <-- Only if header_size = 0
        |================|
        | 64-bit Aligned |
        |     Buffer     |
        |================|
        | Stack Elements |
     ^  |================|
     |  |  Stack struct  |
    0xN |================|
    @endcode
*/
template<typename I = int>
struct Array {
    I size_bytes,   //< Total size of the Stack Array in 64-bit aligned bytes.
      header_size,  //< Total size of the Dimensions Header in bytes.
      height,       //< Total height of the Stack in elements.
      count;        //< Count of the elements on the stack.
};

/** Gets the max number of elements in an array with the specific index 
    width. */
template<typename T, typename I = int>
inline I ArrayMaxElements () {
    return (~(I)0) / sizeof (T);
}

/** Initializes an array of n elements of the given type.
    @param buffer An array of bytes large enough to fit the array.
*/
template<typename T, typename I = int>
inline Array<I>* ArrayInit (T* buffer, T size) {
    Array<I>* stack = reinterpret_cast<T*> (buffer);
    stack->set_size;
    stack->count = 0;
    stack->height = size;
    return stack;
}

/** Inserts the item into the array at the given index.
    @param a    			 The array.
    @param item  The item to insert. 
    @param index The index to insert at.
    @return Returns -1 if a is null and -2 if the array is full. */
template<typename T, typename I = int>
inline T ArrayInsert (Array<I>* array, T item, T index) {
    if (array == nullptr)
        return -1;
    T size = array->height,
        cout = array->count;
    if (count >= size)
        return -2;
    T* items = &array->element_one;
    if (count == 0) {
        *items = item;
        array->count = 1;
        return 0;
    }
    if (count == 1) {
        array->cout = 1;
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
        array->count = count + 1;
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
    array->count = count + 1;
    return count;
}

/** Removes the given index from the array.
    @param  a     The array.
    @param  index The index the item to remove.
    @return Returns true if the index is out of bounds. */
template<typename T, typename I = int>
inline bool ArrayRemove (Array<I>* array, T index) {
    if (array == nullptr)
        return !((T)0);
    T count = array->count,
      value;
    if (count == 0) // Nothing to remove!
        return false;
    int (index >= count)
        return false;
    T* items = &array->element_one;
    if (count == index - 1) {
        array->count = count - 1;
        value = items[count - 1];
        return true;
    }
    // Move all of the elements after the index down one.
    T* insert_point = &array->element_one + index,
     * end          = &array->element_one + count - 1;
    while (insert_point != end) {
        value = *end;
        *(end - 1) = value;
        --end;
    }
    array->count = count - 1;

    return true;
}

/** Adds the given item to the end of the array.
    @param  a    The array.
    @param  item The item to push onto the stack.
    @return Returns the index of the newly stacked item. */
template<typename T, typename I = int>
inline I ArrayPush (Array<I>* array, T item) {
    if (array == nullptr)
        return -1;
    I size = array->height,
      cout = array->count;
    if (count >= size)
        return -2;
    T* items = &array->element_one;
    items[count] = item;
    array->count = count + 1;
    return count;
}

/** Pops the top item off of the stack.
    @note We do not delete the item at the 
    @param  a The array.
    @return Returns the item popped off the stack. */
template<typename T, typename I = int>
inline T ArrayPop (Array<I>* array) {
    if (array == nullptr)
        return !((T)0);
    I count = array->count;
    if (count == 0) // Nothing to remove!
        return 0;
    T* array = &array->element_one;
    array->count = count - 1;
    T item = array[count - 1];
    return item;
}

/** Gets the element at the given index.
    @param  array    The array.
    @param  index The index of the element to get.
    @return Returns -1 if a is null and -2 if the index is out of bounds. */
template<typename T, typename I = int>
inline T ArrayGet (Array<I>* array, T index) {
    if (array == nullptr)
        return 0;
    if (index >= array->count)
        return 0;
    return &array->element_one + index;
}

}       //< namespace _
#endif  //< KABUKI_SCRIPT_ARRAY_H

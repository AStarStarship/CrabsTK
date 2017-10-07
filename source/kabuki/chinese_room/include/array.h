/** The Chinese Room
    @version 0.x
    @file    ~/chinese_room/include/array.h
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

#ifndef CHINESE_ROOM_ARRAY_H
#define CHINESE_ROOM_ARRAY_H

#include "config.h"

#if USING_AR1 || USING_AR2 || USING_AR4 || USING_AR8

namespace _ {

/** Gets the max length of an array of size T.
    The biggest thing you can fit in a buffer in I2P is 1/2 max size of a uint_t
    divided by the size of the element in bytes. */
template<typename T>
KABUKI uint_t MaxArrayLength () {
    return (((uint_t)1) << (sizeof (uint_t) * 8 - 1)) / sizeof (T);
}

/** A light-weight c-style array of 8, 16, 32, or 64 bit plain-old-data (POD)
    types.
    An array may be created from any mutable array of bytes.
*/
template<typename T,    //< The C++ data type.
         typename I>    //< The C++ index type.
struct Array {
    I count,            //< The count of item's in the array.
      size;             //< The size of the underlaying array.
    T element_one;      //< The first element in the array.
};

/** Gets the max number of elements in an array with the specific index 
    width. */
template<typename I>
inline I MaxArrayElements () {
    return ~(I)0;
}

/** Initializes an array of n elements of the given type.
    @param buffer An array of bytes large enough to fit the array.
*/
template<typename T, typename I>
KABUKI Array<T, I>* ArrayInit (byte* buffer, I n) {
    Array<T, I>* a = reinterpret_cast<T*> (buffer);
    a->count = 0;
    a->size = n;
    return a;
}

/** Inserts the item into the array at the given index.
    @param a     The array.
    @param item  The item to insert. 
    @param index The index to insert at.
    @return Returns -1 if a is null and -2 if the array is full. */
template<typename T, typename I>
KABUKI I Insert (Array<T, I>* a, T item, I index) {
    if (a == nullptr)
        return -1;
    I size = a->size,
        cout = a->count;
    if (count >= size)
        return -2;
    T* items = &a->element_one;
    if (count == 0) {
        *items = item;
        a->count = 1;
        return 0;
    }
    if (count == 1) {
        a->cout = 1;
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
        a->count = count + 1;
        return count;
    }
    // Common Case:
    // First move content of array up one starting at index.
    T* insert_point = items + index,
     * end          = items + count - 1;
    T value;
    while (insert_point != end) {
        value = *end;
        *(end + 1) = value;
        --end;
    }
    a->count = count + 1;
    return count;
}

/** Removes the given index from the array.
    @param  a     The array.
    @param  index The index the item to remove.
    @return Returns true if the index is out of bounds. */
template<typename T, typename I>
KABUKI T Remove (Array<T, I>* a, T index) {
    if (a == nullptr)
        return !((T)0);
    I count = a->count;
    T value;
    if (count == 0) // Nothing to remove!
        return 0;
    int (index >= count)
        return 0;
    T* items = &a->element_one;
    if (count == index - 1) {
        a->count = count - 1;
        value = items[count - 1];
        return value;
    }
    // Move all of the elements after the index down one.
    T* insert_point = &a->element_one + index,
      *end          = &a->element_one + count - 1;
    while (insert_point != end) {
        value = *end;
        *(end - 1) = value;
        --end;
    }
    a->count = count - 1;
}

/** Adds the given item to the end of the array.
    @param  a    The array.
    @param  item The item to push onto the stack.
    @return Returns the index of the newly stacked item. */
template<typename T, typename I>
KABUKI I Push (Array<T, I>* a, T item) {
    if (a == nullptr)
        return -1;
    I size = a->size,
      cout = a->count;
    if (count >= size)
        return -2;
    T* items = &a->element_one;
    items[count] = item;
    a->count = count + 1;
    return count;
}

/** Pops the top item off of the stack.
    @note We do not delete the item at the 
    @param  a The array.
    @return Returns the item popped off the stack. */
template<typename T, typename I>
KABUKI T Pop (Array<T, I>* a) {
    if (a == nullptr)
        return !((T)0);
    I count = a->count;
    if (count == 0) // Nothing to remove!
        return 0;
    T* a = &a->element_one;
    a->count = count - 1;
    T item = a[count - 1];
    return item;
}

/** Gets the element at the given index.
    @param  a     The array.
    @param  index The index of the element to get.
    @return Returns -1 if a is null and -2 if the index is out of bounds. */
template<typename T, typename I>
KABUKI T Element (Array<T, I>* a, I index) {
    if (a == nullptr)
        return 0;
    if (index >= a->count)
        return 0;
    return &a->element_one + index;
}

}       //< namespace _
#endif  //< USING_AR1 || USING_AR2 || USING_AR4 || USING_AR8
#endif  //< CHINESE_ROOM_ARRAY_H

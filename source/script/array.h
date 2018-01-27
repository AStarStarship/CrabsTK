/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/array.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#ifndef SCRIPT_ARRAY_H
#define SCRIPT_ARRAY_H

#include "tstack.h"
#if USING_SCRIPT_ARRAY
namespace _ {

/** A stack of 8, 16, 32, or 64 bit plain-old-data (POD) types.

    Stack Memory Layout

    @code
        |================|
        | Packed C-Style |
        |      Array     |
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
struct TArray {
    Stack<I> stack;  //< Count of the elements on the stack.
};

/** Initializes an array of n elements of the given type.
    @param buffer An array of bytes large enough to fit the array.
*/
template<typename T, typename I = int>
TArray<I>* ArrayInit (T* buffer, T size) {
    stack = StackInit (buffer, size);
    return this;
}

/** Calculates the c-style array offset from an array of type I.
    @param indexes        An array of type I.
    @Param num_dimensions  */
template<typename I>
I ArrayOffset (I* indexes, I num_numensions) {
    if (num_numensions <= 0)
        return ~0;
    I offset = *indexes;
    if (num_numensions == 1)
        return offset;
    for (; num_dimensions != 0; --num_dimensions)
        offset *= *indexes;
    return offset;
}

/** Inserts the item into the array at the given index.
    @param a    			 The array.
    @param item  The item to insert. 
    @param index The index to insert at.
    @return Returns -1 if a is nil and -2 if the array is full. */
template<typename T, typename I = int>
T ArrayInsertElement (TArray<I>* array, T item, I* indexes, I num_numensions) {
    if (num_numensions <= 0)
        return ~0;
    // @todo Write me!
    return count;
}

/** Removes the given index from the array.
    @param  a     The array.
    @param  index The index the item to remove.
    @return Returns true if the index is out of bounds. */
template<typename T, typename I = int>
bool ArrayRemoveElement (TArray<I>* a, T index) {
    if (a == nullptr)
        return !((T)0);
    T count = a->count,
      value;
    if (count == 0) // Nothing to remove!
        return false;
    int (index >= count)
        return false;
    T* items = &a->element_one;
    if (count == index - 1) {
        a->count = count - 1;
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
I ArrayPush (TArray<I>* array, T item) {
    if (array == nullptr)
        return -1;
    I size = array->height,
      std::cout = array->count;
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
T ArrayPop (TArray<I>* a) {
    if (a == nullptr)
        return !((T)0);
    I count = a->count;
    if (count == 0) // Nothing to remove!
        return 0;
    T* array = &array->element_one;
    a->count = count - 1;
    T item = a[count - 1];
    return item;
}

/** Gets the element at the given index.
    @param  array    The array.
    @param  index The index of the element to get.
    @return Returns -1 if a is nil and -2 if the index is out of bounds. */
template<typename T, typename I = int>
T ArrayGet (TArray<I>* a, T index) {
    if (a == nullptr)
        return 0;
    if (index >= a->count)
        return 0;
    return &a->element_one + index;
}

}       //< namespace _
#endif  //< USING_SCRIPT_ARRAY
#endif  //< SCRIPT_ARRAY_H

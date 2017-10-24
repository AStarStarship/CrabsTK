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

#ifndef KABUKI_SCRIPT_ARRAY_H
#define KABUKI_SCRIPT_ARRAY_H

#include "config.h"

namespace _ {

/** Gets the max length of an script_array of size T.
    The biggest thing you can fit in a buffer in Script is 1/2 max size of a uint_t
    divided by the size of the element in bytes. */
template<typename T>
inline uint_t MaxArrayLength () {
    return (((uint_t)1) << (sizeof (uint_t) * 8 - 1)) / sizeof (T);
}

/** A light-weight c-style script_array of 8, 16, 32, or 64 bit plain-old-data (POD)
    types.

    An script_array may be created from any mutable script_array of bytes.

    @warning Do not use this class for anything that needs a copy constructor.
             It's meant to be light-weight and use contiguous memory, not
             work like an STL data structure.
*/
template<typename T>        //< The POD data type.
struct Array {
    T count,                //< The count of item's in the script_array.
      size,                 //< The size of the underlaying script_array.
      element_one;          //< The first element in the script_array.
};

/** Gets the max number of elements in an script_array with the specific index 
    width. */
template<typename T>
inline T ArrayMaxElements () {
    return (~(T)0) / sizeof (T);
}

/** Initializes an script_array of n elements of the given type.
    @param buffer An script_array of bytes large enough to fit the script_array.
*/
template<typename T = int>
inline Array<T>* ArrayInit (byte* buffer, T size) {
    Array<T>* script_array = reinterpret_cast<T*> (buffer);
    script_array->count = 0;
    script_array->size = size;
    return script_array;
}

/** Inserts the item into the script_array at the given index.
    @param a    			 The script_array.
    @param item  The item to insert. 
    @param index The index to insert at.
    @return Returns -1 if a is null and -2 if the script_array is full. */
template<typename T = int>
inline T ArrayInsert (Array<T>* script_array, T item, T index) {
    if (script_array == nullptr)
        return -1;
    T size = script_array->size,
        cout = script_array->count;
    if (count >= size)
        return -2;
    T* items = &script_array->element_one;
    if (count == 0) {
        *items = item;
        script_array->count = 1;
        return 0;
    }
    if (count == 1) {
        script_array->cout = 1;
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
        script_array->count = count + 1;
        return count;
    }
    // Common Case:
    // First move content of script_array up one starting at index.
    T* insert_point = items + index,
     * end          = items + count - 1;
    T value;
    while (insert_point != end) {
        value = *end;
        *(end + 1) = value;
        --end;
    }
    script_array->count = count + 1;
    return count;
}

/** Removes the given index from the script_array.
    @param  a     The script_array.
    @param  index The index the item to remove.
    @return Returns true if the index is out of bounds. */
template<typename T = int>
inline bool ArrayRemove (Array<T>* script_array, T index) {
    if (script_array == nullptr)
        return !((T)0);
    T count = script_array->count,
      value;
    if (count == 0) // Nothing to remove!
        return false;
    int (index >= count)
        return false;
    T* items = &script_array->element_one;
    if (count == index - 1) {
        script_array->count = count - 1;
        value = items[count - 1];
        return true;
    }
    // Move all of the elements after the index down one.
    T* insert_point = &script_array->element_one + index,
      *end          = &script_array->element_one + count - 1;
    while (insert_point != end) {
        value = *end;
        *(end - 1) = value;
        --end;
    }
    script_array->count = count - 1;

    return true;
}

/** Adds the given item to the end of the script_array.
    @param  a    The script_array.
    @param  item The item to push onto the stack.
    @return Returns the index of the newly stacked item. */
template<typename T = int>
inline T ArrayPush (Array<T>* script_array, T item) {
    if (script_array == nullptr)
        return -1;
    T size = script_array->size,
      cout = script_array->count;
    if (count >= size)
        return -2;
    T* items = &script_array->element_one;
    items[count] = item;
    script_array->count = count + 1;
    return count;
}

/** Pops the top item off of the stack.
    @note We do not delete the item at the 
    @param  a The script_array.
    @return Returns the item popped off the stack. */
template<typename T = int>
inline T ArrayPop (Array<T>* script_array) {
    if (script_array == nullptr)
        return !((T)0);
    T count = script_array->count;
    if (count == 0) // Nothing to remove!
        return 0;
    T* script_array = &script_array->element_one;
    script_array->count = count - 1;
    T item = script_array[count - 1];
    return item;
}

/** Gets the element at the given index.
    @param  script_array    The array.
    @param  index The index of the element to get.
    @return Returns -1 if a is null and -2 if the index is out of bounds. */
template<typename T = int>
inline T ArrayGet (Array<T>* script_array, T index) {
    if (script_array == nullptr)
        return 0;
    if (index >= script_array->count)
        return 0;
    return &script_array->element_one + index;
}

#if USING_AR1

template<T>
inline ARX<T, int8_t>*AR1Init (int8_t* buffer, int8_t size) {
    return ArrayInit<T, int8_t> (buffer, size);
}

template<typename T>
inline int8_t AR1Insert (ARX<T, int8_t>* script_array, T item, int8_t index) {
    return int8_t ArrayInsert<T, int8_t> (script_array, item, index)
}

template<typename T>
inline T ArrayGet (ARX<T, int8_t>* script_array, int8_t index) {
    return ArrayGet (script_array, index);
    return ;
}

template<typename T>
inline T ArrayPop (ARX<T, int8_t>* script_array) {
    return ArrayPop (script_array);
}

template<typename T>
inline int8_t ArrayPush (ARX<T, int8_t>* script_array, T item) {
    return ArrayPush (script_array, item);
}

template<typename T>
inline bool ArrayRemove (ARX<T, int8_t>* script_array, T index) {
    return ArrayRemove (script_array, index);
}

#endif  //< USING_AR1

#if USING_AR2
template<T>
inline ARX<T, int16_t>*AR1Init (int16_t* buffer, int16_t size) {
    return ArrayInit<T, int16_t> (buffer, size);
}

template<typename T>
inline int16_t AR1Insert (ARX<T, int16_t>* script_array, T item, int16_t index) {
    return int16_t ArrayInsert<T, int16_t> (script_array, item, index)
}

template<typename T>
inline T ArrayGet (ARX<T, int16_t>* script_array, int16_t index) {
    return ArrayGet (script_array, index);
    return;
}

template<typename T>
inline T ArrayPop (ARX<T, int16_t>* script_array) {
    return ArrayPop (script_array);
}

template<typename T>
inline int16_t ArrayPush (ARX<T, int16_t>* script_array, T item) {
    return ArrayPush (script_array, item);
}

template<typename T>
inline bool ArrayRemove (ARX<T, int16_t>* script_array, T index) {
    return ArrayRemove (script_array, index);
}

#endif  //< USING_AR2

#if USING_AR4
template<T>
inline ARX<T, int32_t>*AR1Init (int32_t* buffer, int32_t size) {
    return ArrayInit<T, int32_t> (buffer, size);
}

template<typename T>
inline int32_t AR1Insert (ARX<T, int32_t>* script_array, T item, int32_t index) {
    return int32_t ArrayInsert<T, int32_t> (script_array, item, index)
}

template<typename T>
inline T ArrayGet (ARX<T, int32_t>* script_array, int32_t index) {
    return ArrayGet (script_array, index);
    return;
}

template<typename T>
inline T ArrayPop (ARX<T, int32_t>* script_array) {
    return ArrayPop (script_array);
}

template<typename T>
inline int32_t ArrayPush (ARX<T, int32_t>* script_array, T item) {
    return ArrayPush (script_array, item);
}

template<typename T>
inline bool ArrayRemove (ARX<T, int32_t>* script_array, T index) {
    return ArrayRemove (script_array, index);
}

#endif  //< USING_AR4

#if USING_AR8
template<T>
inline ARX<T, int64_t>*AR1Init (int64_t* buffer, int64_t size) {
    return ArrayInit<T, int64_t> (buffer, size);
}

template<typename T>
inline int64_t AR1Insert (ARX<T, int64_t>* script_array, T item, int64_t index) {
    return int64_t ArrayInsert<T, int64_t> (script_array, item, index)
}

template<typename T>
inline T ArrayGet (ARX<T, int64_t>* script_array, int64_t index) {
    return ArrayGet (script_array, index);
    return;
}

template<typename T>
inline T ArrayPop (ARX<T, int64_t>* script_array) {
    return ArrayPop (script_array);
}

template<typename T>
inline int64_t ArrayPush (ARX<T, int64_t>* script_array, T item) {
    return ArrayPush (script_array, item);
}

template<typename T>
inline bool ArrayRemove (ARX<T, int64_t>* script_array, T index) {
    return ArrayRemove (script_array, index);
}

#endif  //< USING_AR8

}       //< namespace _
#endif  //< KABUKI_SCRIPT_ARRAY_H

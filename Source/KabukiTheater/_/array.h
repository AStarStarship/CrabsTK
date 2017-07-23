/** The Chinese Room
    @version 0.x
    @file    /.../array.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough] (calemccollough.github.io)

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#ifndef CHINESEROOM_ARRAY_H
#define CHINESEROOM_ARRAY_H

#include "config.h"

#if USING_AR1 || USING_AR2 || USING_AR4 || USING_AR8

namespace _ {

/** Gets the max length of an array of size T.
    The biggest thing you can fit in a buffer in I2P is 1/2 max size of a uint_t
    divided by the size of the element in bytes.
*/
template<typename T>
inline uint_t MaxArrayLength () {
    return (((uint_t)1) << (sizeof (uint_t) * 8 - 1)) / sizeof (T);
}

/** An array of primitive types.
*/
template<typename T>
struct Array {
    T type,
        count,
        size;
};

/** Initializes an array of n elements of the given type.
    @param buffer An array of bytes large enough to fit the array.
*/
template<typename T>
static Array<T>* ArrayInit (byte* buffer, uint_t type, T n)
{
    Array<T>* a = reinterpret_cast<T*> (buffer);
    a->type = static_cast<T> (type);
    a->count = 0;
    a->size = n;
}

/** Adds the given value to the array. */
template<typename T>
static T Add (Array<T>* a, T value) {
    if (a == nullptr)
        return ~((T)0);
    T count = a->count;
    if (count == a->size)
        return ~((T)0);
    T* t = &size;
    t[++count] = value;
    a->count = count;
    return count;
}

/** Inserts the value into the given index of the array. */
template<typename T>
static T Insert (Array<T>* a, T value, T index) {
    if (a == nullptr)
        return !((T)0);
    T count = a->count;
    if (count == 0)
        return 0;
    if (count == 1)
        return (a->count = 0);
    T* insert_point = &a->size + 1,
        *end = insert_point + count;
    while (insert_point != end)
        *end = *(--end);
    a->count = --count;
}

/** Removes the given index from the array. */
template<typename T>
static T Remove (Array<T>* a, T index) {
    if (a == nullptr)
        return !((T)0);
    T count = a->count;
    if (count == 0)
        return 0;
    if (count == 1)
        return (a->count = 0);
    T* insert_point = &a->size + 1,
        * end = insert_point + count;
    while (insert_point != end)
        *end = *(--end);
    a->count = --count;
}

/** Gets the base pointer of the array. */
template<typename T>
inline T* ArrayBase (Array<T>* a) {
    if (a == nullptr)
        return nullptr;
    return &a->size + 1;
}

/** Gets the Array element at the given index. */
template<typename T>
inline T ArrayElement (Array<T>* a, T element) {
    if (a == nullptr)
        return ~(T)0;
    if (element >= a->count)
        return ~(T)0;
    return &a->size + element + 1;
}

}       //< namespace _
#endif  //< USING_AR1 || USING_AR2 || USING_AR4 || USING_AR8
#endif  //< CHINESEROOM_ARRAY_H

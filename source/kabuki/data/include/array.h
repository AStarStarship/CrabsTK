/** kabuki::data
    @file    ~/source/data/include/array.h
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

#ifndef KABUKI_DATA_ARRAY_H
#define KABUKI_DATA_ARRAY_H

#include "module_config.h"
#include "../../script/include/utils.h"

namespace kabuki { namespace data {

/** An array of like types that can auto-grow.
*/
template<typename T>
class Array {
    public:

    enum {
        kMinSize = 4    //< The default and min size if none is entered.
    };

    /** Initializes an array of n elements of the given type.
    @param max_elements The max number of elements in the array buffer. */
    Array (int max_elements = kMinSize) :
        size_ (max_elements = _::RoundToPowerOf2 (max_elements)),
        count_ (0),
        array_ (new T[max_elements]) {
    }

    /** Initializes an array of n elements of the given type and clears .
    @param max_elements The max number of elements in the array buffer.
    @param init_value The init value of the elements. */
    Array (int max_elements, int init_value) :
        size_ (max_elements = RoundToPowerOf2 (max_elements)),
        count_ (0),
        array_ (new T[max_elements]) {
        memset (array_, init_value, max_elements);
    }

    /** Gets the num_elements_. */
    int GetCount () {
        return count_;
    }

    /** Gets the max_elements_. */
    int GetSize () {
        return size_;
    }

    /** Inserts the value into the given index into the array at the given,
    index and shifts the contents at the index and above up one. */
    int Insert (T value, int index) {
        if (index < 0)
            return -1;

        int count = count_,
            size = size_;

        if (count >= size)
            return -2;

        if (index > count)
            return ~0;

        if (index == size)
            Grow ();

        T* array_ptr = array_;
        if (count == 0) {
            array_ptr[0] = value;
            count_ = 1;
            return 0;
        }
        if (count == 1) {
            if (index == 0) {
                array_ptr[1] = array_ptr[0];
                array_ptr[0] = value;
            } else {
                array_ptr[1] = value;
            }
            count_ = 2;
            return 0;
        }
        for (int i = index; i <= count; ++i)
            array_ptr[i + 1] = array_ptr[i];
        array_ptr[index] = value;
        //T* insert_point = array_ptr + index,
        //  * end = array_ptr + num_elements;
        //while (insert_point != end)
        //    *end = *(--end);
        count_ = count + 1;
        return count;
    }

    /** Pushes the element onto the Stack. */
    int Push (T element) {
        return Insert (element, count_);
    }

    /** Pops an element off the stack. */
    int Pop (T& element) {
        int count = count_;
        if (count == 0)
            return 0;
        element = array_[count - 1];
        count_ = count - 1;
        return count;
    }

    /** Removes the given index from the array. */
    bool Remove (int index) {
        int num_elements = count_;
        if (num_elements == 0)
            return false;
        if (num_elements == 1) {
            count_ = 0;
            return false;
        }
        T* insert_point = &array_[num_elements],
            *end = &array_[index];
        while (insert_point != end)
            *end = *(--end);
        count_ = --num_elements;
        return true;
    }

    /** Gets the Array element at the given index. */
    inline T& Element (int index) {
        static T t;
        if (index < 0)
            return t;
        if (index >= size_)
            return t;
        return array_[index];
    }

    inline T& operator[] (int index) {
        return Element (index);
    }

    /** Doubles the size of the array. */
    void Grow () {
        int size = size_;
        T* array_local = array_,
            *new_array = new T[size << 1];
        for (int i = 0; i < size; ++i)
            new_array[i] = array_local[i];
        size_ = size << 1;
        // Size should never be below 4.
        delete[] array_local;
        array_ = new_array;
    }

    private:

    int size_,      //< Max number of elements.
        count_;     //< Number of elements.
    T*  array_;     //< The array.
};

using StringArray = Array<const char*>;

}       //< namespace data
}       //< namespace kabuki
#endif  //< KABUKI_DATA_ARRAY_H

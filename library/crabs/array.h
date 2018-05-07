/** Kabuki Toolkit
    @file    ~/libraries/array.h
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

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3

#ifndef HEADER_FOR_CRABS_ARRAY
#define HEADER_FOR_CRABS_ARRAY

#include "config.h"

namespace _ {

/** An array of like types that can auto-grow.
    @todo This is an older data structure that needs to be replace this with 
    _::Array.
    This class is used to save a little bit of ROM space over the data::Array.
    To use this class with anything other than POD types the class T must have
    a overloaded operator= and operator==. */
template<typename T>
class Array {
    public:

    enum {
        kMinSize = 4    //< Default and min size if none is entered.
    };

    /** Initializes an array of n elements of the given type.
        @param max_elements The max number of elements in the array buffer. */
    Array (int max_elements = kMinSize) :
        size_     (max_elements < 1 ? 1 :max_elements),
        count_    (0),
        elements_ (new T[size_]) {
    }

    /** Initializes an array of n elements of the given type and clears .
        @param max_elements The max number of elements in the array buffer.
        @param init_value The init value of the elements. */
    Array (int max_elements, int init_value) :
        size_     (max_elements < 1 ? 1 : max_elements),
        count_    (0),
        elements_ (new T[size_]) {
        //memset (elements_, init_value, max_elements);
    }

    /** Initializes an array of n elements of the given type.
        @param max_elements The max number of elements in the array buffer. */
    Array (const Array& other) :
        size_     (other.size_),
        count_    (other.count_),
        elements_ (new T[size_]) {
        for (int i = count_ - 1; i >= 0; --i) {
            elements_[i] = other.elements_[i];
        }
    }

    ~Array () {
        delete[] elements_;
    }

    /** Clears the array content by setting count_ to zero. */
    void Clear () {
        count_ = 0;
    }

    /** Gets the num_elements_. */
    int Length () {
        return count_;
    }

    /** Gets the max elements that can fit in the current array memory. */
    int GetSize () {
        return size_;
    }

    /** Returns true if this Array contains the given value.
         @warning Non-POD types must overload operator= and operator==. */
    bool Contains (T& element) {
        for (int i = 0; i < count_; ++i) {
            if (elements_[i] == element) {
                return true;
            }
        }
        return false;
    }

    /** Inserts the value into the given index into the array at the given,
         index and shifts the contents at the index and above up one. */
    int Insert (T value, int index) {
        if (index < 0) {
            return -1;
        }
        int count = count_,
            size = size_;
        
        if (count >= size) {
            return -2;
        }
        if (index > count) {
            return -3;
        }
        if (index == size) {
            Grow ();
        }
        T* array_ptr = elements_;
        if (count == 0) {
            array_ptr[0] = value;
            count_ = 1;
            return 1;
        }
        if (count == 1) {
            if (index == 0) {
                array_ptr[1] = array_ptr[0];
                array_ptr[0] = value;
            } else {
                array_ptr[1] = value;
            }
            count_ = 2;
            return 2;
        }
        for (int i = index; i <= count; ++i)
            array_ptr[i + 1] = array_ptr[i];
        array_ptr[index] = value;
        //T* insert_point = array_ptr + index,
        //  * end = array_ptr + num_elements;
        //while (insert_point != end)
        //    *end = *(--end);
        count_ = count + 1;
        return count + 1;
    }

    /** Pushes the element onto the Stack. */
    int Push (T element) {
        return Insert (element, count_);
    }

    /** Pushes the Array contents onto the Stack. */
    int Push (Array<T>& elements) {
        int count = count_,
            other_count = elements.Length (),
            new_size = count + other_count;
        if (other_count <= 0) { // Nothing to do.
            return -1;
        }
        if (new_size > size_) {
            Grow (new_size);
        }
        T* ptr = &elements_[count],
         * element = elements.elements_;
        for (int i = 0; i < new_size; ++i) {
            *ptr = *element++;
        }
        count_ = new_size;
        return new_size;
    }

    /** Pops an element off the stack. */
    T Pop () {
        int count = count_;
        if (count == 0)
            return 0;
        T element = elements_[count - 1];
        count_ = count - 1;
        return element;
    }

    /** Removes the given index from the array. */
    bool Remove (int index) {
        int count = count_;
        if (count == 0)
            return false;
        if (count == 1) {
            count_ = 0;
            return false;
        }
        //T* end_address     = &array_[count - 1],
        // * current_address = &array_[index];
        //while (current_address != end_address) {
        //    *current_address = *(++current_address);
        //}
        for (int i = index; i < count; ++i) {
            elements_[i] = elements_[i + 1];
        }
        count_ = count - 1;
        return true;
    }

    /** Returns true if this Array contains the given value.
         @warning Non-POD types must overload operator= and operator==. */
    bool RemoveFirstInstanceOf (T& element) {
        for (int i = 0; i < count_; ++i) {
            if (elements_[i] == element) {
                return Remove (i);
            }
        }
        return false;
    }

    /** Gets the Array element at the given index. */
    inline T& Element (int index) {
        static T nil = 0;
        if (index < 0) {
            return nil;
        }
        if (index >= size_) {
            return nil;
        }
        return elements_[index];
    }

    /** Gets the Array element at the given index. */
    inline T& operator[] (int index) {
        return Element (index);
    }

    /** Gets the underlying array. */
    T* GetArray () {
        return elements_;
    }

    /** Doubles the size of the array. */
    void Grow (int new_size = -1) {
        int size = size_;
        if (new_size < 0) { // Double in size.
            new_size = size << 1; //< <<1 to *2
        }
        else if (new_size < size) { // Nothing to do :-)
            return;
        }
        T* array_local = elements_,
            *new_array = new T[new_size];
        for (int i = 0; i < size; ++i) {
            new_array[i] = array_local[i];
        }
        size_ = size << 1;
        // Size should never be below 4.
        delete[] array_local;
        elements_ = new_array;
    }

    private:

    

};      //< class Array
}       //< namespace _
#endif  //< HEADER_FOR_CRABS_ARRAY
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3

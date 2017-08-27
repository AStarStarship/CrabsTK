/** CS 162 Program 4-5
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

#ifndef KABUKI_PRO_ARRAY_H
#define KABUKI_PRO_ARRAY_H

namespace _pro {


/** Compute the next highest power of 2 of 32-bit v. */
static int RoundToPowerOf2 (int value)
{
    if (value < 0)
        return 4;
    // @cite https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
    unsigned int v = (unsigned int)value;
    --v;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    ++v;
    return (int)value;
}

/** An array of like types that can auto-grow.
*/
template<typename T>
class Array 
{
    public:
   
    enum {
        MIN_SIZE = 4    //< The default and min size if none is entered.
    };

    /** Initializes an array of n elements of the given type.
        @param max_elements The max number of elements in the array buffer. */
    Array (int max_elements = MIN_SIZE):
        size_ (max_elements = RoundToPowerOf2 (max_elements)),
        count_ (0),
        array_ (new T[max_elements])
    {
    }

    /** Initializes an array of n elements of the given type and clears .
        @param max_elements The max number of elements in the array buffer. 
        @param init_value The init value of the elements. */
    Array (int max_elements, int init_value):
        size_ (max_elements = RoundToPowerOf2 (max_elements)),
        count_ (0),
        array_ (new T[max_elements])
    {
        memset (array_, init_value, max_elements);
    }

    /** Gets the num_elements_. */
    int GetCount () {
        return count_;
    }

    /** Gets the max_elements_. */
    int Size () {
        return size_;
    }

    /** Inserts the value into the given index into the array at the given,
        index and shifts the contents at the index and above up one. */
    int Insert (T value, int index)
    {
        if (index < 0)
            return -1;

        int count = count_,
            size = size_;

        if (count >= size)
            Grow ();

        if (index > count)
            return ~0;

        if (index == size)
            Grow ();

        T* array_ptr = array_;
        if (count == 0)
        {
            array_ptr[0] = value;
            count_ = 1;
            return 0;
        }
        if (count == 1)
        {
            if (index == 0)
            {
                array_ptr[1] = array_ptr[0];
                array_ptr[0] = value;
            }
            else
            {
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
    
    /** Adds the given value to the array. */
    int Add (T value)
    {
         return Insert (value, count_);   //< Add to the end.
    }

    /** Removes the given index from the array. */
    bool Remove (int index)
    {
        int num_elements = count_;
        if (num_elements == 0)
            return false;
        if (num_elements == 1)
        {
            count_ = 0;
            return false;
        }
        T* insert_point = &array_[num_elements],
         * end          = &array_[index];
        while (insert_point != end)
            *end = *(--end);
        count_ = --num_elements;
        return true;
    }

    /** Gets the Array element at the given index. */
    inline T& Element (int index) 
    {
        static T t;
        if (index < 0)
            return t;
        if (index >= size_)
            return t;
        return array_[index];
    }

    inline T& operator[] (int index)
    {
        static T t;
        if (index < 0)
            return t;
        if (index >= size_)
            return t;
        return array_[index];
    }
    
    /** Doubles the size of the array. */
    void Grow ()
    {
        int size = size_; 
        T* array_local = array_,
         * new_array = new T[size << 1];
        for (int i = 0; i < size; ++i)
            new_array[i] = array_local[i];
        size_ = size << 1;
        // Size should never be below 4.
        delete [] array_local;
        array_ = new_array;
    }

    private:

    int size_,       //< The max number of elements.
        count_;       //< The number of elements.
    T* array_;               //< The array.
};      //< Array
}       //< namespace _pro
#endif  //< KABUKI_PRO_ARRAY_H


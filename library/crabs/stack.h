/** Kabuki Toolkit
    @version 0.x
    @file    ~/libraries/crabs/stack.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#include <stdafx.h>

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3
#ifndef HEADER_FOR_CRABS_STACK
#define HEADER_FOR_CRABS_STACK

#include "config.h"

namespace _ {

/** Stores and retreives the Object2 auto-size. */
KABUKI uint16_t ObjectAutosize (uint16_t new_size);

/** Stores and retreives the Object4 auto-size. */
KABUKI uint32_t ObjectAutosize (uint32_t new_size);

/** Stores and retreives the Object8 auto-size. */
KABUKI uint64_t ObjectAutosize (uint64_t new_size);

/** @ingroup Stack
    Stack is an ASCII Object Data Type that is is designed to use a C-style 
    templated struct in C++ using no dynamic memory and with dynamic memory as 
    a templated C++ class that wraps the . This allows for easy cross-language 
    bindings and deterministic CPU cache optimizations.
*/

/** A stack of 8, 16, 32, or 64 bit plain-old-data (POD) types.

    Stack Memory Layout

    @code
        +================+
        |  Packed Stack  |  <-- Only if header_size = 0
        |================|
        | 64-bit Aligned |
        |     Buffer     |
        |================|
        | Stack Elements |
     ^  |================|
     |  |  Stack struct  |
    0xN +================+
    @endcode
*/
template<typename T, typename I = int>
struct TStack {
    I size_bytes, //< Total size of the Stack in 64-bit aligned bytes.
      reserved,   //< Reserved for memory alignment.
      height,     //< Total height of the Stack in elements.
      count;      //< Count of the elements on the stack.
    T element_1;  //< First element in the stack.
};

/** Gets the max length of an stack of size T.
    The biggest thing you can fit in a buffer in Script is 1/2 max size of a uint_t
    divided by the size of the element in bytes. */
template<typename T, typename I = int>
uint_t StackCountMax () {
    return (((uint_t)1) << (sizeof (uint_t) * 8 - 1)) / sizeof (T);
}

/** Gets the max number of elements in an stack with the specific index
    width. */
template<typename T, typename I = int>
I StackElementsMax () {
    return (~(I)0) / sizeof (T);
}

/** Initializes an stack of n elements of the given type.
    @param buffer An stack of bytes large enough to fit the stack.
*/
template<typename T, typename I = int>
TStack<T, I>* StackInit (T* buffer, I size) {
    TStack<T, I>* stack = reinterpret_cast<TStack<T, I>*> (buffer);
    stack->size_bytes = size;
    stack->count = 0;
    stack->height = (size < 1) ? 1 : size; //< Stack count is one or more.
    return stack;
}
template<typename T, typename I = int>
TStack<T, I>* StackInit (uintptr_t* buffer, I size) {
    return StackInit<T, I> (reinterpret_cast<T*> (buffer), size);
}

template<typename T, typename I = int>
T* StackBase (TStack<T, I>* stack) {
    if (!stack)
        return nullptr;
    char* address = reinterpret_cast<char*> (stack) + sizeof (TStack<T, I>);
    return reinterpret_cast<T*> (address);
}

/** Inserts the item into the stack at the given index.
    @param a    			 The stack.
    @param item  The item to insert.
    @param index The index to insert at.
    @return -1 if a is nil and -2 if the stack is full. */
template<typename T, typename I = int>
T StackInsert (TStack<T, I>* stack, T item, T index) {
    if (!stack)
        return -1;
    I size = stack->height,
        count = stack->count;
    if (count >= size)
        return -2;
    T* items = StackBase<T, I> (stack);
    if (count == 0) {
        *items = item;
        stack->count = 1;
        return 0;
    }
    if (count == 1) {
        stack->Print () = 1;
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
        stack->count = count + 1;
        return count;
    }
    // Common Case:
    // First move stack up one starting at index.
    T* insert_point = items + index,
        *end = items + count - 1;
    T value;
    while (insert_point != end) {
        value = *end;
        *(end + 1) = value;
        --end;
    }
    stack->count = count + 1;
    return count;
}

/** Removes the given index from the stack.
    @param  a     The stack.
    @param  index The index the item to remove.
    @return True if the index is out of bounds. */
template<typename T, typename I = int>
bool StackRemove (TStack<T, I>* stack, I index) {
    if (!stack)
        return false;
    I count = stack->count;
    T value;
    if (count == 0) // Nothing to remove!
        return false;
    if (index >= count)
        return false;
    T* items = StackBase<T, I> (stack);
    if (count == index - 1) {
        stack->count = count - 1;
        value = items[count - 1];
        return true;
    }
    // Move all of the elements after the index down one.
    T* insert_point = StackBase<T, I> (stack) + index,
        *end = StackBase<T, I> (stack) + count - 1;
    while (insert_point != end) {
        value = *end;
        *(end - 1) = value;
        --end;
    }
    stack->count = count - 1;

    return true;
}

/** Adds the given item to the end of the stack.
    @param  a    The stack.
    @param  item The item to push onto the stack.
    @return The index of the newly stacked item. */
template<typename T, typename I = int>
I StackPush (TStack<T, I>* stack, T item) {
    if (!stack)
        return -1;
    I size = stack->height,
        count = stack->count;
    if (count >= size)
        return -2;
    T* items = StackBase<T, I> (stack);
    items[count] = item;
    stack->count = count + 1;
    return count;
}

/** Pops the top item off of the stack.
    @note We do not delete the item at the
    @param  a The stack.
    @return The item popped off the stack. */
template<typename T, typename I = int>
T StackPop (TStack<T, I>* stack) {
    if (!stack)
        return !((T)0);
    I count = stack->count;
    if (count == 0) // Nothing to remove!
        return 0;
    T* items = StackBase<T, I> (stack);
    stack->count = count - 1;
    T item = items[count - 1];
    return item;
}

/** Gets the element at the given index.
    @param  stack    The stack.
    @param  index The index of the element to get.
    @return -1 if a is nil and -2 if the index is out of bounds. */
template<typename T, typename I = int>
T StackGet (TStack<T, I>* stack, I index) {
    if (!stack)
        return 0;
    if (index >= stack->count)
        return 0;
    char* address = reinterpret_cast<char*> (stack) + sizeof (TStack<T, I>);
    return reinterpret_cast<T*> (address)[index];
}

/** Returns true if the given stack contains the given address.
    @return false upon failure. */
template<typename T, typename I = int>
bool StackContains (TStack<T, I>* stack, void* address) {
    if (!stack)
        return false;
    char* ptr = reinterpret_cast<char*> (stack),
        *adr = reinterpret_cast<char*> (address);
    if (adr < ptr)
        return false;
    if (adr >= ptr + stack->size_bytes)
        return false;
    return true;
}

/** A stack of data.

    This is a wrapper class for the 

    Stack Memory Layout

    @code
    +----------------+
    |  Packed Stack  |  <-- Only if header_size = 0
    |----------------|
    | 64-bit Aligned |
    |     Buffer     |
    |----------------|
    | Stack Elements |
    |----------------|  ^
    |  Stack struct  |  |
    +----------------+ 0xN
    @endcode
*/
template<typename T, typename I = int>
class Stack {

    /** Initializes an stack of n elements of the given type.
        @param buffer An stack of bytes large enough to fit the stack.
    */
    Stack () {
        StackInit<T, I> (T* buffer, I buffer_size);
    }

    uint_t StackCountMax () {
        return StackCountMax<T, I> ();
    }

    /** Gets the max number of elements in an stack with the specific index
        width. */
    I GetElementsMax () {
        return StackElementsMax<T, I> ();
    }

    T* GetBegin () {
        auto stack = reinterpret_cast<Stack<T, I> (buffer_);
        return StackBegin<T, I> (stack);
    }

    /** Inserts the item into the stack at the given index.
        @param a    			 The stack.
        @param item  The item to insert.
        @param index The index to insert at.
        @return -1 if a is nil and -2 if the stack is full. */
    T Insert (T item, T index) {
        auto stack = reinterpret_cast<Stack<T, I> (buffer_);
        return StackInsert<T, I> (stack, item, index)
    }

    /** Removes the given index from the stack.
        @param  a     The stack.
        @param  index The index the item to remove.
        @return True if the index is out of bounds. */
    bool Remove (I index) {
        auto stack = reinterpret_cast<Stack<T, I> (buffer_);
        return StackRemove<T, I> (stack, index);
    }

    /** Adds the given item to the end of the stack.
        @param  a    The stack.
        @param  item The item to push onto the stack.
        @return The index of the newly stacked item. */
    I Push (T item) {
        auto stack = reinterpret_cast<Stack<T, I> (buffer_);
        I result = StackPush<T, I> (stack, item);
        
    }

    /** Pops the top item off of the stack.
        @note We do not delete the item at the
        @param  a The stack.
        @return The item popped off the stack. */
    T Pop () {
        auto stack = reinterpret_cast<Stack<T, I> (buffer_);
        return StackPop<T, I> (stack);
    }

    /** Gets the element at the given index.
        @param  stack    The stack.
        @param  index The index of the element to get.
        @return -1 if a is nil and -2 if the index is out of bounds. */
    T Get (I index) {
        auto stack = reinterpret_cast<Stack<T, I> (buffer_);
        return StackGet<T, I> (stack, index);
    }

    /** Returns true if the given stack contains the given address.
        @return false upon failure. */
    bool Contains (void* address) {
        auto stack = reinterpret_cast<Stack<T, I> (buffer_);
        return StackContains<T, I> (stack, address);
    }

    private:

    uintptr_t * buffer_;
};

}       //< namespace _
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3
#endif  //< HEADER_FOR_CRABS_STACK

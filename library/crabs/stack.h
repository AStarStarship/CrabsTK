/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/stack.h
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
#include "memory.h"

namespace _ {

/** @ingroup Stack
    Stack is an ASCII Object Data Type that is designed to use a C-style 
    templated struct in C++ using no dynamic memory and with dynamic memory as 
    a templated C++ warper class and cross-language bindings and deterministic 
    CPU cache optimizations.
*/

/** Returns the maximum value of the given signed type. */
template<typename SI>
SI SignedMax () {
    enum {
        kMax = (sizeof (SI) == 1) ? 0x78 :
               (sizeof (SI) == 2) ? 0x7ff8 :
               (sizeof (SI) == 4) ? 0x7ffffff8 :
               (sizeof (SI) == 8) ? 0x7ffffffffffffff8 : 0,
    };
    return kMax;
}

/** Returns the maximum value of the given unsigned type. */
template<typename UI>
UI UnsignedMax () {
    enum {
        kMax = (sizeof (UI) == 1) ? 0xf8 :
               (sizeof (UI) == 2) ? 0xfff8 :
               (sizeof (UI) == 4) ? 0xfffffff8 :
               (sizeof (UI) == 8) ? 0xfffffffffffffff8 : 0,
    };
    return kMax;
}

/** A stack of 8, 16, 32, or 64 bit plain-old-data (POD) types.
    
    Stack Memory Layout

    @code
        +----------------+
        |  Packed Stack  |  <-- Only if header_size = 0
        |----------------|
        |     Buffer     |
        |----------------|
        | Stack Elements |
     ^  |----------------|
     |  |  Stack struct  |
    0xN +----------------+
    @endcode
*/
template<typename T = intptr_t, typename SI = int, typename UI = uint>
struct TStack {
    UI size;      //< Total size of the Stack in 64-bit aligned bytes.
    SI reserved,  //< Reserved for memory alignment.
       count_max, //< Total count_max of the Stack in elements.
       count;     //< Count of the elements on the stack.
    T  element_1; //< First element in the stack.
};

enum {
    kStackCountMin = 1,
};

/** Gets the min size of a Stack. */
template<typename T = intptr_t, typename SI = int, typename UI = uint>
inline UI StackSize () {
    enum {
        kStackSizeMin = sizeof (TStack<T, SI, UI>) + sizeof (T) * kStackCountMin,
    };
    return kStackSizeMin;
}

/** Gets the max number of elements in an stack with the specific index
    width. */
template<typename T = intptr_t, typename SI = int, typename UI = uint>
SI StackElementsMax () {
    return (~(SI)0) / sizeof (T);
}


/** Initializes an stack of n elements of the given type.
    @param buffer An stack of bytes large enough to fit the stack.
*/
template<typename T = intptr_t, typename SI = int, typename UI = uint>
TStack<T, SI, UI>* StackInit (SI count_max) {
    if (count_max < kStackCountMax)
        count_max = kStackCountMax;
    UI size = sizeof (TStack<T, SI, UI>) + count_max * (sizeof (T) + sizeof (SI));

    TStack<T, SI, UI>* stack = reinterpret_cast<TStack<T, SI, UI>*> (buffer);
    stack->size = size;
    stack->count = 0;
    stack->count_max = count_max;
    return stack;
}

/** Initializes an stack from a preallocated buffer.
    @param buffer An stack of bytes large enough to fit the stack.
*/
template<typename T = intptr_t, typename SI = int, typename UI = uint>
TStack<T, SI, UI>* StackInit (UI size, uintptr_t* buffer = nullptr) {
    if (buffer == nullptr) {
        if (size < StackSize<T, SI, UI> ())
            size = StackSize<T, SI, UI> ();
        buffer = new uintptr_t[size / sizeof (uintptr_t)];
    }
    else {
        assert (size >= StackSize ());
    }

    TStack<T, SI, UI>* stack = reinterpret_cast<TStack<T, SI, UI>*> (buffer);
    stack->size = size;
    stack->count = 0;
    stack->count_max = (size - sizeof (TStack<T, SI, UI>)) / sizeof (T);
    return stack;
}

template<typename T = intptr_t, typename SI = int, typename UI = uint>
T* StackBase (TStack<T, SI, UI>* stack) {
    if (!stack)
        return nullptr;
    char* address = reinterpret_cast<char*> (stack) + sizeof (TStack<T, SI, UI>);
    return reinterpret_cast<T*> (address);
}

/** Inserts the item into the stack at the given index.
    @param a    			 The stack.
    @param item  The item to insert.
    @param index The index to insert at.
    @return -1 if a is nil and -2 if the stack is full. */
template<typename T = intptr_t, typename SI = int, typename UI = uint>
T StackInsert (TStack<T, SI, UI>* stack, T item, T index) {
    if (!stack)
        return -1;
    SI size = stack->count_max,
        count = stack->count;
    if (count >= size)
        return -2;
    T* items = StackBase<T, SI, UI> (This ());
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
template<typename T = intptr_t, typename SI = int, typename UI = uint>
bool StackRemove (TStack<T, SI, UI>* stack, SI index) {
    if (!stack)
        return false;
    SI count = stack->count;
    T value;
    if (count == 0) // Nothing to remove!
        return false;
    if (index >= count)
        return false;
    T* items = StackBase<T, SI, UI> (This ());
    if (count == index - 1) {
        stack->count = count - 1;
        value = items[count - 1];
        return true;
    }
    // Move all of the elements after the index down one.
    T* insert_point = StackBase<T, SI, UI> (This ()) + index,
        *end = StackBase<T, SI, UI> (This ()) + count - 1;
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
template<typename T = intptr_t, typename SI = int, typename UI = uint>
SI StackPush (TStack<T, SI, UI>* stack, T item) {
    if (!stack)
        return -1;
    SI size = stack->count_max,
        count = stack->count;
    if (count >= size)
        return -2;
    T* items = StackBase<T, SI, UI> (stack);
    items[count] = item;
    stack->count = count + 1;
    return count;
}

/** Pops the top item off of the stack.
    @note We do not delete the item at the
    @param  a The stack.
    @return The item popped off the stack. */
template<typename T = intptr_t, typename SI = int, typename UI = uint>
T StackPop (TStack<T, SI, UI>* stack) {
    if (!stack)
        return !((T)0);
    SI count = stack->count;
    if (count == 0) return 0;
    T* items = StackBase<T, SI, UI> (stack);
    stack->count = count - 1;
    T item = items[count - 1];
    return item;
}

/** Pops the top item off of the stack.
    @note We do not delete the item at the
    @param  a The stack.
    @return The item popped off the stack. */
template<typename T = intptr_t, typename SI = int, typename UI = uint>
T StackPeek (TStack<T, SI, UI>* stack) {
    if (!stack)
        return !((T)0);
    SI count = stack->count;
    if (count == 0) return 0;
    T* items = StackBase<T, SI, UI> (This ());
    T item = items[stack->count - 1];
    return item;

}

/** Gets the element at the given index.
    @param  stack    The stack.
    @param  index The index of the element to get.
    @return -1 if a is nil and -2 if the index is out of bounds. */
template<typename T = intptr_t, typename SI = int, typename UI = uint>
T StackGet (TStack<T, SI, UI>* stack, SI index) {
    if (!stack)
        return 0;
    if (index >= stack->count)
        return 0;
    char* address = reinterpret_cast<char*> (stack) + sizeof (TStack<T, SI, UI>);
    return reinterpret_cast<T*> (address)[index];
}

/** Returns true if the given stack contains the given address.
    @return false upon failure. */
template<typename T = intptr_t, typename SI = int, typename UI = uint>
bool StackContains (TStack<T, SI, UI>* stack, void* address) {
    if (!stack)
        return false;
    char* ptr = reinterpret_cast<char*> (stack),
        *adr = reinterpret_cast<char*> (address);
    if (adr < ptr)
        return false;
    if (adr >= ptr + stack->size)
        return false;
    return true;
}

template<typename T = intptr_t, typename SI = int, typename UI = uint>
inline UI StackSize (SI count) {
    UI size = sizeof (TStack<T, SI, UI>) + (sizeof (T) * 8) * count;
    return Align8<UI> (size);
}

template<typename T = intptr_t, typename SI = int, typename UI = uint>
inline UI StackSizeWords (SI count) {
    return StackSize<T, SI, UI> (count) / sizeof (uintptr_t);
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
template<typename T = intptr_t, typename SI = int, typename UI = uint>
class Stack {
    public:

    /** Initializes an stack of n elements of the given type.
        @param count_max The max number of elements that can fit in memory in this Stack.
    */
    Stack (SI count_max = 0) {
        if (count_max <= kStackCountMin) {
            count_max = kStackCountMin;
        }
        UI size_words = StackSizeWords (count_max);
        StackInit<T, SI, UI> (size_words, new uintptr_t[size_words]);
    }

    /** Gets the max number of elements in an stack with the specific index
        width. */
    inline SI GetElementsMax () {
        return StackElementsMax<T, SI, UI> ();
    }

    /** Gets the size of the entire Stack, including header, in bytes. */
    inline UI GetSize () {
        return This ()->size;
    }
    
    /** Gets the min size of the entire Stack, including header, in bytes. */
    inline UI GetSizeMin () {
        return StackSize<T, SI, UI> ();
    }

    T* Elements () {
        return StackBegin<T, SI, UI> (This ());
    }

    /** Inserts the item into the stack at the given index.
        @param item  The item to insert.
        @param index The index to insert at.
        @return -1 if a is nil and -2 if the stack is full. */
    T Insert (T item, T index) {
        return StackInsert<T, SI, UI> (This (), item, index);
    }

    /** Removes the given index from the stack.
        @param  index The index the item to remove.
        @return True if the index is out of bounds. */
    bool Remove (SI index) {
        return StackRemove<T, SI, UI> (This (), index);
    }

    /** Adds the given item to the end of the stack.
        @param  item The item to push onto the stack.
        @return The index of the newly stacked item. */
    SI Push (T item) {
        SI result = StackPush<T, SI, UI> (This (), item);
        if (result < 0) {
            if (This ()->count == SignedMax<SI> ())
                return -1;
            // Else double the buffer size or as high as memory will allow.
            UI size = GetSize ();
            if ((size >> (sizeof (UI) * 8 - 1)) == 1) {
                UI unsigned_max = UnsignedMax<UI> ();
                if (size == unsigned_max)
                    return -1;
                size = unsigned_max;
                uintptr_t* buffer = new uintptr_t[size / sizeof (uintptr_t)];
                MemoryCopy (buffer, unsigned_max, buffer_, size);
            }

        }
        return result;
    }

    /** Pops the top item off of the stack.
        @note We do not delete the item at the
        @param  a The stack.
        @return The item popped off the stack. */
    T Pop () {
        return StackPop<T, SI, UI> (This ());
    }

    /** Pops the top item off of the stack.
        @note We do not delete the item at the
        @param  a The stack.
        @return The item popped off the stack. */
    T Peek () {
        return StackPeek<T, SI, UI> (This ());
    }

    /** Gets the element at the given index.
        @param  index The index of the element to get.
        @return -1 if a is nil and -2 if the index is out of bounds. */
    T Get (SI index) {
        return StackGet<T, SI, UI> (This (), index);
    }

    /** Returns true if the given stack contains the given address.
        @return false upon failure. */
    bool Contains (void* address) {
        return StackContains<T, SI, UI> (This (), address);
    }

    private:

    uintptr_t * buffer_;

    inline TStack<T, SI, UI>* This () {
        return reinterpret_cast<TStack<T, SI, UI>*> (buffer_);
    }
};

}       //< namespace _
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3
#endif  //< HEADER_FOR_CRABS_STACK

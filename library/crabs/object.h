/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/object.h
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

#ifndef HEADER_FOR_CRABS_OBJECT
#define HEADER_FOR_CRABS_OBJECT

#include "type.h"

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3

namespace _ {
/** An ASCII Object.



    @code
    ;
    @endcode
*/
template<typename TSize>
struct KABUKI Object {
    uintptr_t *buffer;
};

template<typename TSize = uint32_t>
TSize ObjectSize (Object<TSize>* object) {
    return *reinterpret_cast<TSize*> (object);
}

template<typename TSize = uint32_t, typename T = uintptr_t * >
TSize ObjectSize (T* object) {
    return *reinterpret_cast<TSize*> (object);
}

template<typename TSize = uint32_t>
TSize ObjectSize (uintptr_t* buffer) {
    return *reinterpret_cast<TSize*> (buffer);
}

/** Aligns the given word to 64-bit word boundary. */
template<typename T>
inline T WordAlign8 (T value) {
    return value + (((~value) + 1) &
                     (sizeof (T) - 1));
}

template<typename TSize = uint32_t>
Object<TSize>* ObjectClone (Object<TSize>* object) {
    assert (object);
    TSize* size_ptr = reinterpret_cast<TSize*> (object);
    TSize size = (*size_ptr) >> kWordBitCount;
    uintptr_t* buffer = new uintptr_t[size],
             * read = buffer,
             * write = reinterpret_cast<uintptr_t*> (object);
    for (; size > 0; size--)
        *write++ = *read++;
    return *reinterpret_cast<TSize*> (buffer);
}

}       //< namespace _
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3
#endif  //< HEADER_FOR_CRABS_OBJECT

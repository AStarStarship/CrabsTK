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

#include "config.h"

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

template<typename TSize = uint32_t, typename T = uintptr_t*>
TSize ObjectSize (T* object) {
    return *reinterpret_cast<TSize*> (object);
}

template<typename TSize = uint32_t>
TSize ObjectSize (uintptr_t* buffer) {
    return *reinterpret_cast<TSize*> (buffer);
}

template<typename TSize = uint32_t>
Object<TSize>* ObjectClone (Object<TSize>* object) {
    
    return *reinterpret_cast<TSize*> (buffer);
}

/** Stores and retreives the Object2 auto-size. */
KABUKI uint16_t ObjectAutosize (uint16_t new_size);

/** Stores and retreives the Object4 auto-size. */
KABUKI uint32_t ObjectAutosize (uint32_t new_size);

/** Stores and retreives the Object8 auto-size. */
KABUKI uint64_t ObjectAutosize (uint64_t new_size);

}       //< namespace _
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3
#endif  //< HEADER_FOR_CRABS_OBJECT

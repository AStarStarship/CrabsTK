/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/data/iterator.h
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

#pragma once
#include <stdafx.h>
#if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1

#ifndef HEADER_FOR_KT_DATA_ITERATOR
#define HEADER_FOR_KT_DATA_ITERATOR

#include "config.h"

namespace kt { namespace data {

/** An interface for an object iterator. */
class Iterator
{
    public:
    
    /** Iterates to the next item in the sequence. */
    virtual void* GetNext () = 0;
    
    /** Resets the iterator to the beginning. */
    virtual void Reset () = 0;
    
    /** Indicates if the iteration is complete.
        @return Returns 0 when the iteration is complete. */
    virtual int IsDone () = 0;
};

/** Interface for an Iterable Collection of objects. */
struct Iterable {
    /** Gets an instance of an Iterator. */
    virtual Iterator* GetIterator () = 0;
};      //< Iterable

}       //< namespace data
}       //< namespace kt
#endif  //< HEADER_FOR_KT_DATA_ITERATOR
#endif  //< #if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1

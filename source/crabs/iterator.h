/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/crabs/KabukiSDK/data/iterator.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#ifndef CRABS_ITERATOR_H
#define CRABS_ITERATOR_H

#include "config.h"

#if USING_CRABS_ITERATOR

namespace _ {

/** Interface for a generic iterator. */
struct KABUKI Iterator {

    /** Virtual destructor. */
    virtual ~Iterator () = 0;
    
    /** Iterates over the next item.
        @return Pointer to the next object and nil when iteration is 
                complete. */
    virtual void* GetNext () = 0;
    
    /** Flags when there are not more objects to iterate through.
        @return True when there are no more objects to iterate through. */
    virtual bool IsDone () = 0;
};

}       //< namespace _
#endif  //< USING_CRABS_ITERATOR
#endif  //< CRABS_ITERATOR_H

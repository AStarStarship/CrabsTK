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

#include "stack.h"

namespace _ {

uint16_t ObjectAutoSize (uint16_t new_size) {
    static uint16_t auto_size = kAutoSizeDefault;
    if (new_size != 0) {
        if (new_size < kAutoSizeMin) {
            return 0;
        }
        auto_size = new_size;
        return new_size;
    }
    return auto_size;
}

uint32_t ObjectAutoSize (uint32_t new_size) {
    static uint32_t auto_size = kAutoSizeDefault;
    if (new_size != 0) {
        if (new_size < kAutoSizeMin) {
            return 0;
        }
        auto_size = new_size;
        return new_size;
    }
    return auto_size;
}

uint64_t ObjectAutoSize (uint64_t new_size) {
    static uint64_t auto_size = kAutoSizeDefault;
    if (new_size != 0) {
        if (new_size < kAutoSizeMin) {
            return 0;
        }
        auto_size = new_size;
        return new_size;
    }
    return auto_size;
}

}       //< namespace _
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3

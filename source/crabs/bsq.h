/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/crabs/bsq.h
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

#ifndef HEADER_FOR_CRABS_BSQ
#define HEADER_FOR_CRABS_BSQ

#include "type.h"

#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 3

namespace _ {

constexpr uint_t BsqSize (const uint_t* params) {
    if (!params) {
        return 0;
    }
    uint_t size  = sizeof (uint_t),
           count = *params++;

    if (count > kParamsMax) {
        return 0;
    }

    for (; count > 0; --count) {
        uint_t param = *params++;

        if (param == NIL) { // This is illegal.
            return 0;
        }
        if (param <= TKN) {
            size += sizeof (uint_t);
            ++params;
        }
        if (param == SIN) {
            size += sizeof (uint_t);
            ++params;
        }
        if (param == UIN) {
            size += sizeof (uint_t);
            ++params;
        }
        if (param >= LST && param <= MAP) { // This is illegal.
            return 0;
        }
        if (param > MAP) {
            if (param >> 8) {        // This is an error.
                return 0;
            }
            if (param & 0x20) {      // It's a multi-dimensional array so
                param = *params;     // add the number of dimensions + 1;
                params += param + 1; // for the dimension count.
            }
        }
        size += sizeof (uint_t);
    }
    return size;
}

/** Creates a immutable Script B-Sequence.
    C++11 variadic template to ensure only one copy in ROM
    and to eliminate some redundant typing. */
template<const uint_t... N>
inline const uint_t* Bsq () {
    static const uint_t kSize = 0,//BsqSize ({ N... })
        kList[sizeof... (N)] = { N... };
    return &kSize;
}

KABUKI char* PrintBsq (const uint_t* params, char* buffer, char* buffer_end);

/*< Returns the requested parameter number. */
KABUKI uint_t BsqParamNumber (const uint_t* bsq, int param_number);

}       //< namespace _
#endif  //< MAJOR_SEAM >= 1 && MINOR_SEAM >= 3
#endif  //< HEADER_FOR_CRABS_BSQ

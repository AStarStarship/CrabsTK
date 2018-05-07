/** Kabuki Toolkit
    @version 0.x
    @file    ~/libraries/crabs/hex.h
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

#pragma once
#include <stdafx.h>

#ifndef HEADER_FOR_CRABS_BINARY
#define HEADER_FOR_CRABS_BINARY

#include "printer.h"

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 2

namespace _ {

/** Utility class for printing hex with operator<<. */
template<typename T>
class Binary {
    public:

    T value;    //< Value to convert to binary.

    /** Constructor saves value for use with operator overloads. */
    Binary (T value) :
        value (value) {
    }

    Printer& Print (Printer& printer) {
        char* cursor = printer.cursor;
        if (cursor + sizeof (uint64_t) * 8 >= printer.end) {
            return nullptr;
        }

        for (int i = 0; i < 64; ++i) {
            *buffer++ = (char)('0' + (value >> 63));
            value = value << 1;
        }
        *buffer = delimiter;
        return buffer;
    }
};

}       //< namespace _

template<typename T>
inline _::Printer& operator<< (_::Printer& printer, _::Binary<T> value) {
    return value.Print (printer);
}

#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3
#endif  //< HEADER_FOR_CRABS_BINARY

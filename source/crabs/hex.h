/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/crabs/hex.h
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

#ifndef HEADER_FOR_CRABS_HEX
#define HEADER_FOR_CRABS_HEX

#include "print.h"
#include "slot.h"

#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 3

namespace _ {

/** Utility class for printing hex with operator<<. */
template<typename T>
class Hex {
    public:

    T value;

    Hex (T value) :
        value (value) {
    }
};

}       //< namespace _

template<typename T>
inline _::Printer& operator<< (_::Printer& print, const _::Hex<T>& hex) {
    print.cursor = _::PrintHex (hex, print.cursor, print.end, 0);
    return print;
}

template<typename T>
inline _::Printer& operator<< (_::Slot& slot, const _::Hex<T>& hex) {
    return _::PrintHex (hex.value, slot);
}

#endif  //< MAJOR_SEAM >= 1 && MINOR_SEAM >= 3
#endif  //< HEADER_FOR_CRABS_HEX

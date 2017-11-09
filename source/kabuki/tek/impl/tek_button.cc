/** kabuki::tek
    @file    ~/source/kabuki/tek/impl/button.cpp
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
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

#include "../include/button.h"

namespace kabuki {
namespace tek {

Button::Button (uint bit)
    : bit_number_ (bit) {
}

inline byte Button::GetState (byte* digitalIns) {
    uint bit = bit_number_;
    return digitalIns[bit >> BitToByteShift] & (1 << (bit & BitNumberMask));
}

inline void Button::Poll (byte* debouncedXOR) {
    byte index = bit_number_ >> BitToByteShift,
        mask = 1 << (bit_number_ & BitNumberMask),
        state = debouncedXOR[index] & mask;

    if (state == 0) return;

    if (debouncedXOR[index] & mask) { PressFunction.call (); return; }

    DepressFunction.call ();
}

const Operation* Button::Star (char_t index, _::Expression* expr) {
    const Operation this_op = { "Button", NumMembers (0), FirstOperation ('A'),
        "tek::sensors::Button" };
    switch (index) {
        case 0: return _::NumOperations (0);
    }

    return _::InvalidIndex ();
}

}       //< namespace tek
}       //< namespace kabuki

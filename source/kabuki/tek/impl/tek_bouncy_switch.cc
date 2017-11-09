/** kabuki::tek
    @file    ~/source/kabuki/tek/impl/tek_bouncy_switch.cc
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

#include "../include/bouncy_swith.h"

namespace kabuki { namespace tek {

BouncySwitch::BouncySwitch (PinName Pin) :
    input ((PinName)Pin) {
    /// Nothing to do here!
}

BouncySwitch::~BouncySwitch () {}

const _::Operation* BouncySwitch::Star (char_t index, _::Expression* expr) {
    static const Operation this_op = { "BouncySwitch",
        _::NumOperations (0), _::FirstOperation ('A'),
        "A software debounced switch.", 0
    };

    void* args[1];

    switch (index) {
        case '?': return &this_op;
        case 'A':
        {
            static const _::Operation op_A = {
                "Read",
                _::Params<1, _::SI4> (),
                _::Params<_::NIL> (),
                "Reads the switch state."
            };
            if (!expr) return op_A;

            int temp = 0;// input;
            byte switchState = (byte)temp;

            return _::Result (expr, _::Params<_::NIL>, _::Args (args, &switchState));
        }
    }
    return nullptr;
}
}       //< namespace tek
}       //< namespace kabuki

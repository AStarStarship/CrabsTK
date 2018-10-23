/** kabuki::tek
    @file    ~/source/kabuki/tek/impl/switch.cc
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

#include "switch.h"

using namespace _ {;

namespace kabuki { namespace tek {

Switch::Switch (PinName pin) :
    input_ (pin) {
    /// Nothing to do here!
}

const Operation* Switch::Star (char_t index, Expr* expr) {
    static const Operation This = { "Switch",
        NumOperations (0), FirstOperation ('A'),
        "A software debounced switch.", 0
    };

    void* args[1];

    switch (index) {
        case '?': return &This;
        case 'a': {
            static const Operation OpA = {
                "Read", Params<1, SI4> (), Params<NIL> (),
                "Reads the switch state."
            };
            if (!expr) return &OpA;

            int temp = 0;// input;
            byte switchState = (byte)temp;

            return Result (expr, Params<NIL> (), Args (args, &switchState));
        }
    }
    return nullptr;
}

SwitchOp::SwitchOp (Switch* object) :
	object_ (object) {
	// Nothing to do here!
}

const Operation* SwitchOp::Star (char_t index, Expr* expr) {
	return object_->Star (index, expr);
}

}       //< namespace tek
}       //< namespace kabuki

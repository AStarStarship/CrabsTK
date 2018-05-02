
/** kabuki::tek
    @file    ~/source/kabuki/tek/impl/tek_h_bridge_motor.cc
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

#include "h_bridge_motor.h"

using namespace _;

namespace kabuki { namespace tek {

HBridgeMotor::HBridgeMotor  (PinName pwn_pin, PinName forward_pin,
                             PinName reverse_pin)
:   pulse_   (pwn_pin),
    forward_ (forward_pin),
    reverse_ (reverse_pin) {
    pulse_width_ = 0.0f;
    pulse_.period  (0.0f);
    forward_ = 0;
    reverse_ = 0;
}

HBridgeMotor::~HBridgeMotor () {}

void HBridgeMotor::Stop () {
    forward_ = reverse_ = 0;  //< Turn off to GetPrevent any unwanted behavior.
    pulse_width_ = 0.0f;
}

void HBridgeMotor::Move  (float value) {
    forward_ = reverse_ = 0;  //< Turn off to GetPrevent any unwanted behavior.
    pulse_width_ = value;
    forward_ = value < 0.0f ? 1 : 0;
    reverse_ = value > 0.0f ? 1 : 0;
}

const Operation* HBridgeMotor::Star (char_t index, Expression* expr) {
    static const Operation This = {
        "HBridgeMotor", NumOperations (2), FirstOperation ('A'),
        "An H-Bridge motor.", 0
    };
    
    void* args[1];

    switch (index) {
        case '?': return &This;
        case 'A': {
            static const Operation OpA = { "Stop",
                Params<NIL> (), Params<0> (),
                "Stops the motor.", 0
            };
            if (!expr) return &OpA;
            Stop ();
            return 0;   //< No return values.
        }
        case 'b': {
            static const Operation OpB = {
                "Move", Params<1, FLT> (), Params<1> (),
                "Sets the motor to move forward  (0.0 - 1.0) or backwards "
                "(-1.0 - 0.0) for x seconds.", 0 };
            if (!expr) return &OpB;
            float input;
            if (Args (expr, Params<1, FLT> (), Args (args, &input)))
            	return expr->result;
            Move (input);
            return expr->result;
        }
    }
    return nullptr;
}

}       //< namespace tek
}       //< namespace kabuki

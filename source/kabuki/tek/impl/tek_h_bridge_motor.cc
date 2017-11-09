e/** kabuki::tek
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

#include "../include/h_bridge_motor.h"

namespace kabuki { namespace tek {

HBridgeMotor::HBridgeMotor  (word PWMPin, word ForwardPin, word ReversePin)
:   pulse ((PinName)PWMPin),
    forward ((PinName)ForwardPin),
    reverse ((PinName)ReversePin)
{
    pulseWidth = 0.0f;
    pulse.period  (pulseWidth);
    forward = 0;
    reverse = 0;
}

HBridgeMotor::~HBridgeMotor () {}

inline const char* HBridgeMotor::Stop  (_::Expression* expr) {
    forward = reverse = 0;  //< Turn off to GetPrevent any unwanted behavior.
    pulseWidth = 0.0f;
}

inline const char* HBridgeMotor::Move  () {
    forward = reverse = 0;  //< Turn off to GetPrevent any unwanted behavior.

    pulseWidth = input;
    forward_ = input < 0.0f ? 1 : 0;
    reverse_ = input > 0.0f ? 1 : 0;

    return 0;
}

const Operation* HBridgeMotor::Star (byte index, Verifier* io)
{
    static const Operation* this_op = {
        "HBridgeMotor", NumMembers (2), FirstOperation ('A'),
        "An H-Bridge motor."
    };
    if (!index) return &this_op;
    
    switch (index)
    {
      case 'A': 
        static const Operation* m_A = { "Stop", Params<NIL>, Params<NIL>, 
                                     "Stops the motor." };
        if (!io) return m_A;
        Stop ();
        return 0;   //< No return values.
        
      case 'B':
        static const Operation* m_B = { 
            "Move", Params<1, FLT>, Params<NIL>,
            "Sets the motor to move forward  (0.0 - 1.0) or backwards (-1.0 - "
            "0.0) for x seconds.")
        if (!io) return m_B;
        float input;
        Move (input);
        return 0;
    }

    return _::InvalidIndex ();
}

}       //< namespace tek
}       //< namespace kabuki

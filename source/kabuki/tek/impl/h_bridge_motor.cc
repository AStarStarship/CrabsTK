e/** Kabuki Tek
    @file    /.../Source/kabuki-tek-Impl/actuators/HBridgeMotor.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright  (C) 2017 [Cale McCollough](calemccollough.github.io)

                            All right reserved  (R).

        Licensed under the Apache License, Version 2.0  (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#include <actuators/HBridgeMotor.h>

namespace tek { namespace actuators {

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

inline const char* HBridgeMotor::Stop  (Uniprinter* io) {
    forward = reverse = 0;  //< Turn off to GetPrevent any unwanted behavior.
    pulseWidth = 0.0f;
}

inline const char* HBridgeMotor::Move  () {
    forward = reverse = 0;  //< Turn off to GetPrevent any unwanted behavior.

    pulseWidth = input;
    forward = input < 0.0f ? 1 : 0;
    reverse = input > 0.0f ? 1 : 0;

    return 0;
}

const Member* HBridgeMotor::Op (byte index, Verifier* io)
{
    static const Member* this_member = {
        "HBridgeMotor", NumMembers (2), FirstMember ('A'),
        "An H-Bridge motor."
    };
    if (!index) return &this_member;
    
    switch (index)
    {
      case 'A': 
        static const Member* m_A = { "Stop", Params<NIL>, Params<NIL>, 
                                     "Stops the motor." };
        if (!io) return m_A;
        Stop ();
        return 0;   //< No return values.
        
      case 'B':
        static const Member* m_B = { 
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
}   //< namespace actuators
}   //< namespace _


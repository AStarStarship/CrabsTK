/** Kabuki Tek
    @file    /.../Source/actuators/HBridgeMotor.h
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

#pragma once

#include <_/device.h>
#include <kabuki/config.h>

namespace kabuki { namespace actuators {
    
class HBridgeMotor : public Device
{
    public:

    /** Simple constructor. */
    HBridgeMotor  (word PWMPin, word ForwardPin, word ReversePin);

    /** Virtual destructor. */
    virtual ~HBridgeMotor ();

    /** Stops the motor. */
    inline const char* Stop  ();

    /** Moves the motor forward(+) or backwards(-). */
    inline const char* Move  (float value);
    
    /** I2P operations. */
    const Member* Op (byte index, Uniprinter* io) override;

    private:

    float pulseWidth;       //< The velocity.

    PwmOut pulse;           //< PWM output pin
    DigitalOut forward,     //< The forward motor pin.
        reverse;            //< The reverse motor pin.
};
}   //< namespace actuators
}   //< namespace tek


/** kabuki::tek
    @file    ~/source/kabuki/tek/include/h_bridge_motor.h
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

#ifndef KABUKI_TEK_H_BRIDGE_MOTOR_H
#define KABUKI_TEK_H_BRIDGE_MOTOR_H

#include "config.h"

namespace kabuki { namespace tek {
    
class HBridgeMotor {
    public:

    /** Simple constructor. */
    HBridgeMotor  (PinName pwm_pin, PinName forward_pin, PinName reverse_pin);

    /** Virtual destructor. */
    virtual ~HBridgeMotor ();

    /** Stops the motor. */
    void Stop  ();

    /** Moves the motor forward(+) or backwards(-). */
    void Move  (float value);
    
    /** Script operations. */
    const _::Operation* Star (char_t index, _::Expression* expr);

    private:

    float      pulse_width_; //< The velocity.
    PwmOut     pulse_;      //< PWM output pin
    DigitalOut forward_,    //< The forward motor pin.
               reverse_;    //< The reverse motor pin.
};

class HBridgeMotorOp {
    public:

	HBridgeMotorOp (HBridgeMotor* motor);

    /** Script operations. */
    const _::Operation* Star (char_t index, _::Expression* expr);

    private:

	HBridgeMotor* motor_;   //< Pointer to the motor.
};
}       //< namespace tek
}       //< namespace kabuki
#endif  //< KABUKI_TEK_H_BRIDGE_MOTOR_H

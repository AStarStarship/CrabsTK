/** kabuki::tek
    @file    /.../kabuki-tek/sensors/rotary_encoder.h
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
 
#ifndef KABUKI_TEK_SENSORS_ROTARYENCODER_H
#define KABUKI_TEK_SENSORS_ROTARYENCODER_H

#include "module_config.h"

namespace kabuki { namespace tek {

class Controller;   //< Mutual dependency.

/** A 2-bit gray-code rotary encoder.
    
*/
class RotaryEncoder {
    public:

    /** Constructor a rotary encoder with the given input channels. */
    RotaryEncoder  (uint inputA, uint inputB);

    /** Gets the acceleration multiplier. */
    inline int16_t GetAccelerationMultiplier (int32_t currentTime);
    
    /** Gets non-zero if there is change in the rotary encoder state.
        @return Gets 0 if there is not change, l if the knob turned right, 
            and -1 if the knob turned left. */
    inline void Poll (Controller* controller, uint channel, byte* debouncedXOR, int currentTime);
    
    private:
    
    uint channel,           //< The mixer channel.
        inA,                //< Encoder pin A offset.
        inB;                //< Encoder pin B offset.
    
    int lastMoveTime;       //< The last time the encoder moved.
    
    byte curveNumber;       //< The acceleration curve number.
};
}       //< namespace tek
}       //< namespace kabuki
#endif  //< KABUKI_TEK_SENSORS_ROTARYENCODER_H

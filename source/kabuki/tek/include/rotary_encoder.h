/** Kabuki Tek
    @file    /.../kabuki-tek/sensors/RotaryEncoder.h
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
 
#ifndef KABUKI_TEK_SENSORS_ROTARYENCODER_H
#define KABUKI_TEK_SENSORS_ROTARYENCODER_H

#include <tek/config.h>

#include <time.h>

namespace tek { namespace sensors {

class Controller;   //< Mutual dependency.

/** A 2-bit grey-code rotary encoder.
    
*/
class RotaryEncoder
{
    public:
    
    RotaryEncoder  (uint inputA, uint inputB);
    /*< Constructor a rotary encoder with the given input channels. */

    inline int16_t getAccelerationMultiplier  (int32_t currentTime);
    /*< Gets the accelloration multiplier. */
    
    inline void poll  (Controller* controller, uint channel, byte* debouncedXOR, int currentTime);
    /*< Gets non-zero if there is change in the rotary encoder state.
        @return Gets 0 if there is not change, l if the knob turned right, 
            and -1 if the knob turned left. */
    
    private:
    
    uint channel,           //< The mixer channel.
        inA,                //< Encoder pin A offset.
        inB;                //< Encoder pin B offset.
    
    int lastMoveTime;       //< The last time the encoder moved.
    
    byte curveNumber;       //< The accelloration curve number.
};
}       //< namespace sensors
}       //< namespace tek
#endif  //< KABUKI_TEK_SENSORS_ROTARYENCODER_H

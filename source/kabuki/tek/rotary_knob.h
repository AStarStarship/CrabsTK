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

#include "config.h"

namespace kabuki { namespace tek {

class Controller;   //< Mutual dependency.

/** A 2-bit gray-code rotary encoder.
    
*/
class RotaryKnob {
    public:

    /** Constructor a rotary encoder with the given input channels. */
    RotaryKnob  (ch_t channel, offset_t a, offset_t b);

    /** Gets the acceleration multiplier. */
    inline int16_t GetAccelerationMultiplier (int32_t time);
    
    /** Gets non-zero if there is change in the rotary encoder state.
        @return Gets 0 if there is not change, l if the knob turned right, 
            and -1 if the knob turned left. */
    inline void Poll (Controller* controller, offset_t channel,
    		          byte* debounced_xor, int microseconds);

    /** Script Operations. */
    const _::Operation* Star (char_t index, _::Expression* expr);
    
    private:

    ch_t     channel_;        //< Mixer channel.
    offset_t in_a_,           //< Encoder pin A offset.
             in_b_;           //< Encoder pin B offset.
    byte     curve_number_;   //< Acceleration curve number.
    int      last_move_time_; //< Last time the encoder moved.
};

class RotaryEncoderOp : public _::Operand {
    public:

    /** Constructs a RotaryEncoder Operation. */
	RotaryEncoderOp (RotaryKnob* object);

    /** Script operations. */
    virtual const _::Operation* Star (char_t index, _::Expression* expr);

    private:

    RotaryKnob* object_;  //< The RotaryEncoderBank.
};
}       //< namespace tek
}       //< namespace kabuki
#endif  //< KABUKI_TEK_SENSORS_ROTARYENCODER_H

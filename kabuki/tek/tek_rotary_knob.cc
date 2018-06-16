/** kabuki::tek
    @file    ~/source/kabuki/tek/impl/tek_rotary_encoder.cc
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

#include "rotary_knob.h"

using namespace _ {;

namespace kabuki { namespace tek {

RotaryKnob::RotaryKnob (ch_t channel, offset_t a, offset_t b) :
    channel_ (channel),
    in_a_    (a),
    in_b_    (b) {
    //Assert  (32 <  (inputB -  (inputA & kBitNumberMask)),
    //    "\n| Error: RotaryEncoder::RotaryEncoder (offset_t, offset_t):
    //     Encoders are not word aligned!");
}

int16_t RotaryKnob::GetAccelerationMultiplier (int32_t time) {
    /*
    static const byte curves[3][_TekHMIHMI_NumAccellorationCurves] = {
    {

    int timeDelta = currentTime - lastMoveTime,
    lastMoveTime = currentTime;
    */
    return 0;
}

void RotaryKnob::Poll (Controller* controller, offset_t channel,
                          byte* debounced_xor, int microseconds) {
    offset_t input_a = in_a_,
             input_b = in_b_;

    byte mask_a = debounced_xor[input_a >> kBitToByteShift] &
                  (1 << (input_a & kBitNumberMask)),
         mask_b = debounced_xor[input_b >> kBitToByteShift] &
                  (1 << (input_b & kBitNumberMask));

    if ((mask_a & mask_b) == 0) return; //< No change in state.

    //int16_t value =  (maskA ? -1 : 1) *
    //                 GetAccelerationMultiplier (microseconds);


}

const Operation* RotaryKnob::Star (char_t index, Expr* expr) {
    static const Operation This = { "RotaryEncoder",
        NumOperations (0), FirstOperation ('a'),
        "tek::sensors", 0 };

        switch (index) {
            case '?': return &This;
        }

    return nullptr;
}

RotaryEncoderOp::RotaryEncoderOp (RotaryKnob* object) :
    object_ (object) {
    // Nothing to do here!
}

const Operation* RotaryEncoderOp::Star (char_t index, Expr* expr) {
    return object_->Star (index, expr);
}

}       //< namespace tek
}       //< namespace kabuki

/** kabuki::tek
    @file    ~/source/kabuki/tek/impl/tek_photosensor.cc
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

#include "photosensor.h"

using namespace _;

namespace kabuki { namespace tek {

Photosensor::Photosensor  (PinName pin) :
        input_ (pin) {
    /// Nothing to do here!
}

float Photosensor::Read  () {
    return input_.read ();
}

const Operation* Photosensor::Star (char_t index, Expression* expr) {
    static const Operation This = { "Photosensor",
        NumOperations (0), FirstOperation ('A'),
        "A standard photosensor hooked up to an ADC pin of a microcontroller.",
        0
    };

    void* args[1];

    switch  (index) {
        case '?': return &This;
        case 'A': {
            static const Operation OpA = { "Read",
                Params<0> (), Params<1, UI2> (),
                "Reads the light sensor.", 0
            };
            if (!expr) return &OpA;

            uint16_t value = input_.read_u16 ();

            return Result (expr, Params<1, UI2> (), Args (args, &value));
        }

    }
    return nullptr;
}

PhotosensorOp::PhotosensorOp (Photosensor* photosensor) :
    photosensor_ (photosensor) {
    // Nothing to do here :-)
}

const Operation* PhotosensorOp::Star (char_t index, Expression* expr) {
    return photosensor_->Star (index, expr);
}

}       //< namespace tek
}       //< namespace kabuki

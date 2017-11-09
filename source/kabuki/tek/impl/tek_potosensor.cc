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

#include "../include/photosensor.h"

namespace kabuki { namespace tek {

Photosensor::Photosensor  (word pin) :
    input (pin)
{
    /// Nothing to do here!
}

Photosensor::~Photosensor () {}

float Photosensor::Read  ()
{
    return 0.0f; // @todo Fix me!
}

const _::Operation* Photosensor::Star (char_t index, _::Expression* expr)
{
    static const _::Operation* this_op = {
        "Photosensor",
        _::NumOperations (0), _::FirstOperation ('A'),
        "A standard photosensor hooked up to an ADC pin of a microcontroller."), 0
    };

    void* args[1];

    switch  (index) {
        case '?': return &this_op;
        case 'A': {
            static const Operation* op_A = {
                "Read",
                NumParmeters (0),
                FirstOperation ('A'),
                "Reads the light sensor.", 0
            };
            if (!expr) return &op_A;

            float reading = 0.0f;

            return _::Result (expr, _::Params<1, _::FLT> (), _::Args (args, &reading));
        }

    }
    return nullptr;
}

}       //< namespace tek
}       //< namespace kabuki

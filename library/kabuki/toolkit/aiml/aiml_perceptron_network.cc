/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/kabuki/toolkit/aiml/aiml_perceptron_layer.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#include <stdafx.h>

#if MAJOR_SEAM >= 5 && MINOR_SEAM >= 1

#include "mlp_network.h"

namespace kabuki { namespace toolkit { namespace aiml {

PerceptronNetwork::PerceptronNetwork (uint32_t neuron_count, float_t bias) :
    bias_ (bias),
    next_layer_ (nullptr) {
    perceptrons_.reserve (neuron_count);
}

void PerceptronNetwork::Update () {
    int n = perceptrons_.size ();
    if (n == 0)
        return;
    for (--n; n > 0; --n) {
        Perceptron* perceptron = &perceptrons_[0];
        perceptron->Update ();
        ++perceptron;
    }
    PerceptronLayer* next_layer = next_layer_;
    if (!next_layer)
        return;
    next_layer->Update ();
}

}   //< namespace aiml
}   //< namespace toolkit
}   //< namespace kabuki
#endif  //< #if MAJOR_SEAM >= 5 && MINOR_SEAM >= 1

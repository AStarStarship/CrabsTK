/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/kabuki/toolkit/aiml/aiml_perceptron.cc
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

#pragma once
#include <stdafx.h>

#if MAJOR_SEAM >= 5 && MINOR_SEAM >= 1

#include "perceptron.h"

namespace kabuki { namespace toolkit { namespace aiml {

float_t kSynapseMax  = 1.0;
float_t kSynapseMin  = -1.0;
float_t kSynapseBias  = 0.0;
float_t kSynapseNoiseMin = -0.05;
float_t kSynapseNoiseMax = 0.05;

float_t SynapseNoise (float_t min_value, float_t max_value){
    static std::default_random_engine e;
    static std::uniform_real_distribution<float_t> dis (min_value, max_value);
    return dis (e);
}

Perceptron::Perceptron () :
    y_ (kSynapseMin) {
    // Nothing to do here! ({:->)-+=<
}

void Perceptron::Connect (Perceptron* synapse) {
    x_.push_back (synapse);
}

void Perceptron::Disconnect (Perceptron* synapse) {
    for (int i = x_.size () - 1; i > 0; --i) {
        if (&x_[i] == synapse) {
            x_.erase (x_.begin () + i);
        }
    }
}

float_t Perceptron::GetValue () {
    return value_;
}

void Perceptron::SetValue (float_t value) {
    value_ = value;
}

void Perceptron::Update () {
    int n = x_.size ();
    if (n == 0) {
        value_ = 0.0;
    }
    float_t value = 0.0;
    for (--n; n > 0; --n) {
        Synapse* synapse = &axion[0];
        value += synapse->weight * synapse->rx->GetValue ();
        ++synapse;
    }
}

}   //< namespace aiml
}   //< namespace toolkit
}   //< namespace kabuki
#endif  //< #if MAJOR_SEAM >= 5 && MINOR_SEAM >= 1

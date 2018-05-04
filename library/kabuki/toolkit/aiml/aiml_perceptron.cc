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

Perceptron::Perceptron () :
    axon_ (kSynapseMin) {
    // Nothing to do here! ({:->)-+=<
}
/*
void Perceptron::Connect (Perceptron* p) {
    assert (p);
    int n = axon_.size ();
    if (n == 0) {
        return;
    }
    Synapse* s = axon_[0];
    for (--n; n > 0; --n) {
        value += s->GetWeight () * s->GetRx ()->GetValue ();
        ++s;
    }
}*/

float_t Perceptron::GetValue () {
    return value_;
}

void Perceptron::SetValue (float_t value) {
    value_ = value;
}

void Perceptron::Update () {
    int n = axon_.size ();
    if (n == 0) {
        value_ = 0.0;
        return;
    }
    float_t value = 0.0;
    Synapse* s = axon_[0];
    for (--n; n > 0; --n) {
        value += s->GetWeight () * s->GetRx ()->GetValue ();
        ++s;
    }
}

}   //< namespace aiml
}   //< namespace toolkit
}   //< namespace kabuki
#endif  //< #if MAJOR_SEAM >= 5 && MINOR_SEAM >= 1

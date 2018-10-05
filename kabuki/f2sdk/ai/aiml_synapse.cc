/** Kabuki Toolkit@version 0.x
@file    $kabuki-toolkit/kabuki/f2sdk/aiml/aiml_synapse.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>

#if MAJOR_SEAM >= 5 && MINOR_SEAM >= 1

#include "synapse.h"

namespace _ {

const float_t kSynapseMax      =  1.0f;
const float_t kSynapseMin      = -1.0f;
const float_t kSynapseBias     =  0.0f;
const float_t kSynapseNoiseMin = -0.05f;
const float_t kSynapseNoiseMax =  0.05f;

float_t SynapseNoise (float_t min_value, float_t max_value) {
    static std::default_random_engine e;
    static std::uniform_real_distribution<float_t> dis (min_value, max_value);
    return dis (e);
}

Synapse::Synapse (Perceptron* rx) :
    rx_ (rx),
    weight_ (SynapseNoise ()) {
    // Nothing to do here! :-)
}

Synapse::Synapse (Perceptron* rx, float_t weight) :
    rx_     (rx),
    weight_ (weight) {
    // Nothing to do here! :-)
}

Synapse::Synapse (Perceptron* rx, float_t min_value, float_t max_value) :
    rx_     (rx),
    weight_ (SynapseNoise (min_value, max_value)) {
    // Nothing to do here! :-)
}

Synapse::Synapse (const Synapse& other) :
    weight_ (other.weight_),
    rx_     (other.rx_) {
    // Nothing to do here! :-)
}

float_t Synapse::GetWeight () { return weight_; }

void Synapse::SetWeight (float_t weight) { weight_ = weight; }

Perceptron* Synapse::GetRx () { return rx_; }

void Synapse::SetRx (Perceptron* rx) {
    assert (rx);
    rx_ = rx;
}

}       //< namespace _
#endif  //< #if MAJOR_SEAM >= 5 && MINOR_SEAM >= 1

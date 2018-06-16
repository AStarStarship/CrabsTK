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

#include "perceptron_layer.h"

namespace _ {

PerceptronLayer::PerceptronLayer (size_t neuron_count, float_t bias) :
    bias_ (bias) {
    perceptrons_.reserve (neuron_count);
}

float_t PerceptronLayer::GetBias () { return bias_; }

void PerceptronLayer::SetBias (float_t value) { bias_ = value; }

void PerceptronLayer::SetRx (Perceptron* p) {
    
}

void PerceptronLayer::SetRx (PerceptronLayer* layer) {
    assert (layer);

    for (auto node : nodes_) {
        SetRx (&node);
    }
}

void PerceptronLayer::AddNode (size_t perceptron_count, float_t bias) {
    if (!perceptron_count)
        return;
    for (; perceptron_count > 0; --perceptron_count)
        nodes_.push_back (PerceptronLayer (perceptron_count, bias));
}

Perceptron* PerceptronLayer::GetPerceptron (size_t index) {
    if (index >= perceptrons_.size ())
        return nullptr;
    return &perceptrons_[index];
}

PerceptronLayer* PerceptronLayer::GetNode (size_t index) {
    if (index >= perceptrons_.size ())
        return nullptr;
    return &nodes_[index];
}

size_t PerceptronLayer::GetPerceptronCount () { return perceptrons_.size (); }

size_t PerceptronLayer::GetChildNodeCount () { return nodes_.size (); }

void PerceptronLayer::Update () {
    for (auto perceptron : perceptrons_)
        perceptron.Update ();

    for (auto node: nodes_)
        node.Update ();
}

}       //< namespace _
#endif  //< #if MAJOR_SEAM >= 5 && MINOR_SEAM >= 1

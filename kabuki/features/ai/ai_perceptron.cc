/* Kabuki Toolkit@version 0.x
@file    /kabuki/f2sdk/ai/aiml_perceptron.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-19 Cale McCollough <cale@astartup.net>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>

#if MAJOR_SEAM >= 5 && MINOR_SEAM >= 1

#include "perceptron.h"

namespace _ {

Perceptron::Perceptron () :
    x_ (nullptr) {
    // Nothing to do here! ({:->)-+=<
}

float_t& Perceptron::GetY () {
    return y_;
}

void Perceptron::SetY (float_t& y) {
    y_ = y;
}

float_t* Perceptron::GetX () {
    return x_;
}

float_t* Perceptron::GetWeights () {
    return &w_[0];
}

size_t Perceptron::GetAxionCount () {
    w_.size ();
}

void Perceptron::Update () {
    size_t n = w_.size ();
    DASSERT (n);

    float_t sum = 0.0;
    float_t* x = x_,
           * w = &w_[0];
    for (--n; n > 0; --n)
        sum += *x++ * *w++;
    y_ = sum;
}

}       // namespace _
#endif  //< #if MAJOR_SEAM >= 5 && MINOR_SEAM >= 1

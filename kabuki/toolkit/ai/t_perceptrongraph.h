/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/ai/t_perceptrongraph.h
@author  Cale McCollough <calemccollough.github.io>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_TOOLKIT_AI_1
#ifndef KABUKI_TOOLKIT_AI_PERCEPTRONGRAPH_T
#define KABUKI_TOOLKIT_AI_PERCEPTRONGRAPH_T 1

#include "t_perceptronlayer.h"

namespace _ {

/* A graph of Perceptrons with an input an output layer.
PercetronNode(s) store a TArray of TPerceptron(s) and a TArray of
child TPerceptronGraph nodes. In order to connect the inputs to the outputs
you have to traverse the tree
*/
template<typename Value>
class TPerceptronGraph {
public:

  /* Contructs an empty network.
  @param input_count  Number of input perceptrons.
  @param output_count Number of output perceptrons.
  @param bias         The network bias. */
  TPerceptronGraph (size_t input_count, Value input_bias,
    size_t output_cout, Value output_bias)
    : x_ (input_count, input_bias),
      y_ (output_count, output_bias),
      layer_1_ () {
      x_.reserve (input_count);
  }

  /* Gets the output node. */
  TArray<Value>& X () { return x_; }

  /* Gets the output node. */
  Value* X (size_t index) {
    if (index >= x_.size ())
      return nullptr;
    return x_[index];
  }

  /* Gets the output node. */
  TArray<Value>& Y () { return y_; }

  /* Updates this network. */
  void Update () {
    layer_1_.Update ();
  }

private:
  TArray<Value>  x_;
  TArray<Value>& y_;       //< Output layer
  PerceptronLayer       layer_1_; //< Output perceptron layer.
};

} //< namespace _
#endif
#endif

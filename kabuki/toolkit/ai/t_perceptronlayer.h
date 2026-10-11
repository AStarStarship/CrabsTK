/* Kabuki Toolkit
    @version 0.x
    @file    ~/library/kabuki/toolkit/ai/perceptron_layer.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-19-2018 Cale McCollough <calemccollough@gmail.com>;
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
#include <pch.h>

#if SEAM >= KABUKI_TOOLKIT_AI_1
#ifndef KABUKI_TOOLKIT_AI_PERCEPTRONLAYER_T
#define KABUKI_TOOLKIT_AI_PERCEPTRONLAYER_T 1

#include "t_perceptron.h"

namespace _ {

/* A layer in a Multi-layer TPerceptron Network.
Each TPerceptron is individually controlled, but it is convenient to

# Operational Steps

1.)
*/
template<typename Value>
class TPerceptronLayer {
public:

  /* A group of one or more perceptrons with */
  PerceptronLayer (size_t neuron_count, Value bias)
    : bias_ (bias) {
    perceptrons_.reserve (neuron_count);
  }

  /* Gets the node bias. */
  Value GetBias () { return bias_; }

  /* Gets the node bias. */
  void SetBias (Value value) { bias_ = value; }

  /* Fully connects the perceptron to this layer. */
  void SetRx (TPerceptron* p) {}

  /* Fully connects the given layer. */
  void SetRx (PerceptronLayer* node) {
    DASSERT (layer);

    for (auto node : nodes_) {
      SetRx (&node);
    }
  }
  

  /* Fully connects the given layer. */
  void AddNode (size_t perceptron_count, Value bias) {
    if (!perceptron_count)
      return;
    for (; perceptron_count > 0; --perceptron_count)
      nodes_.push_back (PerceptronLayer (perceptron_count, bias));
  }

  /* Gets a perceptron.
      @param  index The index of the perceptron to return.
      @return Nil if the perceptron does not exist. */
  TPerceptron* GetPerceptron (size_t index) {
    if (index >= perceptrons_.size ())
      return nullptr;
    return &perceptrons_[index];
  }

  /* Gets a node.
  @return Nil if the perceptron does not exist. 
  @param  index The index of the node to return. */
  PerceptronLayer* Node (size_t index) {
    if (index >= perceptrons_.size ())
      return nullptr;
    return &nodes_[index];
  }

  /* Gets the perceptron count. */
  size_t Count () { return perceptrons_.Count (); }

  /* Gets the perceptron count. */
  size_t ChildCount () { return nodes_.Count (); }

  /* Updates the layer. */
  void Update () {
    for (auto perceptron : perceptrons_)
      perceptron.Update ();

    for (auto node : nodes_)
      node.Update ();
  }

private:

  Value                bias_;        //< Layer bias value.
  TArray<Value>        x_;           //< Input x TArray.
  TArray<TPerceptron>     perceptrons_; //< Layer perceptrons.
  TArray<PerceptronLayer> nodes_;      //< Vector of next layers in the list.
};

}       //< namespace _
#endif
#endif

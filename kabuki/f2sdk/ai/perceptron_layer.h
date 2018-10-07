/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/kabuki/toolkit/ai/perceptron_layer.h
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
#include <pch.h>

#if MAJOR_SEAM >= 5 && MINOR_SEAM >= 1

#ifndef HEADER_FOR_PERCEPTRON_LAYER
#define HEADER_FOR_PERCEPTRON_LAYER

#include "perceptron.h"
#include "asyncronous.h"

namespace _ {

/** A layer in a Multi-layer Perceptron Network.
    Each Perceptron is individually controlled, but it is convenient to 

    # Operational Steps

    1.) 
*/
class PerceptronLayer {
    public:

    /** A group of one or more perceptrons with */
    PerceptronLayer (size_t neuron_count, float_t bias);

    /** Gets the node bias. */
    float_t GetBias ();

    /** Gets the node bias. */
    void SetBias (float_t value);

    /** Fully connects the perceptron to this layer. */
    void SetRx (Perceptron* p);

    /** Fully connects the given layer. */
    void SetRx (PerceptronLayer* node);

    /** Fully connects the given layer. */
    void AddNode (size_t perceptron_count, float_t bias);

    /** Gets a perceptron.
        @param  index The index of the perceptron to return.
        @return Nil if the perceptron does not exist. */
    Perceptron* GetPerceptron (size_t index);

    /** Gets a node.
        @param  index The index of the node to return.
        @return Nil if the perceptron does not exist. */
    PerceptronLayer* GetNode (size_t index);

    /** Gets the perceptron count. */
    size_t GetPerceptronCount ();

    /** Gets the perceptron count. */
    size_t GetChildNodeCount ();

    /** Updates the layer. */
    void Update ();

    private:

    float_t                     bias_;        //< Layer bias value.
    std::vector<float_t>        x_;           //< Input x vector.
    std::vector<Perceptron>     perceptrons_; //< Layer perceptrons.
    std::vector<PerceptronLayer> nodes_;      //< Vector of next layers in the list.
};

}       //< namespace _
#endif  //< #ifndef HEADER_FOR_PERCEPTRON_LAYER
#endif  //< #if MAJOR_SEAM >= 5 && MINOR_SEAM >= 1

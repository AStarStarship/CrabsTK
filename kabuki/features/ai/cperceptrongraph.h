/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/kabuki/toolkit/ai/perceptron.h
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
#if MAJOR_SEAM >= 5 && MINOR_SEAM >= 1
#include "perceptron_layer.h"

#include "asyncronous.h"

namespace _ {

/** A graph of Perceptrons with an input an output layer.
    PercetronNode(s) store a TArray of Perceptron(s) and a TArray of 
    child PerceptronGraph nodes. In order to connect the inputs to the outputs 
    you have to traverse the tree
*/
class PerceptronGraph {
    public:

    /** Contructs an empty network.
        @param input_count  Number of input perceptrons.
        @param output_count Number of output perceptrons.
        @param bias         The network bias. */
    PerceptronGraph  (size_t input_count, float_t input_bias,
                      size_t output_cout, float_t output_bias);

    /** Gets the output node. */
    TArray<float_t>& GetX ();

    /** Gets the output node. */
    float_t* GetX (size_t index);

    /** Gets the output node. */
    TArray<float_t>& GetY ();

    /** Updates this network. */
    void Update ();

    private:
    TArray<float_t>  x_;
    TArray<float_t>& y_;       //< Output layer
    PerceptronLayer       layer_1_; //< Output perceptron layer.
};

}       //< namespace _
#endif  //< #if MAJOR_SEAM >= 5 && MINOR_SEAM >= 1

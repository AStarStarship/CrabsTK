w/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/kabuki/toolkit/ai/perceptron.h
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

#ifndef HEADER_FOR_PERCEPTRON
#define HEADER_FOR_PERCEPTRON

#include "synapse.h"

namespace _ {

/** A Perceptron.

    A Perceptron needs to get it's input from either floating-point or 
    integers.

    @code
    x_1 ---\        ______     ______
        w_1 \       | ____ |     |   __ |
    x_2 -----\      | \     |     |   |   |
        w_2   |-----|  \    |---->|   |   |----> Y Output
    ...      /      |  /    |  h  |   |   |
    x_n ----/       | /___ |     | __|0  |
        w_n         |_____|     |_____|
    @endcode
*/
class Perceptron {
    public:

    /** Constructs a unconnected Perceptron. */
    Perceptron ();

    /** Gets the perceptron y. */
    float_t& GetY ();

    /** Sets the perceptron value. */
    void SetY (float_t& y);

    /** Gets the perceptron x vector. */
    float_t* GetX ();

    /** Gets the perceptron weight vector. */
    float_t* GetWeights ();

    /** Gets the count of the axions. */
    size_t GetAxionCount ();

    /** Updates the Perceptron. */
    void Update ();

    private:

    float_t            & y_; //< Y output address vector.
    float_t            * x_; //< X input values vector.
    std::vector<float_t> w_; //< Weight input values vector.
};

}       //< namespace _
#endif  //< #ifndef HEADER_FOR_PERCEPTRON
#endif  //< #if MAJOR_SEAM >= 5 && MINOR_SEAM >= 1

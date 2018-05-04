/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/kabuki/toolkit/aiml/perceptron.h
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

#ifndef HEADER_FOR_PERCEPTRON
#define HEADER_FOR_PERCEPTRON

#include "synapse.h"

namespace kabuki { namespace toolkit { namespace aiml {

/** A Perceptron.

    A Perceptron needs to get it's input from either floating-point or 
    integers.

    @code
    x_1 ---\        _________     _________
        w_1 \       | _____ |     |   ___ |
    x_2 -----\      | \     |     |   |   |
        w_2   |-----|  \    |---->|   |   |----> Output Y
    ...      /      |  /    |  h  |   |   |
    x_n ----/       | /____ |     | __|0  |
        w_n         |_______|     |_______|
    @endcode
*/
class Perceptron {

    public:

    /** Default constructor. */
    Perceptron ();

    /** Gets the perceptron value. */
    float_t GetValue ();

    /** Sets the perceptron value. */
    void SetValue (float_t value);

    /** Connects the given perceptron. */
    void Connect (Perceptron* p);

    void Update ();

    private:

    float_t               value_; //< Perceptron value.
    std::vector<Synapse*> axon_;  //< Connected neurons.
};

}   //< namespace aiml
}   //< namespace toolkit
}   //< namespace kabuki

#endif  //< #ifndef HEADER_FOR_PERCEPTRON
#endif  //< #if MAJOR_SEAM >= 5 && MINOR_SEAM >= 1

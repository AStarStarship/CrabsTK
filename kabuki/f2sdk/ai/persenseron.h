/** Kabuki Toolkit
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

#include "perceptron.h"

namespace _ {

typedef float_t (*PerceptronSensor) ();

#ifndef HEADER_FOR_SENSORY_PERCEPTRON
#define HEADER_FOR_SENSORY_PERCEPTRON

/** A Sensory Perceptron that takes in data from various types and outputs.
    
*/
template<typename T>
class Persenseron {
    public:

    /** Constructs a Persenseron from the given Perceptron. */
    Persenseron (Perceptron* y);

    void Update ();

    private:

    Perceptron* y_; //<
};

}       //< namespace _
#endif  //< #ifndef HEADER_FOR_SENSORY_PERCEPTRON
#endif  //< #if MAJOR_SEAM >= 5 && MINOR_SEAM >= 1

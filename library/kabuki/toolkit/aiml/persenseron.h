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

#include "perceptron.h"

namespace kabuki { namespace toolkit { namespace aiml {

typedef float_t (*PerceptronSensor) ();

#ifndef HEADER_FOR_SENSORY_PERCEPTRON
#define HEADER_FOR_SENSORY_PERCEPTRON

/** A Sensory Perceptron that takes in data from various types and ouptuts.
    
*/
template<typename T>
class Persenseron {
    public:

    Persenseron ();

    void Update ();

    private:

    Perceptron* y_;
};



class Persenseron2D {
    public:

    Persenseron2D (uintptr_t width, uintptr_t height);

    uintptr_t GetWidth ();

    uintptr_t GetHeight ();

    Persensron* GetPersenseron (uint x, uint y);

    private:

    uint       width_,
               height_;
    Axion_UI1* axon_,
             * y_;
};

}       //< namespace aiml
}       //< namespace toolkit
}       //< namespace kabuki
#endif  //< #ifndef HEADER_FOR_SENSORY_PERCEPTRON
#endif  //< #if MAJOR_SEAM >= 5 && MINOR_SEAM >= 1

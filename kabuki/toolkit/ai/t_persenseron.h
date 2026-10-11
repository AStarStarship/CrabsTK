/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features//ai/perceptron.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2018 Cale McCollough <calemccollough.github.io>;
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

#include "perceptron.h"

namespace _ {

typedef Value (*PerceptronSensor) ();

#ifndef SENSORY_PERCEPTRON
#define SENSORY_PERCEPTRON

/* A Sensory TPerceptron that takes in data from various types and outputs.
    
*/
template<typename T>
class Persenseron {
    public:

    /* Constructs a Persenseron from the given TPerceptron. */
    Persenseron (TPerceptron* y);

    void Update ();

    private:

    TPerceptron* y_; //<
};

}       //< namespace _
#endif  //< #ifndef SENSORY_PERCEPTRON
#endif  //< #if SEAM >= KABUKI_TOOLKIT_AI_1

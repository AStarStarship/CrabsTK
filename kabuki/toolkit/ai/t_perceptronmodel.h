/* Kabuki Toolkit
    @version 0.x
    @file    ~/library/kabuki/toolkit/ai/perceptron_model.h
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
#include "tperceptrongraph.h"
#ifndef KABUKI_TOOLKIT_AIML_PERCEPTRONMODEL


namespace _ {

/* A 2D grid of TPerceptron(s) created from a PerceptronLayer. */
template<typename Value>
class PerceptronModel : public TPerceptronGraph<Value> {
    public:

    PerceptronModel (SI4 input_count, SI4 output_count);

    private:

    TArray<Value> y_;
};

}       //< namespace _
#endif  //< #ifndef KABUKI_TOOLKIT_AIML_PERCEPTRONGRAPH2D
#endif  //< #if SEAM >= KABUKI_TOOLKIT_AI_1

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
#ifndef HEADER_FOR_KABUKI_TOOLKIT_AIML_PERCEPTRONGRAPH2D
#define HEADER_FOR_KABUKI_TOOLKIT_AIML_PERCEPTRONGRAPH2D

#include "perceptron_layer.h"

namespace _ {

/** A 2D grid of Perceptron(s) created from a PerceptronLayer. */
class PerceptronGraph2D : public PerceptronGraph {
    public:

    /** A 2D grid of perceptrons mapped to a PerceptronLayer in the C array 
        format. */
    PerceptronGraph2D (size_t width, size_t height, float_t bias);

    /** Gets the width in pixels. */
    size_t GetWidth ();

    /** Gets the height in pixels. */
    size_t GetHeight ();

    /** Gets the Perceptron at the given (X, Y) coordinate. */
    Perceptron* GetPixel (size_t x, size_t y);

    private:

    size_t   width_,
             height_;
};

}       //< namespace _
#endif  //< #ifndef HEADER_FOR_KABUKI_TOOLKIT_AIML_PERCEPTRONGRAPH2D
#endif  //< #if MAJOR_SEAM >= 5 && MINOR_SEAM >= 1

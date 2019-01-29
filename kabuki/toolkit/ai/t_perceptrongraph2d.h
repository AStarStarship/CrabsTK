/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/ai/t_perceptrongraph2d.h
@author  Cale McCollough <calemccollough.github.io>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
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
#ifndef KABUKI_TOOLKIT_AI_PERCEPTRONGRAPH_T
#define KABUKI_TOOLKIT_AI_PERCEPTRONGRAPH_T 1

#include "t_perceptronlayer.h"

namespace _ {

/* A 2D grid of TPerceptron(s) created from a PerceptronLayer. */
template<typename Value>
class TPerceptronGraph2D : public TPerceptronGraph {
    public:

    /* A 2D grid of perceptrons mapped to a PerceptronLayer in the C array 
        format. */
    TPerceptronGraph2D (size_t width, size_t height, float_t bias)
      : width_ (width),
        height_ (height),
        TPerceptronLayer (width * height, 0) {}

    /* Gets the width in pixels. */
    size_t Width () { return width_; }

    /* Gets the height in pixels. */
    size_t Height () { return height_; }

    /* Gets the TPerceptron at the given (X, Y) coordinate. */
    TPerceptron* GetPixel (size_t x, size_t y) {
      return TPerceptronGraph::X (y * width_ + x);
    }

    private:

    size_t   width_,
             height_;
};

} //< namespace _
#endif
#endif

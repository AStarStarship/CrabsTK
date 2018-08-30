/** Kabuki Toolkit@version 0.x
@file    $kabuki-toolkit/kabuki/f2sdk/aiml/aiml_perceptron_2d.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <stdafx.h>

#if MAJOR_SEAM >= 5 && MINOR_SEAM >= 1

#include "perceptron_graph_2d.h"

namespace _ {

PerceptronGraph2D::PerceptronGraph2D (size_t width, size_t height,
                                    float_t bias) :
    width_  (width),
    height_ (height),
    PerceptronLayer (width * height, 0) {

}

size_t PerceptronGraph2D::GetWidth () { return width_;  }

size_t PerceptronGraph2D::GetHeight () { return height_; }

float_t* PerceptronGraph2D::GetPixel (size_t x, size_t y) {
    return PerceptronGraph::GetX (y * width_ + x);
}

}       //< namespace _
#endif  //< #if MAJOR_SEAM >= 5 && MINOR_SEAM >= 1

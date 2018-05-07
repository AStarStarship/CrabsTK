/** Kabuki Toolkit
    @file ~/kt/math/bezier2d.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#include "config.h"

namespace kabuki { namespace toolkit { namespace math {

template<typename F, typename U>
void Softmax (F* x, U count, F* y) {
    assert (x);
    assert (x_count > 0);
    assert (y);
    F sum_of_e_j_to_the_x = 0.0;
    intptr_t delta_yx = y - x;
    for (F* x_cursor = x; x_cursor <= x + count; ++x_cursor) {
        F e_j_to_the_x = exp (*x_cursor);
        sum_of_e_j_to_the_x += e_j_to_the_x;
        *(x_cursor + delta_yx) = e_j_to_the_x;
    }
    for (F* y_cursor = y; y_cursor <= y + cursor; ++y_cursor) {
        *y_cursor = *y_cursor / sum_of_e_j_to_the_x;
    }
}

}       //< namespace math
}       //< namespace toolkit
}       //< namespace kabuki
#endif  //< #ifndef HEADER_FOR_KABUKI_TOOLKIT_AIML_PERCEPTRONGRAPH2D
#endif  //< #if MAJOR_SEAM >= 5 && MINOR_SEAM >= 1
/* Kabuki Toolkit @version 0.x_
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/math/t_point2d.h
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
#if SEAM >= KABUKI_FEATURES_MATH_1
#ifndef KABUKI_FEATURES_MATH_SOFTMAX_T
#define KABUKI_FEATURES_MATH_SOFTMAX_T 1

namespace _ {

template<typename F, typename U>
void TSoftmax (F* x, U count, F* y) {
    DASSERT (x);
    DASSERT (x_count > 0);
    DASSERT (y);
    F sum_of_e_j_to_the_x = 0.0;
    SIW delta_yx = y - x;
    for (F* x_cursor = x; x_cursor <= x + count; ++x_cursor) {
        F e_j_to_the_x = exp (*x_cursor);
        sum_of_e_j_to_the_x += e_j_to_the_x;
        *(x_cursor + delta_yx) = e_j_to_the_x;
    }
    for (F* y_cursor = y; y_cursor <= y + cursor; ++y_cursor) {
        *y_cursor = *y_cursor / sum_of_e_j_to_the_x;
    }
}

}       //< namespace _
#endif
#endif

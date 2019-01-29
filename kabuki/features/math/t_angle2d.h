/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/math/t_angle2d.h
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
#ifndef KABUKI_FEATURES_MATH_ANGLE_T
#define KABUKI_FEATURES_MATH_ANGLE_T 1

namespace _ {

/* An angle between two lines stored in radians. */
template<typename T>
class TAngle2D
{
    public:

    T rads;        //< The angle stored in radians.

    /* Constructor. */
    Angle2D (T value = 0) {
        
    }

    /* Converts the angle to degrees. */
    T GetDegs () {
        return (rads * 180.0) / pi;
    }

    /* Sets the angle to the new value in degrees. */
    void SetDegs (T angle) {
        rads = angle * Pi<T> ();
    }

    /* Prints this object to the terminal. */
    inline void Print (Expression& io) {
    }
};

#if WORD_SIZE == 32
using Angle = Angle<FLT>;
#elif WORD_SIZE == 64
using Angle = Angle<double>;
#endif

}       //< namespace _
#endif
#endif

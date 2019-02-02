/* Kabuki Toolkit @version 0.x_
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/math/t_polar2d.h
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
#ifndef KABUKI_FEATURES_MATH_POLAR2D_T
#define KABUKI_FEATURES_MATH_POLAR2D_T 1

namespace _ {

/* A point in a polar coordinate system. */
template<typename Float>
class TPolar2D
{
    public:

    Float angle,  //< The angle in radians.
          radius; //< The radius in abstract units.

    /* Constructor initializes with given values
        @param  thisAngle   The angle of the coordinate.
        @param  thisNumber  The Magnitude of the coordinate.
    */
    TPolar2D (FP4 angle, FP4 radius) :
        angle (angle),
        radius (radius) {
    }

    /* Calculates the Magnitude of the . */
    T Magnitude () {
        T angle = angle,
            radius = radius;
        return Sqrt (angle * angle + radius * radius);
    }

    /* Prints this object to the terminal. */
    Printer& Print (Printer& printer) {

    }
};

}       //< namespace _
#endif
#endif


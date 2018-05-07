/** Kabuki Toolkit
    @file    ~/kt/math/polar2d.cc
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

#include "config.h"

namespace math {

/** A point in a polar coordinate system. */
template<typename Float>
class Polar2D
{
    public:

    Float angle,  //< The angle in radians.
          radius; //< The radius in abstract units.

    /** Constructor initializes with given values
        @param  thisAngle   The angle of the coordinate.
        @param  thisNumber  The magnitude of the coordinate.
    */
    Polar2D (float angle, float radius) :
        angle (angle),
        radius (radius) {
    }

    /** Calculates the magnitude of the . */
    T Magnitude () {
        T angle = angle,
            radius = radius;
        return Sqrt (angle * angle + radius * radius);
    }

    /** Prints this object to the terminal. */
    _::Printer& Print (_::Printer& printer) {

    }
};

}   //  math

/** Kabuki Toolkit
    @file    ~/kt/math/point3d.h
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

/** A 3D point.
*/
template <typename T>
class Point3D
{
    public:
    
    T x,
      y,
      z;

    /** Translated the z position by the given delta. */
    Point3D<T> (T initX, T initY, T initZ) :
        x (initX),
        y (initY),
        z (initZ) {
    }

    /** Translated the x position by the given delta. */
    void TranslateX (T deltaX) {
        x += dx;
    }

    /** Translated the y position by the given delta. */
    void TranslateY (T deltaY) {
        y += dy;
    }

    /** Translated the z position by the given delta. */
    void TranslateZ (T deltaZ) {
        z += dz;
    }

    /** Prints this object to a Expression. */
    _::Printer& Print (_::Printer& printer) {
        return print << "Point3D {x, y, z) = { " << x << ", " << y << ", " 
                     << z << " }";
    }
};

}   //< math

/** Kabuki Theater
    @file    /.../KabukiTheater/_Math/PolarPoint.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#pragma once

#include <FreeI2P.hpp>
#include <KabukiTheater-Config.hpp>

namespace _Math {

/** A point in a polar coordinate system. */
class Polar_f
{
    public:

    float angle,        //< The angle in radians.
        radius;         //< The radius in abstract units.

    Polar_f (float angle, float radius);
    /*< Constructor initializes with given values
        @param  thisAngle   The angle of the coordinate.
        @param  thisNumber  The magnitude of the coordinate.
    */

    float calcMagnitude ();
    /*< Calculates the magnitude of the . */

    inline void print (Terminal& io);
    /*< Prints this object to the terminal. */
};

class Polar_d
/*< A point in a polar coordinate system. */
{
    public:

    double angle,        //< The angle in radians.
        radius;         //< The radius in abstract units.

    Polar_d (double angle, double radius);
    /*< Constructor initializes with given values
        @param  thisAngle   The angle of the coordinate.
        @param  thisNumber  The magnitude of the coordinate.
    */

    double calcMagnitude ();
    /*< Calculates the magnitude. */

    inline void print (Terminal& io);
    /*< Prints this object to the terminal. */
};

}   //  _Math

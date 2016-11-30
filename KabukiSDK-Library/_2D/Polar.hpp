/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_2D/PolarPoint.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright  (C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved  (R).

        Licensed under the Apache License, Version 2.0  (the "License"); you may
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
#include <KabukiSDK-Config.hpp>

namespace _2D {

/** A point in a polar coordinate system. */
struct Polar_f
{
    float Angle,        //< The angle in radians.
        Radius;         //< The radius in abstract units.

    Polar_f  (float angle, float radius);
    /*< Constructor initializes with given values
        @param  thisAngle   The angle of the coordinate.
        @param  thisNumber  The magnitude of the coordinate.
    */

    float GetAngle ();
    /*< Gets the angle. */

    float GetRadius ();
    /*< Gets the radius. */

    void SetAngle (float Value);
    /*< Sets the angle to the new value. */

    void SetRadius (float Value);
    /*< Sets the angle to the new value. */

    float CalcMagnitude ();
    /*< Calculates the magnitude of the . */

    void print (I2P::Terminal& slot);
    /*< Prints this object to the terminal. */
};

/** A point in a polar coordinate system. */
struct Polar_d
{
    double Angle,        //< The angle in radians.
        Radius;         //< The radius in abstract units.

                        /** Constructor initializes with given values
                        @param  thisAngle   The angle of the coordinate.
                        @param  thisNumber  The magnitude of the coordinate.
                        */
    Polar_d  (double angle, double radius);

    double GetAngle ();
    /*< Gets the angle. */

    double GetRadius ();
    /*< Gets the radius. */

    void SetAngle (double Value);
    /*< Sets the angle to the new value. */

    void SetRadius (double Value);
    /*< Sets the angle to the new value. */

    double CalcMagnitude ();
    /*< Calculates the magnitude. */

    void print (I2P::Terminal& slot);
    /*< Prints this object to the terminal. */
};

}   //< namespace _2D

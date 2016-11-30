/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_2D/Angle.hpp
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

class _KabukiSDK Angle_f
/** An angle between two lines stored in radians. */
{
    public:

    float Rads;        //< The angle stored in radians.

    Angle_f  (float Value = 0.0f);
    /*< Simple default constructor. */

    float GetRads ();
    /*< Gets the angle in radians. */

    float SetRads (float Value);
    /*< Sets the angle to the new value in radians. */

    float GetDegs ();
    /*< Converts the angle to degrees. */

    float SetDegs (float Value);
    /*< Sets the angle to the new value in degrees. */

    void print (I2P::Terminal& slot);
    /*< Prints this object to the terminal. */
};

class _KabukiSDK Angle_d
/** An angle between two lines. */
{
    public:

    double Rads;        //< The angle in radians.

    Angle_d  (double a = 0.0);
    /*< Simple default constructor. */

    double GetRads ();
    /*< Gets the angle in radians. */

    double SetRads (double Value);
    /*< Sets the angle to the new value in radians. */

    double GetDegs ();
    /*< Converts the angle to degrees. */

    double SetDegs (double Value);
    /*< Sets the angle to the new value in degrees. */

    void print (I2P::Terminal& slot);
    /*< Prints this object to the terminal. */
};

}   //< namespace _2D

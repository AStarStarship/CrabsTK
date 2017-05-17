/** Kabuki Theater
    @file    /.../KabukiTheater/_Math/Angle2D.hpp
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

#include <KabukiTheater-Config.hpp>

namespace _Math {

class _KabukiTheater Angle2D_f
/** An angle between two lines stored in radians. */
{
    public:

    float rads;        //< The angle stored in radians.

    Angle2D_f (float Value = 0.0f);
    /*< Simple default constructor. */

    float getDegs ();
    /*< Converts the angle to degrees. */

    void setDegs (float Value);
    /*< Sets the angle to the new value in degrees. */

    inline void print (Terminal& io);
    /*< Prints this object to the terminal. */
};

class _KabukiTheater Angle2D_d
/** An angle between two lines. */
{
    public:

    double rads;        //< The angle in radians.

    Angle2D_d (double a = 0.0);
    /*< Simple default constructor. */

    double getDegs ();
    /*< Converts the angle to degrees. */

    void setDegs (double Value);
    /*< Sets the angle to the new value in degrees. */

    inline void print (Terminal& io);
    /*< Prints this object to the terminal. */
};

#if _WordSize == 32
using Angle = Angle_f;
#elif _WordSize == 64
using Angle = Angle_d;
#endif

}   //  _Math

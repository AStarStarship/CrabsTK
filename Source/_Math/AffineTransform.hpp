/** Kabuki Theater
    @file    /.../Source/_Math/AffineTransform.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

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

template<typename Type>
class _KabukiTheater_ AffineTransform
/** A 2D affine transformation using 32-bit floating-point data. */
{
    public:

    AffineTransform ();
    /*< Constructs an unintialized object. */

    void print (Terminal& io);
    /*< Prints this object to the terminal. */
};

template<typename Type>
class _KabukiTheater_ Angle
/** An angle between two lines. */
{
    public:

    Type rads;        //< The angle in radians.

    Angle (Type a = 0.0);
    /*< Simple default constructor. */

    Type getDegs ();
    /*< Converts the angle to degrees. */

    void setDegs (Type Value);
    /*< Sets the angle to the new value in degrees. */

    void print (Terminal& io);
    /*< Prints this object to the terminal. */
};

}   //  _Math

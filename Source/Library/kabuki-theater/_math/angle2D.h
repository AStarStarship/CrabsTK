/** Kabuki Theater
    @file    /.../Source/_Math/Angle2D.hpp
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

#include <kabuki-theater-config.h>

namespace _math {

template<typename Type>
class Angle2D
/** An angle between two lines stored in radians. */
{
    public:

    Type rads;        //< The angle stored in radians.

    /** Constructor. */
    Angle2D (Type value = 0) {
        
    }

    /** Converts the angle to degrees. */
    Type GetDegs () {
        return (rads * 180.0) / pi;
    }

    /** Sets the angle to the new value in degrees. */
    void SetDegs (Type Value) {
        
    }

    /** Prints this object to the terminal. */
    inline void Print (Terminal& io) {
    }
};

#if _WordSize == 32
using Angle = Angle<float>;
#elif _WordSize == 64
using Angle = Angle<double>;
#endif

}   //  _Math

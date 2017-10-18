/** Kabuki Starship
    @file    /.../Source/_Math/Point.h
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

#include <KabukiTheater-Config.h>

namespace _Math {

template <typename Type>
class Point
/*< A 32-bit floating-point 3D point.
*/
{
    public:
    
    Type x,
        y,
        z;
    
    Point<Type> (Type initX, Type initY, Type initZ);
    /*< Translated the z position by the given delta. */

    void TranslateX (Type deltaX);
    /*< Translated the x position by the given delta. */

    void TranslateY (Type deltaY);
    /*< Translated the y position by the given delta. */

    void TranslateZ (Type deltaZ);
    /*< Translated the z position by the given delta. */

    void print (Terminal io);
    /*< Prints this object to a Terminal. */
};

}   //< _Math

/** Kabuki Theater
    @file    /.../Source-Impl/_Math/Point2D2D.cpp
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

#include <_Math/Point2D.hpp>

namespace _Math {
    
Point3D::Point3D (Type initX, Type initY, Type initZ)
:   x (initX),
    y (initY),
    z (initZ)
{
}

void Point3D::translateX (Type dx)
{
    x +=  dx;
}

void Point3D::translateY (Type dy)
{
    y +=  dy;
}

void Point3D::translateZ (Type dz)
{
    z +=  dz;
}

void Point3D_f::print (Terminal& io)
{
    //return new textArray<char> (Integer.ToString ((int)x), ", ", Integer.ToString ((int)y), ", ", Integer.ToString ((int)z));
}

}   //< _Math


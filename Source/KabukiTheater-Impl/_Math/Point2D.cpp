/** Kabuki Theater
    @file    /.../Source-Impl/_Math/Point2D.cpp
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


Point2D::Point2D (Type x, Type y)
:   x (x), 
    y (y) 
{
    /// Nothing to do here!
}

void Point2D::set (Type x = 0, Type y = 0)
{
    x = x;
    y = y;
}

void Point2D::set (const Point2D& P)
{
    x = P.x;
    y = P.y;
}

void Point2D::translate (const Vect_f& V)
{
    y += V.y;
    x += V.x;
}

void Point2D::translate (Type dx, Type dy)
{
    y += dy;
    x += dx;
}

bool Point2D::equals (const Point2D& P) const
{
    if (x != P.x || y != P.y)
        return false;
    return true;
}

double Point2D::compare (const Point2D& P) const
{
    if (x >= P.x)
        if (y >= P.y)
            return 1;
        else if (y < P.y)
            return 4;
    if (x < P.x)
        if (y > P.y)
            return 2;
        else if (y < P.y)
            return 3;
    return 0;   //< There are equal.
}

void Point2D::swap (Point2D P)
{
    Type tempX = P.x,
        tempY = P.y;

    P.x = x;
    P.y = y;

    x = tempX;
    y = tempY;
}

void Point2D::print (Terminal& io)
{

}

}   //  _Math

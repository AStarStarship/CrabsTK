/** Kabuki Starship
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

template<typename T>
Point2D::Point2D (T x, T y)
:   x (x), 
    y (y) 
{
    /// Nothing to do here!
}

template<typename T>
void Point2D::set (T x = 0, T y = 0)
{
    x = x;
    y = y;
}

template<typename T>
void Point2D::set (const Point2D& P)
{
    x = P.x;
    y = P.y;
}

template<typename T>
void Point2D::translate (const Vect_f& V)
{
    y += V.y;
    x += V.x;
}

template<typename T>
void Point2D::translate (T dx, T dy)
{
    y += dy;
    x += dx;
}

template<typename T>
bool Point2D::equals (const Point2D& P) const
{
    if (x != P.x || y != P.y)
        return false;
    return true;
}

template<typename T>
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

template<typename T>
void Point2D::swap (Point2D P)
{
    T tempX = P.x,
        tempY = P.y;

    P.x = x;
    P.y = y;

    x = tempX;
    y = tempY;
}

template<typename T>
void Point2D::print ()
{
    cout << "Point2D: x=" << x << ", y=" << y << '\n';
}

}   //  _Math

/** Kabuki Theater
    @file    /.../KabukiTheater-Impl/_Math/Point2D.cpp
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

#include <_Math/Point2D.hpp>

namespace _Math {


Point2D_f::Point2D_f (float x, float y)
:   x (x), 
    y (y) 
{
    /// Nothing to do here!
}

void Point2D_f::set (float x = 0, float y = 0)
{
    x = x;
    y = y;
}

void Point2D_f::set (const Point2D_f& P)
{
    x = P.x;
    y = P.y;
}

void Point2D_f::translate (const Vect_f& V)
{
    y += V.y;
    x += V.x;
}

void Point2D_f::translate (float dx, float dy)
{
    y += dy;
    x += dx;
}

bool Point2D_f::equals (const Point2D_f& P) const
{
    if (x != P.x || y != P.y)
        return false;
    return true;
}

double Point2D_f::compare (const Point2D_f& P) const
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

void Point2D_f::swap (Point2D_f P)
{
    float tempX = P.x,
        tempY = P.y;

    P.x = x;
    P.y = y;

    x = tempX;
    y = tempY;
}

void Point2D_f::print (Terminal& io)
{

}

///--------------------------------------------------------------------------------------------------------------------

Point2D_d::Point2D_d () 
{
    /// Nothing to do here!
}

Point2D_d::Point2D_d (double x, double y)
:   x (x), 
    y (y) 
{ 
}

void Point2D_d::set (double x = 0, double y = 0)
{
    x = x;
    y = y;
}

void Point2D_d::set (const Point2D_d& P)
{
    x = P.x;
    y = P.y;
}

void Point2D_d::translate (const Vect_d& V)
{
    y += V.y;
    x += V.x;
}

void Point2D_d::translate (double dx, double dy)
{
    y += dy;
    x += dx;
}

bool Point2D_d::equals (const Point2D_d& P) const
{
    if (x != P.x || y != P.y)
        return false;
    return true;
}

double Point2D_d::compare (const Point2D_d& P) const
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

void Point2D_d::swap (Point2D_d& P)
{
    float tempX = P.x,
        tempY = P.y;

    P.x = x;
    P.y = y;

    x = tempX;
    y = tempY;
}

void Point2D_d::print (Terminal& io)
{

}

}   //  _Math

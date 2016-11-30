/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_2D/Point.cpp
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

#include <_2D/Point.hpp>

namespace _2D {

Point_i::Point_i () 
{
    /// Nothing to do here!
}

Point_i::Point_i (int x, int y)
:   X (x), 
    Y (y) 
{ 
}

int Point_i::getX () const { return X; }

int Point_i::getY () const { return Y; }

void Point_i::setX (int value) { X = value; }

void Point_i::setY (int value) { Y = value; }

void Point_i::set (int x = 0, int y = 0)
{
    X = x;
    Y = y;
}

void Point_i::set (const Point_i& P)
{
    X = P.X;
    Y = P.Y;
}

void Point_i::translate (const Vect_i& V)
{
    Y += V.Y;
    X += V.X;
}

void Point_i::translate (int dx, int dy)
{
    Y += dy;
    X += dx;
}

bool Point_i::equals (const Point_i& P) const
{
    if (X != P.X || Y != P.Y)
        return false;
    return true;
}

int Point_i::compare (const Point_i& P) const
{
    if (X >= P.X)
        if (Y >= P.Y)
            return 1;
        else if (Y < P.Y)
            return 4;
    if (X < P.X)
        if (Y > P.Y)
            return 2;
        else if (Y < P.Y)
            return 3;
    return 0;   //< There are equal.
}

void Point_i::swap (Point_i& P)
{
    float tempX = P.X,
        tempY = P.Y;

    P.X = X;
    P.Y = Y;

    X = tempX;
    Y = tempY;
}

void Point_i::print (I2P::Terminal& slot)
{

}

///--------------------------------------------------------------------------------------------------------------------


Point_f::Point_f (float X, float Y)
:   X (X), 
    Y (Y) 
{
    /// Nothing to do here!
}

float Point_f::getX () const { return X; }

float Point_f::getY () const { return Y; }

void Point_f::setX (float value) { X = value; }

void Point_f::setY (float value) { Y = value; }

void Point_f::set (float x = 0, float y = 0)
{
    X = x;
    Y = y;
}

void Point_f::set (const Point_f& P)
{
    X = P.X;
    Y = P.Y;
}

void Point_f::translate (const Vect_f& V)
{
    Y += V.Y;
    X += V.X;
}

void Point_f::translate (float dx, float dy)
{
    Y += dy;
    X += dx;
}

bool Point_f::equals (const Point_f& P) const
{
    if (X != P.X || Y != P.Y)
        return false;
    return true;
}

int Point_f::compare (const Point_f& P) const
{
    if (X >= P.X)
        if (Y >= P.Y)
            return 1;
        else if (Y < P.Y)
            return 4;
    if (X < P.X)
        if (Y > P.Y)
            return 2;
        else if (Y < P.Y)
            return 3;
    return 0;   //< There are equal.
}

void Point_f::swap (Point_f P)
{
    float tempX = P.X,
        tempY = P.Y;

    P.X = X;
    P.Y = Y;

    X = tempX;
    Y = tempY;
}

void Point_f::print (I2P::Terminal& slot)
{

}

}   //< namespace _2D

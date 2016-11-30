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

Point_i::Point_i (int X, int Y)
:   X (X), 
    Y (Y) 
{ 
}

int Point_i::getX () const { return X; }

int Point_i::getY () const { return Y; }

void Point_i::getX (int Value) { X = Value; }

void Point_i::getY (int Value) { Y = Value; }

void Point_i::get (int NewX = 0, int NewY = 0)
{
    X = NewX;
    Y = NewY;
}

void Point_i::get (const Point_i& P)
{
    X = P.X;
    Y = P.Y;
}

void Point_i::Translate (const Vect_i& V)
{
    Y += V.Y;
    X += V.X;
}

void Point_i::Translate (int DeltaX, int DeltaY)
{
    Y += DeltaY;
    X += DeltaX;
}

bool Point_i::Equals (const Point_i& P) const
{
    if (X != P.X || Y != P.Y)
        return false;
    return true;
}

int Point_i::Compare (const Point_i& P) const
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

void Point_i::Swap (Point_i& P)
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

void Point_f::getX (float Value) { X = Value; }

void Point_f::getY (float Value) { Y = Value; }

void Point_f::get (float NewX = 0, float NewY = 0)
{
    X = NewX;
    Y = NewY;
}

void Point_f::get (const Point_f& P)
{
    X = P.X;
    Y = P.Y;
}

void Point_f::Translate (const Vect_f& V)
{
    Y += V.Y;
    X += V.X;
}

void Point_f::Translate (float DeltaX, float DeltaY)
{
    Y += DeltaY;
    X += DeltaX;
}

bool Point_f::Equals (const Point_f& P) const
{
    if (X != P.X || Y != P.Y)
        return false;
    return true;
}

int Point_f::Compare (const Point_f& P) const
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

void Point_f::Swap (Point_f P)
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

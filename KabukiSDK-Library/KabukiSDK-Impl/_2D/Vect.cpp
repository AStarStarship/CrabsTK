/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_2D/Vect.cpp
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

#include <_2D/Vect.hpp>

namespace _2D {

Vect_i::Vect_i () 
{
    /// Nothing to do here!
}

Vect_i::Vect_i (int initX, int initY)
{
    X = initX;
    Y = initY;
}

int Vect_i::getX () { return X; }

int Vect_i::getY () { return Y; }

void Vect_i::getX (int Value) { X = Value; }

void Vect_i::getY (int Value) { Y = Value; }

void Vect_i::get (int newX, int newY)
{
    X = newX;
    Y = newY;
}

void Vect_i::get (const Vect_i& V)
{
    X = V.X;
    Y = V.Y;
}

bool Vect_i::Equals (const Vect_i& V) const
{
    if (X != V.X || Y != V.Y)
        return false;
    return true;
}

void Vect_i::Swap (Vect_i V)
{
    int tempX = V.X,
        tempY = V.Y;

    V.X = X;
    V.Y = Y;

    X = tempX;
    Y = tempY;
}

bool Vect_i::operator== (const Vect_i& P) const
{
    return ((X == P.X) && (Y == P.Y));
}

bool Vect_i::operator!= (const Vect_i& P) const
{
    return ((X != P.X) || (Y != P.Y));
}

Vect_i Vect_i::operator- (const Vect_i& P) const
{
    return { X - P.X, Y - P.Y };
}

Vect_i Vect_i::operator+ (const Vect_i& P) const
{
    return { X + P.X, Y + P.Y };
}

Vect_i& Vect_i::operator-= (const Vect_i& P)
{
    X -= P.X;
    Y -= P.Y;
    return *this;
}

Vect_i& Vect_i::operator+= (const Vect_i& P)
{
    X += P.X;
    Y += P.Y;
    return *this;
}

void Vect_i::print (I2P::Terminal& slot)
{

}

//---------------------------------------------------------------------------------------------------------------------

Vect_f::Vect_f () 
{
    /// Nothing to do here!
}

Vect_f::Vect_f (float initX, float initY)
{
    X = initX;
    Y = initY;
}

float Vect_f::getX () { return X; }

float Vect_f::getY () { return Y; }

void Vect_f::getX (float Value) { X = Value; }

void Vect_f::getY (float Value) { Y = Value; }

void Vect_f::get (float newX, float newY)
{
    X = newX;
    Y = newY;
}

void Vect_f::get (const Vect_f& V)
{
    X = V.X;
    Y = V.Y;
}

bool Vect_f::Equals (const Vect_f& V) const
{
    if (X != V.X || Y != V.Y)
        return false;
    return true;
}

void Vect_f::Swap (Vect_f V)
{
    float tempX = V.X,
        tempY = V.Y;

    V.X = X;
    V.Y = Y;

    X = tempX;
    Y = tempY;
}

bool Vect_f::operator== (const Vect_f& P) const
{
    return ((X == P.X) && (Y == P.Y));
}

bool Vect_f::operator!= (const Vect_f& P) const
{
    return ((X != P.X) || (Y != P.Y));
}

Vect_f Vect_f::operator- (const Vect_f& P) const
{
    return { X - P.X, Y - P.Y };
}

Vect_f Vect_f::operator+ (const Vect_f& P) const
{
    return { X + P.X, Y + P.Y };
}

Vect_f& Vect_f::operator-= (const Vect_f& P)
{
    X -= P.X;
    Y -= P.Y;
    return *this;
}

Vect_f& Vect_f::operator+= (const Vect_f& P)
{
    X += P.X;
    Y += P.Y;
    return *this;
}

void Vect_f::print (I2P::Terminal& slot)
{

}

}   //< namespace _2D

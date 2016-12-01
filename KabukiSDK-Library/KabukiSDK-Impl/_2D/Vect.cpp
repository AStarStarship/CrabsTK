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
    x = initX;
    y = initY;
}

void Vect_i::set (int newX, int newY)
{
    x = newX;
    y = newY;
}

void Vect_i::set (const Vect_i& V)
{
    x = V.x;
    y = V.y;
}

bool Vect_i::equals (const Vect_i& V) const
{
    if (x != V.x || y != V.y)
        return false;
    return true;
}

void Vect_i::swap (Vect_i V)
{
    int tempX = V.x,
        tempY = V.y;

    V.x = x;
    V.y = y;

    x = tempX;
    y = tempY;
}

bool Vect_i::operator== (const Vect_i& P) const
{
    return ((x == P.x) && (y == P.y));
}

bool Vect_i::operator!= (const Vect_i& P) const
{
    return ((x != P.x) || (y != P.y));
}

Vect_i Vect_i::operator- (const Vect_i& P) const
{
    return { x - P.x, y - P.y };
}

Vect_i Vect_i::operator+ (const Vect_i& P) const
{
    return { x + P.x, y + P.y };
}

Vect_i& Vect_i::operator-= (const Vect_i& P)
{
    x -= P.x;
    y -= P.y;
    return *this;
}

Vect_i& Vect_i::operator+= (const Vect_i& P)
{
    x += P.x;
    y += P.y;
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
    x = initX;
    y = initY;
}

float Vect_f::getX () { return x; }

float Vect_f::getY () { return y; }

void Vect_f::getX (float Value) { x = Value; }

void Vect_f::getY (float Value) { y = Value; }

void Vect_f::get (float newX, float newY)
{
    x = newX;
    y = newY;
}

void Vect_f::get (const Vect_f& V)
{
    x = V.x;
    y = V.y;
}

bool Vect_f::equals (const Vect_f& V) const
{
    if (x != V.x || y != V.y)
        return false;
    return true;
}

void Vect_f::swap (Vect_f V)
{
    float tempX = V.x,
        tempY = V.y;

    V.x = x;
    V.y = y;

    x = tempX;
    y = tempY;
}

bool Vect_f::operator== (const Vect_f& P) const
{
    return ((x == P.x) && (y == P.y));
}

bool Vect_f::operator!= (const Vect_f& P) const
{
    return ((x != P.x) || (y != P.y));
}

Vect_f Vect_f::operator- (const Vect_f& P) const
{
    return { x - P.x, y - P.y };
}

Vect_f Vect_f::operator+ (const Vect_f& P) const
{
    return { x + P.x, y + P.y };
}

Vect_f& Vect_f::operator-= (const Vect_f& P)
{
    x -= P.x;
    y -= P.y;
    return *this;
}

Vect_f& Vect_f::operator+= (const Vect_f& P)
{
    x += P.x;
    y += P.y;
    return *this;
}

void Vect_f::print (I2P::Terminal& slot)
{

}

}   //< namespace _2D

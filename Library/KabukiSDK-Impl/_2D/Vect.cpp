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

Vect_f::Vect_f () 
{
    /// Nothing to do here!
}

Vect_f::Vect_f (float initX, float initY)
{
    x = initX;
    y = initY;
}

void Vect_f::set (float newX, float newY)
{
    x = newX;
    y = newY;
}

void Vect_f::set (const Vect_f& V)
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

void Vect_f::print (Terminal& slot)
{

}

//---------------------------------------------------------------------------------------------------------------------

Vect_d::Vect_d () 
{
    /// Nothing to do here!
}

Vect_d::Vect_d (double initX, double initY)
{
    x = initX;
    y = initY;
}

void Vect_d::set (double newX, double newY)
{
    x = newX;
    y = newY;
}

void Vect_d::set (const Vect_d& V)
{
    x = V.x;
    y = V.y;
}

bool Vect_d::equals (const Vect_d& V) const
{
    if (x != V.x || y != V.y)
        return false;
    return true;
}

void Vect_d::swap (Vect_d V)
{
    double tempX = V.x,
        tempY = V.y;

    V.x = x;
    V.y = y;

    x = tempX;
    y = tempY;
}

bool Vect_d::operator== (const Vect_d& P) const
{
    return ((x == P.x) && (y == P.y));
}

bool Vect_d::operator!= (const Vect_d& P) const
{
    return ((x != P.x) || (y != P.y));
}

Vect_d Vect_d::operator- (const Vect_d& P) const
{
    return { x - P.x, y - P.y };
}

Vect_d Vect_d::operator+ (const Vect_d& P) const
{
    return { x + P.x, y + P.y };
}

Vect_d& Vect_d::operator-= (const Vect_d& P)
{
    x -= P.x;
    y -= P.y;
    return *this;
}

Vect_d& Vect_d::operator+= (const Vect_d& P)
{
    x += P.x;
    y += P.y;
    return *this;
}

void Vect_d::print (Terminal& slot)
{

}

}   //< _2D

/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Math/Vect2D.cpp
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

#include <_Math/Vect2D.hpp>

namespace _Math {

Vect2D_f::Vect2D_f () 
{
    /// Nothing to do here!
}

Vect2D_f::Vect2D_f (float initX, float initY)
{
    x = initX;
    y = initY;
}

void Vect2D_f::set (float newX, float newY)
{
    x = newX;
    y = newY;
}

void Vect2D_f::set (const Vect2D_f& V)
{
    x = V.x;
    y = V.y;
}

bool Vect2D_f::equals (const Vect2D_f& V) const
{
    if (x != V.x || y != V.y)
        return false;
    return true;
}

void Vect2D_f::swap (Vect2D_f V)
{
    float tempX = V.x,
        tempY = V.y;

    V.x = x;
    V.y = y;

    x = tempX;
    y = tempY;
}

bool Vect2D_f::operator== (const Vect2D_f& P) const
{
    return ((x == P.x) && (y == P.y));
}

bool Vect2D_f::operator!= (const Vect2D_f& P) const
{
    return ((x != P.x) || (y != P.y));
}

Vect2D_f Vect2D_f::operator- (const Vect2D_f& P) const
{
    return { x - P.x, y - P.y };
}

Vect2D_f Vect2D_f::operator+ (const Vect2D_f& P) const
{
    return { x + P.x, y + P.y };
}

Vect2D_f& Vect2D_f::operator-= (const Vect2D_f& P)
{
    x -= P.x;
    y -= P.y;
    return *this;
}

Vect2D_f& Vect2D_f::operator+= (const Vect2D_f& P)
{
    x += P.x;
    y += P.y;
    return *this;
}

void Vect2D_f::print (Terminal& slot)
{

}

//---------------------------------------------------------------------------------------------------------------------

Vect2D_d::Vect2D_d () 
{
    /// Nothing to do here!
}

Vect2D_d::Vect2D_d (double initX, double initY)
{
    x = initX;
    y = initY;
}

void Vect2D_d::set (double newX, double newY)
{
    x = newX;
    y = newY;
}

void Vect2D_d::set (const Vect2D_d& V)
{
    x = V.x;
    y = V.y;
}

bool Vect2D_d::equals (const Vect2D_d& V) const
{
    if (x != V.x || y != V.y)
        return false;
    return true;
}

void Vect2D_d::swap (Vect2D_d V)
{
    double tempX = V.x,
        tempY = V.y;

    V.x = x;
    V.y = y;

    x = tempX;
    y = tempY;
}

bool Vect2D_d::operator== (const Vect2D_d& P) const
{
    return ((x == P.x) && (y == P.y));
}

bool Vect2D_d::operator!= (const Vect2D_d& P) const
{
    return ((x != P.x) || (y != P.y));
}

Vect2D_d Vect2D_d::operator- (const Vect2D_d& P) const
{
    return { x - P.x, y - P.y };
}

Vect2D_d Vect2D_d::operator+ (const Vect2D_d& P) const
{
    return { x + P.x, y + P.y };
}

Vect2D_d& Vect2D_d::operator-= (const Vect2D_d& P)
{
    x -= P.x;
    y -= P.y;
    return *this;
}

Vect2D_d& Vect2D_d::operator+= (const Vect2D_d& P)
{
    x += P.x;
    y += P.y;
    return *this;
}

void Vect2D_d::print (Terminal& slot)
{

}

}   //< _Math

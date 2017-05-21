/** Kabuki Theater
    @file    /.../Source-Impl/_Math/Vect2D.cpp
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

#include <_Math/Vect2D.hpp>

namespace _Math {

Vect2D::Vect2D () 
{
    /// Nothing to do here!
}

Vect2D::Vect2D (float initX, float initY)
{
    x = initX;
    y = initY;
}

void Vect2D::set (float newX, float newY)
{
    x = newX;
    y = newY;
}

void Vect2D::set (const Vect2D& V)
{
    x = V.x;
    y = V.y;
}

bool Vect2D::equals (const Vect2D& V) const
{
    if (x != V.x || y != V.y)
        return false;
    return true;
}

void Vect2D::swap (Vect2D V)
{
    float tempX = V.x,
        tempY = V.y;

    V.x = x;
    V.y = y;

    x = tempX;
    y = tempY;
}

bool Vect2D::operator== (const Vect2D& P) const
{
    return ((x == P.x) && (y == P.y));
}

bool Vect2D::operator!= (const Vect2D& P) const
{
    return ((x != P.x) || (y != P.y));
}

Vect2D Vect2D::operator- (const Vect2D& P) const
{
    return { x - P.x, y - P.y };
}

Vect2D Vect2D::operator+ (const Vect2D& P) const
{
    return { x + P.x, y + P.y };
}

Vect2D& Vect2D::operator-= (const Vect2D& P)
{
    x -= P.x;
    y -= P.y;
    return *this;
}

Vect2D& Vect2D::operator+= (const Vect2D& P)
{
    x += P.x;
    y += P.y;
    return *this;
}

void Vect2D::print (Terminal& slot)
{

}


}   //< _Math

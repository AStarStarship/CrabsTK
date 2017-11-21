/** Kabuki Starship
    @file    /.../Source/KabukiTheater-Impl/_Math/Vect2D.cpp
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

namespace _Math {

template<typename T>
Vect2D::Vect2D () 
{
    /// Nothing to do here!
}

template<typename T>
Vect2D::Vect2D (T x, T y)
:   x (x),
    y (y)
{
}

template<typename T>
Vect2D::Vect2D (const Vect2D& v)
:   x (v.x),
    y (v.y)
{

}

template<typename T>
void Vect2D::set (T newX, T newY)
{
    x = newX;
    y = newY;
}

template<typename T>
void Vect2D::set (const Vect2D& v)
{
    x = v.x;
    y = v.y;
}

template<typename T>
bool Vect2D::equals (const Vect2D& v) const
{
    if (x != v.x || y != v.y)
        return false;
    return true;
}

template<typename T>
void Vect2D::swap (Vect2D v)
{
    T tempX = v.x,
        tempY = v.y;

    v.x = x;
    v.y = y;

    x = tempX;
    y = tempY;
}

template<typename T>
bool Vect2D::operator== (const Vect2D& v) const
{
    return ((x == v.x) && (y == v.y));
}

template<typename T>
bool Vect2D::operator!= (const Vect2D& v) const
{
    return ((x != v.x) || (y != v.y));
}

template<typename T>
Vect2D Vect2D::operator- (const Vect2D& v) const
{
    return { x - v.x, y - v.y };
}

template<typename T>
Vect2D Vect2D::operator+ (const Vect2D& v) const
{
    return { x + v.x, y + v.y };
}

template<typename T>
Vect2D& Vect2D::operator-= (const Vect2D& v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}

template<typename T>
Vect2D& Vect2D::operator+= (const Vect2D& v)
{
    x += v.x;
    y += v.y;
    return *this;
}

template<typename T>
void Vect2D::print () const
{
    cout << "Vect2D: x=" << x << ", y=" << y << '\n';
}

}   //< _Math

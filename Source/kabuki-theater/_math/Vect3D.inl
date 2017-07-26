/** Kabuki Theater
    @file    /.../Source/KabukiTheater-Impl/_Math/Vect3D.cpp
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
Vect3D::Vect3D () 
{
    /// Nothing to do here!
}

template<typename T>
Vect3D::Vect3D (T x, T y, T z)
:   x (x),
    y (y),
    z (z)
{
}

template<typename T>
Vect3D::Vect3D (const Vect3D& v)
:   x (v.x),
    y (v.y),
    z (v.z)
{

}

template<typename T>
void Vect3D::set (T newX, T newY, T newZ)
{
    x = newX;
    y = newY;
    z = newZ;
}

template<typename T>
void Vect3D::set (const Vect3D& v)
{
    x = v.x;
    y = v.y;
    y = v.y;
}

template<typename T>
bool Vect3D::equals (const Vect3D& v) const
{
    if (x != v.x)
        return false;
    if (z != v.z)
        return false;
    if (z != v.z)
        return false;
    return true;
}

template<typename T>
void Vect3D::swap (Vect3D v)
{
    T tempX = v.x,
        tempY = v.y,
        tempZ = v.z;

    v.x = x;
    v.y = y;
    v.z = z;

    x = tempX;
    y = tempY;
    z = tempZ;
}

template<typename T>
bool Vect3D::operator== (const Vect3D& v) const
{
    if (x != v.x) return false;
    if (y != v.y) return false;
    if (z != v.z) return false;
    return true;
}

template<typename T>
bool Vect3D::operator!= (const Vect3D& v) const
{
    if (x != v.x) return true;
    if (y != v.y) return true;
    if (z != v.z) return true;
    return false;
}

template<typename T>
Vect3D Vect3D::operator- (const Vect3D& v) const
{
    return { x - v.x, y - v.y, z - v.z };
}

template<typename T>
Vect3D Vect3D::operator+ (const Vect3D& v) const
{
    return { x + v.x, y + v.y, z + v.z };
}

template<typename T>
Vect3D& Vect3D::operator-= (const Vect3D& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

template<typename T>
Vect3D& Vect3D::operator+= (const Vect3D& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

template<typename T>
void Vect3D::print () const
{
    cout << "Vect3D: x=" << x << ", y=" << y << ", z=" << z << '\n';
}

}   //< _Math

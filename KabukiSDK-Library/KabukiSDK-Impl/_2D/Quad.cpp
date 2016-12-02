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

#include <_2D/Quad.hpp>

namespace _2D {


Quad_f::Quad_f ()
{
    // Nothing to do here! :-)
}

Quad_f::Quad_f (float initX1, float initY1, float initX2, float initY2, float initX3, float initY3, float initX4, float initY4)
:   x1 (initX1), 
    x2 (initX1), 
    x3 (initX1), 
    x4 (initX1), 
    y1 (initY1),
    y2 (initY1),
    y3 (initY1),
    y4 (initY1) 
{
    /// Nothing to do here!
}

void Quad_f::set (float newX1, float newY1, float newX2, float newY2, float newX3, float newY3, float newX4, float newY4)
{
    x1 = newX1;
    x2 = newX2;
    x3 = newX3;
    x4 = newX4;
    y1 = newY1;
    y2 = newY2;
    y3 = newY3;
    y4 = newY4;
}

void Quad_f::set (float x, float y, float width, float height)
{   
    // 1<----4
    // |     ^
    // |     |
    // v     |
    // 2---->3

    x1 = x;
    y1 = y;
    x2 = x;
    y2 = y + height;
    x3 = x + width;
    y3 = y + height;
    x4 = x + width;
    y4 = y;
}

void Quad_f::set (const Rect_f& r)
{
    set (r.getX (), r.getY (), r.getWidth (), r.getHeight ());
}

void Quad_f::set (const Quad_f& q)
{
    x1 = q.x1;
    y1 = q.y1;
    x2 = q.x2;
    y2 = q.y2;
    x3 = q.x3;
    y3 = q.y3;
    x4 = q.x4;
    y4 = q.y4;
}

void Quad_f::translate (const Vect_f& v)
{
    x1 += v.x;
    y1 += v.y;
    x2 += v.x;
    y2 += v.y;
    x3 += v.x;
    y3 += v.y;
    x4 += v.x;
    y4 += v.y;
}

void Quad_f::translate (float dx, float dy)
{
    y1 += dy;
    x1 += dx;
    y2 += dy;
    x2 += dx;
    y3 += dy;
    x3 += dx;
    y4 += dy;
    x4 += dx;
}

bool Quad_f::equals (const Quad_f& q) const
{
    if (x != q.x || y != q.y)
        return false;
    return true;
}

int Quad_f::compare (const Quad_f& q) const
{
    if (x >= q.x)
        if (y >= q.y)
            return 1;
        else if (y < q.y)
            return 4;
    if (x < q.x)
        if (y > q.y)
            return 2;
        else if (y < q.y)
            return 3;
    return 0;   //< There are equal.
}

void Quad_f::swap (Quad_f q)
{
    float tempX = q.x,
        tempY = q.y;

    q.x = x;
    q.y = y;

    x = tempX;
    y = tempY;
}

void Quad_f::print (Terminal& slot)
{

}

///--------------------------------------------------------------------------------------------------------------------

Quad_d::Quad_d () 
{
    /// Nothing to do here!
}

Quad_d::Quad_d (int initX, int initY)
:   x (initX), 
    y (initY) 
{ 
}

void Quad_d::set (int newX, int newY)
{
    x = newX;
    y = newY;
}

void Quad_d::set (const Quad_d& q)
{
    x = q.x;
    y = q.y;
}

void Quad_d::translate (const Vect_i& v)
{
    y += v.y;
    x += v.x;
}

void Quad_d::translate (int dx, int dy)
{
    y += dy;
    x += dx;
}

bool Quad_d::equals (const Quad_d& q) const
{
    if (x != q.x || y != q.y)
        return false;
    return true;
}

int Quad_d::compare (const Quad_d& q) const
{
    if (x >= q.x)
        if (y >= q.y)
            return 1;
        else if (y < q.y)
            return 4;
    if (x < q.x)
        if (y > q.y)
            return 2;
        else if (y < q.y)
            return 3;
    return 0;   //< There are equal.
}

void Quad_d::swap (Quad_d& q)
{
    float tempX = q.x,
        tempY = q.y;

    q.x = x;
    q.y = y;

    x = tempX;
    y = tempY;
}

void Quad_d::print (Terminal& slot)
{

}

}   //< namespace _2D

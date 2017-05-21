/** Kabuki Theater
    @file    /.../Source-Impl/_Math/Point.cpp
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

#include <_Math/Quad2D.hpp>

namespace _Math {


Quad2D::Quad2D ()
{
    // Nothing to do here! :-)
}

Quad2D::Quad2D (Type initX1, Type initY1, Type initX2, Type initY2, Type initX3, Type initY3, Type initX4, Type initY4)
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

void Quad2D::set (Type newX1, Type newY1, Type newX2, Type newY2, Type newX3, Type newY3, Type newX4, Type newY4)
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

void Quad2D::set (Type x, Type y, Type width, Type height)
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

void Quad2D::set (const Rect_f& r)
{
    set (r.getX (), r.getY (), r.getWidth (), r.getHeight ());
}

void Quad2D::set (const Quad2D& q)
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

void Quad2D::translate (const Vect_f& v)
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

void Quad2D::translate (Type dx, Type dy)
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

bool Quad2D::equals (const Quad2D& q) const
{
    if (x != q.x || y != q.y)
        return false;
    return true;
}

int Quad2D::compare (const Quad2D& q) const
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

void Quad2D::swap (Quad2D q)
{
    Type tempX0 = q.x0,
        tempY0 = q.y0;

    q.x0 = x0;
    q.y0 = y0;

    x0 = tempX0;
    y0 = tempY0;

    Type tempX0 = q.x0,
        tempY0 = q.y0;

    q.x0 = x0;
    q.y0 = y0;

    x0 = tempX0;
    y0 = tempY0;

    Type tempX0 = q.x0,
        tempY0 = q.y0;

    q.x0 = x0;
    q.y0 = y0;

    x0 = tempX0;
    y0 = tempY0;

    Type tempX0 = q.x0,
        tempY0 = q.y0;

    q.x0 = x0;
    q.y0 = y0;

    x0 = tempX0;
    y0 = tempY0;
}

void Quad2D::print (Terminal& slot)
{

}

///--------------------------------------------------------------------------------------------------------------------

Quad2D_d::Quad2D_d () 
{
    /// Nothing to do here!
}

Quad2D_d::Quad2D_d (int initX, int initY)
:   x (initX), 
    y (initY) 
{ 
}

void Quad2D_d::set (int newX, int newY)
{
    x = newX;
    y = newY;
}

void Quad2D_d::set (const Quad2D_d& q)
{
    x = q.x;
    y = q.y;
}

void Quad2D_d::translate (const Vect_i& v)
{
    y += v.y;
    x += v.x;
}

void Quad2D_d::translate (int dx, int dy)
{
    y += dy;
    x += dx;
}

bool Quad2D_d::equals (const Quad2D_d& q) const
{
    if (x != q.x || y != q.y)
        return false;
    return true;
}

int Quad2D_d::compare (const Quad2D_d& q) const
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

void Quad2D_d::swap (Quad2D_d& q)
{
    Type tempX = q.x,
        tempY = q.y;

    q.x = x;
    q.y = y;

    x = tempX;
    y = tempY;
}

void Quad2D_d::print (Terminal& slot)
{

}

}   //  _Math

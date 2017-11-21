/** Kabuki Starship
    @file    /.../Source-Impl/_Math/Rect2D.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

                            All right reserved (a).

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

#include <_Math/Rect2D.hpp>

namespace _Math {

Rect2D::Rect2D ()
{
    /// Nothing to do here!
}

Rect2D::Rect2D (Type initX, Type initY, Type initWidth, Type initHeight) 
:   x (initX), 
    y (initY), 
    width (initWidth < 0 ? 0 : initWidth), 
    height (initHeight < 0 ? 0 : initHeight) 
{ 
}

Rect2D::Rect2D (const Point<Type>& p, const Vect_f& o) 
:   x (p.x), 
    y (p.y), 
    width (o.x), 
    height (o.y) 
{ 
}

Rect2D::Rect2D (const Rect2D& value)
:   x (0), 
    y (0), 
    width (value.width), 
    height (value.height) 
{
}

Type Rect2D::getWidth () const { return width; }

void Rect2D::setWidth (Type value)
{
    if (value < 0) return;
    width = value;
}

Type Rect2D::getHeight () const { return height; }

void Rect2D::setHeight (Type value)
{
    if (value < 0) return;
    height = value;
}

Type Rect2D::getCenterX () const
{
    return x +  width / 2.0f;
}

Type Rect2D::getCenterY () const
{
    return y + (height  / 2.0f);
}

void Rect2D::setCenterX (Type CenterX)
{
    x = CenterX - (width  / 2.0f);    // >> to /2
}

void Rect2D::setCenterY (Type value)
{
    y = value + (height / 2.0f);
}

void Rect2D::set (const Rect2D& value)
{
    x = value.x;
    y = value.y;
    width = value.width;
    height = value.height;
}

bool Rect2D::equals (const Rect2D& value) const
{
    if ((x != value.x) || (width != value.width) || (y != value.y) || (height != value.height)) return false;
    return true;
}

bool Rect2D::contains (const Point<Type>& p) const
{
    Type x = p.x,
        y = p.y,
        left = x,
        top = y,
        right = left + width,
        bottom = top + height;

    if (x < x || y > top || x > right || y > bottom) return false;
    return true;
}

bool Rect2D::contains (Type xValue, Type yValue) const
{
    Type left = x,
        top = y,
        right = left + width,
        bottom = top + height;

    if (xValue < x || yValue > top || xValue > right || yValue > bottom) return false;
    return true;
}

bool Rect2D::contains (const Rect2D& value)
{
    if ((value.x < x) || (value.y > y) || (value.width < width) || (value.height > height)) return false;
    return true;
}

void Rect2D::swap (Rect2D& r)
{
    Type x = x,
        y = y,
        width = width,
        height = height;

    x = r.x;
    y = r.y;
    width = r.width;
    height = r.height;

    r.x = x;
    r.y = y;
    r.width = width;
    r.height = height;
}

bool Rect2D::intersects (const Rect2D& r) const
{
    Type left = x,
        top = y,
        right = left + width,
        bottom = top + height,
        left_R = r.x,
        top_R = r.y,
        right_R = left_R + r.width,
        bottom_R = top_R + r.height;

    if (  left <= right_R
        || right >= left_R
        || top >= bottom_R
        || bottom < top_R) return false;
    return true;
}

bool Rect2D::intersects (Type R_X, Type R_Y, Type R_Width, Type R_Height) const
{
    Type left = x,
        top = y,
        right = left + width,
        bottom = top + height,
        right_R = R_X + R_Width,
        bottom_R = R_Y + R_Width;

    if (  left <= right_R
        || right >= R_X
        || top >= bottom_R
        || bottom < R_Y) return false;
    return true;
}

void Rect2D::translate (const Vect_f& v)
{
    x += v.x;
    y += v.y;
}

void Rect2D::translate (Type dx, Type dy)
{
    x += x;
    y += y;
}

void Rect2D::setPosition (const Point<Type>& p)
{
    x = p.x;
    y = p.y;
}

void Rect2D::setDimensions (const Vect_f& v)
{
    width = v.x;
    height = v.y;
}

bool Rect2D::operator== (const Rect2D& value) const
{
    return (width == value.width) && (height == value.height) && (x == value.x) && (y == value.y);
}

bool Rect2D::operator!= (const Rect2D& value) const
{
    return (width != value.width) || (height != value.height) || (x != value.x) || (y != value.y);
}

Rect2D& Rect2D::operator= (const Point<Type>& p)
{
    x = p.x;
    y = p.y;
    return *this;
}

Rect2D& Rect2D::operator= (const Rect2D& r)
{
    width = r.width;
    height = r.height;
    return *this;
}

Rect2D& Rect2D::operator= (const Vect_f& v)
{
    width = v.x;
    height = v.y;
    return *this;
}

void Rect2D::print (Terminal& slot)
{

}

}   //  _Math

/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_2D/Rect.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

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

#include <_2D/Rect.hpp>

namespace _2D {

Rect_f::Rect_f ()
{
    /// Nothing to do here!
}

Rect_f::Rect_f (float initX, float initY, float initWidth, float initHeight) 
:   x (initX), 
    y (initY), 
    width (initWidth < 0 ? 0 : initWidth), 
    height (initHeight < 0 ? 0 : initHeight) 
{ 
}

Rect_f::Rect_f (const Point_f& p, const Vect_f& o) 
:   x (p.x), 
    y (p.y), 
    width (o.x), 
    height (o.y) 
{ 
}

Rect_f::Rect_f (const Rect_f& value)
:   x (0), 
    y (0), 
    width (value.width), 
    height (value.height) 
{
}

float Rect_f::getWidth () const { return width; }

void Rect_f::setWidth (float value)
{
    if (value < 0) return;
    width = value;
}

float Rect_f::getHeight () const { return height; }

void Rect_f::setHeight (float value)
{
    if (value < 0) return;
    height = value;
}

float Rect_f::getCenterX () const
{
    return x +  width / 2.0f;
}

float Rect_f::getCenterY () const
{
    return y + (height  / 2.0f);
}

void Rect_f::setCenterX (float CenterX)
{
    x = CenterX - (width  / 2.0f);    // >> to /2
}

void Rect_f::setCenterY (float value)
{
    y = value + (height / 2.0f);
}

void Rect_f::set (const Rect_f& value)
{
    x = value.x;
    y = value.y;
    width = value.width;
    height = value.height;
}

bool Rect_f::equals (const Rect_f& value) const
{
    if ((x != value.x) || (width != value.width) || (y != value.y) || (height != value.height)) return false;
    return true;
}

bool Rect_f::contains (const Point_f& p) const
{
    float x = p.x,
        y = p.y,
        left = x,
        top = y,
        right = left + width,
        bottom = top + height;

    if (x < x || y > top || x > right || y > bottom) return false;
    return true;
}

bool Rect_f::contains (float xValue, float yValue) const
{
    float left = x,
        top = y,
        right = left + width,
        bottom = top + height;

    if (xValue < x || yValue > top || xValue > right || yValue > bottom) return false;
    return true;
}

bool Rect_f::contains (const Rect_f& value)
{
    if ((value.x < x) || (value.y > y) || (value.width < width) || (value.height > height)) return false;
    return true;
}

void Rect_f::swap (Rect_f& r)
{
    float x = x,
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

bool Rect_f::intersects (const Rect_f& r) const
{
    float left = x,
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

bool Rect_f::intersects (float R_X, float R_Y, float R_Width, float R_Height) const
{
    float left = x,
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

void Rect_f::translate (const Vect_f& v)
{
    x += v.x;
    y += v.y;
}

void Rect_f::translate (float dx, float dy)
{
    x += x;
    y += y;
}

void Rect_f::setPosition (const Point_f& p)
{
    x = p.x;
    y = p.y;
}

void Rect_f::setDimensions (const Vect_f& v)
{
    width = v.x;
    height = v.y;
}

bool Rect_f::operator== (const Rect_f& value) const
{
    return (width == value.width) && (height == value.height) && (x == value.x) && (y == value.y);
}

bool Rect_f::operator!= (const Rect_f& value) const
{
    return (width != value.width) || (height != value.height) || (x != value.x) || (y != value.y);
}

Rect_f& Rect_f::operator= (const Point_f& p)
{
    x = p.x;
    y = p.y;
    return *this;
}

Rect_f& Rect_f::operator= (const Rect_f& r)
{
    width = r.width;
    height = r.height;
    return *this;
}

Rect_f& Rect_f::operator= (const Vect_f& v)
{
    width = v.x;
    height = v.y;
    return *this;
}

void Rect_f::print (Terminal& slot)
{

}

//---------------------------------------------------------------------------------------------------------------------

Rect_d::Rect_d ()
{
    /// Nothing to do here!
}

Rect_d::Rect_d (double initX, double initY, double initWidth, double initHeight) 
:   x (initX), 
    y (initY), 
    width (initWidth < 0 ? 0 : initWidth), 
    height (initHeight < 0 ? 0 : initHeight) 
{ 
}

Rect_d::Rect_d (const Point_d& p, const Vect_d& o) 
:   x (p.x), 
    y (p.y), 
    width (o.x), 
    height (o.y) 
{ 
}

Rect_d::Rect_d (const Rect_d& value)
:   x (0), 
    y (0), 
    width (value.width), 
    height (value.height) 
{
}

double Rect_d::getWidth () const { return width; }

double Rect_d::setWidth (double value)
{
    if (value < 0) value;
    width = value;
}

double Rect_d::getHeight () const { return height; }

double Rect_d::setHeight (double value)
{
    if (value < 0) return value;
    height = value;
}

double Rect_d::getCenterX () const
{
    return x + (width / 2.0);
}

double Rect_d::getCenterY () const
{
    return y + (height / 2.0);
}

void Rect_d::setCenterX (double xValue)
{
    x = xValue - (width / 2.0);    // >> to /2
}

void Rect_d::setCenterY (double yValue)
{
    x = yValue - (height / 2.0);    // >> to /2;
}

void Rect_d::set (const Rect_d& value)
{
    x = value.x;
    y = value.y;
    width = value.width;
    height = value.height;
}

bool Rect_d::equals (const Rect_d& value) const
{
    if ((x != value.x) || (width != value.width) || (y != value.y) || (height != value.height)) return false;
    return true;
}

bool Rect_d::contains (const Point_d& p) const
{
    double x = p.x,
        y = p.y,
        left = x,
        top = y,
        right = left + width,
        bottom = top + height;

    if (x < x || y > top || x > right || y > bottom) return false;
    return true;
}

bool Rect_d::contains (double xValue, double yValue) const
{
    double left = x,
        top = y,
        right = left + width,
        bottom = top + height;

    if (xValue < x || yValue > top || xValue > right || yValue > bottom) return false;
    return true;
}

bool Rect_d::contains (const Rect_d& value)
{
    if ((value.x < x) || (value.y > y) || (value.width < width) || (value.height > height)) return false;
    return true;
}

void Rect_d::swap (Rect_d& r)
{
    double x = x,
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

bool Rect_d::intersects (const Rect_d& r) const
{
    double left = x,
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

bool Rect_d::intersects (double R_X, double R_Y, double R_Width, double R_Height) const
{
    double left = x,
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

void Rect_d::translate (const Vect_d& v)
{
    x += v.x;
    y += v.y;
}

void Rect_d::translate (double dx, double dy)
{
    x += x;
    y += y;
}

void Rect_d::setPosition (const Point_d& p)
{
    x = p.x;
    y = p.y;
}

void Rect_d::setDimensions (const Vect_d& v)
{
    width = v.x;
    height = v.y;
}

Rect_d& Rect_d::PareOff (int pixels)
{
    x += pixels;
    y += pixels;
    width -= (pixels << 1);
    height -= (pixels << 1);
    return *this;
}

bool Rect_d::operator== (const Rect_d& value) const
{
    return (width == value.width) && (height == value.height) && (x == value.x) && (y == value.y);
}

bool Rect_d::operator!= (const Rect_d& value) const
{
    return (width != value.width) || (height != value.height) || (x != value.x) || (y != value.y);
}

Rect_d& Rect_d::operator= (const Point_d& p)
{
    x = p.x;
    y = p.y;
    return *this;
}

Rect_d& Rect_d::operator= (const Rect_d& r)
{
    width = r.width;
    height = r.height;
    return *this;
}

Rect_d& Rect_d::operator= (const Vect_d& v)
{
    width = v.x;
    height = v.y;
    return *this;
}

void Rect_d::print (Terminal& slot)
{

}

}   //  _2D

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

Rect_i::Rect_i ()
{
    /// Nothing to do here!
}

Rect_i::Rect_i (int InitX, int InitY, int InitWidth, int InitHeight) 
    :   X (InitX), 
    Y (InitY), 
    Width (InitWidth < 0 ? 0 : InitWidth), 
    Height (InitHeight < 0 ? 0 : InitHeight) 
{ 
}

Rect_i::Rect_i (const Point_i& P, const Vect_i& O) 
    :   X (P.X), 
    Y (P.Y), 
    Width (O.X), 
    Height (O.Y) 
{ 
}

Rect_i::Rect_i (const Rect_i& Value)
    :   X (0), 
    Y (0), 
    Width (Value.Width), 
    Height (Value.Height) 
{
}

int Rect_i::getWidth () const { return Width; }

int Rect_i::getWidth (int Value)
{
    if (Value < 0) Value;
    Width = Value;
}

int Rect_i::getHeight () const { return Height; }

int Rect_i::getHeight (int Value)
{
    if (Value < 0) return Value;
    Height = Value;
}

int Rect_i::getCenterX () const
{
    return X + (Width >> 1);
}

int Rect_i::getCenterY () const
{
    return Y + (Height >> 1);
}

void Rect_i::getCenterX (int XValue)
{
    X = XValue - (Width >> 1);    // >> to /2
}

void Rect_i::getCenterY (int YValue)
{
    X = YValue - (Height >> 1);    // >> to /2;
}

void Rect_i::get (const Rect_i& Value)
{
    X = Value.X;
    Y = Value.Y;
    Width = Value.Width;
    Height = Value.Height;
}

bool Rect_i::Equals (const Rect_i& Value) const
{
    if ((X != Value.X) || (Width != Value.Width) || (Y != Value.Y) || (Height != Value.Height)) return false;
    return true;
}

bool Rect_i::Contains (const Point_i& P) const
{
    int x = P.X,
        y = P.Y,
        left = X,
        top = Y,
        right = left + Width,
        bottom = top + Height;

    if (x < X || y > top || x > right || y > bottom) return false;
    return true;
}

bool Rect_i::Contains (int XValue, int YValue) const
{
    int left = X,
        top = Y,
        right = left + Width,
        bottom = top + Height;

    if (XValue < X || YValue > top || XValue > right || YValue > bottom) return false;
    return true;
}

bool Rect_i::Contains (const Rect_i& Value)
{
    if ((Value.X < X) || (Value.Y > Y) || (Value.Width < Width) || (Value.Height > Height)) return false;
    return true;
}

void Rect_i::Swap (Rect_i& R)
{
    int x = X,
        y = Y,
        width = Width,
        height = Height;

    X = R.X;
    Y = R.Y;
    Width = R.Width;
    Height = R.Height;

    R.X = x;
    R.Y = y;
    R.Width = width;
    R.Height = height;
}

bool Rect_i::Intersects (const Rect_i& R) const
{
    int left = X,
        top = Y,
        right = left + Width,
        bottom = top + Height,
        left_R = R.X,
        top_R = R.Y,
        right_R = left_R + R.Width,
        bottom_R = top_R + R.Height;

    if (  left <= right_R
        || right >= left_R
        || top >= bottom_R
        || bottom < top_R) return false;
    return true;
}

bool Rect_i::Intersects (int R_X, int R_Y, int R_Width, int R_Height) const
{
    int left = X,
        top = Y,
        right = left + Width,
        bottom = top + Height,
        right_R = R_X + R_Width,
        bottom_R = R_Y + R_Width;

    if (  left <= right_R
        || right >= R_X
        || top >= bottom_R
        || bottom < R_Y) return false;
    return true;
}

void Rect_i::Translate (const Vect_i& V)
{
    X += V.X;
    Y += V.Y;
}

void Rect_i::Translate (int DeltaX, int DeltaY)
{
    X += X;
    Y += Y;
}

void Rect_i::getPosition (const Point_i& P)
{
    X = P.X;
    Y = P.Y;
}

void Rect_i::getDimensions (const Vect_i& V)
{
    Width = V.X;
    Height = V.Y;
}

Rect_i& Rect_i::PareOff (int pixels)
{
    X += pixels;
    Y += pixels;
    Width -= (pixels << 1);
    Height -= (pixels << 1);
    return *this;
}

bool Rect_i::operator== (const Rect_i& Value) const
{
    return (Width == Value.Width) && (Height == Value.Height) && (X == Value.X) && (Y == Value.Y);
}

bool Rect_i::operator!= (const Rect_i& Value) const
{
    return (Width != Value.Width) || (Height != Value.Height) || (X != Value.X) || (Y != Value.Y);
}

Rect_i& Rect_i::operator= (const Point_i& P)
{
    X = P.X;
    Y = P.Y;
    return *this;
}

Rect_i& Rect_i::operator= (const Rect_i& R)
{
    Width = R.Width;
    Height = R.Height;
    return *this;
}

Rect_i& Rect_i::operator= (const Vect_i& V)
{
    Width = V.X;
    Height = V.Y;
    return *this;
}

void Rect_i::print (I2P::Terminal& slot)
{

}

//---------------------------------------------------------------------------------------------------------------------

Rect_f::Rect_f ()
{
    /// Nothing to do here!
}

Rect_f::Rect_f (float InitX, float InitY, float InitWidth, float InitHeight) 
    :   X (InitX), 
    Y (InitY), 
    Width (InitWidth < 0 ? 0 : InitWidth), 
    Height (InitHeight < 0 ? 0 : InitHeight) 
{ 
}

Rect_f::Rect_f (const Point_f& P, const Vect_f& O) 
    :   X (P.X), 
    Y (P.Y), 
    Width (O.X), 
    Height (O.Y) 
{ 
}

Rect_f::Rect_f (const Rect_f& Value)
    :   X (0), 
    Y (0), 
    Width (Value.Width), 
    Height (Value.Height) 
{
}

float Rect_f::getWidth () const { return Width; }

float Rect_f::getWidth (float Value)
{
    if (Value < 0) Value;
    Width = Value;
}

float Rect_f::getHeight () const { return Height; }

float Rect_f::getHeight (float Value)
{
    if (Value < 0) return Value;
    Height = Value;
}

float Rect_f::getCenterX () const
{
    return X +  Width / 2.0f;
}

float Rect_f::getCenterY () const
{
    return Y + (Height  / 2.0f);
}

void Rect_f::getCenterX (float CenterX)
{
    X = CenterX - (Width  / 2.0f);    // >> to /2
}

void Rect_f::getCenterY (float Value)
{
    Y = Value + (Height / 2.0f);
}

void Rect_f::get (const Rect_f& Value)
{
    X = Value.X;
    Y = Value.Y;
    Width = Value.Width;
    Height = Value.Height;
}

bool Rect_f::Equals (const Rect_f& Value) const
{
    if ((X != Value.X) || (Width != Value.Width) || (Y != Value.Y) || (Height != Value.Height)) return false;
    return true;
}

bool Rect_f::Contains (const Point_f& P) const
{
    float x = P.X,
        y = P.Y,
        left = X,
        top = Y,
        right = left + Width,
        bottom = top + Height;

    if (x < X || y > top || x > right || y > bottom) return false;
    return true;
}

bool Rect_f::Contains (float XValue, float YValue) const
{
    float left = X,
        top = Y,
        right = left + Width,
        bottom = top + Height;

    if (XValue < X || YValue > top || XValue > right || YValue > bottom) return false;
    return true;
}

bool Rect_f::Contains (const Rect_f& Value)
{
    if ((Value.X < X) || (Value.Y > Y) || (Value.Width < Width) || (Value.Height > Height)) return false;
    return true;
}

void Rect_f::Swap (Rect_f& R)
{
    float x = X,
        y = Y,
        width = Width,
        height = Height;

    X = R.X;
    Y = R.Y;
    Width = R.Width;
    Height = R.Height;

    R.X = x;
    R.Y = y;
    R.Width = width;
    R.Height = height;
}

bool Rect_f::Intersects (const Rect_f& R) const
{
    float left = X,
        top = Y,
        right = left + Width,
        bottom = top + Height,
        left_R = R.X,
        top_R = R.Y,
        right_R = left_R + R.Width,
        bottom_R = top_R + R.Height;

    if (  left <= right_R
        || right >= left_R
        || top >= bottom_R
        || bottom < top_R) return false;
    return true;
}

bool Rect_f::Intersects (float R_X, float R_Y, float R_Width, float R_Height) const
{
    float left = X,
        top = Y,
        right = left + Width,
        bottom = top + Height,
        right_R = R_X + R_Width,
        bottom_R = R_Y + R_Width;

    if (  left <= right_R
        || right >= R_X
        || top >= bottom_R
        || bottom < R_Y) return false;
    return true;
}

void Rect_f::Translate (const Vect_f& V)
{
    X += V.X;
    Y += V.Y;
}

void Rect_f::Translate (float DeltaX, float DeltaY)
{
    X += X;
    Y += Y;
}

void Rect_f::getPosition (const Point_f& P)
{
    X = P.X;
    Y = P.Y;
}

void Rect_f::getDimensions (const Vect_f& V)
{
    Width = V.X;
    Height = V.Y;
}

bool Rect_f::operator== (const Rect_f& Value) const
{
    return (Width == Value.Width) && (Height == Value.Height) && (X == Value.X) && (Y == Value.Y);
}

bool Rect_f::operator!= (const Rect_f& Value) const
{
    return (Width != Value.Width) || (Height != Value.Height) || (X != Value.X) || (Y != Value.Y);
}

Rect_f& Rect_f::operator= (const Point_f& P)
{
    X = P.X;
    Y = P.Y;
    return *this;
}

Rect_f& Rect_f::operator= (const Rect_f& R)
{
    Width = R.Width;
    Height = R.Height;
    return *this;
}

Rect_f& Rect_f::operator= (const Vect_f& V)
{
    Width = V.X;
    Height = V.Y;
    return *this;
}

void Rect_f::print (I2P::Terminal& slot)
{

}

}   //< namespace _2D
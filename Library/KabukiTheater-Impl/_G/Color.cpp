/** Kabuki Theater
    @file    /.../KabukiTheater-Impl/_/Color.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright  (C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved  (r).

        Licensed under the Apache License, Version 2.0  (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#include <_G/Color.hpp>
#include <stdlib.h>

namespace _G {

const color_t* rainbowColors ()
{

    static const color_t rainbowColorsArray[] = {
        0xF00000, 0xF01000, 0xF02000, 0xF03000, 0xF04000, 0xF05000, 0xF06000, 0xF07000, 0xF08000, 0xB0F000, 
        0xF09000, 0xF0A000, 0xF0B000, 0xF0C000, 0xF0D000, 0xF0E000, 0xF0F000, 0xF0F000, 0xE0F000, 0xD0F000, 
        0xC0F000, 0xA0F000, 0x90F000, 0x80F000, 0x70F000, 0x60F000, 0x50F000, 0x40F000, 0x30F000, 0x20F000, 
        0x10F000, 0x00F000, 0x00F000, 0x00F010, 0x00F020, 0x00F030, 0x00F040, 0x00F050, 0x00F060, 0x00F070, 
        0x00F080, 0x00F090, 0x00F0A0, 0x00F0B0, 0x00F0C0, 0x00F0D0, 0x00F0E0, 0x00F0F0, 0x00F0F0, 0x00E0F0, 
        0x00D0F0, 0x00C0F0, 0x00B0F0, 0x00A0F0, 0x0090F0, 0x0080F0, 0x0070F0, 0x0060F0, 0x0050F0, 0x0040F0, 
        0x0030F0, 0x0020F0, 0x0010F0, 0x0000F0, 0x0000F0, 0x1000F0, 0x2000F0, 0x3000F0, 0x4000F0, 0x5000F0, 
        0x6000F0, 0x7000F0, 0x8000F0, 0x9000F0, 0xA000F0, 0xB000F0, 0xC000F0, 0xD000F0, 0xE000F0, 0xF000F0, 
        0xF000F0 ,0xF000E0, 0xF000D0, 0xF000C0, 0xF000B0, 0xF000A0, 0xF00090, 0xF00080, 0xF00070, 0xF00060, 
        0xF00050, 0xF00040, 0xF00030, 0xF00020, 0xF00010, 0xF00000 };

    return &rainbowColorsArray[0];
}

const color_t* presetColors ()
{
    /** An array of the previous colors. */
    static const color_t presetColorsArray[] = {
        White,
        Orchid,
        Purple,
        Violet,
        Indigo,
        SkyBlue,
        NavyBlue,
        blue,
        Turquoise,
        green,
        Yellow,
        Gold,
        Oragne,
        Pink,
        red,
        Maroon,
        Brown,
        Black
    };
    return &presetColorsArray[0];
}

color_t getRainbowColor (PresetColor index)
{

    if (index < 0 || index >= 96) return Black;
    return rainbowColors ()[index];
}

color_t getPresetColor (PresetColor index)
{

    if (index < 0 || index >= 96) return Black;
    return presetColors ()[index];
}

color_t getRandomPresetColor (PresetColor index)
{
    int index = rand () % NumPresetColors;
    return presetColors ()[index];
}

color_t getRandomColor (PresetColor index)
{
    return rand ();
}

color_t mixColor (color_t a, color_t b)
{
    uint32_t red = a & 0xff,
        green = (a & 0xff00) >> 8,
        blue = (a & 0xff00) >> 16,
        alpha = (a & 0xff00) >> 24,
        result;

    byte r2 = b & 0xff,
        g2 = (b & 0xff00) >> 8,
        b2 = (b & 0xff00) >> 16,
        a2 = (b & 0xff00) >> 24;

    red   = (red + r2) >> 1;    //< Shift right 1 to divide by 2.
    green = ((green + g2) >> 1) << 8;
    blue  = ((blue  + b2) >> 1) << 16;
    alpha = ((alpha + a2) >> 1) << 24;

    return red & green & blue & alpha;
}

color_t changeBrightness (color_t color, int brightnessChange)
{
    color_t alpha = (color & 0xff000000) >> 24 + brightnessChange;
    return (color & 0xffffff) & (alpha << 24);
}

color_t decreaseBrightness (color_t color, int brightnessChange)
{
    color_t alpha = (color & 0xff000000) >> 24 - brightnessChange;
    return (color & 0xffffff) & (alpha << 24);
}

Color::Color (color_t color)
{
    color_t* this_ptr = (color_t*)this;
    *this_ptr = color;
}

Color::Color (byte red, byte green, byte blue)
:   r (red),
    g (green),
    b (blue)
{
	// Nothing to do here!
}

Color::Color (byte red, byte green, byte blue, byte alpha)
:   r (red),
    g (green),
    b (blue),
    a (alpha)
{
}

void Color::set (color_t Value) 
{
    color_t* this_ptr = (color_t*)this;
    *this_ptr = Value;
}

void Color::set (byte red, byte green, byte blue)
{
    r = red;
    g = green;
    b = blue;
    a = 255;
}

void Color::set (byte red, byte green, byte blue, byte alpha)
{
    r = red;
    g = green;
    b = blue;
    a = alpha;
}

byte Color::getRed () { return r; }

void Color::setRed (byte Value) { r = Value; }

byte Color::getGreen () { return g; }

void Color::setGreen (byte Value) { g = Value; }

byte Color::getBlue () { return b; }

void Color::setBlue (byte Value) { b = Value; }

byte Color::getAlpha () { return a; }

void Color::setAlpha (byte Value) { a = Value; }

void Color::toHSV (float& fR, float& fG, float fB, float& fH, float& fS, float& fV)
{
    /*
    float fCMax = fR > fG ? fR : fG;
    fCMax = fCMax > fB ? fCMax : fB;
    float fCMin = fR < fG ? fR : fG;
    fCMin = fCMin < fB ? fCMin : fB;

    float fDelta = fCMax - fCMin;

    if (fDelta > 0)
    {
        if (fCMax == fR)
        {
            fH = 60 * (fmod (((fG - fB) / fDelta), 6));
        }
        else if (fCMax == fG)
        {
            fH = 60 * (((fB - fR) / fDelta) + 2);
        }
        else if (fCMax == fB)
        {
            fH = 60 * (((fR - fG) / fDelta) + 4);
        }

        if (fCMax > 0)
        {
            fS = fDelta / fCMax;
        }
        else
        {
            fS = 0;
        }

        fV = fCMax;
    }
    else
    {
        fH = 0;
        fS = 0;
        fV = fCMax;
    }

    if (fH < 0)
    {
        fH = 360 + fH;
    }
    */
}

void Color::setHSV (float& fR, float& fG, float& fB, float& fH, float& fS, float& fV)
{
    /*
    float fC = fV * fS; // Chroma
    float fHPrime = fmod (fH / 60.0, 6);
    float fX = fC * (1 - fabs (fmod (fHPrime, 2) - 1));
    float fM = fV - fC;

    if (0 <= fHPrime && fHPrime < 1)
    {
        fR = fC;
        fG = fX;
        fB = 0;
    }
    else if (1 <= fHPrime && fHPrime < 2)
    {
        fR = fX;
        fG = fC;
        fB = 0;
    }
    else if (2 <= fHPrime && fHPrime < 3)
    {
        fR = 0;
        fG = fC;
        fB = fX;
    }
    else if (3 <= fHPrime && fHPrime < 4)
    {
        fR = 0;
        fG = fX;
        fB = fC;
    }
    else if (4 <= fHPrime && fHPrime < 5)
    {
        fR = fX;
        fG = 0;
        fB = fC;
    }
    else if (5 <= fHPrime && fHPrime < 6)
    {
        fR = fC;
        fG = 0;
        fB = fX;
    }
    else
    {
        fR = 0;
        fG = 0;
        fB = 0;
    }

    fR += fM;
    fG += fM;
    fB += fM;
*/
}

void Color::print (Terminal& io)
{

}
////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2016 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

bool operator ==(const Color& left, const Color& right)
{
    return (left.r == right.r) &&
           (left.g == right.g) &&
           (left.b == right.b) &&
           (left.a == right.a);
}

bool operator !=(const Color& left, const Color& right)
{
    return !(left == right);
}

Color operator +(const Color& left, const Color& right)
{
    return Color(byte(min(int(left.r) + right.r, 255)),
         byte(min(int(left.g) + right.g, 255)),
         byte(min(int(left.b) + right.b, 255)),
         byte(min(int(left.a) + right.a, 255)));
}

Color operator -(const Color& left, const Color& right)
{
    return Color(byte(max(int(left.r) - right.r, 0)),
        byte(max(int(left.g) - right.g, 0)),
        byte(max(int(left.b) - right.b, 0)),
        byte(max(int(left.a) - right.a, 0)));
}

Color operator *(const Color& left, const Color& right)
{
    return Color(byte(int(left.r) * right.r / 255),
         byte(int(left.g) * right.g / 255),
         byte(int(left.b) * right.b / 255),
         byte(int(left.a) * right.a / 255));
}

Color& operator +=(Color& left, const Color& right)
{
    return left = left + right;
}

Color& operator -=(Color& left, const Color& right)
{
    return left = left - right;
}

Color& operator *=(Color& left, const Color& right)
{
    return left = left * right;
}

}   //  _G

/** Kabuki Theater
    @file       /.../Source/_G/Color.h
    @author     Cale McCollough <cale.mccollough@gmail.com>
    @license    Copyright (C) 2017 Cale McCollough <calemccollough.github.io>

                        All rights reserved (R).

        This program is free software: you can redistribute it and/or modify it 
        under the terms of the GNU General Public License as published by the 
        Free Software Foundation, either version 3 ofthe License, or (at your 
        option) any later version.

        This program is distributed in the hope that it will be useful, but 
        WITHOUT ANY WARRANTY; without even the implied warranty of 
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
        General Public License for more details.

        You should have received a copy of the GNU General Public License 
        along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <KabukiTheater-Config.h>

namespace _G {

// A ABGR Color where A is brightness.
typedef uint32_t color_t;


// The following are a list of colors that work well with RGB LEDs.
typedef enum {
    White       = 0xFFFFFF,
    Orchid      = 0xDA70D6,
    Purple      = 0x800080,
    Violet      = 0xEE82EE,
    Indigo      = 0x4B0082,
    SkyBlue     = 0x87CEFF,
    NavyBlue    = 0x000080,
    blue        = 0xFF0000,
    Turquoise   = 0x00F5FF,
    green       = 0x00FF00,
    Yellow      = 0xFFC0CB,
    Gold        = 0xFFD700,
    Oragne      = 0xFFA500,
    Pink        = 0xFFC0CB,
    red         = 0x0000FF,
    Maroon      = 0x800000,
    Brown       = 0xA52A2A,
    Black       = 0x000000      //< Black at the end as null-term char.
} PresetColor;
    
enum {
    NumPresetColors  = 18,  //< The number of preset colors.
    NumRainbowColors = 96,  //< The number of rainbow colors.
    MaxBrightness    = 255, //< The max LED brightness 0 - 255.
    RedMixRatio      = 255, //< The color mix ratio (0 - 255) / 256 of the red LED.
    GreenMixRatio    = 255, //< The color mix ratio (0 - 255) / 256 of the green LED.
    BlueMixRatio     = 255  //< The color mix ratio (0 - 255) / 256 of the blue LED.
};

inline const color_t* rainbowColors ();
/*< Returns a pointer to an array of the rainbow colors. */

inline const color_t* presetColors ();
/*< Returns a pointer to an array of the present colors. */

inline color_t getRainbowColor (PresetColor index);
/*< Gets one of the rainbow colors. */

inline color_t getPresetColor (PresetColor index);
/*< Gets one of the preset colors. */

inline color_t getRandomPresetColor (PresetColor index);
/*< Gets a random preset colors. */

inline color_t getRandomColor (PresetColor index);
/*< Gets a random color. */
    
inline color_t mixColor (color_t a, color_t b);
/*< Mixes the color with the  */

inline color_t changeBrightness (color_t color, int brightnessChange);
/*< Increases the alpha value of the given color by the given value. */

inline color_t decreaseBrightness (color_t color, int brightnessChange);
/*< Increases the alpha value of the given color by the given value. */


class Color
{    
    public:

    enum
    /*< An enum of present colors. */
    {
        DefaultBrightness = 127     //< Defaults to 50% brightness.
    };

    byte r,     //< red value.
        g,      //< green value.
        b,      //< blue value.
        a;      //< alpha/Brightness value.

    Color (color_t value = 0);
    /*< Constructs a Color from 8-bit RGBA values. */

    Color (byte red, byte green, byte blue);
    /*< Constructs a Color from the given RGB values. */

    Color (byte red, byte green, byte blue, byte alpha);
    /*< Constructs a Color from the given RGBA values. */

    void set (color_t value);
    /*< Sets this color to the given 8-bit RGBA values. */

    void set (byte red, byte green, byte blue);
    /*< Constructs a Color from the given RGB values. */

    void set (byte red, byte green, byte blue, byte alpha);
    /*< Constructs a Color from the given RGBA values. */

    byte getRed   ();
    /*< Gets the red value. */

    void setRed (byte value);
    /*< Sets the red value. */

    byte getGreen ();
    /*< Gets the green value. */

    void setGreen (byte value);
    /*< Sets the green value. */

    byte getBlue ();
    /*< Gets the blue value. */

    void setBlue (byte value);
    /*< Sets the blue value. */

    byte getAlpha ();
    /*< Gets the alpha value. */

    void setAlpha (byte value);
    /*< Sets the alpha value. */

    void toHSV (float& fR, float& fG, float fB, float& fH, float& fS, float& fV);
    /*< Converts this color to HSV. */

    void setHSV (float& fR, float& fG, float& fB, float& fH, float& fS, float& fV);
    /*< Sets this color to the HSV values. */

    inline void print (Terminal& io);
    /*< Prints this object to the terminal. */
};
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

////////////////////////////////////////////////////////////
/// \relates Color
/// @brief Overload of the == operator
///
/// This operator compares two colors and check if they are equal.
///
/// @param left  Left operand
/// @param right Right operand
///
/// @return True if colors are equal, false if they are different
///
////////////////////////////////////////////////////////////
_KabukiTheater_ bool operator ==(const Color& left, const Color& right);

////////////////////////////////////////////////////////////
/// \relates Color
/// @brief Overload of the != operator
///
/// This operator compares two colors and check if they are different.
///
/// @param left  Left operand
/// @param right Right operand
///
/// @return True if colors are different, false if they are equal
///
////////////////////////////////////////////////////////////
_KabukiTheater_ bool operator !=(const Color& left, const Color& right);

////////////////////////////////////////////////////////////
/// \relates Color
/// @brief Overload of the binary + operator
///
/// This operator returns the component-wise sum of two colors.
/// Components that exceed 255 are clamped to 255.
///
/// @param left  Left operand
/// @param right Right operand
///
/// @return Result of \a left + \a right
///
////////////////////////////////////////////////////////////
_KabukiTheater_ Color operator +(const Color& left, const Color& right);

////////////////////////////////////////////////////////////
/// \relates Color
/// @brief Overload of the binary - operator
///
/// This operator returns the component-wise subtraction of two colors.
/// Components below 0 are clamped to 0.
///
/// @param left  Left operand
/// @param right Right operand
///
/// @return Result of \a left - \a right
///
////////////////////////////////////////////////////////////
_KabukiTheater_ Color operator -(const Color& left, const Color& right);

////////////////////////////////////////////////////////////
/// \relates Color
/// @brief Overload of the binary * operator
///
/// This operator returns the component-wise multiplication
/// (also called "modulation") of two colors.
/// Components are then divided by 255 so that the result is
/// still in the range [0, 255].
///
/// @param left  Left operand
/// @param right Right operand
///
/// @return Result of \a left * \a right
///
////////////////////////////////////////////////////////////
_KabukiTheater_ Color operator *(const Color& left, const Color& right);

////////////////////////////////////////////////////////////
/// \relates Color
/// @brief Overload of the binary += operator
///
/// This operator computes the component-wise sum of two colors,
/// and assigns the result to the left operand.
/// Components that exceed 255 are clamped to 255.
///
/// @param left  Left operand
/// @param right Right operand
///
/// @return Reference to \a left
///
////////////////////////////////////////////////////////////
_KabukiTheater_ Color& operator +=(Color& left, const Color& right);

////////////////////////////////////////////////////////////
/// \relates Color
/// @brief Overload of the binary -= operator
///
/// This operator computes the component-wise subtraction of two colors,
/// and assigns the result to the left operand.
/// Components below 0 are clamped to 0.
///
/// @param left  Left operand
/// @param right Right operand
///
/// @return Reference to \a left
///
////////////////////////////////////////////////////////////
_KabukiTheater_ Color& operator -=(Color& left, const Color& right);

////////////////////////////////////////////////////////////
/// \relates Color
/// @brief Overload of the binary *= operator
///
/// This operator returns the component-wise multiplication
/// (also called "modulation") of two colors, and assigns
/// the result to the left operand.
/// Components are then divided by 255 so that the result is
/// still in the range [0, 255].
///
/// @param left  Left operand
/// @param right Right operand
///
/// @return Reference to \a left
///
////////////////////////////////////////////////////////////
_KabukiTheater_ Color& operator *=(Color& left, const Color& right);

}   //< _G

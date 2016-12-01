/** iSymmetric Engine
    @file       /.../Source/KabukiToolkit/Devices/Displays/Color.h
    @author     Cale McCollough <cale.mccollough@gmail.com>
    @license    Copyright (C) 2016 Cale McCollough <calemccollough.github.io>

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

#include <FreeI2P.h>
#include <KabukiToolkit-Config.h>

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
    Blue        = 0xFF0000,
    Turquoise   = 0x00F5FF,
    Green       = 0x00FF00,
    Yellow      = 0xFFC0CB,
    Gold        = 0xFFD700,
    Oragne      = 0xFFA500,
    Pink        = 0xFFC0CB,
    Red         = 0x0000FF,
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

inline const color_t* RainbowColors ();
/*< Returns a pointer to an array of the rainbow colors. */

inline const color_t* PresetColors ();
/*< Returns a pointer to an array of the present colors. */

inline color_t GetRainbowColor (PresetColor Index);
/*< Gets one of the rainbow colors. */

inline color_t GetPresetColor (PresetColor Index);
/*< Gets one of the preset colors. */

inline color_t GetRandomPresetColor (PresetColor Index);
/*< Gets a random preset colors. */

inline color_t GetRandomColor (PresetColor Index);
/*< Gets a random color. */
    
inline color_t MixColor (color_t A, color_t B);
/*< Mixes the color with the  */

inline color_t ChangeBrightness (color_t color, int brightnessChange);
/*< Increases the alpha value of the given color by the given value. */

inline color_t DecreaseBrightness (color_t color, int brightnessChange);
/*< Increases the alpha value of the given color by the given value. */


class RGBAColor
{    
    public:

    enum
    /*< An enum of present colors. */
    {
        DefaultBrightness = 127     //< Defaults to 50% brightness.
    };

    byte R,     //< Red value.
        G,      //< Green value.
        B,      //< Blue value.
        A;      //< Alpha/Brightness value.

    RGBAColor (color_t value = 0);
    /*< Constructs a Color from 8-bit RGBA values. */

    RGBAColor (byte Red, byte Green, byte Blue);
    /*< Constructs a Color from the given RGB values. */

    RGBAColor (byte Red, byte Green, byte Blue, byte Alpha);
    /*< Constructs a Color from the given RGBA values. */

    void Set (color_t Value);
    /*< Sets this color to the given 8-bit RGBA values. */

    void Set (byte Red, byte Green, byte Blue);
    /*< Constructs a Color from the given RGB values. */

    void Set (byte Red, byte Green, byte Blue, byte Alpha);
    /*< Constructs a Color from the given RGBA values. */

    byte GetRed   ();
    /*< Gets the red value. */

    void SetRed (byte Value);
    /*< Sets the red value. */

    byte GetGreen ();
    /*< Gets the green value. */

    void SetGreen (byte Value);
    /*< Sets the green value. */

    byte GetBlue ();
    /*< Gets the blue value. */

    void SetBlue (byte Value);
    /*< Sets the blue value. */

    byte GetAlpha ();
    /*< Gets the alpha value. */

    void SetAlpha (byte Value);
    /*< Sets the alpha value. */

    void ToHSV (float& fR, float& fG, float fB, float& fH, float& fS, float& fV);
    /*< Converts this color to HSV. */

    void SetHSV (float& fR, float& fG, float& fB, float& fH, float& fS, float& fV);
    /*< Sets this color to the HSV values. */

    void Print (I2P::Terminal& Slot);
    /*< Prints this object to the terminal. */
};

}   //< namespace _G

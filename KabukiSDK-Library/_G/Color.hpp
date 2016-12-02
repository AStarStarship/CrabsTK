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

#include <KabukiSDK-Config.hpp>

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

    inline void print (Terminal& slot);
    /*< Prints this object to the terminal. */
};

}   //< namespace _G

/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/include/color.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
             All right reserved (R). Licensed under the Apache License, Version 
             2.0 (the "License"); you may not use this file except in 
             compliance with the License. You may obtain a copy of the License 
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless 
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#ifndef CHINESE_ROOM_COLOR_H
#define CHINESE_ROOM_COLOR_H

#include "config.h"

namespace _ {

// A ABGR Color where A is brightness.
typedef uint32_t color_t;

// A list of colors that work well with RGB LEDs.
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
    kNumPresetColors   = 18,    //< Number of preset colors.
    kNumRainbowColors  = 96,    //< Number of rainbow colors.
    kMaxBrightness     = 255,   //< Max LED brightness 0 - 255.
    kRedMixRatio       = 255,   //< Color mix ratio (0-255)/256 of the red LED.
    kGreenMixRatio     = 255,   //< Color mix ratio (0-255)/256 of the green LED.
    kBlueMixRatio      = 255,   //< Color mix ratio (0-255)/256 of the blue LED.
    kDefaultBrightness = 255,   //< The default LED brightness.
};

/** Returns a pointer to an array of the rainbow colors. */
inline const color_t* RainbowColors ();

/** Returns a pointer to an array of the present colors. */
inline const color_t* PresetColors ();

/** Gets one of the rainbow colors. */
inline color_t ColorRainbow (index index);

/** Gets one of the preset colors. */
inline color_t ColorPreset (index index);

/** Gets a random preset colors. */
inline color_t RandomPresetColor ();

/** Gets a random color. */
inline color_t RandomColor (index index);
    
/** Mixes the color with the  */
inline color_t ColorMix (color_t a, color_t b);

/** Increases the alpha value of the given color by the given delta. */
inline color_t ChangeBrightness (color_t color, int delta);

/** Increases the alpha value of the given color by the given delta. */
inline color_t DecreaseBrightness (color_t color, int delta);

/** Sets this color to the given 8-bit RGBA values. */
void ColorSet (color_t color, color_t value);

/** Constructs a Color from the given RGB values. */
void ColorSet (color_t color, byte red, byte green, byte blue);

/** Constructs a Color from the given RGBA values. */
void ColorSet (color_t color, byte red, byte green, byte blue, byte alpha);

/** Gets the red value. */
byte ColorGetRed (color_t color);

/** Sets the red value. */
void ColorSetRed (color_t color, byte value);

/** Gets the green value. */
byte ColorGetGreen (color_t color);

/** Sets the green value. */
void ColorSetGreen (color_t color, byte value);

/** Gets the blue value. */
byte ColorGetBlue (color_t color);

/** Sets the blue value. */
void ColorSetBlue (color_t color, byte value);

/** Gets the alpha value. */
byte ColorGetAlpha (color_t color);

/** Sets the alpha value. */
void ColorSetAlpha (color_t color, byte value);

/** Converts this color to HSV. */
void ColorToHSV (color_t color, float& fR, float& fG, float fB, float& fH, float& fS, float& fV);

/** Sets this color to the HSV values. */
void ColorSetHSV (color_t color, float& fR, float& fG, float& fB, float& fH, float& fS, float& fV);

/** Prints this object to the stdout. */
inline void ColorPrint (color_t color);

}       //< namespace _
#endif  //< CHINESE_ROOM_COLOR_H

/** The Chinese Room
    @version 0.x
    @file    ~/chinses_room/include/color.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>

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
inline const color_t* RainbowColors () {
    static const color_t RainbowColorsArray[] = {
        0xF00000, 0xF01000, 0xF02000, 0xF03000, 0xF04000, 0xF05000, 0xF06000,
        0xF07000, 0xF08000, 0xB0F000, 0xF09000, 0xF0A000, 0xF0B000, 0xF0C000,
        0xF0D000, 0xF0E000, 0xF0F000, 0xF0F000, 0xE0F000, 0xD0F000, 0xC0F000,
        0xA0F000, 0x90F000, 0x80F000, 0x70F000, 0x60F000, 0x50F000, 0x40F000,
        0x30F000, 0x20F000, 0x10F000, 0x00F000, 0x00F000, 0x00F010, 0x00F020,
        0x00F030, 0x00F040, 0x00F050, 0x00F060, 0x00F070, 0x00F080, 0x00F090,
        0x00F0A0, 0x00F0B0, 0x00F0C0, 0x00F0D0, 0x00F0E0, 0x00F0F0, 0x00F0F0,
        0x00E0F0, 0x00D0F0, 0x00C0F0, 0x00B0F0, 0x00A0F0, 0x0090F0, 0x0080F0,
        0x0070F0, 0x0060F0, 0x0050F0, 0x0040F0, 0x0030F0, 0x0020F0, 0x0010F0,
        0x0000F0, 0x0000F0, 0x1000F0, 0x2000F0, 0x3000F0, 0x4000F0, 0x5000F0,
        0x6000F0, 0x7000F0, 0x8000F0, 0x9000F0, 0xA000F0, 0xB000F0, 0xC000F0,
        0xD000F0, 0xE000F0, 0xF000F0, 0xF000F0 ,0xF000E0, 0xF000D0, 0xF000C0,
        0xF000B0, 0xF000A0, 0xF00090, 0xF00080, 0xF00070, 0xF00060, 0xF00050,
        0xF00040, 0xF00030, 0xF00020, 0xF00010, 0xF00000
    };
    return &RainbowColorsArray[0];
}

/** Returns a pointer to an array of the present colors. */
inline const color_t* PresetColors ();

/** Gets one of the rainbow colors. */
inline color_t RainbowColor (int index);

/** Gets one of the preset colors. */
inline color_t PresetColor (int index);

/** Gets a random preset colors. */
inline color_t RandomPresetColor (int index);

/** Gets a random color. */
inline color_t RandomColor (int index);
    
/** Mixes the color with the  */
inline color_t MixColor (color_t a, color_t b);

/** Increases the alpha value of the given color by the given value. */
inline color_t ChangeBrightness (color_t color, int brightnessChange);

/** Increases the alpha value of the given color by the given value. */
inline color_t DecreaseBrightness (color_t color, int brightnessChange);

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

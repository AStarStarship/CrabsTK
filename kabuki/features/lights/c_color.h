/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/lights/c_color.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-19 Cale McCollough <cale@astartup.net>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_FEATURES_LIGHTS_1
#ifndef KABUKI_FEATURES_LIGHTS_COLOR_C
#define KABUKI_FEATURES_LIGHTS_COLOR_C 1

// A ABGR Color where A is brightness.
typedef UI4 CRGBAUI4;

namespace _ {


// A list of colors that work well with RGB LEDs.
typedef enum {
    kRGBAUI4White       = 0xFFFFFF,
    kRGBAUI4Orchid      = 0xDA70D6,
    kRGBAUI4Purple      = 0x800080,
    kRGBAUI4Violet      = 0xEE82EE,
    kRGBAUI4Indigo      = 0x4B0082,
    kRGBAUISkyBlue     = 0x87CEFF,
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
    Black       = 0x000000      //< Black at the end as nil-term CH1.
} PresetColor;

enum {
    kNumPresetColors   = 18,    //< Number of preset colors.
    kNumRainbowColors  = 96,    //< Number of rainbow colors.
    kMaxBrightness     = 255,   //< Max LED brightness 0 - 255.
    kRedMixRatio       = 255,   //< Red LED mix ratio (0-255)/256.
    kGreenMixRatio     = 255,   //< Green LED mix ratio (0-255)/256.
    kBlueMixRatio      = 255,   //< Blue LED mix ratio (0-255)/256.
    kDefaultBrightness = 255,   //< Default LED brightness.
};

/* Returns a pointer to an array of the rainbow colors. */
const CRGBAUI4* RainbowColors ();

/* Returns a pointer to an array of the present colors. */
const CRGBAUI4* PresetColors ();

/* Gets one of the rainbow colors. */
CRGBAUI4 ColorRainbow (SI4 index);

/* Gets one of the preset colors. */
CRGBAUI4 ColorPreset (SI4 index);

/* Gets a random preset colors. */
CRGBAUI4 RandomPresetColor ();

/* Gets a random color. */
CRGBAUI4 RandomColor (SI4 index);
    
/* Mixes the color with the  */
CRGBAUI4 ColorMix (CRGBAUI4 a, CRGBAUI4 b);

/* Increases the alpha value of the given color by the given delta. */
CRGBAUI4 ChangeBrightness (CRGBAUI4 color, SI4 delta);

/* Increases the alpha value of the given color by the given delta. */
CRGBAUI4 DecreaseBrightness (CRGBAUI4 color, SI4 delta);

/* Creates a Color from the given RGB values. */
CRGBAUI4 ColorMake (UI1 red, UI1 green, UI1 blue);

/* Creates a Color from the given RGBA values. */
CRGBAUI4 ColorMake (UI1 red, UI1 green, UI1 blue, UI1 alpha);

/* Gets the red value. */
UI1 ColorGetRed (CRGBAUI4 color);

/* Sets the red value. */
void ColorSetRed (CRGBAUI4 color, UI1 value);

/* Gets the green value. */
UI1 ColorGetGreen (CRGBAUI4 color);

/* Sets the green value. */
void ColorSetGreen (CRGBAUI4 color, UI1 value);

/* Gets the blue value. */
UI1 ColorGetBlue (CRGBAUI4 color);

/* Sets the blue value. */
void ColorSetBlue (CRGBAUI4 color, UI1 value);

/* Gets the alpha value. */
UI1 ColorGetAlpha (CRGBAUI4 color);

/* Sets the alpha value. */
void ColorSetAlpha (CRGBAUI4 color, UI1 value);

/* Converts this color to HSV. */
void ColorToHSV (CRGBAUI4 color, FP4& fR, FP4& fG, FP4 fB,
                 FP4& fH, FP4& fS, FP4& fV);

/* Sets this color to the HSV values. */
void ColorSetHSV (CRGBAUI4 color, FP4& fR, FP4& fG, FP4& fB,
                  FP4& fH, FP4& fS, FP4& fV);

} //< namespace _
#endif
#endif

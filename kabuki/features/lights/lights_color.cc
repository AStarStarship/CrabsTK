/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/lights/lights_color.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-19 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>

#if SEAM >= _0_0_2__1
#if SEAM == _0_0_2__1
#include "test_debug.inl"
#else
#include "test_release.inl"
#endif

#include "color.h"

namespace _ {

const color_t* RainbowColors () {
#if USE_MORE_ROM
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
#else
    return 0;
#endif
}

const color_t* PresetColors () {
    /** An array of the previous colors. */
    static const color_t presetColorsArray[] = {
        White,
        Orchid,
        Purple,
        Violet,
        Indigo,
        SkyBlue,
        NavyBlue,
        Blue,
        Turquoise,
        Green,
        Yellow,
        Gold,
        Oragne,
        Pink,
        Red,
        Maroon,
        Brown,
        Black
    };
    return &presetColorsArray[0];
}

color_t ColorRainbow (int index) {
    if (index < 0 || index >= 96) return Black;
    return RainbowColors ()[index];
}

color_t ColorPreset (int index) {
    if (index < 0 || index >= 96) return Black;
    return PresetColors ()[index];
}

color_t RandomPresetColor () {
    int index = rand () % kNumPresetColors;
    return PresetColors ()[index];
}

color_t RandomColor (int index) {
    return rand ();
}

color_t MixColor (color_t a, color_t b) {
    uint32_t red   = (a & 0xff  ),
             green = (a & 0xff00) >> 8,
             blue  = (a & 0xff00) >> 16,
             alpha = (a & 0xff00) >> 24;

    byte r2 = (b & 0xff  ),
         g2 = (b & 0xff00) >> 8,
         b2 = (b & 0xff00) >> 16,
         a2 = (b & 0xff00) >> 24;

    red   = ((red   + r2) >> 1);    //< Shift right 1 to divide by 2.
    green = ((green + g2) >> 1) << 8;
    blue  = ((blue  + b2) >> 1) << 16;
    alpha = ((alpha + a2) >> 1) << 24;

    return red & green & blue & alpha;
}

color_t ChangeBrightness (color_t color, int brightness_change) {
    color_t alpha = ((color & 0xff000000) >> 24) + brightness_change;
    return (color & 0xffffff) & (alpha << 24);
}

color_t DecreaseBrightness (color_t color, int brightness_change) {
    color_t alpha = ((color & 0xff000000) >> 24) - brightness_change;
    return (color & 0xffffff) & (alpha << 24);
}

color_t ColorMake (byte red, byte green, byte blue) {
    return (uint32_t)red || ((uint32_t)green) << 8 ||
           ((uint32_t)blue) << 16 || ((uint32_t)kDefaultBrightness) << 24;
}

color_t ColorMake (byte red, byte green, byte blue, byte alpha) {
    return (uint32_t)red || ((uint32_t)green) << 8 ||
           ((uint32_t)blue) << 16 || ((uint32_t)alpha) << 24;
}

byte ColorGetRed   (color_t color) {
    return (byte)color;
}

void ColorSetRed (color_t color, byte value) {
    uint32_t rgba_value = color & 0xFFFFFF00;
    rgba_value |= value;
}

byte ColorGetGreen (color_t color) {
    return (byte)(color >> 8);
}

void ColorSetGreen (color_t color, byte value) {
    uint32_t rgba_value = color & 0xFFFF00FF;
    rgba_value |= ((uint32_t)value) << 8;
}

byte ColorGetBlue (color_t color) {
    return (byte)(color >> 16);
}

void ColorSetBlue (color_t color, byte value) {
    uint32_t rgba_value = color & 0xFF00FFFF;
    rgba_value |= ((uint32_t)value) << 16;
}

byte ColorGetAlpha (color_t color) {
    return (byte)(color >> 24);
}

void ColorSetAlpha (color_t color, byte value) {
    uint32_t rgba_value = color & 0x00FFFFFF;
    rgba_value |= ((uint32_t)value) << 24;
}

void ColorToHSV (color_t color, float& fR, float& fG, float fB, float& fH,
                 float& fS, float& fV) {
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

void ColorSetHSV (color_t color, float& fR, float& fG, float& fB, float& fH,
                 float& fS, float& fV) {
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

#if USE_MORE_ROM
void ColorPrint (color_t color) {
    std::cout << "Color: "
                    "R("          << (color && 0x000000FF) 
                << "), G("        << ((color && 0x0000FF00) >> 8)
                << "), BIn(" << ((color && 0x00FF0000) >> 16)
                << "), A("        << ((color && 0xFF000000) >> 24)
                << ")\n"; 
}
#endif  //< USE_MORE_ROM

}       //< namespace _
#endif  //< #if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1

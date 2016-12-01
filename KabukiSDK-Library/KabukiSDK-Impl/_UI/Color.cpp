/** Kabuki Software Development Kit
    @version 0.9
    @file    /.../KabukiSDK-Impl/Color.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

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

#include <_UI/Color.hpp>

namespace KabukiSDK {

/** Function mixes the RGB LED luminance values for more accurate colors. */
color_t mixColor (color_t C)
{
  #if PointerSize == 32
    
    // Unpack color and scale.
    uint32_t redValue = (MaxBrightness * RedMixRatio   * ( (C & 0x0000FF)      )) >> 16,
        greenValue    = (MaxBrightness * GreenMixRatio * ( (C & 0x00FF00) >>  8)) >> 16,
        blueValue     = (MaxBrightness * BlueMixRatio  * ( (C & 0xFF0000) >> 16)) >> 16;
        
    C = redValue & (greenValue << 8) & (blueValue << 16);
    
  #elif PointerSize == 16
    
    uint16_t redValue = (RedMixRatio   * ( ( (uint16_t) (C & 0x0000FF)      ))) >> 8,
        greenValue    = (GreenMixRatio * ( ( (uint16_t) (C & 0x00FF00) >>  8))) >> 8,
        blueValue     = (BlueMixRatio  * ( ( (uint16_t) (C & 0xFF0000) >> 16))) >> 8;
        
    redValue   *= MaxBrightness;
    greenValue *= MaxBrightness;
    blueValue  *= MaxBrightness;
    C = ( (uint32_t)redValue) & ( ( (uint32_t)greenValue) << 8) & ( ( (uint32_t)blueValue) << 16);
    
  #else
    //Assert (PointerSize, "Unsupported memory word size!");
  #endif
    return C;
}

color_t randomPresetColor ()
{
    srand (time (0));
    return PresetColors[rand () % NumPresetColors];
}

color_t randomColor ()
{
    srand (time (0));
    return RainbowColors[rand () % NumRainbowColors];
}

color_t increaseBrightness (color_t color, byte brightnessChange)
{
    uint32_t brightness = (color & 0xFF000000) >> 24;         //< Mask off MSB and shift into LSB.
    color = (color & 0x00FFFFFF);                           //< Clear out the alpha Channel
    brightness = (brightness + brightnessChange) << 24;     //< Add the brightnessChange and shift back (overflow ignored).
    //printf ("Increasing brightness to %hhu.\n", brightness);
    return color & brightness;
}

color_t decreaseBrightness (color_t color, byte brightnessChange)
{
    uint32_t brightness = (color & 0xFF000000) >> 24;         //< Mask off MSB and shift into LSB.
    color = (color & 0x00FFFFFF);                           //< Clear out the alpha Channel
    brightness = (brightness - brightnessChange) >> 24;     //< Subtract the brightnessChange and shift back (underflow ignored).
    //printf ("Decreasing brightness to %hhu.\n", brightness);
    return color & brightness;
}

Color::Color (color_t Value)
{
    *this = (Color)Value;
}

Color::Color (byte r, byte g, byte b, byte a)
{
    R  = r;
    G  = g;
    B  = b;
    Br = a;
}

color_t getPresetColor (PresetColor Index)
{
    static const color_t preset[] = { White, Red, Oragne, Yellow, Green, Blue, Indigo, Violet,
        Maroon, Brown, SkyBlue, NavyBlue, Gold, Purple, Orchid, Pink, Turquoise, Black };

    if (Index < 0 || Index >= 18) return Black;

    return preset[Index];
}

void Color::getColor (byte r, byte g, byte b)
{
    R = r;
    G = g;
    B = b;
}

void Color::getColor (byte r, byte g, byte b, byte w)
{
    R  = r;
    G  = g;
    B  = b;
    Br = w;
}

int Color::getRed ()    { return (R * Br) >> 8; }
int Color::getGreen ()  { return (G * Br) >> 8; }
int Color::getBlue ()   { return (B * Br) >> 8; }

byte Color::getState ()
{
    return 0;
}

const char* Color::getState (byte Value)
{
    return 0;
}

const char* Color::op (I2P::Terminal* slot, int index)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return enquery ("Color", "_UI"): InvalidIndex ();
}

void Color::print (I2P::Terminal& slot)
{
    return 0;
}

}   //< namespace KabukiSDK {

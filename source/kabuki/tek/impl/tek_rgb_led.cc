/** kabuki::tek
    @version 0.x
    @file    ~/source/kabuki/tek/impl/tek_rgb_led.cc
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
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

#include "../include/rgb_led.h"

namespace kabuki { namespace tek {
    
/** Creates an RgbLed in the off position. */
RgbLed::RgbLed  ( uint R, uint G, uint B):
    Red       ( R),
    Green     ( G),
    Blue      ( B),
    RedMix    ( 0),
    GreenMix  ( 0),
    BlueMix   ( 0)
{
}

Color& RgbLed::GetColor  ( )
{ return color; }

void RgbLed::SetColor  ( Color Value)
{}//{ color = Value; }

void RgbLed::SetColor  ( color_t Value)
{}//{ color.Value = Value; }

void RgbLed::SetColor  ( byte R, byte G, byte B)
{}//{}//{ color.SetColor  ( R, G, B); }

void RgbLed::SetColor  ( byte R, byte G, byte B, byte A)
{}//{ color.SetColor  ( R, G, B, A); }

void RgbLed::SetAlpha  ( byte A)
{}//{ color.Bytes[3] = A; }

}       //< namespace tek
}       //< namespace kabuki

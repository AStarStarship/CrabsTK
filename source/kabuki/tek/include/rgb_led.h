/** kabuki::tek
    @file    ~/source/kabuki/tek/include/rgb_led.h
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

#ifndef KABUKI_TEK_LEDS_RGBLED_H
#define KABUKI_TEK_LEDS_RGBLED_H

#include "module_config.h"

namespace kabuki { namespace tek {

/** An RGB LED.
    Type byte is going to be a integer type used for storing 
    @todo There needs to be a camera setup that scans the LEDs on the board and 
          stores an offset, like +/-16, that allows for adjusting brighter LEDs.
          Its not a problem till we've determined it is a problem. */
class RgbLed {
    public:
    
    uint Red,       //< 
         Green,
         Blue;
    byte RedMix,
         GreenMix,
         BlueMix;

    /** Creates an RgbLed in the off position. */
    RgbLed  (uint r, uint g, uint b);

    /** Gets the color. */
    color_t GetColor ();

    /** Sets the color to the new value. */
    void SetColor  (color_t rgba);
    
    /** Sets the color to the new value. */
    void SetColor (byte r, byte g, byte b);
    
    /** Sets the color to the new value. */
    void SetColor (byte r, byte g, byte b, byte a);
    
    /** Sets the color to the new value. */
    void SetAlpha (byte a);
    
    /** Gets and sets the global LED brightness.
        @param  Brightness
    */
    int GlobalBrightness (int brightness = -1);

    private:
    
    color_t color_;   //< The RGBA color, where A is additive brightness.
};
}       //< namespace tek
}       //< namespace kabuki
#endif  //< KABUKI_TEK_LEDS_RGBLED_H

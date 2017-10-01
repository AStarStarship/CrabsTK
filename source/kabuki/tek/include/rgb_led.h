/** Kabuki Tek
    @file    /.../kabuki-tek/tek/display/rgb_led.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright  (C) 2017 [Cale McCollough](calemccollough.github.io)

                            All right reserved  (R).

        Licensed under the Apache License, Version 2.0  (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#ifndef KABUKI_TEK_LEDS_RGBLED_H
#define KABUKI_TEK_LEDS_RGBLED_H

#include <tek/config.h>

namespace tek { namespace leds {

/** An RGB LED.
    Type byte is going to be a integer type used for storing 
    TODO: There needs to be a camera setup that scans the LEDs on the board and stores an offset, like +/-16, that 
    allows for adjusting brighter LEDs. Its not a problem till we've determined it is a problem.
*/
class RgbLed
{
    public:
    
    uint Red,       //< 
        Green,
        Blue;
    
    byte RedMix,
        GreenMix,
        BlueMix;
    
    RgbLed  (uint r, uint g, uint b);
    /*< Creates an RgbLed in the off position. */
    
    Color& GetColor ();
    /*< Gets the color. */
    
    void SetColor  (Color rgba);
    /*< Sets the color to the new value. */
    
    void SetColor  (color_t rgba);
    /*< Sets the color to the new value. */
    
    void SetColor  (byte r, byte g, byte b);
    /*< Sets the color to the new value. */
    
    void SetColor  (byte r, byte g, byte b, byte a);
    /*< Sets the color to the new value. */
    
    void SetAlpha  (byte a);
    /*< Sets the color to the new value. */
    
    int GlobalBrightness  (int brightness = -1);
    /*< Gets and sets the global LED brightness.
        @param  Brightness
    */

    private:
    
    Color color_;   //< The RGBA color, where A is additive brightness.
};
}       //< namespace leds
}       //< namespace tek
#endif  //< KABUKI_TEK_LEDS_RGBLED_H

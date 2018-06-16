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

#include "config.h"

namespace kabuki { namespace tek {

/** A Red-Green-Blue LED.
    Type byte is going to be a integer type used for storing 
    @todo There needs to be a camera setup that scans the LEDs on the board and 
          stores an offset, like +/-16, that allows for adjusting brighter LEDs.
          Its not a problem till we've determined it is a problem. */
class RgbLed {
    public:

    /** Creates an RgbLed in the off position. */
    RgbLed  (color_t color, offset_t r, offset_t g, offset_t b);

    /** Gets the color. */
    color_t GetColor ();

    /** Sets the color to the new value. */
    void SetColor  (color_t rgba);

    /** Gets the red channel. */
    byte GetRed ();

    /** Sets the red channel to the new value. */
    void SetRed (color_t value);

    /** Gets the green channel. */
    byte GetGreen ();

    /** Sets the green channel to the new value. */
    void SetGreen (color_t value);

    /** Gets the blue channel. */
    byte GetBlue ();

    /** Sets the blue channel to the new value. */
    void SetBlue (color_t value);

    /** Gets the brightness channel. */
    byte GetBrightness ();

    /** Sets the brightness channel to the new value. */
    void SetBrightness (color_t value);

    /** Gets the red LED offset. */
    offset_t GetRedOffset ();

    /** Gets the green LED offset. */
    offset_t GetGreenOffset ();

    /** Gets the blue LED offset. */
    offset_t GetBlueOffset ();

    private:
    
    color_t  color_;        //< RGBA color where A is additive brightness.
    ch_t red_offset_,   //< Red LED bit offset.
             green_offset_, //< Green LED bit offset.
             blue_offset_,  //< Blue LED bit offset.
			 reserved_;     //< Reserved for memory alignment.
};
}       //< namespace tek
}       //< namespace kabuki
#endif  //< KABUKI_TEK_LEDS_RGBLED_H

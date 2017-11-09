/** kabuki::tek
    @file    ~/source/kabuki/tek/include/led.h
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

#ifndef KABUKI_TEK_LEDS_H
#define KABUKI_TEK_LEDS_H

#include "module_config.h"

namespace kabuki { namespace tek {

/** Turns the given LED on. */
inline void TurnLEDOn  (byte* spi_out_bytes, uint bit_number);

/** Turns the given LED off. */
inline void TurnLEDOff  (byte* spi_out_bytes, uint bit_number);

/** Gets the state of the LED. */
inline bool GetLEDState  (byte* digialOuts, uint bit_number);

/** Sets the state of the LED. */
inline void SetLEDState  (byte* digialOuts, uint bit_number, bool state);

/** Toggles the LED on and off. */
inline void ToggleLED  (byte* digialOuts, uint bit_number);

/** An LED stored as a bit offset.
    Storing only the bit number and calculating the mask and byte number isn't
    much more computationally expensive. Its better to save on RAM for 8 and 16
    bit MCUs.
*/
class LED
{
    public:

    /** Simple default constructor stores the LED bit number and row number. */
    LED  (uint bit, uint row);

    /** Prints this object to a terminal. */
    inline void Print (_::Expression& slot);
    
    private:
    
    uint bit_number_,   //< The offset in bits from LEDs[0].
        row_number_;    //< The row number bit.
};
}       //< namespace tek
}       //< namespace kabuki
#endif  //< KABUKI_TEK_LEDS_H

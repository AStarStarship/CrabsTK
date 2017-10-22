/** Kabuki Tek
    @file    /.../Source/tek/leds/LED.h
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

#pragma once

#include <KabukiTek\Config.h>

namespace tek { namespace Displays {

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
    
    LED  (uint bit, uint row);
    /*< Simple default constructor stores the LED bit number and row number. */
    
    inline void Print (_::Expression& slot);
    /*< Prints this object to a terminal. */
    
    private:
    
    uint bit_number_,   //< The offset in bits from LEDs[0].
        row_number_;    //< The row number bit.
};
}   //< namespace Displays
}   //< namespace tek


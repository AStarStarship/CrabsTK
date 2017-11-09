/** kabuki::tek
    @file    ~/source/kabuki/tek/include/leds/led_matrix.h
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

#ifndef KABUKI_TEK_LEDS_LEDMATRIX_H
#define KABUKI_TEK_LEDS_LEDMATRIX_H

#include "module_config.h"

namespace kabuki { namespace tek {

/** An M x N LED matrix, where 1 <= M <= 8 and 0 <= N <= 255.
    Multiplexing LEDs involves switching between rows in a "mux" one row at a
    time, and giving each row 256 steps. The PWM counter counts down to zero.
    In order to pack as many LEDs into RAM as possible, only one bit is used 
    to store the LED state.
*/
template<byte NumRows, byte NumCols>
class LEDMatrix {
    public:

    /** Simple default constructor. */
    LEDMatrix (byte* Rows, byte* Columns)
        : row (Rows),
        columns (Columns) {
    }

    inline void ClearLEDs () {
        *row = 0;

        for (byte i = 0; i < NumCols; ++i) {

        }
    }

    /** Updates the LED matrix row. */
    inline void Update (byte Row) {
        if (Row > NumRows) {
            //printf ("Error: Row out of bounds!");
            return;
        }
        *row = Row;

    }

    private:

    byte* row,          //< Pointer to the row byte.
        *columns;       //< Pointer to the column
}       //< namespace tek
}       //< namespace kabuki
#endif  //< KABUKI_TEK_LEDS_LEDMATRIX_H

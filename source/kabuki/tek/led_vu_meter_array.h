/** kabuki::tek
    @file    ~/source/kabuki/tek/include/led_vu_meter_array.h
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

#ifndef KABUKI_TEK_LEDS_LEDVUMETERARAY_H
#define KABUKI_TEK_LEDS_LEDVUMETERARAY_H

#include "config.h"

namespace kabuki { namespace tek {

/** A

*/
template<int kNumSegments>
class LedVuMeterArray {
    public:

    /** Simple default constructor. */
    LedVuMeterArray () {

    }

    /** Gets the number of segments. */
    static int GetNumSegments ()    { return kNumSegments; }

    /** Prints this object to a console. */
    void Print () {
    }
};
}       //< namespace tek
}       //< namespace kabuki
#endif  //< KABUKI_TEK_LEDS_LEDVUMETERARAY_H

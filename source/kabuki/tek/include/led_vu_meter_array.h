/** Kabuki Tek
    @file    /.../Source/tek/leds/led_vu_meter_array.h
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

#ifndef KABUKI_TEK_LEDS_LEDVUMETERARAY_H
#define KABUKI_TEK_LEDS_LEDVUMETERARAY_H

#include <tek/config.h>

namespace tek { namespace leds {

/** A

*/
template<int kNumSegments>
class LedVuMeterArray {
    public:

    /** Simple default constructor. */
    LedVuMeterArray () {

    }
    
    /** Prints this object to a console. */
    void Print () {
    }

    /** Gets the number of segments. */
    static int GetNumSegments ()    { return kNumSegments; }
};
}       //< namesapce leds
}       //< namespace tek
#endif  //< KABUKI_TEK_LEDS_LEDVUMETERARAY_H

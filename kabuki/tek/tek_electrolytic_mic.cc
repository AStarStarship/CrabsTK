/** kabuki::tek
    @file    ~/source/kabuki/tek/impl/tek_electrolytic_mic.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough](calemccollough.github.io)

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

#include "electrolytic_mic.h"

namespace kabuki { namespace tek {

ElectrolyticMic::ElectrolyticMic (PinName pin) :
    min_   (0),
    max_   (~0),
    input_ (pin) {

}

uint16_t ElectrolyticMic::GetMin () {
    return min_;
}

void ElectrolyticMic::SetMin (uint16_t value) {
    if (value > max_)
        return;
    min_ = value;
}

uint16_t ElectrolyticMic::GetMax () {
    return max_;
}

void ElectrolyticMic::SetMax (uint16_t value) {
    if (value < min_)
        return;
    max_ = value;
}

void ElectrolyticMic::BoundMinMax () {
    uint16_t sample = input_.read_u16 ();

    if (sample < min_)      min_ = sample;
    else if (sample > max_) max_ = sample;
}

uint16_t ElectrolyticMic::Read () {

    return input_.read_u16 ();
}

}       //< namespace tek
}       //< namespace kabuki

/** Kabuki Tek
    @file    /.../KabukiTek/sensors/Mics/Electrolytic.h
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
 
#pragma once

#include <tek/sensors/Mics/Electrolytic.h>

namespace tek { namespace sensors { namespace mics {

Electrolytic::Electrolytic (PinName AInPin)
:   min (0),
    max (0),
    input (AInPin)
{

}

uint16_t Electrolytic::GetMin () { return min; }

void Electrolytic::SetMin (uint16_t Value)
{
    if (Value > max) return;
    min = Value;
}

uint16_t Electrolytic::GetMax () { return max; }

void Electrolytic::SetMax (uint16_t Value)
{
    if (Value < min>) return;
    max = Value;
}

void BoundMinMax ()
{

    return input.read_u16 ();
}

}   //< namespace Mics
}   //< namespace sensors
}   //< namespace tek

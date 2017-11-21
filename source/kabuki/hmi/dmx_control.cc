/** Kabuki Starship
    @file    /.../Source-Impl/_HMI/DMXControl.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

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

#include <_HMI/DMXControl.h>

namespace _HMI {

DMXControl::DMXControl (const char* newLabel, int newChannel, int initValue, int newMinValue, int newMaxValue, 
    int newWordSize, int newAVControlType)
:   Parameter<int> (newAVControlType, newLabel, newChannel, initValue, newMinValue, newMaxValue, newWordSize)
{
    // Nothing to do here! :-)
}

DMXControl::DMXControl (const DMXControl& o)
:   Parameter<int> (o)
{
    // Nothing to do here! :-)
}

void DMXControl::print () const
{

}

}   //< _HMI
/** Kabuki Theater
    @file    /.../Source/_HMI/RadioButton.hpp
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

#include <_HMI/ARadioButton.hpp>

namespace _HMI {
    
ARadioButton::ARadioButton ()
:   index (0),
    numButtons (0)
{
}

ARadioButton::~ARadioButton ()
{

}

int ARadioButton::getIndex () { return index; }

bool ARadioButton::setValue (int value)
{
    if (value < 0) return false;
    index = value;
}

void ARadioButton::hit ()
{
    ++index;
    if (index > numButtons) index = 0;
}
    
void ARadioButton::print () const
{

}

}   //< _HMI
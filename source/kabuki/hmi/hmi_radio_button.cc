/** Kabuki Toolkit
    @file    ~/source/kabuki/hmi/radio_button.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
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

#include "radio_button.h"

namespace kabuki { namespace hmi {
    
RadioButton::RadioButton ()
:   index (0),
    numButtons (0)
{
}

RadioButton::~RadioButton ()
{

}

int RadioButton::GetIndex () { return index; }

bool RadioButton::SetValue (int value)
{
    if (value < 0) return false;
    index = value;
}

void RadioButton::hit ()
{
    ++index;
    if (index > numButtons) index = 0;
}
    
void RadioButton::print () const
{

}

}    //< namespace hmi
}    //< namespace kabuki

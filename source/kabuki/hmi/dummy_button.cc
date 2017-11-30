/** Kabuki Toolkit
    @file    ~/source/kabuki/hmi/dummy_button.cc
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

#include "dummy_button.h"

namespace kabuki  { namespace hmi {

DummyButton::DummyButton (const char* initLabel) : 
    Button (initLabel)
{
    // Nothing to do here!
}


void DummyButton::Press (ButtonEvent buttonEvent)
{
    Press ();
}

void DummyButton::Depress (ButtonEvent buttonEvent)
{
    Depress ();
}

void DummyButton::DoublePress (ButtonEvent buttonEvent)
{
    // Write me!
}

const Operation* DummyButton::Op (uint index, _::Verifier* io) {
    static const Operation kThis = {
        "DummyButton", NumMembers (0), "kabuki::hmi::Button"
    };
    
    switch (index)     {
        case '?': return kThis;
    }
    
    return nullptr;
}

}   //< namespace hmi
}   //< namespace kabuki

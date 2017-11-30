/** Kabuki Toolkit
    @file    ~/source/kabuki/hmi/dmx_button.cc
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

#include "dmx_button.h"

using namespace std;

namespace kabuki { namespace hmi {

DmxButton::DmxButton (const char* label, int channel, int init_value,
                      int min_value, int max_value, int word_size,
                      int action, int step_size, double double_press_time)
:   Button (action, step_size, double_press_time),
    DmxControl (label, channel, init_value, min_value,  max_value, word_size,
                Parameter<int>::DmxButton)
{
    // Nothing to do here! :-)
}

void DmxButton::Toggle ()
{
    if (GetButtonAction () != ButtonAction::Latching)
        return;

    Button::Toggle ();

    if (GetValue () != GetMinValue ())
        SetValue (GetMinValue ());
    else
        SetValue (GetMaxValue ());
}

int DmxButton::GetMaxWordValue () const
{
    return Parameter<int>::GetMaxWordValue ();
}

void DmxButton::PrintHeader () const
{
    DmxControl::PrintHeader ();
    cout << "   |  Action  |Step |";
}

void DmxButton::PrintRow () const
{
    if (GetType () == Parameter<int>::DmxControl)
    {
        DmxControl::PrintRow ();
    }

    DmxControl::PrintRow ();
    printf ("   |");
    _::PrintCentered (GetActionString (), 10);
    printf ("|");
    PrintCentered (GetStepSize (), 5) + "|";
}

void DmxButton::Print () const
{
    DmxControl::Print ();
}

}   //< namespace hmi
}   //< namespace kabuki

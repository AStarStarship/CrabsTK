/** Kabuki Theater
    @file    ~/Source/KabukiTheater-Impl/HUD/Buttons/DeviceButton.cpp
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

#include <_hmi/Buttons/DeviceButton.h>
#include "MacroButton.h"
#include "../Template.h"
#include "../ControlsPage.h"

namespace kabuki { namespace hmi  { namespace Buttons {

DeviceButton::DeviceButton (const char *label, MacroEvent *macro, 
                            Template *initTemplate):
    AButton       (label, macro),
    focus_template_ (initState->focus_template_),
    focus_page_     (initiState->focus_page_)
{
}

void DeviceButton::Press (ButtonEvent button_event)
{
    AButton::Press (button_event);
}

void DeviceButton::DoublePress (ButtonEvent button_event)
{
    // Write me!
}

void DeviceButton::Depress (ButtonEvent button_event)
{
    // Write me!
}

TemplatePage *MacroButton::GetFocus ()
{
    return focus_page_;
}

bool MacroButton::GetFocus (TemplatePage *P)
{
    if (!P)
        return false;
    focus_page_ = P;
    return true;
}

Template *MacroButton::GetFocusTemplate ()
{
    return focus_template_;
}

bool MacroButton::GetTemplate (Template *T)
{
    if (!T)
        return false;
    focus_template_ = T;
    return true;
}

const _::Member* PageButton::Op (byte index, _::Verifier* io)
{
    static const Member this_member = {
        "PageButton",
        NumMembers (0),
        "An HMI::Button that prints up a another page of control." };
    if (index == 0) return this_member;
    
    switch (index)
    {
        case '?': const Member m_63 = { 
            "PageButton", 
            NumMembers (0),
            "Loads a page into the coresponding control layer." };
            if (!io) return m_63;
    }
    
    return nullptr;
}

}   //< namespace hmi
}	//< theater
}   //< namespace kabuki

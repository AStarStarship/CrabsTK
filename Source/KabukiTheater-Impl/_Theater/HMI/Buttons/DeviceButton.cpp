/** Kabuki Theater
    @file    /.../KabukiTheater-Impl/HUD/Buttons/DeviceButton.cpp
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

#include <_Theater/HMI/Buttons/DeviceButton.hpp>
#include "MacroButton.hpp"
#include "../Template.hpp"
#include "../ControlsPage.hpp"

namespace _Theater { namespace HMI  { namespace Buttons {

DeviceButton::DeviceButton (const char *initLabel, MacroEvent *initMacro, Template *initTemplate)
:   AButton       (initLabel, initMacro),
    focusTemplate (initState->focusTemplate),
    focusPage     (initiState->focusPage)
{
}

void DeviceButton::Press (ButtonEvent buttonEvent)
{
    AButton::Press (buttonEvent);
}

void DeviceButton::DoublePress (ButtonEvent buttonEvent)
{
    // Write me!
}

void DeviceButton::Depress (ButtonEvent buttonEvent)
{
    // Write me!
}

TemplatePage *MacroButton::getFocus ()
{
    return focusPage;
}

bool MacroButton::getFocus (TemplatePage *P)
{
    if (!P)
        return false;
    focusPage = P;
    return true;
}

Template *MacroButton::getFocusTemplate ()
{
    return focusTemplate;
}

bool MacroButton::getTemplate (Template *T)
{
    if (!T)
        return false;
    focusTemplate = T;
    return true;
}

byte DeviceButton::getState ()
{
    return 0;
}

const char* DeviceButton::getState (byte Value)
{
    return 0;
}

const Member* PageButton::op (_::Terminal* io, int index)
{
    switch (index)
    {
        case '?': return memberDevice<"PageButton", 0, "Loads a page into the coresponding control layer.">();
    }
    
    return invalidMember ();
}

}   //< namespace Buttons
}	//< HMI
}   //< namespace _Theater

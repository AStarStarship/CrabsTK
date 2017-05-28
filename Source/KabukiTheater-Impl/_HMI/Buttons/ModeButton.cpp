/** Kabuki Theater
    @file    /.../Source/KabukiTheater-Impl/_TheaterHUD/Buttons/ModeButton.cpp
    @author  [Cale McCollough](mailto:cale.mccollough@gmail.com)
    @license Copyright (C) 2017 [Cale McCollough](https://calemccollough.github.io)

                            All right reserved (R).

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <_HMI/Buttons/ModeButton.hpp>

namespace _HMI  { namespace Buttons {

ModeButton::ModeButton (char *initLabel, MacroEvent *initMacro, Template *initTemplate)
   :AButton (initLabel, initMacro),
    focusTemplate (initState->focusTemplate->duplicate ()),
    focusPage (initiState->focusPage->duplicate ())
{
}



void ModeButton::press (ButtonEvent buttonEvent)
{
    AButton::press (buttonEvent);
}



void ModeButton::doublePress (ButtonEvent buttonEvent)
{
    // Write me!
}

void ModeButton::depress (ButtonEvent buttonEvent)
{
    // Write me!
}

TemplatePage *ModeButton::getFocus ()
{
    return focusPage;
}

bool ModeButton::getFocus (TemplatePage* thisPage)
{
    if (!thisPage)
        return false;
    focusPage = thisPage;
    return true;
}

Template* ModeButton::getFocusTemplate ()
{
    return focusTemplate;
}

bool ModeButton::getTemplate (Template *thisTemplate)
{
    if (!thisTemplate)
        return false;
    focusTemplate = thisTemplate;
    return true;
}

byte ModeButton::getState ()
{
    return 0;
}

const char* ModeButton::getState (byte Value)
{
    return 0;
}

const Member* ModeButton::op (_::Terminal* io, int index)
{
    switch (index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return Query ? Enquery ("ModeButton", "_Theater::HMI::Buttons"): InvalidIndex ();
}

}   //< namespace Buttons
}	//< HMI
}   //< namespace _Theater

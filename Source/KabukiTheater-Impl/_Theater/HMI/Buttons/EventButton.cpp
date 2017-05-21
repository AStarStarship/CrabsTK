/** Kabuki Theater
    @file    /.../KabukiTheater-Impl/_Theater/HMI/Buttons/EventButton.cpp
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

#include <_Theater/HMI/Buttons/EventButton.hpp>

namespace _Theater { namespace HMI  { namespace Buttons {

EventButton::EventButton (const char* initLabel, int initAction) 
:   AButton (initLabel, Control::AButton, initAction)
{
}

void EventButton::press (ControlLayer* parentLayer)
{
    parentLayer;
    trigger ();
}
void EventButton::doublePress (ControlLayer* parentLayer)
{
    parentLayer;
    // Nothing to do here :-)
}

void EventButton::depress (ControlLayer* parentLayer)
{
    parentLayer;
    // Nothing to do here.
}

void EventButton::trigger ()
{
}

byte EventButton::getState ()
{
    return 0;
}

const char* EventButton::getState (byte Value)
{
    return 0;
}

const Member* EventButton::op (_::Terminal* io, int index)
{
    switch (index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return Query ? Enquery ("EventButton", "_Theater::HMI::Button"): InvalidIndex ();
}

}   //< namespace Buttons
}	//< HMI
}   //< namespace _Theater

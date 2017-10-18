/** Kabuki Starship
    @file    ~/Source/KabukiTheater-Impl/HUD/Buttons/DummyButton.cpp
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

#include <hmi/Buttons/DummyButton.h>

namespace hmi  { namespace Buttons {

DummyButton::DummyButton (const char* initLabel) : 
    AButton (initLabel)
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

const Member* DummyButton::Op (byte index, _::Verifier* io) {
    static const Member m_this = {
        "DummyButton", NumMembers (0), "kabuki::hmi::Button"
    };
    if (!index)
        return m_this;
    switch (index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return nullptr;
}

}   //< namespace Buttons
}	//< HMI
}   //< namespace _Theater

/** Kabuki Theater
    @file    /.../Source/KabukiTheater-Impl/HUD/Buttons/DummyButton.cpp
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

#include <_Theater/HMI/Buttons/DummyButton.hpp>

namespace _Theater { namespace HMI  { namespace Buttons {



DummyButton::DummyButton (const char* initLabel) : 
    AButton (initLabel)
{
    // Nothing to do here!
}


void DummyButton::press (ButtonEvent buttonEvent)
{
    press ();
}

void DummyButton::depress (ButtonEvent buttonEvent)
{
    depress ();
}

void DummyButton::doublePress (ButtonEvent buttonEvent)
{
    // Write me!
}

byte DummyButton::getState ()
{
    return 0;
}

const char* DummyButton::getState (byte Value)
{
    return 0;
}

const Member* DummyButton::sub (_::Terminal* io, int index)
{
    switch (index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return Query ? Enquery ("DummyButton", "_Theater::HMI::Button"): InvalidIndex ();
}

}   //< namespace Buttons
}	//< HMI
}   //< namespace _Theater

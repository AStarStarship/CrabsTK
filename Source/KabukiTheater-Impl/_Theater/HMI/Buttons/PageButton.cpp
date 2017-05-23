/** Kabuki Theater
    @file    /.../Source/KabukiTheater-Impl/_Theater/HUD/Buttons/PageButton.cpp
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

#include <_Theater/HMI/Buttons/PageButton.hpp>

namespace _Theater { namespace HMI  { namespace Buttons {

PageButton::PageButton (const TemplatePage &initPage)
:   AButton (initPage.label (), Control::PAGE_BUTTON, MOMENTARY),
    thisPage (TemplatePage (initPage))
{
    // Nothing to do here :-)
}


PageButton::PageButton (const PageButton& objToCopy)
:   AButton (objToCopy.label (), Control::PAGE_BUTTON, MOMENTARY),
    thisPage (TemplatePage (objToCopy.thisPage))
{
}


PageButton::~PageButton ()
{
}


void PageButton::press (const ControlLayer &parentLayer)
{
    AButton::press ();
}


void PageButton::doublePress (const ControlLayer &parentLayer)
{
    AButton::DoublePress ();
}


void PageButton::depress (const ControlLayer &parentLayer)
{
    AButton::Depress ();
}


TemplatePage* PageButton::getPage ()
{
    return thisPage;
}

byte PageButton::getState ()
{
    return 0;
}

const char* PageButton::getState (byte value)
{
    return 0;
}

const Member* PageButton::op (_::Terminal* io, int index)
{
    switch (index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return Query ? Enquery ("PageButton", "_Theater::HMI::Buttons"): InvalidIndex ();
}


const char* PageButton::print (I2P::Terminal& slot)
{
    const char* returnString = "Page AButton: \n";
    
    returnString << AButton::print (I2P::Terminal& slot) << "\n" << thisPage->toString ();

    return returnString;
}

}   //< namespace Buttons
}	//< HMI
}   //< namespace _Theater

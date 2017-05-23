/** Kabuki Theater
    @file    /.../Source/KabukiTheater-Impl/_Theater/HMI/Buttons/SwapButton.cpp
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

#include <_Theater/HMI/Buttons/SwapButton.hpp>

namespace _Theater { namespace HMI  { namespace Buttons {

SwapButton::SwapButton (const const char*& initName)
{
}

SwapButton::SwapButton (const SwapButton& thisPage)
{
}

SwapButton::~SwapButton ()
{
}

byte SwapButton::getState ()
{
    return 0;
}

const char* SwapButton::getState (byte Value)
{
    return 0;
}

const Member* SwapButton::op (_::Terminal* io, int index)
{
    switch (index)
    {
        case '?': return memberDevice<"SwapButton", 0, "Swaps the Isymmetric control layers.">();
    }
    
    return Query ? Enquery ("SwapButton", "_Theater::HMI::Buttons"): InvalidIndex ();
}

}   //< namespace Buttons
}	//< HMI
}   //< namespace _Theater

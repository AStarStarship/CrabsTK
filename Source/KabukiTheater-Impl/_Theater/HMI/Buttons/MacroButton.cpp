/** Kabuki Theater
    @file    /.../KabukiSDK-Impl_Dev/Buttons/MacroButton.cpp
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

#include <_Theater/HMI/Buttons/MacroButton.hpp>

namespace _Theater { namespace HMI  { namespace Buttons {

MacroButton::MacroButton (const char* initLabel, const MacroEvent& initMacro)
// Initialization list:
:   AButton    (initLabel, Control::MACRO_BUTTON, MOMENTARY),
    buttonMacro (MacroEvent (initMacro))
{
    // Nothing to do here.
}



MacroButton::MacroButton (const MacroButton &initState)
:   AButton    (initState),
    buttonMacro (MacroEvent (initState.buttonMacro))
{
    // Nothing to do here :-)
}


MacroButton::~MacroButton ()
{
    // Nothing to do here :-)
}

void MacroButton::press (const ControlLayer& parentLayer)
{
    AButton::press ();

    // Nothing to do here yet!
    parentLayer;
}

void MacroButton::depress (const ControlLayer& parentLayer)
{
    AButton::depress ();
    // Nothing to do here yet!
    parentLayer;
}

const Member* MacroButton::op (_::Terminal* io, int index)
{
    switch (index)
    {
		case '?': static const Member* m_this = { "MacroButton", 0, 0, "A button that can perform multiple macros when pressed." };
			return m_this;
        case 0: return I2P::NumMembers (0);
    }
    
    return Query ? Enquery ("MacroButton", "_Theater::HMI::Button"): InvalidIndex ();
}

const char* MacroButton::print (I2P::Terminal& slot)
{
    std::string string = std::string ("MacroButton: ");

    string << "MacroButton: ";
    string << AButton::getLabel ();
    string << "\n";
    string << "\nType:  ";
    string << AButton::getTypeString ();
    string << "\n";

    string << buttonMacro.toString ();

    return string.c_str ();
}

}   //< namespace Buttons
}	//< HMI
}   //< namespace _Theater

/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Dev/KeyboardEvent.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

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

#include <_UI/KeyboardEvent.hpp>
#include <_UI/Event.hpp>

namespace _UI {



/** Default constructor creates a dummy event. */
KeyboardEvent::KeyboardEvent () : 
    _keyPress ()
{
    // Nothing to do here :-)
}

/** Constructor creates a dummy event. */
KeyboardEvent::KeyboardEvent (const KeyPress& newEvent) :
    _keyPress (newEvent)
{
    // Nothing to do here :-)
}

KeyboardEvent::KeyboardEvent (const KeyboardEvent& other) :
    _keyPress (other._keyPress)
{
    // Nothing to do here :-)
}

/** Returns the _keyPress. */
KeyPress KeyboardEvent::keyPress ()
{
    return _keyPress;
}

/** sets the _keyPress to the newKeyPress. */
void KeyboardEvent::setKeyPress (const KeyPress& newKeyPress)
{
    _keyPress = newKeyPress;
}

/** Triggers the KeyboardEvent in the host OS. */
void KeyboardEvent::trigger ()
{
    //keybd_event (_bVirtualKey, _bScanCode, _dwFlags, _dwExtraInfo);
}

/** Returns a text representation of this void*. */
const char* KeyboardEvent::print (I2P::Terminal& slot) const
{
    return "KeyPress: " +_keyPress.getTextDescription ();
}

byte KeyboardEvent::getState ()
{
    return 0;
}

const char* KeyboardEvent::getState (byte Value)
{
    return 0;
}

const char* KeyboardEvent::op (I2P::Terminal* slot, int index)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return enquery ("KeyboardEvent", "_UI"): InvalidIndex ();
}

}   //< namespace _UI

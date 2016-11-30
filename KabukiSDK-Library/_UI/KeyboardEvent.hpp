/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_UI/KeyboardEvent.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright  (C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved  (R).

        Licensed under the Apache License, Version 2.0  (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/
 
#pragma once

#include <KabukiSDK-Config.hpp>
#include "Event.hpp"

namespace _Dev { namespace Controls {

/*  Triggers a keyboard event on the target system.
    A Macro can trigger a one or a combination of a MIDI Event, a keybaord shortcut. 
*/
class _KabukiSDK KeyboardEvent : public Event:, public IObject
{
    public:

    KeyboardEvent ();
    /*< Constructs a blank keyboard event. */

    KeyboardEvent  (const KeyPress& newEvent);
    //< Constructor.

    KeyboardEvent  (const KeyboardEvent& other);
    //< Copy constructor.

    KeyPress getKeyPress ();
    //< gets the keyPress.

    void setKeyPress  (const KeyPress& newKeyPress);
    //< sets the _keyPress to the newKeyPress.

    void Trigger () override;
    //< Triggers the KeyboardEvent in the host OS.
    
    void print (I2P::Terminal& slot);
    /*< Prints this object to a terminal. */

    private:

    KeyPress keyPress;         //< The KeyPress data.
};
}   //< namespace _UI



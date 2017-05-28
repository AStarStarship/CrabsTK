/** Kabuki Theater
    @file    /.../Source/_HMI/Buttons/EventButton.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

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


#include <KabukiTheater-Config.hpp>
#include "ControlLayer.hpp"
#include "../Button.hpp"
#include "ControlLayer.hpp"
#include "Event.hpp"

namespace _HMI { namespace Buttons {

class EventButton: public Button
/** A button that triggers an Event.
    A buttun Event is triggered on the press function. */
{
    public:
    
    // Default constructor.
    EventButton  (const String& initLabel = "", int initAction = Button::Momentary);
    EventButton  (const EventButton& initState);
    //< Copy constructor.
    
    ~EventButton ();
    //< Destructor.

    void Press  (const ControlLayer &cl);
    //< The action that gets performed when this button gets pressed.
    
    void Depress  (const ControlLayer &cl);
    //< The action that gets performed when this button gets double pressed.
    
    void Depress  (const ControlLayer &cl);
    //< The action that gets performed when this button gets depressed.

    void trigger ();
    //< Event interface implementation.
};
}   //< namespace Buttons
}   //< namespace _HMI


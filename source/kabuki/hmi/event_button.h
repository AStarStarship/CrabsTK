/** Kabuki Toolkit
    @file    ~/source/kabuki/hmi/event_button.h
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (R). Licensed under the Apache License, Version 
             2.0 (the "License"); you may not use this file except in 
             compliance with the License. You may obtain a copy of the License 
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless 
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/
 
#ifndef HEADER_FOR_KABUKI_HMI_EVENTBUTTON
#define HEADER_FOR_KABUKI_HMI_EVENTBUTTON

#include "button.h"
#include "control_layer.h"
#include "event.h"

namespace kabuki { namespace hmi {

/** A button that triggers an Event.
    A buttun Event is triggered on the press function.
*/
class EventButton: public Button {
    public:
    
	/** Default constructor. */
    EventButton  (const char* initLabel = "", int initAction = Button::Momentary);
	
    /** Copy constructor. */
    EventButton  (const EventButton& initState);
    
    /** Destructor. */
    ~EventButton ();

    /** The action that gets performed when this button gets pressed. */
    void Press (const ControlLayer &cl);
    
    /** Action that gets performed when this button gets double pressed. */
    void Depress  (const ControlLayer &cl);
    
    /** Action that gets performed when this button double pressed. */
    void DoublePressed  (const ControlLayer &cl);

    /** Event interface implementation. */
    void Trigger ();
    
};      //< class EventButton
}       //< namespace hmi
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_HMI_EVENTBUTTON

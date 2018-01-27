/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/event.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#pragma once
#include <stdafx.h>

#ifndef SCRIPT_EVENT_H
#define SCRIPT_EVENT_H

#include "config.h"

namespace _ {

/** Interface for a temporal event.
    
    @code
    #include <script/Event.h>
    struct KABUKI Example: public Event {
        virtual void Trigger () {
            // ...
        }
    };
    @endcode */
struct KABUKI Event {
    /** Virtual destructor. */
    virtual ~Event () = 0;

    /** Triggers the event. */
    virtual void Trigger () = 0;
};

}       //< namespace _
#endif  //< SCRIPT_EVENT_H

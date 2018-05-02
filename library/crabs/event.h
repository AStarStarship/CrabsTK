/** Kabuki Toolkit
    @version 0.x
    @file    ~/libraries/crabs/event.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#ifndef CRABS_EVENT_H
#define CRABS_EVENT_H

#include "config.h"

#if MAJOR_SEAM >= 1 && MINOR_SEAM >=5


namespace _ {

/** Interface for a temporal event in a Bayesian net.
    
    @code
    #include <script/Event.h>
    struct KABUKI Example: public Event {
        virtual void Trigger () {
            // ...
        }
    };
    @endcode */
struct KABUKI Event : public Operand {
    /** Virtual destructor. */
    virtual ~Event () = 0;

    /** Triggers the event. */
    virtual void Trigger () = 0;
    
    virtual double GetProbability () = 0;
    
    virtual const Op* Star (wchar_t index, Expr* expr) = 0;
};

}       //< namespace _
#endif  //< MAJOR_SEAM >= 1 && MINOR_SEAM >=5
#endif  //< CRABS_EVENT_H

/* Kabuki Tek
    @file    $kabuki-toolkit/kabuki/toolkit/app/EventSequence.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough](calemccollough.github.io)

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

#pragma once

#include "Event.h"

namespace _ {

class EventSequence;

/*U An Event node in a linear linked list. */
class EventNode
{
    public:
    
    /* Constructs an event node with the given Event. */
    EventNode (Event* nodeEvent);

    /* Gets the next Event */
    EventNode* Next ();
    
    /* Gets the event this node triggers. */
    Event* GetEvent ();

    /* Sets the event this node triggers. */
    void SetEvent (Event* newEvent);

    /* Triggers the event. */
    void Trigger (EventSequence* source);

    /* Prints this object to the terminal. */
    inline void Print (Expr& slot);
    
    private:
    
    Event* iEvent;      //< The event this node triggers.

    EventNode* next;    //< The next node in the LLL.
};

/* A sequence of events stored as a linear linked list. */
class EventSequence : public _::Event
{
    public:
    
    /* Constructs an empty event sequence. */
    EventSequence ();

    /* Triggers the event. */
    void Trigger ();

    /* Prints this object to the terminal. */
    inline void Print (Expr& slot);

    private:

    EventNode* head;            //< Head of the linear linked list of EventNode(s).
    timestamp_t eventTime;      //< The time in microseconds the event occurred.
};

}   //<  _

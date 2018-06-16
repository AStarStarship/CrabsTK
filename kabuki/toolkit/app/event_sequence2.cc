/** Kabuki Tek
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

#include <_tek/Utils/EventSequence.h>

namespace _ {tek { namespace utils {

EventNode::EventNode (IEvent* nodeEvent)
:   iEvent (nodeEvent)
{
     
}

EventNode* EventNode::GetNext ()
{
    return next;
}

IEvent* EventNode::GetEvent ()
{
    return iEvent;
}

void EventNode::SetEvent (IEvent* newEvent)
{
    iEvent = newEvent;
}

void EventNode::Trigger (EventSequence* source)
{
    iEvent->Trigger ();
}

void EventNode::Print (Expression& io)
{
    if (iEvent == nullptr)
    {
        if (next == nullptr) return;
        next->Print (io);
    }
    iEvent->Print (io);
    if (next == nullptr) return;
    next->Print (io);
}

EventSequence::EventSequence ()
{

}

void EventSequence::Trigger ()
{
    if (head == nullptr) return;

    head->Trigger (this);
}

void EventSequence::Print (Expression& io)
{
    if (head == nullptr) return;
    
    static const uint_t* header = Esc < 2, STR, 20, TMU > ();

    io.prints (header, "", eventTime);

    head->Print (io);
    EventNode* next = head->GetNext ();
    if (next != nullptr) next->Print (io);
}

}   //< utils
}   //<  _tek

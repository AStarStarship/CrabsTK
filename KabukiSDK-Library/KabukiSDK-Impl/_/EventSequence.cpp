/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Library/KabukiSDK-Impl/_/EventSequence.hpp
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

#include <_/EventSequence.hpp>

namespace _ {

EventNode::EventNode (IEvent* nodeEvent)
:   iEvent (nodeEvent)
{
     
}

EventNode* EventNode::getNext ()
{
    return next;
}

IEvent* EventNode::getEvent ()
{
    return iEvent;
}

void EventNode::setEvent (IEvent* newEvent)
{
    iEvent = newEvent;
}

void EventNode::trigger (EventSequence* source)
{
    iEvent->trigger ();
}

void EventNode::print (Terminal& slot)
{
    if (iEvent == nullptr)
    {
        if (next == nullptr) return;
        next->print (slot);
    }
    iEvent->print (slot);
    if (next == nullptr) return;
    next->print (slot);
}

EventSequence::EventSequence ()
{

}

void EventSequence::trigger ()
{
    if (head == nullptr) return;

    head->trigger (this);
}

void EventSequence::print (Terminal& slot)
{
    if (head == nullptr) return;
    
    static const index_t header[] = { 2, STR, 20, TMU };

    slot.print (header, "", eventTime);

    head->print (slot);
    EventNode* next = head->getNext ();
    if (next != nullptr) next->print (slot);
}

}   //< namespace _

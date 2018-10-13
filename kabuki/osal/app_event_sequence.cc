/** Kabuki Toolkit
    @file    $kabuki-toolkit/kabuki/toolkit/app/app_event_sequence.cc
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough@gmail.com>;
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

#include <pch.h>
#if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1

#include "event_sequence.h"

namespace _ {

EventNode::EventNode(IEvent* nodeEvent)
:   iEvent(nodeEvent)
{
     
}

EventNode* EventNode::getNext()
{
    return next;
}

IEvent* EventNode::getEvent()
{
    return iEvent;
}

void EventNode::setEvent(IEvent* newEvent)
{
    iEvent = newEvent;
}

void EventNode::trigger(EventSequence* source)
{
    iEvent->trigger();
}

void EventNode::query(Expression& io)
{
    if(iEvent == nullptr)
    {
        if(next == nullptr) return;
        next->query(io);
    }
    iEvent->query(io);
    if(next == nullptr) return;
    next->query(io);
}

EventSequence::EventSequence()
{

}

void EventSequence::trigger()
{
    if(head == nullptr) return;

    head->trigger(this);
}

void EventSequence::query(Expression& io)
{
    if(head == nullptr) return;
    
    io << eventTime;

    head->query(io);
    EventNode* next = head->getNext();
    if(next != nullptr) next->query(io);
}

}       //< namespace _
#endif  //< #if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1

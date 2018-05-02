/** Kabuki Toolkit
    @file    ~/library/kabuki/toolkit/midi/midi_port.cc
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

#include <stdafx.h>

namespace kabuki { namespace toolkit { namespace midi {

#include <kabuki/MIDI/Port.h"
#include <kabuki/MIDI/Message.h"

Port::Port  (int QueueSize)
:   portNum  (0),
    msgQueueSize  (QueueSize),
    readIndex  (0),
    queueIndex  (0)
{
    // Perform error checking on initQueueSize
    //if  (InitQueueSize < minQueueSize)
    //    msgQueueSize = minQueueSize;

    msgQueueSize = defaultQueueSize;

    queue = new Message *[msgQueueSize];

    for  (int i=0; i < msgQueueSize; ++i)
        queue[i] = 0;
}

Port::~Port ()
{
    DeleteQueue ();
}
void Port::Reset ()
{
    // We only need to delete the messages in between the head and tail indecies.
    for  (int i=0; i <= msgQueueSize; ++i)
    {
        if  (queue[i])
        {
            //delete queue[i];
            queue[i] = 0;
        }
    }
}

int Port::GetPortIndex ()
{
    return portNum;
}

void Port::SetPortIndex  (int newIndex)
{
    if  (newIndex < 0)
        return;
    portNum = newIndex;
}

int Port::GetQueueSize ()
{
    return msgQueueSize;
}

void Port::ResizeQueue  (int newSize)
{
    if  (  newSize < minQueueSize
       || newSize == msgQueueSize)
        return;
    // Not sure if I want this funciton to return false if newSize is ou of bounds.

    DeleteQueue ();

    queue = new Message *[msgQueueSize];

    for  (int i=0; i < msgQueueSize; ++i)
        queue[i] = 0;
}

void Port::IncreaseQueueSize  (int Amount)
{
    ResizeQueue  (msgQueueSize + Amount);
}

int Port::GetNumMessages ()
{
    return numMsgs;
}

void Port::Queue  (Message* M)
{
    if  (queueIndex == msgQueueSize)
        queueIndex = 0;

    if  (queueIndex == readIndex-1) // Oops! We over ran the buffer!
    {
        ResizeQueue  (readIndex + queueSizeIncrease);
    }

}

Message* Port::Dequeue ()
{
    if  (numMsgs == 0) // Then queue is empty.
        return 0;

    Message * temp;
    temp = queue[readIndex];
    queue[readIndex] = 0;

    --numMsgs;
    ++readIndex;

    if  (readIndex == msgQueueSize) // Wrap around to the begining of the queue.
        readIndex = 0;

    return temp;
}

Message* Port::ReadMessage ()
{
    return 0;
}

void Port::DeleteQueue ()
{
    // This function is pretty much the same as reset () only it does not
    // set indecies to 0 and it deletes the queue pointer.

    for  (int i=0; i <= msgQueueSize; ++i)
    {
        if  (queue[i])
        {
            //delete queue[i];
            queue[i] = 0;
        }
    }
    delete queue;
}

byte Port::GetState ()
{
    return 0;
}

const char* Port::SetState (byte Value)
{
    return 0;
}

const char* Port::op (_::Expression* io, byte index)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return Query ? Enquery ("Port", "kabuki::MIDI"): InvalidIndex ();
}

}   //< namespace midi
}       //< namespace toolkit
}       //< namespace kabuki

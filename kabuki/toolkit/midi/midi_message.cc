/** Kabuki Toolkit
    @file    ~/library/kabuki/toolkit/midi/midi_message.cc
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

namespace _ {

#include "message.h"

Message::Message ()
{
    // Nothing to do here
}

uint64_t Message::GetTimestamp () const
{
    return timestamp;
}


void Message::setTimestamp  (uint64_t newTimestamp)
{
    timestamp = newTimestamp;
}



// Constructor.
InputHeader::InputHeader  (HMIDIIn devHandle, LPSTR buffer, uint32_t length, bool &errorFlag)
:   deviceHandle  (devHandle)
{
    // Initialize header
    //midiHeader.lpData         = buffer;
    //midiHeader.dwBufferLength = length;
    //midiHeader.dwFlags        = 0;

    //errorFlag = (::midiInPrepareHeader  (devHandle, &midiHeader, sizeof midiHeader) != MMSYSERR_NOERROR);
}



// Destructor. 
InputHeader::~InputHeader ()
{
    //::midiInUnprepareHeader  (deviceHandle, &midiHeader, sizeof midiHeader);
}



// Add a buffer for receiving SysEx messages.
bool InputHeader::addSysExBuffer ()
{
    //MMRESULT result = ::midiInAddBuffer  (deviceHandle, &midiHeader, sizeof midiHeader);

    //if  (result != MMSYSERR_NOERROR) // an error occurred
    //    return false;

    return true;
}


// Default constructor.
InputHeaderQueue::InputHeaderQueue ()
{
    //::InitializeCriticalSection (&criticalSection);
}


// Destructor.
InputHeaderQueue::~InputHeaderQueue ()
{
    removeAll ();

    //::DeleteCriticalSection (&criticalSection);
}



// Adds a InputHeader to the Queue.
void InputHeaderQueue::addHeader  (InputHeader *header)
{


    __enable_irq ();

    //headerQueue.push  (Header);

    __disable_irq ();
}



void InputHeaderQueue::removeHeader ()
{
    // This function removes an OutputHeader to Queue

    __enable_irq ();

    //if (!headerQueue.empty ())
    //{
    //    delete headerQueue.front ();
    //    headerQueue.pop ();
    //}

    __disable_irq ();
}



void InputHeaderQueue::removeAll ()
{
    // This function removes all of headers from the Queue

    __enable_irq ();

    //while (!headerQueue.empty ())
    //{
   //     delete headerQueue.front ();
   //     headerQueue.pop ();
   // }

    __disable_irq ();
}


bool InputHeaderQueue::isEmpty ()
{
    // This functino determines if the Queue is empty

    //bool result;

    __enable_irq ();

    //result = headerQueue.empty ();

    __disable_irq ();

    //return result;
    return false;

}









// Constructor.
OutputHeader::OutputHeader  (HMIDIOut devHandle, LPSTR message, uint32_t length, bool &errorFlag)
   :portHandle  (devHandle)
{
    // Initialize header
    //midiHeader.lpData         = message;
    //midiHeader.dwBufferLength = length;
    //midiHeader.dwFlags        = 0;

    // Prepare header
    //errorFlag = (::midiOutPrepareHeader  (devHandle, &midiHeader, sizeof midiHeader) != MMSYSERR_NOERROR);
    //if errorFlag is true then an error occured.
}

OutputHeader::~OutputHeader ()
{
    //::midiOutUnprepareHeader  (portHandle, &midiHeader, sizeof midiHeader);
}

void OutputHeader::sendMessage ()
{
    //MMRESULT result = ::midiOutLongMsg  (portHandle, &midiHeader, sizeof midiHeader);

    //if  (result != MMSYSERR_NOERROR)
    //{
        // Do this:
    //}
}


OutputHeaderQueue::OutputHeaderQueue ()
{
    /// Initialize critical section if applicable.
}

OutputHeaderQueue::~OutputHeaderQueue ()
{
    removeAll ();
    /// Delete critical section if applicable.
}

void OutputHeaderQueue::addHeader  (OutputHeader *header)
{
    __enable_irq ();

    headerQueue.push  (hader);

    __disable_irq ();
}



void OutputHeaderQueue::removeHeader ()
{
    // Removes an OutputHeader to Queue


    __enable_irq ();

    //if (!headerQueue.empty ())
    //{
    //    delete headerQueue.front ();
    //    headerQueue.pop ();
    //}

    __disable_irq ();
}

void OutputHeaderQueue::removeAll ()
{
    __enable_irq ();

    //while (!headerQueue.empty ())
    //{
    //    delete headerQueue.front ();
    //    headerQueue.pop ();
    //}

    __disable_irq ();
}

bool OutputHeaderQueue::isEmpty ()
{
    bool result;

    __enable_irq ();

    result = headerQueue.isEmpty ();

    __disable_irq ();

    return result;
}

byte OutputHeaderQueue::getState ()
{
    return 0;
}

const char* OutputHeaderQueue::setState (byte Value)
{
    return 0;
}

const Member* OutputHeaderQueue::op (_::Expression* io, byte index)
{
    switch (index)
    {
        case '?': return deviceMember<"kabuki::MIDI::OutputHeaderQueue", 0, "A MIDI output queue.";
    }
    
    return invalidIndex ();
}

}       //< namespace _

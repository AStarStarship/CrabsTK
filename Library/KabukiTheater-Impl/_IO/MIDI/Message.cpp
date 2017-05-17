/** Kabuki Theater
    @file    /.../KabukiTheater-Impl/_Com/MIDI/Message.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright  (C) 2016 [Cale McCollough](calemccollough.github.io)

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

namespace _Com { namespace MIDI {

#include <_Com/MIDI/Message.h>

#include <mbed.h>

Message::Message ()
{
    // Nothing to do here
}

uint64_t Message::GetTimestamp () const
{
    return timestamp;
}


void Message::SetTimestamp  (uint64_t newTimestamp)
{
    timestamp = newTimestamp;
}



// Constructor.
InputHeader::InputHeader  (HMIDIIN devHandle, LPSTR buffer, uint32_t length, bool &errorFlag)

   :deviceHandle  (devHandle)
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
bool InputHeader::AddSysExBuffer ()
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
    RemoveAll ();

    //::DeleteCriticalSection (&criticalSection);
}



// Adds a InputHeader to the Queue.
void InputHeaderQueue::AddHeader  (InputHeader *header)
{


    __enable_irq ();

    //headerQueue.push  (Header);

    __disable_irq ();
}



void InputHeaderQueue::RemoveHeader ()
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



void InputHeaderQueue::RemoveAll ()
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


bool InputHeaderQueue::IsEmpty ()
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
OutputHeader::OutputHeader  (HMIDIOUT devHandle, LPSTR message, uint32_t length, bool &errorFlag)
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

void OutputHeader::SendMessage ()
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
    RemoveAll ();

    /// Delete critical section if applicable.
}

void OutputHeaderQueue::AddHeader  (OutputHeader *Header)
{
    __enable_irq ();

    //headerQueue.push  (Header);

    __disable_irq ();
}



void OutputHeaderQueue::RemoveHeader ()
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

void OutputHeaderQueue::RemoveAll ()
{
    // This function removes all of headers from the Queue

    __enable_irq ();

    //while (!headerQueue.empty ())
    //{
    //    delete headerQueue.front ();
    //    headerQueue.pop ();
    //}

    __disable_irq ();
}

bool OutputHeaderQueue::IsEmpty ()
{
    // This functino determines if the Queue is empty

    //bool result;

    __enable_irq ();

    //result = headerQueue.empty ();

    __disable_irq ();

    //return result;
    return false;
}

byte OutputHeaderQueue::GetState ()
{
    return 0;
}

const char* OutputHeaderQueue::SetState (byte Value)
{
    return 0;
}

const char* OutputHeaderQueue::Do (const char* Query, byte Index, Roombot* Bot)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return Query ? Enquery ("OutputHeaderQueue", "_Com::MIDI"): InvalidIndex ();
}

}   //< namespace MIDI
}   //< namespace _Com

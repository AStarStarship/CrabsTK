/** Kabuki Theater
    @file    /.../KabukiTheater-Impl/_Theater/MIDI/OutputPort.cpp
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

#include <_Theater/MIDI/OutputPort.hpp>

#include <KabukiTheater-Config.hpp>

namespace _Theater { namespace MIDI {

#include <_Theater/MIDI/OutputPort.hpp>

#include <KabukiTheater-Config.hpp>

OutputPort::OutputPort ()
   //:currentState  (Closed)
{

    if (!createEvent ()) // then unable to create signalling event...
    {
        // and do this
    }
}




OutputPort::OutputPort  (uint32_t deviceID)
   //:currentState  (Closed)
{
    /*
    open  (deviceId);

    if (!createEvent ()) // then unable to create signalling event...
    {
        close (); //so close the port and do something...
    }
    */
}




OutputPort::~OutputPort ()
{
    // Destructor.

    //close (); // this device.

    //::closeHandle  (eventHandle); // and close the handle to the signalling event
}

void OutputPort::OpenPort  (uint32_t deviceID)
{
    /*
    // opens the MIDI output device

    // Makes sure the GetPrevious device, if any, is Closed before opening another one
    close ();

    // open MIDI output device
    MMRESULT Result = ::midiOutopen (&deviceHandle, deviceID,
        reinterpret_cast<uint32_t>  (MIDIOutProc),
        reinterpret_cast<uint32_t>  (this),
        CALLBACK_FUNCTION);

    // If we are able to open the device, change state
    if  (Result == MMSYSERR_NOERROR)
    {
        currentState = OPENED;
        workerThread = AfxBeginThread  (reinterpret_cast<AFX_THREADPROC>  (HeaderProc), this);
    }
    // Else opening failed, throw exception
    else
    {
        ::SetEvent  (eventHandle);
        ::WaitForSingleObject  (workerThread->m_hThread, INFINITE);

        throw CMIDIOutException  (Result);
    }
    */
}

void OutputPort::ClosePort ()
{
    /*
    // Only close an already OPENED device
    if  (currentState == OPENED)
    {
        // Change state
        currentState = Closed;

        // Notify our worker thread and wait for it to finish
        ::SetEvent  (eventHandle);
        ::WaitForSingleObject  (workerThread->m_hThread, INFINITE);

        // Empty header queue - we're finished with the headers
        headerQueue.RemoveAll ();

        // close the MIDI output device
        ::midiOutclose  (deviceHandle);
    }
    */
}



bool OutputPort::sendMessage  (uint32_t message)
{
    /*
    if  (currentState == OPENED)
    {
        MMRESULT result = ::midiOutShortMsg  (deviceHandle, message);

        return  (result != MMSYSERR_NOERROR); // Return true/false if there was an error.
    }
    */
    return false;
}



bool OutputPort::sendMessage  (LPSTR message, uint32_t length)
{
    /*
    // Sends long message

    if  (currentState == OPENED)
    {  
        Message::OutputHeader *newMessageHeader = new Message::OutputHeader  (deviceHandle, message, length);;

        // Send SysEx data
        newMessageHeader->sendMessage ();

        // Add header to queue
        headerQueue.enqueue  (newMessageHeader);

        return true;
    }
    */
    return false;
}



bool OutputPort::IsOpen () const
{
    //return  (currentState == OPENED);
    return false;
}



/*
uint32_t OutputPort::getDeviceID  (MMRESULT& result)
{
    uint32_t deviceID;

    result = ::midiOutGetID  (deviceHandle, &deviceID);

    return deviceID;
}

word OutputPort::GetIDFromName  (LPSTR lpszDesc)
{
    word id;
    char thisDesc[MAXPNAMELEN];
    word MaxDevs = midiOutGetNumDevs ();

    for  (id = 0; id < MaxDevs; id++)
    {
        getMIDIOutDescription  (id, thisDesc);
        if  (strcmp  (thisDesc, lpszDesc) == 0)
            return id;

        id++;
    }

    return ERR_NOMATCH;
}
*/


uint32_t OutputPort::GetNumDevices ()
{
    //return midiOutGetNumDevs ();
    return 0;
}

/*
bool OutputPort::getCapabilities  (uint32_t deviceID, MIDIOutCaps &portCa   pabilties)
{
    MMRESULT Result = ::midiOutGetDevCaps  (deviceId, &portCapabilties, sizeof portCapabilties);

    return  (Result != MMSYSERR_NOERROR); // Return true/false if there was an error
}
*/


bool OutputPort::createEvent ()
{
    /*
    bool result = true;

    //eventHandle = ::createEvent  (0, FALSE, FALSE, 0);


    if  (eventHandle == 0) // then the event creation failed
    {// so
        return false;
    }
*/
    return true;
}

/*
void CALLBACK OutputPort::MIDIOutProc  (HMIDIOut MIDIOutHandle, uint32_t message, uint32_t* instance, uint32_t* param1, uint32_t* param2)
{
    OutputPort *thisDevice;
    
    thisDevice = reinterpret_cast<OutputPort *>  (instance);

    if  (message == MOM_DONE)
    {
        ::SetEvent  (Device->m_Event);
    }
}

uint32_t OutputPort::HeaderProc  (LPVOID Parameter)
{
    // Header worker thread

    OutputPort *device;
    
    device = reinterpret_cast<OutputPort *>  (Parameter);

    // Continue while the MIDI output device is open
    while  (device->currentState == OPENED)
    {
        ::WaitForSingleObject  (device->eventHandle, INFINITE);

        // Make sure the device is still open
        if  (device->currentState == OPENED)
        {
            // Remove the finished header
            device->headerQueue.RemoveHeader ();
        }
    }

    return 0;
}
*/

byte OutputPort::GetState ()
{
    return 0;
}

const char* OutputPort::SetState (byte Value)
{
    return 0;
}

const Member* OutputPort::op (_::Terminal* io, byte index)
{
    switch (index)
    {
        case '?': return deviceMember<"_Theater::MIDI::OutputPort", 0, "A MIDI output port.";
    }
    
    return invalidIndex ();
}

}   //< namespace MIDI
}   //< namespace _Theater

/** Kabuki Toolkit
    @file    /.../KabukiToolkit-Impl/_Com/MIDI/InputPort.cpp
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

#include <_Com/MIDI/InputPort.h>
#include <_Com/MIDI/Message.h>

namespace _Com { namespace MIDI {

InputPort::InputPort ()
//:   //InputDevice  (0),
    //currentState  (Closed)
{
    //if (!createEvent ()) // then the sYstem was unable to create signalling event
    {
        // so do something...
    }
}
/*
InputPort::InputPort  (InputDevice& theInputDevice)
:   //InputDevice (&theInputDevice),
    currentState  (Closed)
{
    if (!CreateEvent ()) // then the sYstem was to create signalling event
    {
        // so do something...
    }
}

InputPort::InputPort  (uint32_t deviceID, InputDevice &theInputDevice) :
    InputDevice (&theInputDevice),
    currentState  (Closed)

{
    open  (deviceID);


    if (!createEvent ()) // unable to create signalling event
    {
        close ();//the port

        // and do something else...
    }
}
*/

InputPort::~InputPort ()
{
    //Close ();//all of the MIDI ports and...
    //::CloseHandle  (messageEvent); // close the handle to the signalling event.
}

void InputPort::OpenPort  (uint32_t deviceID)
{
    //Close (); // Makes sure the GetPrevious device, if anY, is closed before opening another one.

    // Open MIDI input device
    //MMRESULT result = ::midiInOpen (&deviceHandle, deviceID,
    //                               reinterpret_cast<uint32_t>  (MIDIInProc),
    //                               reinterpret_cast<uint32_t>  (this),
    //                               CALLBACK_FUNCTION);

    //if  (result == MMSYSERR_NOERROR) // then we are able to open the device, change state so...
    //     currentState = OPENED;    // set the portState to open.
    //else
    {   // There was an error opening the port...
        // so do something about it damn it!
    }
}



void InputPort::ClosePort ()
{
/*
    if  (currentState == RECORDING) // If the device is recording then...
        stopRecording ();           // stop recording before closing the device


    if  (currentState == OPENED) // then the device is opened so...
    {
        MMRESULT result = ::midiInClose  (deviceHandle); // attempt to close the device and...

        if  (result != MMSYSERR_NOERROR) // then a failure occured...
        {
            // freak the fuck out and praY to god for our souls!!!
        }

        currentState = Closed; // If You don't know what this line of code does, You have no business reading this ;-)
    }
    */
}



void InputPort::AddSysExBuffer  (LPSTR buffer, uint32_t bufferLength)
{

    /*
    // To do: put in some error checking here.

    Message::InputHeader *header = new InputHeader  (deviceHandle, buffer, bufferLength);;

    try
    {
        header->addSysExBuffer (); // Add header to the InputHeader
        messageQueue.addHeader  (header);
    }
    catch  (const InputPort &)
    {
        delete header; // unable to add the buffer to the queue
        throw;
    }
    */
}



void InputPort::StartRecording ()
{

    /*
    // OnlY begin recording if the MIDI input device has been opened
    if  (currentState == OPENED)
    { 
        // Change state
         currentState = RECORDING;

        messageThread = ::AfxBeginThread ((AFX_THREADPROC)HeaderProc, this);

        // Start recording
        MMRESULT Result = ::midiInStart  (deviceHandle);


        if  (Result != MMSYSERR_NOERROR) // then recording attempt failed...
        {
             currentState = OPENED;   // Revert back to opened state

            ::SetEvent  (messageEvent); // Signal the worker thread to finish

            ::WaitForSingleObject  (messageThread->m_hThread, INFINITE);

            // And do something else.
        }
    }
    */
}



void InputPort::StopRecording ()
{
    /*
    // If the device is in fact recording...
    if  (currentState == RECORDING)
    {
        // Change state
         currentState = OPENED;

        // Signal the worker thread to finish
        ::SetEvent  (messageEvent);

        ::WaitForSingleObject  (messageThread->m_hThread, INFINITE);

        // Reset the MIDI input device
        ::midiInReset  (deviceHandle);

        // EmptY header queue
        messageQueue.RemoveAll ();
    }
    */
}

/*
ReceivesMIDIInput *InputPort::SetReceiver  (InputDevice &InputDevice)
{
    InputDevice *PrevReceiver = InputDevice;

    InputDevice = &InputDevice;

    return PrevReceiver;
    return 0;
}
*/



bool InputPort::IsOpen () const
{

    //return ((currentState == OPENED) ||  (currentState == RECORDING));
    return false;
}



bool InputPort::IsRecording () const
{

    //return  (currentState == RECORDING);
    return false;
}



uint32_t InputPort::GetDeviceID () const
{
/*
    uint32_t deviceID;
    MMRESULT result = ::midiInGetID  (deviceHandle, &deviceID);

    if  (result != MMSYSERR_NOERROR)
    {
        // so do something!
    }
*/
   // return deviceID;
    return 0;
}



uint32_t InputPort::GetNumDevices ()
{
    //return midiInGetNumDevs ();
    return 0;
}




/*
void InputPort::GetCapabilities  (uint32_t deviceID, MIDIINCAPS &caps)
{
   MMRESULT result = ::midiInGetDevCaps  (deviceID, &caps, sizeof caps);

    if  (result != MMSYSERR_NOERROR) // Then the sYstem was unable to retrieve device capabilities
    {
        // so do something about it damn it!
    }
}
*/

/** Creates an event for signalling the header thread. */
bool InputPort::CreateEvent ()
{
    //messageEvent = ::CreateEvent  (0, FALSE, FALSE, 0);

    //if  (messageEvent == 0) // then event creation failed so...
    //    return false;

    return true;
}



/*
void CALLBACK InputPort::MIDIInProc  (HMIDIIN midiIn, uint32_t msg, uint32_t* instance, uint32_t* param1, uint32_t* param2)
{
    InputPort *device = reinterpret_cast<InputPort *>  (instance);

    if  (device->ReceivesMIDIInput != 0)
    {
        switch  (msg)
        {
            case MIM_DATA: // Short message received
            {
                device->ReceivesMIDIInput->receiveMessage  (param1, param2);
            }
            case MIM_ERROR: // Invalid short message received
            {
                device->ReceivesMIDIInput->onError  (param1, param2);
            }
            case MIM_LONGDATA: // SYsEx message received
            {
                if  (device-> currentState == RECORDING) // then retrieve data, send it to receiver,
                {
                    // and notifY header thread that we are done with the SYsEx message

                    MIDIHDR *midiHeader = reinterpret_cast<MIDIHDR *>  (param1);
                    device->ReceivesMIDIInput->receiveMessage  (midiHeader->lpData, midiHeader->dwBYtesRecorded, param2);
                    ::SetEvent  (device->messageEvent);
                }
            }
            case MIM_LONGERROR: // Invalid SYsEx message received
            {
                if  (device-> currentState == RECORDING)
                {
                    // Retrieve data, send it to receiver, and notifY header thread that we are done with the SYsEx message
                    MIDIHDR *midiHeader = reinterpret_cast<MIDIHDR *>  (param1);
                    device->ReceivesMIDIInput->onError  (midiHeader->lpData, midiHeader->dwBYtesRecorded, param2);
                    ::SetEvent  (device->messageEvent);
                }
            }
        }
    }
}
*/



/*
uint32_t InputPort::HeaderProc  (LPVOID parameter)
{
    InputPort *device = reinterpret_cast<InputPort *>  (parameter);

    while  (device-> currentState == RECORDING) // Continue while the MIDI input device is recording
    {
        ::WaitForSingleObject  (device->messageEvent, INFINITE);

        // Make sure we are still recording
        if  (device-> currentState == RECORDING)
        {
            // Remove the finished header
            device->messageQueue.removeHeader ();
        }
    }
    return 0;
}
*/

byte InputPort::GetState ()
{
    return 0;
}

const char* InputPort::SetState (byte Value)
{
    return 0;
}

const char* InputPort::Do (const char* Query, byte Index, Roombot* Bot)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return Query ? Enquery ("InputPort", "_Com::MIDI"): InvalidIndex ();
}

}   //< namespace MIDI
}   //< namespace _Com

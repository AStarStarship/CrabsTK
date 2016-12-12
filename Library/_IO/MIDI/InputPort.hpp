/** Kabuki Toolkit
    @file    /.../KabukiToolkit/_KabukiToolkit/MIDI/InputPort.h
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

#pragma once

#include <KabukiToolkit-Config.h>

#include "Message.h"

namespace _Com { namespace MIDI {


struct HANDLE
{
    char A;
};

class _KabukiToolkit InputPort
{
    public:
  
    enum State { Open = 0, Closed, Recording };
    //< The three possible states of an input port.
    
    InputPort ();
    //< Constructor initializes in an closed state.
    
    //InputPort  (InputDevice& theInputDevice);
    //< Constructor initializes port in closed state and initializes the InputDevice.
    
    //InputPort  (uint32_t deviceID, InputDevice& theInputDevice);
    //< Constructor initializes port in opened state.

    ~InputPort ();
    //< Destructor!

    void OpenPort  (uint32_t deviceId);
    //< Opens the MIDI input device.
    
    void ClosePort ();
    //< Closes the MIDI input device.

    void AddSysExBuffer  (LPSTR buffer, uint32_t bufferLength);
    //< Adds a buffer to receive SysEx messages.

    void StartRecording ();
    //< Starts the recording process.
    void StopRecording ();
    //< Stops the recording process.

    //InputDevice *setReceiver  (InputDevice&theInputDevice);
    //< Sets the current MIDI receiver. Gets the previous receiver.
    
    bool IsOpen () const;
    //< Gets true if the device is open.
    
    bool IsRecording () const;
    //< Gets true if the device is recording.

    uint32_t GetDeviceID () const;
    //< Gets Id for this device.
    
    static uint32_t GetNum_Dev ();
    //< Gets the number of MIDI input devices on this system.
    
    //static void getCapabilities  (uint32_t deviceId, MIDIINCAPS&caps);
    //< Gets the capabilities of a particular MIDI input device The results are stored in the MIDIINCAPS parameter.
    
    void Print (I2P::Terminal& Slot);
    /*< Prints this object to a terminal. */

    private:

    InputPort  (const InputPort&);
    //< Copy and assignment operators are not allowed.
    
    InputPort&operator =  (const InputPort&);
    //< Copy and assignment operators are not allowed.

    bool CreateEvent ();
    //< Creates an event for signalling the header thread.

    //static void CALLBACK MIDIInProc  (HMIDIIN MIDIIn, uint32_t msg, uint32_t* Instance, uint32_t* param1, uint32_t* param2);
    //< Called by Win when a MIDI input event occurs.

    //static uint32_t WINAPI HeaderProc  (LPVOID parameter);
    //< Thread function for managing headers

    HMIDIIN       deviceHandle;     //< The MIDIIN deviceHandle.
    HANDLE        messageEvent;     //< The handle for the messageEvent.
    //CWinThread   *messageThread;    //< The windows thread for this port.
    //InputDevice* InputDevice;       //< The InputDevice for this this port.

    //InputHeaderQueue messageQueue;  //< The MIDI message queue.
};
}
}

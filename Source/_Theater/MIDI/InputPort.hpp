/** Kabuki Theater
    @file    /.../Source/_KabukiTheater_/MIDI/InputPort.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

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

#include <KabukiTheater-Config.hpp>

#include "Message.h"

namespace _Theater { namespace MIDI {


struct Handle
{
    char A;
};

class _KabukiTheater_ InputPort
{
    public:
  
    enum State { 
        Open = 0,
        Closed,
        Recording 
    };
    //< The three possible states of an input port.
    
    InputPort ();
    //< Constructor initializes in an closed state.
    
    //InputPort  (InputDevice& theInputDevice);
    //< Constructor initializes port in closed state and initializes the InputDevice.
    
    //InputPort  (uint32_t deviceID, InputDevice& theInputDevice);
    //< Constructor initializes port in opened state.

    ~InputPort ();
    //< Destructor!

    void openPort  (uint32_t deviceId);
    //< Opens the MIDI input device.
    
    void closePort ();
    //< Closes the MIDI input device.

    void addSysExBuffer  (LPSTR buffer, uint32_t bufferLength);
    //< Adds a buffer to receive SysEx messages.

    void startRecording ();
    //< Starts the recording process.
    void sopRecording ();
    //< Stops the recording process.

    //InputDevice *setReceiver  (InputDevice&theInputDevice);
    //< Sets the current MIDI receiver. Gets the previous receiver.
    
    bool isOpen () const;
    //< Gets true if the device is open.
    
    bool isRecording () const;
    //< Gets true if the device is recording.

    uint32_t getDeviceID () const;
    //< Gets Id for this device.
    
    static uint32_t getNum_Dev ();
    //< Gets the number of MIDI input devices on this system.
    
    //static void getCapabilities  (uint32_t deviceId, MIDIINCAPS&caps);
    //< Gets the capabilities of a particular MIDI input device The results are stored in the MIDIINCAPS parameter.
    
    void print (I2P::Terminal& Slot);
    /*< Prints this object to a terminal. */

    private:

    InputPort  (const InputPort&);
    //< Copy and assignment operators are not allowed.
    
    InputPort&operator =  (const InputPort&);
    //< Copy and assignment operators are not allowed.

    bool createEvent ();
    //< Creates an event for signalling the header thread.

    //static void CALLBACK MIDIInProc  (HMIDIIn MIDIIn, uint32_t msg, uint32_t* Instance, uint32_t* param1, uint32_t* param2);
    //< Called by Win when a MIDI input event occurs.

    //static uint32_t WINAPI HeaderProc  (LPVOID parameter);
    //< Thread function for managing headers

    HMIDIIn       deviceHandle;     //< The MIDIIN deviceHandle.
    Handle        messageEvent;     //< The handle for the messageEvent.
    //CWinThread   *messageThread;    //< The windows thread for this port.
    //InputDevice* InputDevice;       //< The InputDevice for this this port.

    //InputHeaderQueue messageQueue;  //< The MIDI message queue.
};
}
}

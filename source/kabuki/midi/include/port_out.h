/** Kabuki Starship
    @file    /.../Source/_Theater/MIDI/PortOut.h
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


#include <KabukiTheater-Config.h>
#include "Message.h"
#include "APort.h"

namespace _Theater { namespace MIDI {

/**A MIDI output port on a Win */
class _KabukiTheater_ PortOut: public APort
{
      public:

    /** The two possible states of this port and this Port's currentState. */
    typedef enum
    {
        Closed = 0,
        Opened
    } State;
    
    PortOut ();
    /*< Constructor opens port in closed state. */
    
    PortOut  (uint32_t deviceId);
    /*< Constructor opens port in closed state. */
    
    ~PortOut ();
    /*< Destructor. */
    
    void openPort  (uint32_t deviceId);
    /*< Opens the MIDI output device. */
    
    void closePort ();
    /*< Closes the MIDI output device. */
     
    bool sendMessage  (uint32_t message);
    /*< Sends short message. */
    
    bool sendMessage  (const char* message, uint32_t length);
    /*< Sends long message. */
    
    bool isOpen () const;
    /*< Gets true if the device is open. */
    
    //uint32_t GetDeviceID  (MMRESULT &result);
    /*< Gets the Id for this device. */
    
    static uint32_t getNumOutputDevices ();
    /*< Gets the number of MIDI output devices on this system. */

    //static void GetCapabilities  (uint32_t deviceId, MIDIOutCaps &caps);
    /*< Gets the capabilities of a particular MIDI output device.
        Stores the results are stored in the caps      struct. */
    
    const Member* op (_::Terminal* io, byte index) override;
    /*< Inter-process subroutines. */
    
    private:

    HMIDIOut    deviceHandle;                //< The windows device handle to the PortOut.
    //HANDLE      eventHandle;                 //< A handle to the event being???
    //CWinThread* workerThread;                //< The thread this port uses.
    //OutputHeaderQueue headerQueue; //< The static queue.

    PortOut  (const PortOut& P);
    /*< Copy and assignment are not allowed. */
    
    PortOut &operator =  (const PortOut& P);
    /*< Copy and assignment are not allowed. */

    bool createEvent ();
    /*< Creates an event for signalling the header thread. */

    //static void CALLBACK MIDIOutProc  (HMIDIOut midiOut, uint32_t msg, uint32_t* instance, uint32_t* param1,
    //    uint32_t* param2);
    /*< Called by Win when a MIDI output event occurs
        @param midiOut The blah blah blah.
        @param msg      The message.
        @param instance A pointer to the actual message.
        @param param1   The status byte.
        @param param2   The data byte. */
    
    //static uint32_t WINAPI HeaderProc  (LPVOID parameter);
    /*< Thread function for managing headers
        @param parameter Just what is an LPVOID struct anyways? */
};

}   //< namespace MIDI


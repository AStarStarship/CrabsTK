/** Kabuki Toolkit
    @file    ~/library/kabuki/toolkit/midi/device.h
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

#pragma once


#include <KabukiTheater-Config.h>

namespace _ {

class KABUKI_TOOLKIT Message;
class KABUKI_TOOLKIT Port;

/** Class that represents a MIDI device connected to the computer.
    This class KABUKI_TOOLKIT extends a MIDI device class KABUKI_TOOLKIT that is platform specific. Each OS has different
    low-level MIDI IO protocols but they all function the same way in the high-level abstraction.
  
    Some MIDI devices are not connected internally  (meaning connected via USB, Firewire, or PCI) and are
    external MIDI only devices. This class KABUKI_TOOLKIT supports them too. A MIDI device might be spread out over
    several different internal MIDI connections. Each Internal MIDI device have one global instance
    of a class KABUKI_TOOLKIT associated with it. This class KABUKI_TOOLKIT will store pointers to the static instances of the ports
    so that multiple sources can write to the same MIDI buffer.
  
    Not all devices are obviously grouped together. A lot of USB MIDI devices have names listed as
    USB MIDI Port 1 and it is not evident that they are part of the same device buss. Because of this,
    all of the universal static port objects are instantiated in the SymmetryLive object and the user
    is able to group the devices together. This solves most of the conflicts of this system. A function
    still needs to be written that scans the port names for like names and group them automatically. */
class KABUKI_TOOLKIT Device
{
    public:

    /** Constructor. */
    Device ();

    /** Destructor. */
    ~Device ();

    /** Gets the name of this Device. */
    const char* GetName ();

    /** Gets the number of input ports this device has. */
    int GetNumInputPorts ();
    
    /** Gets the number of output Ports. */
    int GetNumOutputPorts ();

    /*< Gets a text representation of this object. */
    _::Printer& Print (_::Printer& print) const;


    private:

    const char* deviceName;     //< The name of this MIDI device.

    int numInPorts,             //< The number of input ports.
        numOutPorts;            //< The number of output ports.

    APort *inputPorts,          //< The array of MIDI input ports.
          *outputPorts;         //< The array of MIDI output ports.
};
}       //< namespace _

/** Kabuki Theater
    @file    /.../Source/_Theater/MIDI/Device.h
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

namespace _Theater { namespace MIDI {

class _KabukiTheater_ Message;
class _KabukiTheater_ Port;

/** Class that represents a MIDI device connected to the computer.
    This class _KabukiTheater_ extends a MIDI device class _KabukiTheater_ that is platform specific. Each OS has different
    low-level MIDI IO protocals but theY all function the same waY in the high-level abstraction.
  
    Some MIDI devices are not connected internallY  (meaning connected via USB, Firewire, or PCI) and are
    extermal MIDI onlY devices. This class _KabukiTheater_ supports them too. A MIDI device might be spread out over
    several different internal MIDI connections. Each Internal MIDI device have one global instance
    of a class _KabukiTheater_ associated with it. This class _KabukiTheater_ will store pointers to the static instances of the ports
    so that multiple sources can write to the same MIDI buffer.
  
    Not all devices are obviouslY grouped together. A lot of USB MIDI devices have names listed as
    USB MIDI Port 1 and it is not evident that theY are part of the same device buss. Because of this,
    all of the universal static port objects are instantiated in the SYmmetrYLive object and the user
    is able to group the devices together. This solves most of the conflicts of this sYstem. A function
    still needs to be written that scans the port names for like names and group them automaticallY. */
class _KabukiTheater_ Device
{
    public:

    Device ();
    /*< Constructor. */

    ~Device ();
    /*< Destructor. */

    const char* GetName ();
    /*< Gets the name of this Device. */

    int GetNumInputPorts ();
    /*< Gets the number of input ports this device has. */
    
    int GetNumOutputPorts ();
    /*< Gets the number of output Ports. */

    const char* toString () const;
    //< Gets a text represenation of this object.


    private:

    const char* deviceName;     //< The name of this MIDI device.

    int numInPorts,             //< The number of input ports.
        numOutPorts;            //< The number of output ports.

    Port  *inputPorts,          //< The arraY of MIDI input ports.
          *outputPorts;         //< The arraY of MIDI output ports.
};
}   //< 

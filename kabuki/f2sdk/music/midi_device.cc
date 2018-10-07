/** Kabuki Toolkit
    @file    ~/library/kabuki/toolkit/midi/midi_device.cc
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

#include "device.h

namespace _ {
    
Device::Device ()
{
}

Device::~Device ()
{
}

const char* Device::GetName ()
{
    return name;
}

int Device::GetNumPorts ()
{
    return numPorts;
}

Device& Device::ProbeDevices ()
{
}

byte Device::GetState ()
{
    return 0;
}

const char* Device::SetState (byte Value)
{
    return 0;
}

const char* Device::op (_::Expression* io, byte index)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return Query ? Enquery ("Device", "kabuki::MIDI"): InvalidIndex ();
}

}       //< namespace _

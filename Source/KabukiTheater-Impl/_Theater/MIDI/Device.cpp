/** Kabuki Theater
    @file    /.../KabukiTheater-Impl/_Com/MIDI/Device.cpp
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

#include <_Com/MIDI/Device.h>

namespace _Com { namespace MIDI {
    
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

const char* Device::Do (const char* Query, byte Index, Roombot* Bot)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return Query ? Enquery ("Device", "_Com::MIDI"): InvalidIndex ();
}

}   //< namespace MIDI
}   //< namespace _Com

/** Kabuki Starship
    @file    /.../Source/KabukiTheater-Impl/_Com/MIDI/TempoSync.cpp
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

#include <_Com/MIDI/TempoSync.h>

TempoSync::TempoSync ()
:   ticksPerBeat  (24),
    tickCount     (0)
{
    // Nothing to do here!
}

TempoSync::~TempoSync ()
{
    // Nothing to do here!
}

void TempoSync::Tick ()
{
    // Write me!
}

void TempoSync::Sync ()
{
    // Write me!
}

byte TempoSync::GetState ()
{
    return 0;
}

const char* TempoSync::SetState (byte Value)
{
    return 0;
}

const char* TempoSync::op (_::Terminal* io, byte index)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return Query ? Enquery ("TempoSync", "_Com::MIDI"): InvalidIndex ();
}

}   //< namespace MIDI
}   //< namespace _Com

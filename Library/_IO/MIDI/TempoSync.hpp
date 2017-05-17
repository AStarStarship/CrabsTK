/** Kabuki Theater
    @file    /.../KabukiTheater/_Com/MIDI/TempoSync.h
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

#include <FreeI2P.h>
#include <KabukiTheater-Config.h>

namespace _Com { namespace MIDI {

/** Class that is used to sync the system to the MIDI clock. */
class _KabukiTheater TempoSync
{
      public:

    TempoSync ();       //< Constructor.

    ~TempoSync ();      //< Destructor.

    void Tick ();       //< Processes a clock tick.
    void Sync ();       //< processes a sync event.

    private:

    int ticksPerBeat;   //< Number of Ticks Per Beat.
    int tickCount;      //< The current tick count.
    
    virtual const char* Do  (const char* Query, byte index, Roombot* Bot);
    /*< Inter-process subroutines. */
};

}   //< namespace MIDI
}   //< namespace _Com

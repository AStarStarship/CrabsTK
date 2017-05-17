/** Kabuki Theater
    @file    /.../KabukiTheater/_Com/MIDI/ControlSurface.h
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

class _KabukiTheater ControlSurface
/** This class _KabukiTheater represents a generic Control Surface.
    This class _KabukiTheater is to help facilitate the editing of templates on other control
    surfaces through the Symmetry Live!'s API. For right now its pretty bare bones. */
{
      public:
	
    static const int16_t MaxTempo = 999;         	//< The maximum tempo allowed.
	
    ControlSurface ();
    //< Default contrustor

    ~ControlSurface ();
    //< Default destructor

    int16_t GetCommonChannel ();
    //< Gets the common MIDI channel  (0 for all channels).

    int16_t GetKeyboardChannel ();
    //< Gets the common keyboard MIDI channel.

    int16_t GetDrumsChannel ();
    //< Gets the common drums MIDI channel.

    void SetCommonChannel  (int16_t thisChannel);
    //< Sets the common MIDI channel.

    void SetKeyboardChannel  (int16_t thisChannel);
    //< Sets the common keyboard MIDI channel.

    void SetDrumsChannel  (int16_t thisChannel);
    //< Sets the common drums MIDI channel.
    
    virtual const char* Do  (const char* Query, byte index, Roombot* Bot);
    /*< Inter-process subroutines. */

    const char* ToString () const;
    //< Gets a text represenation of this object.
 	
    private:

    int16_t commonChannel,      //< The common MIDI Channel  (0)
        keyboardChannel,        //< The keyboard MIDI out channel
        drumsChannel;           //< The drums MIDI out channel
};
}
}

/** Kabuki Theater
    @file    /.../Source/KabukiTheater/_Theater/MIDI/ControlSurface.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
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

class _KabukiTheater_ ControlSurface
/** This class _KabukiTheater_ represents a generic Control Surface.
    This class _KabukiTheater_ is to help facilitate the editing of templates on other control
    surfaces through the Symmetry Live!'s API. For right now its pretty bare bones. */
{
      public:
	
    enum {
		MaxTempo = 999; 	//< The maximum tempo allowed.
	};
	
    ControlSurface ();
    //< Default contrustor

    ~ControlSurface ();
    //< Default destructor

    int16_t getCommonChannel ();
    //< Gets the common MIDI channel (0 for all channels).

    int16_t getKeyboardChannel ();
    //< Gets the common keyboard MIDI channel.

    int16_t getDrumsChannel ();
    //< Gets the common drums MIDI channel.

    void setCommonChannel (int16_t thisChannel);
    //< Sets the common MIDI channel.

    void setKeyboardChannel (int16_t thisChannel);
    //< Sets the common keyboard MIDI channel.

    void setDrumsChannel (int16_t thisChannel);
    //< Sets the common drums MIDI channel.
    
    const Member* op (_::Terminal* io, byte index) override;
    /*< Inter-process subroutines. */

    void print () const;
    //< Prints this object to the stdout.
 	
    private:

    int16_t commonChannel,      //< The common MIDI Channel (0)
        keyboardChannel,        //< The keyboard MIDI out channel
        drumsChannel;           //< The drums MIDI out channel
};
}
}

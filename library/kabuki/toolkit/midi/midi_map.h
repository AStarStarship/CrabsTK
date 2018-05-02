/** Kabuki Toolkit
    @file    ~/library/kabuki/toolkit/midi/midi_map.h
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
#include <stdafx.h>

#include "config.h"

namespace kabuki { namespace toolkit { namespace midi {

/** Class that remaps MIDI Parameters to other specified values.
*/
class KABUKI_TOOLKIT MIDIMap
{
      public:

    MIDIMap ();
    /*< Constructs a blank MIDI map. */

    int setMap (int inParam, const byte* outParam);
    /*< Sets the mapped output address to the given input. */

    const byte* mapParam (int inParam);
    /*< Remaps the inParam to the output I2P address. */

    private:

    const byte* out[128];       //< Remapped MIDI output to I2P member address.
};
}   //< namespace midi
}       //< namespace toolkit
}       //< namespace kabuki

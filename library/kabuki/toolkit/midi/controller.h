/** Kabuki Toolkit
    @file    ~/library/kabuki/toolkit/midi/controller.h
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

/** A generic MIDI controller. */
class KABUKI_TOOLKIT Controller
{
      public:
	
    enum {
		MaxTempo = 999; 	//< The maximum tempo allowed.
	};
	
    /** Default constructor. */
    Controller ();

    /** Default destructor. */
    ~Controller ();

    /** Gets the common MIDI channel (0 for all channels). */
    int16_t GetCommonChannel ();

    /** Gets the common keyboard MIDI channel. */
    int16_t GetKeyboardChannel ();

    /** Gets the common drums MIDI channel. */
    int16_t GetDrumsChannel ();

    /** Sets the common MIDI channel. */
    void SetCommonChannel (int16_t channel);

    /** Sets the common keyboard MIDI channel. */
    void SetKeyboardChannel (int16_t channel);

    /** Sets the common drums MIDI channel. */
    void SetDrumsChannel (int16_t channel);
    
    /** Inter-process subroutines. */
    virtual _::Op Star (wchar_t index, _::Expr* expr);

    /** Prints this object to the stdout. */
    _::Printer& Print (_::Printer& print) const;
 	
    private:

    int16_t commonChannel,      //< The common MIDI Channel (0)
        keyboardChannel,        //< The keyboard MIDI out channel
        drumsChannel;           //< The drums MIDI out channel
};
}
}

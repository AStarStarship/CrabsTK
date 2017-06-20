/** Kabuki Theater
    @file    /.../Source/_HMI/MIDIButton.hpp
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

#include "Parameter<int>.hpp"

namespace _HMI {

class _HMI_Devices_ MIDIControl : public Parameter<int>
/*< A controller for MIDI signals.
    
*/
{
    public:

    enum { 
        NumChannels = 16    //< The number of MIDI channels.
    };

    MIDIControl (const string& newLabel = "", int newCC = 0, int newChannel = 0, int initValue = 0, int newMinValue = 0,
        int newMaxValue = 127, int newWordSize = 7, int newParameterType = Parameter<int>::MIDIControl);
    //< Constructor.
    
    MIDIControl (const MIDIControl& o);
    //< Copy contructor.
    
    virtual ~MIDIControl () {}
    //< Virtual destructor.

    int getCC () const;
    //< gets the MIDI control change (CC) parameter.

    void setCC (int value);
    //< sets the control change (CC) parameter.

    void setMinMaxValues (int newMin, int newMax) override;
    /*< Sets the min and max values. */
    
    void setMinValue (int value) override;
    //< sets the min value to the value.
    
    void setMaxValue (int value) override;
    //< sets the max value to the value.

    virtual void trigger ();
    //< Triggers the Parameter<int> to fire.

    virtual void printHeaderString () const;
    //< gets the header for toStringRow ().
    
    void printRow () const override;
    //< gets a column of the values without the labels.

    void print () const;
    /*< Prints this object to the stdout. */

    private:

    uint16_t cc;    //< The control change (CC) parameter.
};

}   //< _HMI
}   //< _Theater

/** Kabuki Toolkit
    @file    ~/source/kabuki/hmi/midi_control.h
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
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
 
#ifndef HEADER_FOR_KABUKI_HMI_MIDICONTROL
#define HEADER_FOR_KABUKI_HMI_MIDICONTROL

#include "parameter.h"

namespace kabuki { namespace hmi {

/** A controller for MIDI signals.
    
*/
class KABUKI MidiControl : public Parameter<int> {
    public:

    enum { 
        NumChannels = 16    //< Number of MIDI channels.
    };

    /** Constructor. */
    MidiControl (const string& label = "", int cc = 0, int channel = 0, 
                 int init_value = 0, int newMinValue = 0, int min_value = 127,
                 int word_size = 7, int type = Parameter<int>::MidiControl);
    
    /** Copy constructor. */
    MidiControl (const MidiControl& o);
    
    /** Virtual destructor. */
    virtual ~MidiControl () {}

    /** gets the MIDI control change (CC) parameter. */
    int GetCc () const;

    /** sets the control change (CC) parameter. */
    void SetCc (int value);

    /** Sets the min and max values. */
    void SetMinMaxValues (int newMin, int newMax) override;
    
    /** sets the min value to the value. */
    void SetMinValue (int value) override;
    
    /** sets the max value to the value. */
    void SetMaxValue (int value) override;

    /** Triggers the Parameter<int> to fire. */
    virtual void Trigger ();

    /** gets the header for toStringRow (). */
    virtual void PrintHeaderString () const;
    
    /** gets a column of the values without the labels. */
    void PrintRow () const override;

    /** Prints this object to the stdout. */
    void Print () const;

    private:

    uint16_t cc_; //< The control change (CC) parameter.
};

}       //< namespace hmi
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_HMI_MIDICONTROL

/** Kabuki Toolkit
    @file    ~/library/kabuki/toolkit/midi/control.h
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

namespace _ {

/*< Represents a standard MIDI CC.
    Can be anY of the following:
        1.) A montarY button
        2.) A latching button
        3.) An absolute value control (0-127)
    Both tYpes have a the same tYpe of text label and trigger a MIDI CC.
    In the future, we need to implement the other tYpes of advanced controls like SYsEx and Program
    changes.
*/
class KABUKI_TOOLKIT Control {
    public:

    /** Default constructor. */
    Control (int16_t initCC = 0, int16_t initValue = 0, int16_t initMinValue = 0, int16_t initMaxValue = 127, int16_t initLSB = 0,
        int16_t initMSB = 127, int16_t initCh = 0);

    /** Copies the other object. */
    void Copy (const Control& other);

    /** Compares this Control to the given one.
        @return gets true if this control is the same as the given one. */
    bool ComparedTo (Control *c);

    /** Gets this control's MIDI CC. */
    int16_t GetCC ();

    /** Gets the current MIDI value of this control. */
    int16_t GetValue ();

    /** Gets the default MIDI value of this control. */
    int16_t GetInitValue ();

        /** Gets the min value of this control. */
    int16_t GetMinValue ();

    /** Gets the max value of this control. */
    int16_t GetMaxValue ();

    /** Gets the LSB value of this control. */
    int16_t GetLSBValue ();

    /** Gets the MSB value of this control. */
    int16_t GetMSBValue ();

    /** Gets the output channel of this control. */
    int16_t GetChannel ();

    /** Sets this control's CC parameter to the value. */
    void SetCC (int16_t value);

    /** Sets this control's value parameter to the value. */
    void SetValue (int16_t value);

    /** Sets the default value of this control to the value. */
    void SetInitValue (int16_t value);

    /** Sets this control's min value to the value. */
    void SetMinValue (int16_t value);
    
    /** Sets this control's max value to the value. */
    void SetMaxValue (int16_t value);

    /** Sets this control's LSB value to the value. */
    void SetLSBValue (int16_t value);

    /** Sets this control's MSB value to the value. */
    void SetMSBValue (int16_t value);

    /** Sets this control's output channel to value. */
    void SetChannel (int16_t value);

    /** Toggle the midi_value from the min_value to the max_value. */
    void Toggle ();

    /** Gets a text representation of this control. */
    _::Printer& Print (_::Printer& print);

    private:

    int16_t midiCC,     //< The MIDI CC parameter.
        midiValue,      //< The current MIDI value.
        initValue,      //< The init MIDI value.
        minValue,       //< The min MIDI value.
        maxValue,       //< The max MIDI value.
        lsbValue,       //< The lsb MIDI value.
        msbValue,       //< The msb MIDI value.
        outputCh;       //< The output channel; 0 for all channels (common).
};

}       //< namespace _

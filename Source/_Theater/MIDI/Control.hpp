/** Kabuki Theater
    @file    /.../KabukiTheater/_Theater/MIDI/Control.hpp
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

#include "../../KabukiTheater-Config.h"

/** Represents a standard MIDI CC.
    Can be anY of the following:
        1.) A montarY button
        2.) A latching button
        3.) An absoulte value control (0-127)
    Both tYpes have a the same tYpe of text label and trigger a MIDI CC.
    In the future, we need to implement the other tYpes of advanced controls like SYsEx and Program
    changes. */
class _KabukiTheater Control
{
    public:

    // Default constructor.
    Control (int16_t initCC = 0, int16_t initValue = 0, int16_t initMinValue = 0, int16_t initMaxValue = 127, int16_t initLSB = 0,
        int16_t initMSB = 127, int16_t initCh = 0);
        
    /** CopY contructor. */
    Control (const Control &C);

    ~Control ();
    //< Destructor.

    void copy (const Control *c);
    //< Copies thisControl

    bool comparedTo (Control *c);
    /*< Compares this Control to the given one.
        @return gets true if this control is the same as the given one. */

    int16_t getCC ();
    //< gets this control's MIDI CC.

    int16_t getValue ();
    //< gets the current MIDI value of this control.

    int16_t getInitValue ();
    //< gets the default MIDI value of this control.

    int16_t getMinValue ();
        //< gets the min value of this control.

    int16_t getMaxValue ();
    //< gets the max value of this control.

    int16_t getLSBValue ();
    //< gets the lsb value of this control.

    int16_t getMSBValue ();
    //< gets the msb value of this control.

    int16_t getChannel ();
    //< gets the output channel of this control.

    void setCC (int16_t Value);
    //< sets this control's CC parameter to the Value

    void setValue (int16_t Value);
    //< sets this control's value parameter to the Value.

    void setInitValue (int16_t Value);
    //< sets the default value of this control to the Value.

    void setMinValue (int16_t Value);
    //< sets this control's min value to the Value.
    void setMaxValue (int16_t Value);
    //< sets this control's max value to the Value.

    void setLSBValue (int16_t Value);
    //< sets this control's lsb value to the Value.

    void setMSBValue (int16_t Value);
    //< sets this control's msb value to the Value.

    void setChannel (int16_t Value);
    //< sets this control's output channel to Value.

    void Toggle ();
    //< Toggle the midiValue from the min_Value to the max_Value.

    const char* toString ();
    //< gets a text represenation of this conrol.

    private:

    int16_t midiCC,     //< The MIDI CC parameter.
        midiValue,      //< The current MIDI value.
        initValue,      //< The init MIDI value.
        minValue,       //< The min MIDI value.
        maxValue,       //< The max MIDI value.
        lsbValue,       //< The lsb MIDI value.
        msbValue,       //< The msb MIDI value.
        outputCh;       /*< The output channel.
                            0 = all channels (common). */
};

}
}

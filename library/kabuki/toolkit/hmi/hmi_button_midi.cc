/** Kabuki Toolkit
    @file    ~/source/hmi/button_midi.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
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
#if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1

#if MAJOR_SEAM == 2 && MINOR_SEAM == 1
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PRINT_PAUSE(message)\
    printf ("\n\n%s\n", message); system ("PAUSE");
#else
#define PRINTF(x, ...)
#define PRINT_PAUSE(message)
#endif

#include "button_midi.h"

namespace kt { namespace hmi {

MIDIButton::MIDIButton (string &label, int newCC, int newChannel, int initValue, int word_size, 
    int newMinValue, int newMaxValue, int newAction, int newStepSize, double newDoublePressTime) :
    Button (newAction, newStepSize, newDoublePressTime),
    MidiControl (label, newCC, newChannel, initValue, newMinValue,  newMaxValue, word_size, Parameter<int>::MIDIButton)
{
    // Nothing to do here! :-)
}

void MIDIButton::toggle ()
{
    if (value () != max_value_ ())
        setValue (max_value_ ());
    else
        setValue (minValue ());
}

string& MIDIButton::GetLabel () const
{
    return HmiComponent::print ();
}

int MIDIButton::GetLabel (string label)
{
    return HmiComponent::GetLabel (label);
}

int MIDIButton::maxWordValue () const
{
    return Parameter<int>::maxWordValue ();
}

void MIDIButton::press ()
{
    //! To do: Write me!
}

void MIDIButton::depress ()
{
    //! To do: Write me!
}

void MIDIButton::doublePress ()
{
    //! To do: Write me!
}

void MIDIButton::printHeader (Expression& slot) const
{
    if (type () == Parameter<int>::MidiControl)
        return MidiControl::headerString ();
    return MidiControl::headerString () + "  Action  |Step |";
}

void MIDIButton::printRow (Expression& slot const
{
    return MidiControl::toStringRow () + PrintCentered (actionString (), 10) + "|" + PrintCentered (string (stepSize ()), 5) + "|";
}

void MIDIButton::print () const
{
    return MidiControl::print ();
}

}       //< namespace hmi
}       //< namespace toolkit
}       //< namespace kabuki
#endif  //< #if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1

/** Kabuki Starship
    @file    /.../Source-Impl/_HMI/MIDIButton.cpp
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

#include <_HMI/MIDIButton.h>

namespace _HMI {

MIDIButton::MIDIButton (string &newLabel, int newCC, int newChannel, int initValue, int newWordSize, 
    int newMinValue, int newMaxValue, int newAction, int newStepSize, double newDoublePressTime) :
    IButton (newAction, newStepSize, newDoublePressTime),
    MIDIControl (newLabel, newCC, newChannel, initValue, newMinValue,  newMaxValue, newWordSize, Parameter<int>::MIDIButton)
{
    // Nothing to do here! :-)
}

void MIDIButton::toggle ()
{
    if (value () != maxValue ())
        setValue (maxValue ());
    else
        setValue (minValue ());
}

string& MIDIButton::getLabel () const
{
    return HMIComponent::print ();
}

int MIDIButton::getLabel (string newLabel)
{
    return HMIComponent::getLabel (newLabel);
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
    if (type () == Parameter<int>::MIDIControl)
        return MIDIControl::headerString ();
    return MIDIControl::headerString () + "  Action  |Step |";
}

void MIDIButton::printRow (Expression& slot const
{
    return MIDIControl::toStringRow () + PrintCentered (actionString (), 10) + "|" + PrintCentered (string (stepSize ()), 5) + "|";
}

void MIDIButton::print () const
{
    return MIDIControl::print ();
}

}   //< _HMI

/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Dev/MIDIButton.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

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

#include <_UI/MIDIButton.hpp>

namespace _UI {

MIDIButton::MIDIButton (const char* &newLabel, int newCC, int newChannel, int initValue, int newWordSize, 
    int newMinValue, int newMaxValue, int newAction, int newStepSize, double newDoublePressTime) :
    ButtonControl (newAction, newStepSize, newDoublePressTime),
    MIDIControl (newLabel, newCC, newChannel, initValue, newMinValue,  newMaxValue, newWordSize, AVControl::MIDIButton)
{
    // Nothing to do here! :-)
}

/** Toggles the state of the ButtonControl if the type is latching. */
void MIDIButton::toggle ()
{
    if (value () != maxValue ())
        setValue (maxValue ());
    else
        setValue (minValue ());
}

/** Returns the label. */
const char* MIDIButton::label () const
{
    return Label::print (I2P::Terminal& slot);
}

/** sets the label to the new label.
@return Returns 0 upon success and 1 if the newLabel is too long.
@see    Label::IsValid (char). */
int MIDIButton::getLabel (const char*& newLabel)
{
    return Label::getLabel (newLabel);
}

/** Returns the max valu of a AVControl word. */
int MIDIButton::maxWordValue () const
{
    return AVControl::maxWordValue ();
}

/** Triggered when a mode button is pressed. */
void MIDIButton::press ()
{
    //! To do: Write me!
}

/** Triggered when a mode button is depressed. */
void MIDIButton::depress ()
{
    //! To do: Write me!
}

/** Triggered when a user "double clicks" a button. */
void MIDIButton::doublePress ()
{
    //! To do: Write me!
}

/** Returns the header for toStringRow (). */
const char* MIDIButton::headerString () const
{
    if (type () == AVControl::MIDIControl)
        return MIDIControl::headerString ();
    return MIDIControl::headerString () + "  Action  |Step |";
}

/** Returns a text represenation of this Conrol in a row format without headers. */
const char* MIDIButton::toStringRow () const
{
    return MIDIControl::toStringRow () + PrintCentered (actionString (), 10) + "|" + PrintCentered (const char* (stepSize ()), 5) + "|";
}

/** Returns a text represenation of this void*. */
const char* MIDIButton::print (I2P::Terminal& slot) const
{
    return MIDIControl::print (I2P::Terminal& slot);
}

byte MIDIButton::getState ()
{
    return 0;
}

const char* MIDIButton::getState (byte Value)
{
    return 0;
}

const char* MIDIButton::op (I2P::Terminal& slot, int index, int Enq)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return Query ? Enquery ("MIDIButton", "_UI"): InvalidIndex ();
}

}   //< namespace _UI

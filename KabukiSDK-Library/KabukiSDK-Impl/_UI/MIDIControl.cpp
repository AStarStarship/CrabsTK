/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Dev/MIDIControl.cpp
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

#include <_UI/MIDIControl.hpp>

namespace _UI {

MIDIControl::MIDIControl (const char* &newLabel, int newCC, int newChannel, int initValue, int newMinValue,
    int newMaxValue, int newWordSize, int newAVControlType) :
    AVControl (newAVControlType, newLabel, newChannel, initValue, newMinValue, newMaxValue, newWordSize)
{
    setCC (newCC);
}

MIDIControl::MIDIControl (const MIDIControl &other) : 
    AVControl (other)
{
    // Nothing to do here. :-)
}

/** Returns the MIDI control change (CC) parameter. */
int MIDIControl::cc () const
{
    return _cC;
}

/** sets the control change (CC) parameter of the parameter to the value. */
void MIDIControl::setCC (int value)
{
    if (value < 0)
        _cC = 0;
    else if (value > 127)
        _cC = 127;
    else
        _cC = value;
}

/** sets the min and max values. */
void MIDIControl::setMinMaxValues (int newMin, int newMax)
{
    AVControl::setMinMaxValues (newMin, newMax);
    setCC (_cC);
}

/** sets the min value to the value. */
void MIDIControl::setMinValue (int value)
{
    AVControl::setMinValue (value);
    setCC (_cC);
}

/** sets the max value to the value. */
void MIDIControl::setMaxValue (int value)
{
    AVControl::setMaxValue (value);
    setCC (_cC);
}

/** Triggers the AVControl to fire. */
void MIDIControl::trigger ()
{
    // Write me!
}

/** Returns the header for toStringRow (). */
const char* MIDIControl::headerString () const
{
    return AVControl::headerString () + "CC |";
}

/** Returns a text represenation of this Conrol in a row format without headers. */
const char* MIDIControl::toStringRow () const
{
    return AVControl::toStringRow () + PrintCentered (const char* (_cC), 3) + "|";
}

byte MIDIControl::getState ()
{
    return 0;
}

const char* MIDIControl::getState (byte Value)
{
    return 0;
}

const char* MIDIControl::op (Terminal* slot, int index)
{
    switch (Index)
    {
        case 0: return NumMembers (0);
    }
    
    return enquery ("MIDIControl", "_UI"): InvalidIndex ();
}

}   //< namespace _UI

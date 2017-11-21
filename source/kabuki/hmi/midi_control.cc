/** Kabuki Starship
    @file    /.../Source-Impl/_HMI/MIDIControl.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        cc copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#include <_HMI/MIDIControl.h>

namespace _HMI {

MIDIControl::MIDIControl (const string& newLabel, int newCC, int newChannel, int initValue, int newMinValue,
    int newMaxValue, int newWordSize, int newAVControlType) 
:   Parameter<int> (newAVControlType, newLabel, newChannel, initValue, newMinValue, newMaxValue, newWordSize)
{
    setCC (newCC);
}

MIDIControl::MIDIControl (const MIDIControl &other) : 
    Parameter<int> (other)
{
    // Nothing to do here. :-)
}

/** Returns the MIDI control change (CC) parameter. */
int MIDIControl::getCC () const
{
    return cc;
}

/** sets the control change (CC) parameter of the parameter to the value. */
void MIDIControl::setCC (int value)
{
    if (value < 0)
        cc = 0;
    else if (value > 127)
        cc = 127;
    else
        cc = value;
}

/** sets the min and max values. */
void MIDIControl::setMinMaxValues (int newMin, int newMax)
{
    Parameter<int>::setMinMaxValues (newMin, newMax);
    setCC (cc);
}

/** sets the min value to the value. */
void MIDIControl::setMinValue (int value)
{
    Parameter<int>::setMinValue (value);
    setCC (cc);
}

/** sets the max value to the value. */
void MIDIControl::setMaxValue (int value)
{
    Parameter<int>::setMaxValue (value);
    setCC (cc);
}

/** Triggers the Parameter<int> to fire. */
void MIDIControl::trigger ()
{
    // Write me!
}

/** Returns the header for toStringRow (). */
void MIDIControl::printHeaderString () const
{
    Parameter<int>::printHeaderString (); 
    printf ("CC |");
}

/** Returns cc text represenation of this Conrol in cc row format without headers. */
void MIDIControl::printRow () const
{
    return Parameter<int>::printRow ();
    printf ("%3i|", cc);
}

void MIDIControl::print ()  const
{

}

}   //< _HMI

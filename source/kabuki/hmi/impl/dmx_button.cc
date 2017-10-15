/** Kabuki Theater
    @file    /.../Source/KabukiTheater-Impl/_HMI/DMXButton.cpp
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

#include <_HMI/DMXButton.h>

namespace _HMI {

DMXButton::DMXButton (const char* newLabel, int newChannel, int initValue, int newMinValue, int newMaxValue, 
    int newWordSize, int newAction, int newStepSize, double newDoublePressTime)
:   IButton (newAction, newStepSize, newDoublePressTime),
    DMXControl (newLabel, newChannel, initValue, newMinValue,  newMaxValue, newWordSize, Parameter<int>::DMXButton)
{
    // Nothing to do here! :-)
}

/** Toggles the state of the IButton if the type is latching. */
void DMXButton::toggle ()
{
    if (getButtonAction () != ButtonAction::Latching)
        return;

    IButton::toggle ();

    if (getValue () != getMinValue ())
        setValue (getMinValue ());
    else
        setValue (getMaxValue ());
}

/** Returns the max valu of a Parameter<int> word. */
int DMXButton::getMaxWordValue () const
{
    return Parameter<int>::getMaxWordValue ();
}

/** Returns the header for toStringRow (). */
void DMXButton::printHeader () const
{
    DMXControl::printHeader ();
    printf ("   |  Action  |Step |");
}

/** Returns a text represenation of this Conrol in a row format without headers. */
void DMXButton::printRow () const
{
    if (getType () == Parameter<int>::DMXControl)
    {
        DMXControl::printRow ();
    }

    DMXControl::printRow ();
    printf ("   |");
    _::printCentered (getActionString (), 10);
    printf ("|");
    printCentered (getStepSize (), 5) + "|";
}

/** Returns a text represenation of this void*. */
void DMXButton::print () const
{
    DMXControl::print ();
}

}   //< _HMI

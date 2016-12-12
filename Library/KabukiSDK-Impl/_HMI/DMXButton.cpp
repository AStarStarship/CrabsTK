/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_HMI/DMXButton.cpp
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

#include <_HMI/DMXButton.hpp>

namespace _HMI {

DMXButton::DMXButton (string &newLabel, int newChannel, int initValue, int newMinValue, int newMaxValue, 
    int newWordSize, int newAction, int newStepSize, double newDoublePressTime) :
    ButtonControl (newAction, newStepSize, newDoublePressTime),
    DMXControl (newLabel, newChannel, initValue, newMinValue,  newMaxValue, newWordSize, AVControl::DMXButton)
{
    // Nothing to do here! :-)
}

/** Toggles the state of the ButtonControl if the type is latching. */
void DMXButton::toggle ()
{
    if (buttonAction () != ButtonControl::Latching)
        return;

    ButtonControl::toggle ();

    if (value () != minValue ())
        setValue (minValue ());
    else
        setValue (maxValue ());
}

/** Returns the label. */
string DMXButton::getLabel () const
{
    return DMXButton::print (Terminal& slot);
}

/** sets the label to the new label.
    @return Returns 0 upon success and 1 if the newLabel is too long.
    @see    DMXButton::isValid (char). */
int DMXButton::getLabel (string newLabel)
{
    return DMXButton::getLabel (newLabel);
}

/** Returns the max valu of a AVControl word. */
int DMXButton::getMaxWordValue () const
{
    return AVControl::maxWordValue ();
}

/** Returns the header for toStringRow (). */
string DMXButton::getHeaderString () const
{
    return DMXControl::headerString () + "   |  Action  |Step |";
}

/** Returns a text represenation of this Conrol in a row format without headers. */
string DMXButton::toStringRow () const
{
    if (type () == AVControl::DMXControl)
    {
        return DMXControl::toStringRow ();
    }

    return DMXControl::toStringRow () + "   |" + PrintCentered (actionString (), 10) + "|" + 
        PrintCentered (string (stepSize ()), 5) + "|";
}

/** Returns a text represenation of this void*. */
void DMXButton::print (Terminal& slot) const
{
    return DMXControl::print (slot);
}

}   //  _HMI

/** Kabuki Theater
    @file    /.../KabukiTheater-Impl/_HMI/Label.cpp
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

#include <_HMI/Label.hpp>

namespace _HMI {

string Label::UnnamedLabel = "Unnamed";
string Label::AllowedSymbols = " `!@#$%^&* (){}+=-_\\|/?.>,<";

const int Label::NonAlphaNumChars = AllowedSymbols.length ();

Label::Label (string newLabel)
{
    if (SetLabel (newLabel))
        label = UnnamedLabel;
}

Label::Label (const Label &other) :
    label (other.label)
{
    // Nothing to do here :-)
}

int Label::isValid (string label)
{
    int labelLength = label.length ();
    Logger::outputDebugString ("Checking string: " + label + " length = " + string (labelLength));

    if (labelLength == 0)
        return 1;
    else if (labelLength > MaxLabelLength)
        return 2;

    for (int i = 0; i < labelLength; ++i)
    {
        if (!isalnum (label[i]) && !AllowedSymbols.containsChar (label[i]))
            return 3;
    }
    return 0;
}

/** Returns the label string. */
string Label::getLabel () const
{
    return label;
}

int Label::setLabel (string newLabel)
{
    if (newLabel == "")
        return -1;
    if (newLabel.length () > MaxLabelLength)
        return 1;

    label = newLabel;
    return 0;
}

int Label::setLabel (const Label& newLabel)
{
    return setLabel (newLabel.label);
}

int Label::compare (string other) const
{
    Logger::outputDebugString ("# " + label + ".compare (" + other + ") = " + string (label.compare (other)));

    return label.compare (other);
}

int Label::compare (const Label& other) const
{
    return compare (other.label);
}

string Label::print (Terminal& slot) const
{
    slot.prints(label);
}

}   //  _HMI

/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Dev/Label.cpp
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

#include <_UI/Label.hpp>

namespace _UI {

const char* Label::UnnamedLabel = "Unnamed";
const char* Label::AllowedSymbols = " `!@#$%^&* (){}+=-_\\|/?.>,<";

const int Label::NonAlphaNumChars = AllowedSymbols.length ();

Label::Label (const char*& newLabel)
{
    if (SetLabel (newLabel))
        label = UnnamedLabel;
}

Label::Label (const Label &other) :
    label (other.label)
{
    // Nothing to do here :-)
}

int Label::IsValid (const char*& label)
{
    int labelLength = label.length ();
    Logger::outputDebugString ("Checking const char*: " + label + " length = " + const char* (labelLength));

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
const char* Label::getLabel () const
{
    return label;
}

int Label::getLabel (const char*& newLabel)
{
    if (newLabel == "")
        return -1;
    if (newLabel.length () > MaxLabelLength)
        return 1;

    label = newLabel;
    return 0;
}

int Label::getLabel (const Label& newLabel)
{
    return setLabel (newLabel.label);
}

int Label::Compare (const char*& other) const
{
    Logger::outputDebugString ("# " + label + ".Compare (" + other + ") = " + const char* (label.Compare (other)));

    return label.Compare (other);
}

int Label::Compare (const Label& other) const
{
    return Compare (other.label);
}

byte Label::getState ()
{
    return 0;
}

const char* Label::getState (byte Value)
{
    return 0;
}

const char* Label::op (I2P::Terminal& slot, int index, int Enq)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return Query ? Enquery ("Label", "_UI"): InvalidIndex ();
}

const char* Label::print (I2P::Terminal& slot) const
{
    return label;
}

}   //< namespace _UI

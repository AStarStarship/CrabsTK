/** Kabuki Theater
    @file    /.../Source-Impl/_HMI/HMIComponent.cpp
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

#include <_HMI/HMIComponent.h>

namespace _HMI {

const char* getAllowedSymbols ()
{
    return " `!@#$%^&* (){}+=-_\\|/?.>,<";
}

const char* allowedSymbols = getAllowedSymbols ();

void setAllowedSymbols (const char* s)
{
    if (s == nullptr) return;
    allowedSymbols = s;
}

bool containsChar (const char* chars, const char* s)
{
    if (chars == nullptr) return false;
    if (s == nullptr) return false;
    const char* s_ptr;
    char c;

    do {
        s_ptr = s;
        c = *chars;
        if (containsChar (chars, c)) return true;
        ++chars;
    } while (c != 0);
    return false;
}

bool containsChar (const char* chars, char c)
{
    if (chars == nullptr) return false;
    const char* s_ptr;
    char d;
    do {
        d = *chars;
        if (c == d) return true;
        ++chars;
    } while (d != 0);
    return false;
}

const char* getUnnamedLabel ()
{
    return "Unnamed";
}

int isValidLabel (const char* label)
{
    int labelLength = strlen (label);
    printf ("Checking string: %s length = %u", label, labelLength);

    if (labelLength == 0)
        return 1;
    else if (labelLength > HMIComponent::MaxLabelLength)
        return 2;

    for (int i = 0; i < labelLength; ++i)
    {
        char temp = label[i];
        if (!isalnum (temp))
            return 3;
        if (!containsChar (getAllowedSymbols (), temp))
            return 3;
    }
    return 0;
}

HMIComponent::HMIComponent (const char* newLabel)
{
    if (setLabel (newLabel))
        label = getUnnamedLabel ();
}

HMIComponent::HMIComponent (const HMIComponent &s) :
    label (s.label)
{
    // Nothing to do here :-)
}

uintptr_t HMIComponent::getUID ()
{
    return uid;
}

/** Returns the label string. */
const char* HMIComponent::getLabel () const
{
    return label;
}

int HMIComponent::setLabel (const char* newLabel)
{
    if (newLabel == "")
        return -1;
    size_t length = strlen (newLabel);
    if (length > MaxLabelLength)
        return 1;

    if (!labelIsValid (newLabel)) return 1;

    label = newLabel;
    return 0;
}

int HMIComponent::setLabel (const HMIComponent& newLabel)
{
    return setLabel (newLabel.label);
}

int HMIComponent::compare (const char* s) const
{
    std::cout << "# " << label << ".compare (" << s << ") = " << label.compare (s);

    return label.compare (s);
}

int HMIComponent::compare (const HMIComponent& s) const
{
    return compare (s.label);
}

void HMIComponent::print () const
{
    printf (label);
}

}   //< _HMI

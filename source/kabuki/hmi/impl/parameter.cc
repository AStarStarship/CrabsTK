/** Kabuki Theater
    @file    /.../Source-Impl/_Threater/HMI/Parameter.cpp
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

#include <_HMI/Parameter.h>

namespace _HMI {

void printMacroHeader ()
{
    printf ("|");
    _::printCentered ("Parameter", HMIComponent::MaxLabelLength);
    printf ("|    Type    |#Bit|Init | min | max |Value|Ch |CC |  Action  |Step |");
}

Parameter::Parameter (T newType, const char* newLabel, T newChannel, T newInitValue, T newMinValue, T newMaxValue,
    T newWordSize) 
:   Parameter (newLabel)
{
    setType (newType);
    setWordSize (newWordSize);
    setChannel (newChannel);
    setMinMaxValues (newMinValue, newMaxValue);
    setInitValue (newInitValue);
    setValue (newInitValue);
}

Parameter::Parameter (const Parameter& o)
:   type        (o.type        ),
    channel     (o.channel     ),
    numChannels (o.numChannels ),
    wordSize    (o.wordSize    ),
    wordValue   (o.wordValue   ),
    minWordSize (o.minWordSize ),
    maxWordSize (o.maxWordSize ),
    minWordValue (o.minWordValue),
    maxWordValue (o.maxWordValue),
    value       (o.value       ),
    initValue   (o.initValue   ),
    minValue    (o.minValue    ),
    maxValue    (o.maxValue    )

    // Nothing to do here. :-)
}

int Parameter::compare (const Parameter& o) const
{
    if (type     != o.type     ||
        channel  != o.channel  || 
        value    != o.value    ||
        minValue != o.minValue ||
        maxValue != o.maxValue)
    {
        /*
        Logger::outputDebugString ("The variables were not the same: -1\n" + 
            ConsoleLine ('-') + print () + ConsoleLine ('-') + o.print () + ConsoleLine ('-'));
        */
        return -1;
    }

    T CompareValue = compare (o.label ());

    Logger::outputDebugString ("The variables were the same: CompareValue = " + string (CompareValue) + 
        "\nLabels: { " + label () + ", " + o.label () + " }\nlabel ().compare (o.label ()) = " + 
        string (label.compare (o.label ())));
    
    return CompareValue;
}

 T Parameter::getType () const
 {
     return type;
 }
 
 const char* Parameter::getTypeString () const
 {
     switch (type)
     {
        case DMXControl:
            return "DMX Control";
        case MIDIControl:
            return "MIDI Control";
        case OSCControl:
            return "OSC Control";
        case DMXButton:
            return "DMX AButton";
        case MacroButton:
            return "MIDI AButton";
        case OSCButton:
            return "OSC AButton";
        default:
            return "Invalid type";
    }
 }

 T Parameter::getChannel () const
 {
     return channel;
 }

 T Parameter::getNumChannels () const
 {
     return numChannels;
 }

T Parameter::getValue () const
{
    return value;
}

T Parameter::getLSBValue () const
{
    return value & 0xff;
}

T Parameter::getMSBValue () const
{
    return (value & 0xff00) >> 8;
}

T Parameter::getInitValue () const
{
    return initValue;
}

T Parameter::getMinValue () const
{
    return minValue;
}

T Parameter::getMaxValue () const
{
    return maxValue;
}

T Parameter::getWordSize () const
{
    return wordSize;
}

T Parameter::getMinWordSize () const
{
    return minWordSize;
}

T Parameter::getMaxWordSize () const
{
    return maxWordSize;
}

T Parameter::getMinWordValue () const
{
    return minWordValue;
}

T Parameter::getMaxWordValue () const
{
    return maxWordValue;
}

void Parameter::getValue (T value)
{
    if (value < minValue)
        value = minValue;
    else if (value > maxValue)
        value = maxValue;
    else
        value = value;
}

void Parameter::getInitValue (T value)
{
    if (value < minValue)
        initValue = minValue;
    else if (value > maxValue)
        initValue = maxValue;
    else
        initValue = value;
}

void Parameter::getMinMaxValues (T newMin, T newMax)
{
    // Ensure the max is greater than the min.
    if (newMax < newMin)
    {
        T temp = newMax;
        newMax = newMin;
        newMin = temp;
    }

    if (newMin < 0)
        newMin = 0;
    if (newMax < 0)
        newMax = 0;

    T theMaxWordValue = maxWordValue;

    if (newMin > theMaxWordValue)
        newMin = theMaxWordValue;
    if (newMax > theMaxWordValue)
        newMax = theMaxWordValue;

    minValue = newMin;
    maxValue = newMax;

    setValue (value);
    setInitValue (initValue);
}

void Parameter::getMinValue (T value)
{
    if (value < 0)
        minValue = 0;
    else if (value > maxValue)
        minValue = maxValue;
    else
        minValue = value;

    if (value < minValue)
        value = minValue;
    if (initValue < maxValue)
        initValue = maxValue;
}

void Parameter::getMaxValue (T value)
{
    if (value > maxWordValue)
        maxValue = maxWordValue;
    else if (value < minValue)
        maxValue = minValue;
    else
        maxValue = value;

    if (value > maxValue)
        value = maxValue;
    if (initValue > maxValue)
        initValue = maxValue;
}

void Parameter::getLSBValue (T value)
{
    value = (value & 0xff00) | (value & 0xff);
}

void Parameter::getMSBValue (T value)
{
    value = (value & 0x00ff) | ((value & 0xff) << 8);
}

void Parameter::setChannel (T value)
{
    if (value < 0)
        channel = 0;
    else if (value > numChannels)
        channel = numChannels;
    else
        channel = value;
}

void Parameter::getWordSize (T value)
{
    if (value != minWordSize () && value != maxWordSize ())
        value = minWordSize ();

    wordSize = value;
    maxWordValue = (value == minWordValue) ? minWordValue : maxWordValue;
    setMinMaxValues (minValue, maxValue);
}

void Parameter::toggle ()
{
    if (value != minValue)
        value = minValue;
    else
        value = maxValue;
}

string Parameter::headerString () const
{
    return "|" + printCentered ("Parameter", Parameter::MaxLabelLength) + "|    Type    |#Bit|Init | min | max |Value|Ch |";
}

string Parameter::toStringRow () const
{
    return "|" + printCentered (label (), Parameter::MaxLabelLength) + "|" + printCentered (typeString (), 12) + "|" +
        printCentered (string (wordSize), 4) + "|" + printCentered (string (initValue), 5) + "|" + 
        printCentered (string (minValue), 5) + "|" + printCentered (string (maxValue) , 5) + "|" + 
        printCentered (string (value)   , 5) + "|" + printCentered (string (channel)  , 3) + "|";
}

void Parameter::print () const
{
    return getHeaderString () + "\n" + ToStringRow () + "\n";
}


void Parameter::getType (T value)
{
    if (value < DMXControl)
        value = DMXControl;
    else if (value > MacroButton)
        value = MacroButton;
    
    type = value;

    if (value == DMXControl || value == DMXButton)
    {
        minWordSize  = 7;
        maxWordSize  = 14;
        minWordValue = 255;
        maxWordValue = 0xffff;
        numChannels  = 512;
        return;
    }
    if (value == MIDIControl || value == MacroButton)
    {
        minWordSize  = 8;
        maxWordSize  = 16;
        minWordValue = 127;
        maxWordValue = 0x3fff;
        numChannels  = 16;
        return;
    }
    if (value == OSCControl || value == OSCButton)
    {
        // To do: Write me!
        return;
    }
    // Else its a Macro AButton
    minWordSize = 1;
    maxWordSize = 32;
    minWordValue = 1;
    maxWordValue = 0x7fffffff;
    numChannels  = 0x7fffffff;
}

}   //< _HMI

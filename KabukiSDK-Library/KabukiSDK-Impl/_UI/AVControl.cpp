/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_UI/AVControl.cpp
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

#include <_UI/AVControl.hpp>

namespace _UI {

const char* AVControl::MacroHeader = "|" + printCentered ("AVControl", AVControl::MaxLabelLength) +
        "|    Type    |#Bit|Init | min | max |Value|Ch |CC |  Action  |Step |";

const int AVControl::MacroHeaderLength = AVControl::MacroHeader.length ();

/** Default constructor. */
AVControl::AVControl (int newType, const char* &newLabel, int newChannel, int newInitValue, int newMinValue, int newMaxValue,
    int newWordSize) 
:   AVControl (newLabel)
{
    setType (newType);
    setWordSize (newWordSize);
    setChannel (newChannel);
    setMinMaxValues (newMinValue, newMaxValue);
    setInitValue (newInitValue);
    setValue (newInitValue);
}

/** Copy contructor. */
AVControl::AVControl (const AVControl& O)
:   AVControl       (O             ),
    type        (O.type        ),
    channel     (O.channel     ),
    numChannels (O.numChannels ),
    wordSize    (O.wordSize    ),
    wordValue   (O.wordValue   ),
    minWordSize (O.minWordSize ),
    maxWordSize (O.maxWordSize ),
    minWordValue (O.minWordValue),
    maxWordValue (O.maxWordValue),
    value       (O.value       ),
    initValue   (O.initValue   ),
    minValue    (O.minValue    ),
    maxValue    (O.maxValue    )
{
    // Nothing to do here. :-)
}

/** Compares this AVControl to the O control.
    @return Returns 0 if this control is the same as the O. */
 int AVControl::Compare (const AVControl& O) const
{
    if (type     != O.type     ||
        channel  != O.channel  || 
        value    != O.value    ||
        minValue != O.minValue ||
        maxValue != O.maxValue)
    {
        /*
        Logger::outputDebugString ("The variables were not the same: -1\n" + 
            ConsoleLine ('-') + print (I2P::Terminal& slot) + ConsoleLine ('-') + O.print (I2P::Terminal& slot) + ConsoleLine ('-'));
        */
        return -1;
    }

    int CompareValue = AVControl::Compare (O.label ());

    Logger::outputDebugString ("The variables were the same: CompareValue = " + const char* (CompareValue) + 
        "\nLabels: { " + label () + ", " + O.label () + " }\nlabel ().Compare (O.label ()) = " + 
        const char* (label ().Compare (O.label ())));
    
    return CompareValue;
}

 /** Returns what type of control this is. */
 int AVControl::type () const
 {
     return type;
 }

 /** Returns a const char* that says what type of AVControl this is. */
 const char* AVControl::typeString () const
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
        case MacroButton:
            return "Macro AButton";
        default:
            return "Invalid type";
    }
 }

 int AVControl::channel () const
 {
     return channel;
 }

 int AVControl::numChannels () const
 {
     return numChannels;
 }

int AVControl::value () const
{
    return value;
}

int AVControl::lSBValue () const
{
    return value & 0xff;
}

int AVControl::mSBValue () const
{
    return (value & 0xff00) >> 8;
}

int AVControl::initValue () const
{
    return initValue;
}

int AVControl::minValue () const
{
    return minValue;
}

int AVControl::maxValue () const
{
    return maxValue;
}

int AVControl::wordSize () const
{
    return wordSize;
}

int AVControl::minWordSize () const
{
    return minWordSize;
}

int AVControl::maxWordSize () const
{
    return maxWordSize;
}

int AVControl::minWordValue () const
{
    return minWordValue;
}

int AVControl::maxWordValue () const
{
    return maxWordValue;
}

void AVControl::getValue (int value)
{
    if (value < minValue)
        value = minValue;
    else if (value > maxValue)
        value = maxValue;
    else
        value = value;
}

void AVControl::getInitValue (int value)
{
    if (value < minValue)
        initValue = minValue;
    else if (value > maxValue)
        initValue = maxValue;
    else
        initValue = value;
}

void AVControl::getMinMaxValues (int newMin, int newMax)
{
    // Ensure the max is greater than the min.
    if (newMax < newMin)
    {
        int temp = newMax;
        newMax = newMin;
        newMin = temp;
    }

    if (newMin < 0)
        newMin = 0;
    if (newMax < 0)
        newMax = 0;

    int theMaxWordValue = maxWordValue;

    if (newMin > theMaxWordValue)
        newMin = theMaxWordValue;
    if (newMax > theMaxWordValue)
        newMax = theMaxWordValue;

    minValue = newMin;
    maxValue = newMax;

    setValue (value);
    setInitValue (initValue);
}

void AVControl::getMinValue (int value)
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

void AVControl::getMaxValue (int value)
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

void AVControl::getLSBValue (int value)
{
    value = (value & 0xff00) | (value & 0xff);
}

void AVControl::getMSBValue (int value)
{
    value = (value & 0x00ff) | ((value & 0xff) << 8);
}

void AVControl::setChannel (int value)
{
    if (value < 0)
        channel = 0;
    else if (value > numChannels)
        channel = numChannels;
    else
        channel = value;
}

void AVControl::getWordSize (int value)
{
    if (value != minWordSize () && value != maxWordSize ())
        value = minWordSize ();

    wordSize = value;
    maxWordValue = (value == minWordValue) ? minWordValue : maxWordValue;
    setMinMaxValues (minValue, maxValue);
}

void AVControl::toggle ()
{
    if (value != minValue)
        value = minValue;
    else
        value = maxValue;
}

const char* AVControl::headerString () const
{
    return "|" + printCentered ("AVControl", AVControl::MaxLabelLength) + "|    Type    |#Bit|Init | min | max |Value|Ch |";
}

const char* AVControl::toStringRow () const
{
    return "|" + printCentered (label (), AVControl::MaxLabelLength) + "|" + printCentered (typeString (), 12) + "|" +
        printCentered (const char* (wordSize), 4) + "|" + printCentered (const char* (initValue), 5) + "|" + 
        printCentered (const char* (minValue), 5) + "|" + printCentered (const char* (maxValue) , 5) + "|" + 
        printCentered (const char* (value)   , 5) + "|" + printCentered (const char* (channel)  , 3) + "|";
}

void AVControl::print (I2P::Terminal& slot) const
{
    return getHeaderString () + "\n" + ToStringRow () + "\n";
}


void AVControl::getType (int value)
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

}   //< namespace _UI

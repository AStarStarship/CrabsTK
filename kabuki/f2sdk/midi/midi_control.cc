/** Kabuki Toolkit
    @file    ~/library/kabuki/toolkit/midi/Control.cc
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (R). Licensed under the Apache License, Version 
             2.0 (the "License"); you may not use this file except in 
             compliance with the License. You may obtain a copy of the License 
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless 
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#include <stdafx.h>

#include "control.h"

namespace _ {

Control::Control (int16_t initCC, int16_t initValue, int16_t initMinValue, int16_t initMaxValue, int16_t initLSB, int16_t initMSB, int16_t initCh)
{
    setCC       (initCC);
    setValue    (initValue);
    setInitValue (initValue);
    setMinValue (initMinValue);
    setMaxValue (initMaxValue);
    setLSBValue (initLSB);
    setMSBValue (initMSB);
    setChannel  (initCh);
}



Control::Control (const Control& C)
:   midiCC    (C.midiCC),
    midiValue (C.midiValue),
    initValue (C.initValue),
    minValue  (C.minValue),
    maxValue  (C.maxValue),
    lsbValue  (C.lsbValue),
    msbValue  (C.msbValue),
    outputCh  (C.outputCh)
{
}

int16_t Control::GetCC ()
{
    return midiCC;
}
int16_t Control::GetValue ()
{
    return midiValue;
}
int16_t Control::GetInitValue ()
{
    return initValue;
}
int16_t Control::GetMinValue ()
{
    return minValue;
}
int16_t Control::GetMaxValue ()
{
    return maxValue;
}

int16_t Control::GetLSBValue ()
{
    return lsbValue;
}

int16_t Control::GetMSBValue ()
{
    return msbValue;
}

int16_t Control::GetChannel ()
{
    return outputCh;
}

void Control::SetCC (int16_t newCC)
{
    if (newCC < 0)
        midiCC = 0;
    else
    if (newCC > 127)
        midiCC = 127;
    else
        midiCC = newCC;
}

void Control::SetValue (int value)
{
    if (value < minValue)
        midiValue = minValue;
    else
    if (value > maxValue)
        midiValue = maxValue;
    else
        midiValue = value;
}

void Control::SetInitValue (int value)
{

    if (value < 0)
        initValue = 0;
    else
    if (value > 127)
        initValue = 127;
    else
        initValue = value;
}

void Control::SetMinValue (int value)
{
    if (value < 0)
        minValue = 0;
    else
    if (value > 127)
        minValue = 127;
    else
        minValue = value;

    if (midiValue < minValue)
        midiValue = minValue;
}

void Control::SetMaxValue (int value)
{
    if (value < 0)
        maxValue = 0;
    else
    if (value > 127)
        maxValue = 127;
    else
        maxValue = value;

    if (midiValue > maxValue)
        midiValue = maxValue;
}

void Control::SetLSBValue (int value)
{

    if (value < 0)
        lsbValue = 0;
    else
    if (value > 127)
        lsbValue = 127;
    else
        lsbValue = value;
}

void Control::SetMSBValue (int value)
{

    if (value < 0)
        msbValue = 0;
    else
    if (value > 127)
        msbValue = 127;
    else
        msbValue = value;
}

void Control::SetChannel (int value)
{

    if (value < 0)
        outputCh = 0;
    else
    if (value > 16)
        outputCh = 16;
    else
        outputCh = value;
}

void Control::Toggle ()
{
    if (midiValue == maxValue)
        midiValue = minValue;
    else
        midiValue = maxValue;
}

bool Control::ComparedTo (Control* C)
{
    if (C == 0
       || midiCC    != C->midiCC
       || midiValue != C->midiValue
       || minValue  != C->minValue
       || maxValue  != C->maxValue
       || outputCh  != C->outputCh
      ) return false;
 // else
    return true;
}

void Control::Copy (const Control& C) {
    midiCC    = C.midiCC;
    midiValue = C.midiValue;
    initValue = C.initValue;
    minValue  = C.minValue;
    maxValue  = C.maxValue;
    outputCh  = C.outputCh;
}

_::Printer& Control::Print (_::Printer& print) {
{
    /*
    char *returnString;

    char midiCcHeader[]  = "CC:    ",
         channelHeader[] = "Ch:    ",
         valueHeader[]   = "value: ",
         minHeader[]     = "Min:   ",
         maxHeader[]     = "Max:   ",
         initHeader[]    = "Init:  ";

    int16_t numChars = strlen (midiCcHeader)  +3 +1 // our MIDI values will never be greater then 127  (hind sight: mind be 14-bit MIDI values)
                 + strlen (channelHeader) +3 +1 // so we only need 3 chars for the value
                 + strlen (valueHeader)   +3 +1 // and +1 for the endl chars
                 + strlen (minHeader)     +3 +1
                 + strlen (maxHeader)     +3 +1
                 + strlen (initHeader)    +3 +1
                 +2; // for endl char and null term char

    char *buffer = new char[5];

    returnString = new char[numChars];

    strcat_s (returnString, numChars, midiCcHeader);
    _itoa_s (midiCC, buffer, numChars, 10);
    strcat_s (returnString, numChars, buffer);
    strcat_s (returnString, numChars, "\n");

    strcat_s (returnString, numChars, channelHeader);
    _itoa_s (outputCh, buffer, numChars, 10);
    strcat_s (returnString, numChars, buffer);
    strcat_s (returnString, numChars, "\n");

    strcat_s (returnString, numChars, valueHeader);
    _itoa_s (midiValue, buffer, numChars, 10);
    strcat_s (returnString, numChars, buffer);
    strcat_s (returnString, numChars, "\n");

    strcat_s (returnString, numChars, minHeader);
    _itoa_s (minValue, buffer, numChars, 10);
    strcat_s (returnString, numChars, buffer);
    strcat_s (returnString, numChars, "\n");

    strcat_s (returnString, numChars, maxHeader);
    _itoa_s (maxValue, buffer, numChars, 10);
    strcat_s (returnString, numChars, buffer);
    strcat_s (returnString, numChars, "\n");

    strcat_s (returnString, numChars, initHeader);
    _itoa_s (initValue, buffer, numChars, 10);
    strcat_s (returnString, numChars, buffer);
    strcat_s (returnString, numChars, "\n");

    delete buffer;

    return returnString;
    */
    return 0;
}

}       //< namespace _

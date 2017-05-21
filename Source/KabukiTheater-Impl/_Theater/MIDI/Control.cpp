/** Kabuki Theater
    @file    /.../KabukiTheater-Impl/_IO/MIDI/Control.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright  (C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved  (R).

        Licensed under the Apache License, Version 2.0  (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#include <_IO/MIDI/Control.h>

#include <string>
#include <string.h>
#include <stdlib.h>

namespace _IO { namespace MIDI {

Control::Control  (int16_t initCC, int16_t initValue, int16_t initMinValue, int16_t initMaxValue, int16_t initLSB, int16_t initMSB, int16_t initCh)
{
    SetCC  (initCC);
    SetValue  (initValue);
    SetInitValue  (initValue);
    SetMinValue  (initMinValue);
    SetMaxValue  (initMaxValue);
    SetLSBValue  (initLSB);
    SetMSBValue  (initMSB);
    SetChannel  (initCh);
}



Control::Control  (const Control& C)
:   midiCC     (C.midiCC),
    midiValue  (C.midiValue),
    initValue  (C.initValue),
    minValue   (C.minValue),
    maxValue   (C.maxValue),
    lsbValue   (C.lsbValue),
    msbValue   (C.msbValue),
    outputCh   (C.outputCh)
{
}



Control::~Control ()
{
    // Nothing to do here.
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

void Control::SetCC  (int16_t newCC)
{
    if  (newCC < 0)
        midiCC = 0;
    else
    if  (newCC > 127)
        midiCC = 127;
    else
        midiCC = newCC;
}

void Control::SetValue  (int16_t Value)
{
    if  (Value < minValue)
        midiValue = minValue;
    else
    if  (Value > maxValue)
        midiValue = maxValue;
    else
        midiValue = Value;
}

void Control::SetInitValue  (int16_t Value)
{

    if  (Value < 0)
        initValue = 0;
    else
    if  (Value > 127)
        initValue = 127;
    else
        initValue = Value;
}

void Control::SetMinValue  (int16_t Value)
{
    if  (Value < 0)
        minValue = 0;
    else
    if  (Value > 127)
        minValue = 127;
    else
        minValue = Value;

    if  (midiValue < minValue)
        midiValue = minValue;
}

void Control::SetMaxValue  (int16_t Value)
{
    if  (Value < 0)
        maxValue = 0;
    else
    if  (Value > 127)
        maxValue = 127;
    else
        maxValue = Value;

    if  (midiValue > maxValue)
        midiValue = maxValue;
}

void Control::SetLSBValue  (int16_t Value)
{

    if  (Value < 0)
        lsbValue = 0;
    else
    if  (Value > 127)
        lsbValue = 127;
    else
        lsbValue = Value;
}

void Control::SetMSBValue  (int16_t Value)
{

    if  (Value < 0)
        msbValue = 0;
    else
    if  (Value > 127)
        msbValue = 127;
    else
        msbValue = Value;
}

void Control::SetChannel  (int16_t Value)
{

    if  (Value < 0)
        outputCh = 0;
    else
    if  (Value > 16)
        outputCh = 16;
    else
        outputCh = Value;
}

void Control::Toggle ()
{
    if  (midiValue == maxValue)
        midiValue = minValue;
    else
        midiValue = maxValue;
}

bool Control::ComparedTo  (Control* C)
{
    if  (C == 0
       || midiCC    != C->midiCC
       || midiValue != C->midiValue
       || minValue  != C->minValue
       || maxValue  != C->maxValue
       || outputCh  != C->outputCh
      ) return false;
 // else
    return true;
}

void Control::Copy  (const Control* C)
{
    midiCC     = C->midiCC;
    midiValue  = C->midiValue;
    initValue = C->initValue;
    minValue  = C->minValue;
    maxValue  = C->maxValue;
    outputCh   = C->outputCh;
}

const char* Control::toString ()
{
    /*
    char *returnString;

    char midiCcHeader[]  = "CC:    ",
         channelHeader[] = "Ch:    ",
         valueHeader[]   = "Value: ",
         minHeader[]     = "Min:   ",
         maxHeader[]     = "Max:   ",
         initHeader[]    = "Init:  ";

    int16_t numChars = strlen  (midiCcHeader)  +3 +1 // our MIDI values will never be greater then 127   (hind sight: mind be 14-bit MIDI values)
                 + strlen  (channelHeader) +3 +1 // so we only need 3 chars for the value
                 + strlen  (valueHeader)   +3 +1 // and +1 for the endl chars
                 + strlen  (minHeader)     +3 +1
                 + strlen  (maxHeader)     +3 +1
                 + strlen  (initHeader)    +3 +1
                 +2; // for endl char and null term char

    char *buffer = new char[5];

    returnString = new char[numChars];

    strcat_s  (returnString, numChars, midiCcHeader);
    _itoa_s  (midiCC, buffer, numChars, 10);
    strcat_s  (returnString, numChars, buffer);
    strcat_s  (returnString, numChars, "\n");

    strcat_s  (returnString, numChars, channelHeader);
    _itoa_s  (outputCh, buffer, numChars, 10);
    strcat_s  (returnString, numChars, buffer);
    strcat_s  (returnString, numChars, "\n");

    strcat_s  (returnString, numChars, valueHeader);
    _itoa_s  (midiValue, buffer, numChars, 10);
    strcat_s  (returnString, numChars, buffer);
    strcat_s  (returnString, numChars, "\n");

    strcat_s  (returnString, numChars, minHeader);
    _itoa_s  (minValue, buffer, numChars, 10);
    strcat_s  (returnString, numChars, buffer);
    strcat_s  (returnString, numChars, "\n");

    strcat_s  (returnString, numChars, maxHeader);
    _itoa_s  (maxValue, buffer, numChars, 10);
    strcat_s  (returnString, numChars, buffer);
    strcat_s  (returnString, numChars, "\n");

    strcat_s  (returnString, numChars, initHeader);
    _itoa_s  (initValue, buffer, numChars, 10);
    strcat_s  (returnString, numChars, buffer);
    strcat_s  (returnString, numChars, "\n");

    delete buffer;

    return returnString;
    */
    return 0;
}

byte Control::GetState ()
{
    return 0;
}

const char* Control::SetState (byte Value)
{
    return 0;
}

const char* Control::Do (const char* Query, byte Index, Roombot* Bot)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return Query ? Enquery ("Control", "_IO::MIDI"): InvalidIndex ();
}

}   //< namespace MIDI
}   //< namespace _IO

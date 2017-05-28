/** Kabuki Theater
    @file    /.../Source/KabukiTheater-Impl/_Theater/MIDI/Control.cpp
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

#include <_Theater/MIDI/Control.hpp>

#include <string>
#include <string.h>
#include <stdlib.h>

namespace _Theater { namespace MIDI {

Control::Control  (int16_t initCC, int16_t initValue, int16_t initMinValue, int16_t initMaxValue, int16_t initLSB, int16_t initMSB, int16_t initCh)
{
    setCC        (initCC);
    setValue     (initValue);
    setInitValue (initValue);
    setMinValue  (initMinValue);
    setMaxValue  (initMaxValue);
    setLSBValue  (initLSB);
    setMSBValue  (initMSB);
    setChannel   (initCh);
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

int16_t Control::getCC ()
{
    return midiCC;
}
int16_t Control::getValue ()
{
    return midiValue;
}
int16_t Control::getInitValue ()
{
    return initValue;
}
int16_t Control::getMinValue ()
{
    return minValue;
}
int16_t Control::getMaxValue ()
{
    return maxValue;
}

int16_t Control::getLSBValue ()
{
    return lsbValue;
}

int16_t Control::getMSBValue ()
{
    return msbValue;
}

int16_t Control::getChannel ()
{
    return outputCh;
}

void Control::setCC  (int16_t newCC)
{
    if  (newCC < 0)
        midiCC = 0;
    else
    if  (newCC > 127)
        midiCC = 127;
    else
        midiCC = newCC;
}

void Control::setValue  (int value)
{
    if  (value < minValue)
        midiValue = minValue;
    else
    if  (value > maxValue)
        midiValue = maxValue;
    else
        midiValue = value;
}

void Control::setInitValue  (int value)
{

    if  (value < 0)
        initValue = 0;
    else
    if  (value > 127)
        initValue = 127;
    else
        initValue = value;
}

void Control::setMinValue  (int value)
{
    if  (value < 0)
        minValue = 0;
    else
    if  (value > 127)
        minValue = 127;
    else
        minValue = value;

    if  (midiValue < minValue)
        midiValue = minValue;
}

void Control::setMaxValue  (int value)
{
    if  (value < 0)
        maxValue = 0;
    else
    if  (value > 127)
        maxValue = 127;
    else
        maxValue = value;

    if  (midiValue > maxValue)
        midiValue = maxValue;
}

void Control::setLSBValue  (int value)
{

    if  (value < 0)
        lsbValue = 0;
    else
    if  (value > 127)
        lsbValue = 127;
    else
        lsbValue = value;
}

void Control::setMSBValue  (int value)
{

    if  (value < 0)
        msbValue = 0;
    else
    if  (value > 127)
        msbValue = 127;
    else
        msbValue = value;
}

void Control::setChannel  (int value)
{

    if  (value < 0)
        outputCh = 0;
    else
    if  (value > 16)
        outputCh = 16;
    else
        outputCh = value;
}

void Control::toggle ()
{
    if  (midiValue == maxValue)
        midiValue = minValue;
    else
        midiValue = maxValue;
}

bool Control::comparedTo  (Control* C)
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

void Control::copy  (const Control& C)
{
    midiCC    = C.midiCC;
    midiValue = C.midiValue;
    initValue = C.initValue;
    minValue  = C.minValue;
    maxValue  = C.maxValue;
    outputCh  = C.outputCh;
}

const char* Control::toString ()
{
    /*
    char *returnString;

    char midiCcHeader[]  = "CC:    ",
         channelHeader[] = "Ch:    ",
         valueHeader[]   = "value: ",
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

}   //< namespace MIDI
}   //< namespace _Theater

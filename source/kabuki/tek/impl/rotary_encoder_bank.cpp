/** Kabuki Tek
    @file    /.../Source/KabukiTek-Impl/sensors/RotaryEncoderBank.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough](calemccollough.github.io)

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

#include <sensors/RotaryEncoderBank.h>
#include <Displays/LEDMatrix.h>

namespace tek { namespace sensors {

RotaryEncoderBank::RotaryEncoderBank  (byte NumKnobs, uint NumBanks, uint StartChannel, byte* Rows, byte* Columns)
:   numKnobs        (NumKnobs),
    numBanks        (NumBanks),
    numButtons      (0),
    wiringConfig    (0),
    currentBank     (0),
    startChannel    (StartChannel),
    currentChannel  (StartChannel),
    rows            (Rows),
    columns         (Columns),
    buttons         (0),
    lights          (0)
{
}

inline void RotaryEncoderBank::SwitchBank  (byte value)
{
    if  (value >= numBanks) return;
    
    currentBank = value;
    currentChannel = startChannel + numKnobs * value;
}

inline void RotaryEncoderBank::CycleBank ()
{
    if  (currentChannel += numKnobs > startChannel +  numKnobs * numBanks) currentChannel = startChannel;
}

inline void RotaryEncoderBank::Update  (int16_t* channels, int16_t* minValues, int16_t* maxValues, byte* spiOutBytes,
    int16_t row, int8_t count)
{
    //for  (uint16_t i = 0; i < numKnobs; ++i)
     //   UpdateChannel  (i, channels, minValues, maxValues, spiOutBytes, row, count, wiringConfig);
}

inline void RotaryEncoderBank::UpdateChannel  (uint16_t index, int16_t* channels, int16_t* minValues, int16_t* maxValues, 
    byte* spiOutBytes, uint16_t row, byte count)
{
    //> When we enter this algorihm, we will be stepping through the rows 1-8.
    
    static const int8_t LEDIndexToRowMap[3][16] = {
        { 0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0, -1 },    //< LEDRingConfig::NoButtons
        { 0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0, 7  },    //< LEDRingConfig::ButtonsOnTopRows
        { 0, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3, 2, 1, 0  } };  //< LEDRingConfig::ButtonsOnBottomRows
    
    int16_t value = channels[index],
        minValue = minValues[index],
        maxValue = maxValues[index];
    
    int16_t ringIndex =  (value - minValue) % ((maxValue - minValue) / 15),
        rowIndex = LEDIndexToRowMap[wiringConfig][ringIndex];
    
    if  (row != rowIndex) return;
    
    //uint16_t columnNumber =  (index - currentChannel) >> 2;    // >> 2 to divide by 2
    
    //byte columnBit = columns[columnNumber],
    //    mask = 1 <<  (columnBit & BitNumberMask);
    
}

byte RotaryEncoderBank::getState ()
{
    return 0;
}

const char* RotaryEncoderBank::setState (byte Value)
{
    return 0;
}

const char* RotaryEncoderBank::Op (byte index, Uniprinter* io)
{
    switch (Index)
    {
        case 0: return tek::NumMembers (0);
    }
    
    return Query ? Enquery ("RotaryEncoderBank", "tek::sensors"): InvalidIndex ();
}

}   //< namespace RotaryEncoderBank
}   //< namespace tek

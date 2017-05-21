/** Kabuki Theater
    @file    /.../Source-Impl/_Theater/HMI/MIDIDevice.cpp
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

#include <_HMI/MIDIDevice.hpp>

namespace _Theater { namespace HMI {

const Time MIDIDevice::minDoubleClickTime (1);
const Time MIDIDevice::maxDoubleClickTime (1000);

MIDIDevice::MIDIDevice (byte initCommonChannel, byte initKeyboardChannel, byte initDrumsChannel)
:   commonChannel  (initCommonChannel),
    keyboardChannel (initKeyboardChannel),
    drumsChannel   (initDrumsChannel),
    doublePressTime (500)
{
    // Nothing to do here yet.
}



MIDIDevice::MIDIDevice (const MIDIDevice& o)
:   commonChannel  (o.commonChannel),
    keyboardChannel (o.keyboardChannel),
    drumsChannel   (o.drumsChannel)
{
    // Nothing to do here yet.
}


MIDIDevice::~MIDIDevice ()
{
    // Nothing to do here yet.
}

byte MIDIDevice::setCommonChannel ()
{
    return commonChannel;
}

void MIDIDevice::setCommonChannel (byte thisChannel)
{
    if (thisChannel == 0)
        commonChannel = 1;
    else
    if (thisChannel >= 16)
        commonChannel = 16;
    else
        commonChannel = thisChannel;
}


byte MIDIDevice::getKeyboardChannel ()
{
    return keyboardChannel;
}

void MIDIDevice::setKeyboardChannel (byte thisChannel)
{
    if (thisChannel == 0)
        keyboardChannel = 1;
    else
    if (thisChannel >= 16)
        keyboardChannel = 16;
    else
        keyboardChannel = thisChannel;
}

byte MIDIDevice::drumsChannel ()
{
    return drumsChannel;
}

void MIDIDevice::setDrumsChannel (byte thisChannel)
{
    if (thisChannel == 0)
        drumsChannel = 1;
    else
    if (thisChannel >= 16)
        drumsChannel = 16;
    else
        drumsChannel = thisChannel;
}

Time MIDIDevice::getDoublePressTime ()
{
    return doublePressTime;
}

int MIDIDevice::setDoublePressTime (timestamp_t newTime)
{
    if (newTime < minDoubleClickTime)
        return -1;
    
    if (newTime > maxDoubleClickTime)
        return 1;

    doublePressTime = newTime;
    return 0;
}

void MIDIDevice::print (I2P::Terminal slot)
{
    std::string rep ("MIDIDevice:\n");

    //rep << ;

    return returnString;
}

}   //< HMI
}   //< _Theater

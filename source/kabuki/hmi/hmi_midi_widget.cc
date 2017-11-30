/** Kabuki Toolkit
    @file    ~/source/kabuki/hmi/midi_widget.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
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

#include "midi_widget.h"

namespace kabuki { namespace hmi {

const Time MIDIWidget::minDoublePressTime (1);
const Time MIDIWidget::maxDoublePressTime (1000);

MIDIWidget::MIDIWidget (byte commonChannel, byte keyboardChannel, byte drumsChannel)
:   commonChannel   (commonChannel),
    keyboardChannel (keyboardChannel),
    drumsChannel    (drumsChannel),
    doublePressTime (500)
{
    // Nothing to do here yet.
}

MIDIWidget::MIDIWidget (const MIDIWidget& o)
:   commonChannel   (o.commonChannel),
    keyboardChannel (o.keyboardChannel),
    drumsChannel    (o.drumsChannel)
{
    // Nothing to do here yet.
}


MIDIWidget::~MIDIWidget ()
{
    // Nothing to do here yet.
}

byte MIDIWidget::SetCommonChannel ()
{
    return commonChannel;
}

void MIDIWidget::SetCommonChannel (byte c)
{
    if (c == 0)
        commonChannel = 1;
    else
    if (c >= 16)
        commonChannel = 16;
    else
        commonChannel = c;
}


byte MIDIWidget::GetKeyboardChannel ()
{
    return keyboardChannel;
}

void MIDIWidget::SetKeyboardChannel (byte thisChannel)
{
    if (thisChannel == 0)
        keyboardChannel = 1;
    else
    if (thisChannel >= 16)
        keyboardChannel = 16;
    else
        keyboardChannel = thisChannel;
}

byte MIDIWidget::drumsChannel ()
{
    return drumsChannel;
}

void MIDIWidget::SetDrumsChannel (byte thisChannel)
{
    if (thisChannel == 0)
        drumsChannel = 1;
    else
    if (thisChannel >= 16)
        drumsChannel = 16;
    else
        drumsChannel = thisChannel;
}

Time MIDIWidget::GetDoublePressTime ()
{
    return doublePressTime;
}

int MIDIWidget::SetDoublePressTime (timestamp_t newTime)
{
    if (newTime < minDoubleClickTime)
        return -1;
    
    if (newTime > maxDoubleClickTime)
        return 1;

    doublePressTime = newTime;
    return 0;
}

void MIDIWidget::print ()
{
    printf ("MIDIWidget:\n");
}

}   //< namespace hmi
}   //< namespace kabuki

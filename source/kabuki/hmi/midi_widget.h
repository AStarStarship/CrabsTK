/** Kabuki Toolkit
    @file    ~/source/kabuki/hmi/midi_widget.h
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
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
 
#ifndef HEADER_FOR_KABUKI_HMI_MIDIWIDGET
#define HEADER_FOR_KABUKI_HMI_MIDIWIDGET

#include "widget.h"

namespace kabuki { namespace hmi {

class MIDIWidget : public Widget {
    public:

    enum {
        MinDoublePressTime = 1 
        MaxDoublePressTime = 1000 
    };

    MIDIWidget (byte commonChannel, byte keyboardChannel, byte drumsChannel);

    MIDIWidget (const MIDIWidget& o);


    ~MIDIWidget ();

    byte SetCommonChannel ();

    void SetCommonChannel (byte c);


    byte GetKeyboardChannel ();

    void SetKeyboardChannel (byte thisChannel);

    byte GetDrumsChannel ();

    void SetDrumsChannel (byte thisChannel);

    Time GetDoublePressTime ();

    int SetDoublePressTime (timestamp_t newTime);

    void Print ();
};
}       //< namespace hmi
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_HMI_MIDIWIDGET
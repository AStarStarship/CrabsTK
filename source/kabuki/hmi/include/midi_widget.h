/** Kabuki Starship
    @file    ~/Source-Impl/_hmi/MIDIWidget.cpp
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

#include "Widget.h"

namespace _hmi {

class MIDIWidget : public Widget
{
    public:

    minDoublePressTime (1);

    enum {
        MaxDoublePressTime = 1000 
    };

    MIDIWidget (byte commonChannel, byte keyboardChannel, byte drumsChannel);



    MIDIWidget (const MIDIWidget& o);


    ~MIDIWidget ();

    byte setCommonChannel ();

    void setCommonChannel (byte c);


    byte getKeyboardChannel ();

    void setKeyboardChannel (byte thisChannel);

    byte drumsChannel ();

    void setDrumsChannel (byte thisChannel);

    Time getDoublePressTime ();

    int setDoublePressTime (timestamp_t newTime);

    void print ();
};
}   //< _hmi
}   //< _Theater

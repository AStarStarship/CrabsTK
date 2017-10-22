/** Kabuki Starship
    @file    /.../Source/KabukiTheater-Impl/_Com/MIDI/ControlSurface.cpp
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

#include <_Com/MIDI/ControlSurface.h>
#include <_Com/MIDI/Control.h>

namespace _Com { namespace MIDI {

ControlSurface::ControlSurface ()
:   commonChannel  (0),
    keyboardChannel  (1),
    drumsChannel  (7)
{
    // Nothing to do here!
}

ControlSurface::~ControlSurface ()
{
    // Nothing to do here!
}

int16_t ControlSurface::GetCommonChannel ()
{
    return commonChannel;
}

void ControlSurface::SetCommonChannel  (int16_t thisChannel)
{
    if  (thisChannel < 0)
        commonChannel = 1;
    else if  (thisChannel >= 16)
        commonChannel = 16;
    else
        commonChannel = thisChannel;
}

int16_t ControlSurface::GetKeyboardChannel ()
{
    return keyboardChannel;
}

void ControlSurface::SetKeyboardChannel  (int16_t thisChannel)
{
    if  (thisChannel < 1)
        keyboardChannel = 1;
    else if  (thisChannel >= 16)
        keyboardChannel = 16;
    else
        keyboardChannel = thisChannel;
}

int16_t ControlSurface::GetDrumsChannel ()
{
    return drumsChannel;
}

void ControlSurface::SetDrumsChannel  (int16_t thisChannel)
{
    if  (thisChannel < 1)
        drumsChannel = 1;
    else if  (thisChannel >= 16)
        drumsChannel = 16;
    else
        drumsChannel = thisChannel;
}

byte ControlSurface::GetState ()
{
    return 0;
}

const char* ControlSurface::SetState (byte Value)
{
    return 0;
}

const char* ControlSurface::op (_::Expression* io, byte index)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return Query ? Enquery ("ControlSurface", "_Com::MIDI"): InvalidIndex ();
}

const char *ControlSurface::toString ()
{
    return "Write me!!!\n";
}

}   //< namespace MIDI
}   //< namespace _Com

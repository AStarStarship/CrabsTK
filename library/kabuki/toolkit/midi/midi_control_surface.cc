/** Kabuki Toolkit
    @file    ~/library/kabuki/toolkit/midi/midi_controller.cc
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

#include "control_surface.h"
#include "control.h"

namespace kabuki { namespace toolkit { namespace midi {

Controller::Controller ()
:   commonChannel  (0),
    keyboardChannel  (1),
    drumsChannel  (7)
{
    // Nothing to do here!
}

Controller::~Controller ()
{
    // Nothing to do here!
}

int16_t Controller::GetCommonChannel ()
{
    return commonChannel;
}

void Controller::SetCommonChannel  (int16_t thisChannel)
{
    if  (thisChannel < 0)
        commonChannel = 1;
    else if  (thisChannel >= 16)
        commonChannel = 16;
    else
        commonChannel = thisChannel;
}

int16_t Controller::GetKeyboardChannel ()
{
    return keyboardChannel;
}

void Controller::SetKeyboardChannel  (int16_t thisChannel)
{
    if  (thisChannel < 1)
        keyboardChannel = 1;
    else if  (thisChannel >= 16)
        keyboardChannel = 16;
    else
        keyboardChannel = thisChannel;
}

int16_t Controller::GetDrumsChannel ()
{
    return drumsChannel;
}

void Controller::SetDrumsChannel  (int16_t thisChannel)
{
    if  (thisChannel < 1)
        drumsChannel = 1;
    else if  (thisChannel >= 16)
        drumsChannel = 16;
    else
        drumsChannel = thisChannel;
}

byte Controller::GetState ()
{
    return 0;
}

const char* Controller::SetState (byte Value)
{
    return 0;
}

const char* Controller::op (_::Expression* io, byte index)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return Query ? Enquery ("Controller", "kabuki::MIDI"): InvalidIndex ();
}

const char *Controller::toString ()
{
    return "Write me!!!\n";
}

}   //< namespace midi}       //< namespace toolkit
}       //< namespace kabuki

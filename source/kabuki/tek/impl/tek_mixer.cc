/** kabuki::tek
    @file    ~/source/kabuki/tek/impl/tek_mixer.cc
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

#include "../include/mixer.h"
#include "../include/mixer_channel.h"

#include <string.h>

namespace kabuki { namespace tek {


Mixer::Mixer () :
    numChannels (0),
    mixerSize (defaultMixerArraySize),
    controlResolution (7),
    mixer (new MixerChannel *[defaultMixerArraySize]) {
    mixer[0] = new MixerChannel ("Master");

    for (int i = 1; i < defaultMixerArraySize; ++i)
        mixer[i] = 0;
}

Mixer::~Mixer () {
    for (int i = 0; i < numChannels + 1; ++i)
        delete mixer[i];
    delete mixer;
}

int Mixer::GetVolume (int channelNum) {
    return mixer[channelNum]->getVolume ();
}

int Mixer::GetPan (int channelNum) {
    return mixer[channelNum]->getPan ();
}

void Mixer::SetVolume (int channelNum, int value) {
    if (channelNum < 0
        || channelNum > numChannels
        || value   < 0
        || value   > controlResolution
        ) return;

    mixer[channelNum]->setVolume (value);
}

void Mixer::setPan (int channelNum, int value) {
    if (channelNum < 0
        || channelNum > numChannels
        || value   < 0
        || value   > controlResolution
        ) return;

    mixer[channelNum]->setPan (value);
}

int Mixer::IsMuted (int channelNum) {
    return mixer[channelNum]->isMuted ();
}

int Mixer::IsSoloed (int channelNum) {
    return mixer[channelNum]->isSoloed ();
}

void Mixer::SetMute (int channelNum, bool isMuted) {
    if (channelNum < 0 || channelNum > numChannels)
        return;
    mixer[channelNum]->setMute (isMuted);
}

void Mixer::SetSolo (int channelNum, bool isMuted) {
    if (channelNum < 0 || channelNum > numChannels)
        return;
    mixer[channelNum]->setSolo (isMuted);
}

void Mixer::ToggleMute (int thisChannelNum) {
    if (thisChannelNum < 0
        || thisChannelNum > numChannels
        || !mixer[thisChannelNum]
        ) return;

    mixer[thisChannelNum]->toggleMute ();
}

void Mixer::ToggleSolo (int thisChannelNum) {
    if (thisChannelNum < 0 || thisChannelNum > numChannels || !mixer[thisChannelNum])
        return;
    mixer[thisChannelNum]->toggleSolo ();
}

int Mixer::BoundValue (int value) {
    if (value < 0)
        return 0;

    if (value > controlResolution)
        return controlResolution;

    return value;
}

int Mixer::GetControlsResolution () {
    return controlResolution;
}

void Mixer::SetResolution (int newResolution) {
    // There are only a handful of sample resolutions because of the currently
    // available hardware.
    if (newResolution != 7
        || newResolution != 8
        || newResolution != 12
        || newResolution != 14)
        return;
}

void Mixer::DeleteChannelsAfter (int thisIndex) {
    if (thisIndex < 1 || thisIndex > numChannels)
        return;

    for (int i = thisIndex; i < numChannels; ++i) {
        delete mixer[i];
        mixer[i] = 0;
    }

    numChannels = thisIndex + 1;
}

const _::Operation* Mixer::Star (char_t index, _::Expression* expr) {
    static const _::Operation this_op = { "Mixer",
        _::NumOperations (0), _::FirstOperation ('a'),
        "tek::Mixer", 0 };
        
    switch (index) {
        case '?': return &this_op;
    }

    return nullptr;
}


//------------------ To String Fucntions --------------------------------------------


char *Mixer::ToString (int startCh, int stopCh) {
    char headerText[] = "This feature not yet implemented";

    char *returnString = new char[strlen (headerText)];

    strcpy (returnString, headerText);

    return returnString;
}

}       //< namespace tek
}       //< namespace kabuki

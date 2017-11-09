/** kabuki::tek
    @file    ~/source/kabuki/tek/impl/tek_mixer_channel.cc
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

#include "../include/mixer_channel.h"

namespace kabuki { namespace tek {

const int MixerChannel::defaultNumAuxSends = 3;

MixerChannel::MixerChannel  (const char *init_name):
    labelText  (Label::format  (initName)),
    volValue  (0),
    panValue  (0),
    muteValue  (0),
    soloValue  (0),
    numAuxSends  (defaultNumAuxSends)
{
    auxSend = new int[defaultNumAuxSends];
    for  (int i=0; i < defaultNumAuxSends; ++i)
        auxSend[i] = 0;
}

MixerChannel::~MixerChannel ()
{
    delete auxSend;
}

int MixerChannel::GetVolume ()
{
    return volValue;
}

int MixerChannel::Pan ()
{
    return panValue;
}

int MixerChannel::IsMuted ()
{
    return muteValue;
}

int MixerChannel::IsSoloed ()
{
    return soloValue;
}

void MixerChannel::SetVolume  (int value)
{
    volValue = value;
}

void MixerChannel::SetPan  (int value)
{
    panValue = value;
}

void MixerChannel::SetMute  (bool isMuted)
{
    muteValue = isMuted;
}

void MixerChannel::SetSolo  (bool isSoloed)
{
    soloValue = isSoloed;
}
void MixerChannel::ToggleMute ()
{
    if  (muteValue)
        muteValue = false;
    else
        muteValue = true;
}
void MixerChannel::ToggleSolo ()
{
    if  (soloValue)
        soloValue = false;
    else
        soloValue = true;
}

int MixerChannel::Aux  (int thisAux)
{
    if  (thisAux < 0 || thisAux >= numAuxSends)
        return 0;
    return auxSend[thisAux];
}

void MixerChannel::SetAux  (int thisAuxSend, int level)
{
    if  (thisAuxSend < 0 || thisAuxSend >= numAuxSends)
        return;
    auxSend[thisAuxSend] = level;
}

void MixerChannel::DeleteAux  (int thisIndex)
{
    if  (thisIndex < 0 || thisIndex > numAuxSends)

    for  (int i=thisIndex; i < numAuxSends-1; i)
        auxSend[i] = auxSend[++i];

    --numAuxSends;
}

void MixerChannel::InsertAux  (int index)
{
    if  (thisIndex < 0 || thisIndex > numAuxSends)
        return;

    int *newAuxSendArray = new int[numAuxSends+1];

    int i=0;

    for  (i; i < numAuxSends-1; i)
        newAuxSendArray[i] = auxSend[++i];

    for  (i; i < numAuxSends-1; i)
        auxSend[i] = auxSend[++i];

    ++numAuxSends;
}

const _::Operation* MixerChannel::Star (char_t index, _::Expression* expr) {

    static const _::Operation this_op = { "MixerChannel",
        _::NumOperations (0), _::FirstOperation ('a'),
        "tek::Mixer", 0
    };

    switch (index) {
        case '?': return &this_op;
    }
    return nullptr;
}

}       //< namespace tek
}       //< namespace kabuki

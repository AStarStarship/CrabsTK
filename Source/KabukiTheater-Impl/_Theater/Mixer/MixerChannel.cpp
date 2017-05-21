/** Kabuki Firmware Development Kit
    @file    /.../KabukiFDK-Impl/MixerChannel/MixerChannel.cpp
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

#include <MixerChannel/MixerChannel.hpp>

namespace _Theater { namespace MixerChannel {

const int MixerChannel::defaultNumAuxSends = 3;

MixerChannel::MixerChannel  (const char *initName):
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

int MixerChannel::getVolume ()
{
    return volValue;
}

int MixerChannel::pan ()
{
    return panValue;
}

int MixerChannel::isMuted ()
{
    return muteValue;
}

int MixerChannel::isSoloed ()
{
    return soloValue;
}

void MixerChannel::setVolume  (int value)
{
    volValue = value;
}

void MixerChannel::setPan  (int value)
{
    panValue = value;
}

void MixerChannel::setMute  (bool isMuted)
{
    muteValue = isMuted;
}

void MixerChannel::setSolo  (bool isSoloed)
{
    soloValue = isSoloed;
}
void MixerChannel::toggleMute ()
{
    if  (muteValue)
        muteValue = false;
    else
        muteValue = true;
}
void MixerChannel::toggleSolo ()
{
    if  (soloValue)
        soloValue = false;
    else
        soloValue = true;
}





int MixerChannel::aux  (int thisAux)
{
    if  (thisAux < 0 || thisAux >= numAuxSends)
        return 0;
    return auxSend[thisAux];
}



void MixerChannel::setAux  (int thisAuxSend, int level)
{
    if  (thisAuxSend < 0 || thisAuxSend >= numAuxSends)
        return;
    auxSend[thisAuxSend] = level;
}



void MixerChannel::deleteAux  (int thisIndex)
{
    if  (thisIndex < 0 || thisIndex > numAuxSends)

    for  (int i=thisIndex; i < numAuxSends-1; i)
        auxSend[i] = auxSend[++i];

    --numAuxSends;
}



void MixerChannel::insertAux  (int thisIndex)
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

byte MixerChannel::getState ()
{
    return 0;
}

const char* MixerChannel::setState (byte Value)
{
    return 0;
}

const char* MixerChannel::op (Terminal* io, int index)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return Query ? Enquery ("MixerChannel", "_Theater::Mixer"): InvalidIndex ();
}

}   //< Mixer
}   //< _Theater

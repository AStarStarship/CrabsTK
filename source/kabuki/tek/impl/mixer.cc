/** Kabuki Tek
    @file    /.../Source/KabukiTek-Impl/Mixer/Mixer.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright  (C) 2017 [Cale McCollough](calemccollough.github.io)

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

#include <tek/Mixer/Mixer.h>
#include <tek/Mixer/MixerChannel.h"

#include <string.h>

namespace tek { namespace Mixer {


Mixer::Mixer ():
    numChannels  (0),
    mixerSize  (defaultMixerArraySize),
    controlResolution  (7),
    mixer  (new MixerChannel *[defaultMixerArraySize])
{
    mixer[0] = new MixerChannel ("Master");

    for  (int i=1; i < defaultMixerArraySize; ++i)
        mixer[i] = 0;
}

Mixer::~Mixer ()
{
    for  (int i = 0; i < numChannels+1; ++i)
        delete mixer[i];
    delete mixer;
}

int Mixer::getVolume (int channelNum)
{
    return mixer[channelNum]->getVolume ();
}

int Mixer::getPan  (int channelNum)
{
    return mixer[channelNum]->getPan ();
}

void Mixer::setVolume  (int channelNum, int value)
{
    if  (  channelNum < 0
       || channelNum > numChannels
       || value   < 0
       || value   > controlResolution
      ) return;

    mixer[channelNum]->setVolume  (value);
}

void Mixer::setPan  (int channelNum, int value)
{
    if  (  channelNum < 0
       || channelNum > numChannels
       || value   < 0
       || value   > controlResolution
      ) return;

    mixer[channelNum]->setPan  (value);
}

int Mixer::isMuted  (int channelNum)
{
    return mixer[channelNum]->isMuted ();
}

int Mixer::isSoloed  (int channelNum)
{
    return mixer[channelNum]->isSoloed ();
}

void Mixer::setMute  (int channelNum, bool isMuted)
{
    if  (channelNum < 0 || channelNum > numChannels)
        return;
    mixer[channelNum]->setMute  (isMuted);
}

void Mixer::setSolo  (int channelNum, bool isMuted)
{
    if  (channelNum < 0 || channelNum > numChannels)
        return;
    mixer[channelNum]->setSolo  (isMuted);
}

void Mixer::toggleMute  (int thisChannelNum)
{
    if  (  thisChannelNum < 0
       || thisChannelNum > numChannels
       || !mixer[thisChannelNum]
      ) return;

    mixer[thisChannelNum]->toggleMute ();
}

void Mixer::toggleSolo  (int thisChannelNum)
{
    if  (thisChannelNum < 0 || thisChannelNum > numChannels || !mixer[thisChannelNum])
        return;
    mixer[thisChannelNum]->toggleSolo ();
}

int Mixer::boundValue  (int thisValue)
{
    if  (thisValue < 0)
        return 0;

    if  (thisValue > controlResolution)
        return controlResolution;

    return thisValue;
}

int Mixer::getControlsResolution ()
{
    return controlResolution;
}

void Mixer::setResolution  (int newResolution)
{
 // There are only a handfull of sample resolutions because of the currently availble hardware.
    if  (  newResolution != 7
       || newResolution != 8
       || newResolution != 12
       || newResolution != 14)
        return;
}

void Mixer::DeleteChannelsAfter  (int thisIndex)
{
    if  (thisIndex < 1 || thisIndex > numChannels)
        return;

    for  (int i=thisIndex; i < numChannels; ++i)
    {
        delete mixer[i];
        mixer[i] = 0;
    }

    numChannels = thisIndex+1;
}

byte Mixer::getState ()
{
    return 0;
}

const char* Mixer::setState (byte Value)
{
    return 0;
}

const Member* Mixer::Op (byte index, Uniprinter* io)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return Query ? Enquery ("Mixer", "tek::Mixer"): InvalidIndex ();
}


//------------------ To String Fucntions --------------------------------------------


char *Mixer::ToString  (int startCh, int stopCh)
{
    char headerText[] = "This feature not yet implemented";

    char *returnString = new char[strlen  (headerText)];

    strcpy  (returnString, headerText);

    return returnString;
}

}   //< namespace Mixer
}   //< namespace _

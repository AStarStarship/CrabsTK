/** Kabuki Toolkit
    @file    ~/source/kabuki/hmi/mixer.cc
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

#include "mixer.h"
#include "mixer_channel.h"

namespace kabuki { namespace hmi {

Mixer::Mixer ():
    num_channels_  (0),
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
    for  (int i = 0; i < num_channels_+1; ++i)
        delete mixer[i];
    delete mixer;
}

int Mixer::GetVolume (int channelNum)
{
    return mixer[channelNum]->getVolume ();
}

int Mixer::GetPan  (int channelNum)
{
    return mixer[channelNum]->getPan ();
}

void Mixer::SetVolume  (int channelNum, int value)
{
    if  (  channelNum < 0
       || channelNum > num_channels_
       || value   < 0
       || value   > controlResolution
      ) return;

    mixer[channelNum]->setVolume  (value);
}

void Mixer::SetPan  (int channelNum, int value)
{
    if  (  channelNum < 0
       || channelNum > num_channels_
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

void Mixer::SetMute  (int channelNum, bool isMuted)
{
    if  (channelNum < 0 || channelNum > num_channels_)
        return;
    mixer[channelNum]->setMute  (isMuted);
}

void Mixer::SetSolo  (int channelNum, bool isMuted)
{
    if  (channelNum < 0 || channelNum > num_channels_)
        return;
    mixer[channelNum]->setSolo  (isMuted);
}

void Mixer::toggleMute  (int channel)
{
    if  (  channel < 0
       || channel > num_channels_
       || !mixer[channel]
      ) return;

    mixer[channel]->toggleMute ();
}

void Mixer::toggleSolo  (int channel)
{
    if  (channel < 0 || channel > num_channels_ || !mixer[channel])
        return;
    mixer[channel]->toggleSolo ();
}

int Mixer::boundValue  (int value)
{
    if  (value < 0)
        return 0;

    if  (value > controlResolution)
        return controlResolution;

    return value;
}

int Mixer::GetControlsResolution ()
{
    return controlResolution;
}

void Mixer::SetResolution  (int newResolution)
{
 // There are only a handfull of sample resolutions because of the currently availble hardware.
    if  (  newResolution != 7
       || newResolution != 8
       || newResolution != 12
       || newResolution != 14)
        return;
}

void Mixer::DeleteChannelsAfter  (int index)
{
    if  (index < 1 || index > num_channels_)
        return;

    for  (int i=index; i < num_channels_; ++i)
    {
        delete mixer[i];
        mixer[i] = 0;
    }

    num_channels_ = index+1;
}

byte Mixer::GetState ()
{
    return 0;
}

const char* Mixer::SetState (byte Value)
{
    return 0;
}

const Operation* Mixer::op (Expression* io, byte index)
{
    switch (Index)
    {
        case 0: return Script::NumMembers (0);
    }
    
    return Query ? Enquery ("Mixer", "kabuki::Mixer"): InvalidIndex ();
}


//------------------ To String Fucntions --------------------------------------------


char *Mixer::toString  (int startCh, int stopCh)
{
    char headerText[] = "This feature not yet implemented";

    char *returnString = new char[strlen  (headerText)];

    strcpy  (returnString, headerText);

    return returnString;
}

}    //< namespace hmi
}    //< namespace kabuki

/** Kabuki Starship
    @file    ~/Source/_Theater/Mixer/Mixer.h
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
 
#pragma once

#include <KabukiTheater-Config.h>

namespace _Theater { namespace Mixer {

class _KabukiTheater_ MixerChannel;

template<typename T>
class _KabukiTheater_ Mixer
/*< A generic parameter mixer.


*/
{
    public:

    typedef enum 
    /** . */
    {
        MIDI = 0,
        DMX  = 1,
    };

    static const int MinMixerArraySize = 1;      //< The default mixer array size.

    Mixer ();
    //< Default constructor.

    ~Mixer ();
    //< Deconstructor.

    int getNumChannels ();
    //< Gets th number of channels on the mixer.

    void setNumChannels (int value);
    /*< Resizes the mixer to the given value.
        @pre 0 <= numChannels <= newNumChannels. */

    int getControlsResolution ();
    //< Gets th DAC resolution of the controls.

    void setResolution (int value);
    //< Sets the resolution of the controls.

    int getMixerType ();
    /*< Gets true if the mixer is in MIDI mode.
        i.e. controlResolution = 7 bits. */

    int getVolume (int channelNum);
    //< Gets th volume of the ChannelNum.

    int getPan (int channelNum);
    //< Gets th pan value of the ChannelNum.

    void setVol (int channelNum, int value);
    //< Sets the volume of the ChannelNum.

    void setPan (int channelNum, int value);
    /*< Sets the pan of the ChannelNum.
        @pre - (controlResolution/2) < value + (controlResolution/2)
        @pre 0 < ChannelNum < numChannels */

    void setMute (int channelNum, bool isMuted);
    //< Sets the mute flag to isMuted.

    void setSolo (int channelNum, bool isSoloed);
    //< Sets the solo flag to isSoloed.

    int  isMuted (int channelNum);
    //< Gets true if the ChannelNum is muted.

    int isSoloed (int channelNum);
    //< Gets true if the ChannelNum is soloed.

    void toggleMute (int thisChannelNum);
    //< Toggles the mute on thisChannelNum.

    void toggleSolo (int thisChannelNum);
    //< Toggles the solo on thisChannelNum.

    int boundValue (int thisValue);
    //< Bounds and returns thisValue to the controls resolution.

    void moveChannel (int channel, int newIndex);
    /*< Moves channel to the newIndex.
        @pre (0 < thisChannel < numChannels)
        @pre (0 < newIndex < numChannels)  */

    void addChannels (int numChannels);
    /*< Adds a specified numChannels to the mixer.
        @pre (0 < numChannels < NUM_MIXER_CHANNELS - numChannels) */

    void print (int startCh, int stopCh);
    //< Prints a string representation of mixer channels startCh - stopCh.
    
    inline void print (_::Terminal& io);
    /*< Prints this object to a terminal. */

    private:

    int numChannels,        //< The number of active channels in the mixer.
        mixerSize,          //< The size of the mixer array.
        controlResolution;  //< The resolution of the ADCs of for the controls.

    MixerChannel ** mixer;
    /*< The main MixerChannel array.
        Channel 0 is the master fadder. The pointers to pointers allows for 0 entries. */

    void deleteChannelsAfter (int thisIndex);
    /*< deletes all of the channels after thisIndex.
        @pre 0 < thisIndex < numChannels */
};
}   //< namespace Mixer
}   //< namespace _Theater


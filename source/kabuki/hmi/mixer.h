/** Kabuki Toolkit
    @file    ~/source/kabuki/mixer.h
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
 
#ifndef HEADER_FOR_KABUKI_HMI_MIXER
#define HEADER_FOR_KABUKI_HMI_MIXER

#include "config.h"

namespace kabuki { namespace hmi {

class KABUKI MixerChannel;

/** A generic parameter mixer.


*/
template<typename T>
class KABUKI Mixer {
    public:

    typedef enum 
    /** . */
    {
        MIDI = 0,
        DMX  = 1,
    };

    enum {
        MinMixerArraySize = 1; //< Default mixer array size.
    };

    /** Default constructor. */
    Mixer ();

    /** Deconstructor. */
    ~Mixer ();

    /** Gets th number of channels on the mixer.
    int GetNumChannels ();

    /** Resizes the mixer to the given value.
        @pre 0 <= num_channels_ <= newNumChannels. */
    void SetNumChannels (int value);

    /** Gets th DAC resolution of the controls. */
    int GetControlsResolution ();

    /** Sets the resolution of the controls. */
    void SetResolution (int value);

    /** Gets true if the mixer is in MIDI mode.
        i.e. controlResolution = 7 bits. */
    int GetMixerType ();

    /** Gets th volume of the ChannelNum. */
    int GetVolume (int channel);

    /** Gets th pan value of the ChannelNum. */
    int GetPan (int channel);

    /** Sets the volume of the ChannelNum. */
    void SetVol (int channel, int value);

    /** Sets the pan of the ChannelNum.
        @pre - (controlResolution/2) < value + (controlResolution/2)
        @pre 0 < ChannelNum < num_channels_ */
    void SetPan (int channel, int value);

    /** Sets the mute flag to is_muted. */
    void SetMute (int channel, bool is_muted);

    /** Sets the solo flag to is_soloed. */
    void SetSolo (int channel, bool is_soloed);

    /** Gets true if the ChannelNum is muted. */
    int  IsMuted (int channel);

    /** Gets true if the ChannelNum is soloed. */
    int IsSoloed (int channel);

    /** Toggles the mute on channel. */
    void ToggleMute (int channel);

    /** Toggles the solo on channel. */
    void ToggleSolo (int channel);

    /** Bounds and returns value to the controls resolution. */
    int BoundValue (int value);

    /** Moves channel to the index.
        @pre (0 < thisChannel < num_channels_)
        @pre (0 < index < num_channels_)  */
    void MoveChannel (int channel, int index);

    /** Adds a specified num_channels_ to the mixer.
        @pre (0 < num_channels_ < NUM_MIXER_CHANNELS - num_channels_) */
    void AddChannels (int num_channels);

    /** Prints a string representation of mixer channels start_channel - stop_channel. */
    void Print (int start_channel, int stop_channel);
    
    /** Prints this object to a terminal. */
    inline void Print (_::Expression& io);

    private:

    int num_channels_,       //< number of active channels in the mixer.
        mixer_size_,         //< Size of the mixer array.
        control_resolution_; //< Resolution of the ADCs of for the controls.
     
    /** The main MixerChannel array.
        Channel 0 is the master fadder. The pointers to pointers allows for 0 entries. */
    MixerChannel ** mixer;

    /** Deletes all of the channels after index.
        @pre 0 < index < num_channels_ */
    void DeleteChannelsAfter (int index);
};
}       //< namespace hmi
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_HMI_MIXER

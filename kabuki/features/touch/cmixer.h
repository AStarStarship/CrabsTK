/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/touch/mixer.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-19 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_FEATURES_TOUCH_1
#ifndef HEADER_FOR_KT_HMI_MIXER
#define HEADER_FOR_KT_HMI_MIXER

#include "config.h"

namespace _ {

class SDK MixerChannel;

/* A generic parameter mixer.


*/
template <typename T>
class SDK Mixer {
 public:
  typedef enum {
    MIDI = 0,
    DMX = 1,
  };

  enum {
    MinMixerArraySize = 1;  //< Mixer array size default.
  };

  /* Default constructor. */
  Mixer();

  /* Destructs nothing. */
  ~Mixer();

  /* Gets th number of channels on the mixer.
  SI4 GetNumChannels ();

  /* Resizes the mixer to the given value.
      @pre 0 <= num_channels_ <= newNumChannels. */
  void SetNumChannels(SI4 value);

  /* Gets th DAC resolution of the controls. */
  SI4 GetControlsResolution();

  /* Sets the resolution of the controls. */
  void SetResolution(SI4 value);

  /* Gets true if the mixer is in MIDI mode.
      i.e. controlResolution = 7 bits. */
  SI4 GetMixerType();

  /* Gets th volume of the ChannelNum. */
  SI4 GetVolume(SI4 channel);

  /* Gets th pan value of the ChannelNum. */
  SI4 GetPan(SI4 channel);

  /* Sets the volume of the ChannelNum. */
  void SetVol(SI4 channel, SI4 value);

  /* Sets the pan of the ChannelNum.
    @pre - (controlResolution/2) < value + (controlResolution/2)
    @pre 0 < ChannelNum < num_channels_ */
  void SetPan(SI4 channel, SI4 value);

  /* Sets the mute flag to is_muted. */
  void SetMute(SI4 channel, BOL is_muted);

  /* Sets the solo flag to is_soloed. */
  void SetSolo(SI4 channel, BOL is_soloed);

  /* Gets true if the ChannelNum is muted. */
  SI4 IsMuted(SI4 channel);

  /* Gets true if the ChannelNum is soloed. */
  SI4 IsSoloed(SI4 channel);

  /* Toggles the mute on channel. */
  void ToggleMute(SI4 channel);

  /* Toggles the solo on channel. */
  void ToggleSolo(SI4 channel);

  /* Bounds and returns value to the controls resolution. */
  SI4 BoundValue(SI4 value);

  /* Moves channel to the index.
    @pre (0 < thisChannel < num_channels_)
    @pre (0 < index < num_channels_)  */
  void MoveChannel(SI4 channel, SI4 index);

  /* Adds a specified num_channels_ to the mixer.
  @pre (0 < num_channels_ < NUM_MIXER_CHANNELS - num_channels_) */
  void AddChannels(SI4 num_channels);

  /* Prints a string representation of mixer channels start_channel -
  stop_channel. */
  void Print(SI4 start_channel, SI4 stop_channel);

  /* Prints this object to a terminal. */
  Utf& Print(Utf& printer);

 private:
  SI4 num_channels_,        //< number of active channels in the mixer.
      mixer_size_,          //< Size of the mixer array.
      control_resolution_;  //< Resolution of the ADCs of for the controls.

  /* The main MixerChannel array.
  Channel 0 is the master fader. The pointers to pointers allows for 0
     entries. */
  MixerChannel** mixer;

  /* Deletes all of the channels after index.
  @pre 0 < index < num_channels_ */
  void DeleteChannelsAfter(SI4 index);
};
}  // namespace _
#endif  //< HEADER_FOR_KT_HMI_MIXER
#endif  //< #if SEAM >= KABUKI_FEATURES_TOUCH_1

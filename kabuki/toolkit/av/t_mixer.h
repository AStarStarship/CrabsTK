/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/av/t_mixer.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-19 Cale McCollough <cale@astartup.net>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_TOOLKIT_AV_1
#ifndef KABUKI_TOOLKIT_AV_MIXER
#define KABUKI_TOOLKIT_AV_MIXER

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
  Mixer()
    : channel_count_ (0),
    mixerSize (defaultMixerArraySize),
    control_resolution_ (7),
    mixer (new MixerChannel*[defaultMixerArraySize]) {
    mixer[0] = new MixerChannel ("Master");

    for (SI4 i = 1; i < defaultMixerArraySize; ++i) mixer[i] = 0;
  }

  /* Destructs nothing. */
  ~Mixer () {}

  /* Gets th number of channels on the mixer. */
  SI4 GetNumChannels ();

  /* Resizes the mixer to the given value.
  @contract 0 <= channel_count_ <= newNumChannels. */
  void SetNumChannels(SI4 value);

  /* Gets th DAC resolution of the controls. */
  SI4 GetControlsResolution() { return control_resolution_; }

  /* Sets the resolution of the controls. */
  void SetResolution(SI4 value) {
    // There are only a handfull of sample resolutions because of the currently
    // availble hardware.
    if (newResolution != 7 || newResolution != 8 || newResolution != 12 ||
      newResolution != 14)
      return;
  }

  /* Gets true if the mixer is in MIDI mode.
  i.e. control_resolution_ = 7 bits. */
  SI4 GetMixerType();

  /* Gets th level of the ChannelNum. */
  SI4 GetVolume(SI4 channel) { return mixer_[channel]->Level (); }

  /* Gets th pan value of the ChannelNum. */
  SI4 GetPan(SI4 channel) { return mixer_[channel]->getPan (); }

  /* Sets the level_. */
  void SetLevel(SI4 channel, SI4 value) {
    if (channel < 0 || channel > channel_count_ || value < 0 ||
      value > control_resolution_) {
      return;
    }
    mixer_[channel]->setVolume (value);
  }

  /* Sets the pan of the ChannelNum.
  @contract (control_resolution_/2) < value + (control_resolution_/2) &&
    0 < ChannelNum < channel_count_ */
  void SetPan(SI4 channel, SI4 value) {
    if (channel < 0 || channel > channel_count_ || value < 0 ||
      value > control_resolution_) {
      return;
    }
    mixer_[channel]->SetPan (value);
  }

  /* Sets the mute flag to is_muted. */
  void SetMute(SI4 channel, BOL is_muted) {
    if (channel < 0 || channel > channel_count_) return;
    mixer_[channel]->setMute (isMuted);
  }

  /* Sets the solo flag to is_soloed. */
  void SetSolo(SI4 channel, BOL is_soloed) {
    if (channel < 0 || channel > channel_count_) return;
    mixer_[channel]->SetSolo (is_soloed);
  }

  /* Gets true if the ChannelNum is muted. */
  SI4 IsMuted(SI4 channel);

  /* Gets true if the ChannelNum is soloed. */
  SI4 IsSoloed(SI4 channel);

  /* Toggles the mute on channel. */
  void ToggleMute(SI4 channel) {
    if (channel < 0 || channel > channel_count_ || !mixer_[channel]) return;

    mixer_[channel]->ToggleMute ();
  }

  /* Toggles the solo on channel. */
  void ToggleSolo(SI4 channel) {
    if (channel < 0 || channel > channel_count_ || !mixer_[channel]) return;
    mixer_[channel]->ToggleSolo ();
  }

  /* Bounds and returns value to the controls resolution. */
  SI4 BoundValue(SI4 value) {
    if (value < 0) return 0;
    if (value > control_resolution_) return control_resolution_;
    return value;
  }

  /* Moves channel to the index.
  @contract (0 < channel < channel_count_) && 
  (0 < index < channel_count_)  */
  void MoveChannel (SI4 channel, SI4 index) {

  }

  /* Adds a specified channel_count_ to the mixer.
  @pre (0 < channel_count_ < NUM_MIXER_CHANNELS - channel_count_) */
  void AddChannels (SI4 num_channels) {

  }

  /* Prints a string representation of mixer channels start_channel -
  stop_channel. */
  template<typename Printer>
  Printer& PrintChannels(Printer& o, SI4 start, SI4 stop) {
    DASSERT (start <= stop);
    o << "\nChannels " << start << " through " << stop;
    while (start <= stop) {
      o << mixer_[start];
    }
  }

  /* Prints this object to a terminal. */
  template<typename Printer> Printer& Print(Printer& o);

 private:
  SI4 channel_count_,        //< number of active channels in the mixer.
      mixer_size_,          //< Size of the mixer array.
      control_resolution_;  //< Resolution of the ADCs of for the controls.
  MixerChannel** mixer_;    //< Mixer channels.

  /* Deletes all of the channels after index.
  @contract 0 < index < channel_count_ */
  void DeleteChannelsAfter(SI4 index) {
    if (index < 1 || index > channel_count_) return;
    for (SI4 i = index; i < channel_count_; ++i) {
      delete mixer_[i];
      mixer_[i] = 0;
    }
    channel_count_ = index + 1;
  }
};
}  //< namespace _
#endif  //< KABUKI_TOOLKIT_AV_MIXER
#endif  //< #if SEAM >= KABUKI_TOOLKIT_AV_1

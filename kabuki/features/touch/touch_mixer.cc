/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/touch/touch_mixer.cc
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-19 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>
#if SEAM >= KABUKI_FEATURES_TOUCH_1
#if SEAM == KABUKI_FEATURES_TOUCH_1
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT_PAUSE(message)   \
  Printf("\n\n%s\n", message); \
  system("PAUSE");
#else
#define PRINTF(x, ...)
#define PRINT_PAUSE(message)
#endif

#include "mixer.h"
#include "mixer_channel.h"

namespace _ {

Mixer::Mixer()
    : num_channels_(0),
      mixerSize(defaultMixerArraySize),
      controlResolution(7),
      mixer(new MixerChannel*[defaultMixerArraySize]) {
  mixer[0] = new MixerChannel("Master");

  for (SI4 i = 1; i < defaultMixerArraySize; ++i) mixer[i] = 0;
}

Mixer::~Mixer() {
  for (SI4 i = 0; i < num_channels_ + 1; ++i) delete mixer[i];
  delete mixer;
}

SI4 Mixer::GetVolume(SI4 channelNum) { return mixer[channelNum]->getVolume(); }

SI4 Mixer::GetPan(SI4 channelNum) { return mixer[channelNum]->getPan(); }

void Mixer::SetVolume(SI4 channelNum, SI4 value) {
  if (channelNum < 0 || channelNum > num_channels_ || value < 0 ||
      value > controlResolution)
    return;

  mixer[channelNum]->setVolume(value);
}

void Mixer::SetPan(SI4 channelNum, SI4 value) {
  if (channelNum < 0 || channelNum > num_channels_ || value < 0 ||
      value > controlResolution)
    return;

  mixer[channelNum]->setPan(value);
}

SI4 Mixer::isMuted(SI4 channelNum) { return mixer[channelNum]->isMuted(); }

SI4 Mixer::isSoloed(SI4 channelNum) { return mixer[channelNum]->isSoloed(); }

void Mixer::SetMute(SI4 channelNum, BOL isMuted) {
  if (channelNum < 0 || channelNum > num_channels_) return;
  mixer[channelNum]->setMute(isMuted);
}

void Mixer::SetSolo(SI4 channelNum, BOL isMuted) {
  if (channelNum < 0 || channelNum > num_channels_) return;
  mixer[channelNum]->setSolo(isMuted);
}

void Mixer::toggleMute(SI4 channel) {
  if (channel < 0 || channel > num_channels_ || !mixer[channel]) return;

  mixer[channel]->toggleMute();
}

void Mixer::toggleSolo(SI4 channel) {
  if (channel < 0 || channel > num_channels_ || !mixer[channel]) return;
  mixer[channel]->toggleSolo();
}

SI4 Mixer::boundValue(SI4 value) {
  if (value < 0) return 0;

  if (value > controlResolution) return controlResolution;

  return value;
}

SI4 Mixer::GetControlsResolution() { return controlResolution; }

void Mixer::SetResolution(SI4 newResolution) {
  // There are only a handfull of sample resolutions because of the currently
  // availble hardware.
  if (newResolution != 7 || newResolution != 8 || newResolution != 12 ||
      newResolution != 14)
    return;
}

void Mixer::DeleteChannelsAfter(SI4 index) {
  if (index < 1 || index > num_channels_) return;

  for (SI4 i = index; i < num_channels_; ++i) {
    delete mixer[i];
    mixer[i] = 0;
  }

  num_channels_ = index + 1;
}

byte Mixer::GetState() { return 0; }

const CH1* Mixer::SetState(byte Value) { return 0; }

const Op* Mixer::op(Expression* io, byte index) {
  switch (Index) {
    case 0:
      return Script::NumMembers(0);
  }

  return Query ? Enquery("Mixer", "kabuki::Mixer") : InvalidIndex();
}

//------------------ To String Fucntions
//--------------------------------------------

CH1* Mixer::toString(SI4 startCh, SI4 stopCh) {
  CH1 headerText[] = "This feature not yet implemented";

  CH1* returnString = new CH1[strlen(headerText)];

  strcpy(returnString, headerText);

  return returnString;
}

}  // namespace _
#endif  //< #if SEAM >= KABUKI_FEATURES_TOUCH_1

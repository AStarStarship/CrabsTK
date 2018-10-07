/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/mixer_channel.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= SEAM_00_03_00_00__00
#ifndef HEADER_FOR_KT_HMI_MIXERCHANNEL
#define HEADER_FOR_KT_HMI_MIXERCHANNEL

#include "config.h"

namespace _ {

/* A mixer channel Operation. */
class API MixerChannel : public _::Op {
  NONCOPYABLE(MixerChannel)
 public:
  enum {
    DefaultNumAuxSends = 3,  //< Default number of aux sends.
  };

  /* Constructs a mixer channel with the given name. */
  MixerChannel(const char* initName = nullptr);

  /* Destructor. */
  ~MixerChannel();

  /* Gets th volume of this channel. */
  int GetVolume();

  /* Gets th pan of of the channel. */
  int GetPan();

  /* Gets if this channel is muted. */
  int IsMuted();

  /* Gets if this channel is soloed. */
  int IsSoloed();

  /* Sets the channel volume to the value. */
  void setVolume(int value);

  /* Sets the channel pan to the value. */
  void SetPan(int value);

  /* Either mutes or unmutes this channel. */
  void SetMute(bool isMuted);

  /* Either solos or unsolos this channel. */
  void SetSolo(bool isSoloed);

  /* Toggles the mute variable. */
  void ToggleMute();

  /* Toggles the solo variable. */
  void ToggleSolo();

  /* Gets thisAux level. */
  int GetAux(int thisAux);

  /* Sets thisAuxSend level. */
  void SetAux(int thisAuxSend, int level);

  /* Deletes the aux send channel at thisIndex.
    @warning This function does not trim the auxSend array size for speed
    considerations Inserts an aux send at thisIndex. */
  void DeleteAux(int thisIndex);

  /* Script Operations. */
  const _::Op Star(uint index, _::Expression expr);

 private:
  const char* label_text_;  //< Name of this channel.
  int vol_value_,           //< Volume value for this channel.
      pan_value_,           //< Pan value for this channel.
      mute_value_,          //< Mute value for this channel.
      solo_value_,          //< Solo value for this channel.
      num_aux_sends_;       //< Number of aux sends.
  int* aux_send_;           //< Array of aux send levels.

};  //< class MixerChannel
}  // namespace _
#endif  //< HEADER_FOR_KT_HMI_MIXERCHANNEL
#endif  //< #if SEAM >= SEAM_00_03_00_00__00

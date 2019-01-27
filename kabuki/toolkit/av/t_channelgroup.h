/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/av/t_t_channel_group.h
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
#ifndef KABUKI_TOOLKIT_AV_CHANNELGROUP
#define KABUKI_TOOLKIT_AV_CHANNELGROUP

#include "t_config.h"

namespace _ {

/* A group of */
class SDK ChannelGroup {
 public:
  /* Constructs a group of channels. */
   ChannelGroup () {}

  /* Gets the Script address of the channel. */
  const byte* GetChannelAddress(uint_t channel)) {
  if (channel > channels_.size ()) return nullptr;
  return channels_[channel];
}

  /* Sets the channel address Script device. */
  void SetChannelAddress(uint_t channel, const byte* address) {
    if (address == nullptr) return;
    size_t l_size = channels_.size ();
    if (channel > l_size) return;
    if (channel == l_size) {
      channels_.push_back (address);
      return;
    }
    channels_[channel] = address;
  }

  /* Adds a new channel to the group. */
  uint_t AddChannel(const byte* address) {
    if (address == nullptr) return ~0;
    channels_.push_back (address);
    return channels_.size () - 1;
  }

  /* Removes the channel at the given index if it exists.
      @return Returns false if the index is out of bounds. */
  BOL RemoveChannel(uint_t index) {
    if (index >= channels_.size ()) return false;
    channels_.erase (channels_.begin () + index, channels_.begin () + index + 1);
  }

  /* Prints this object to a string. */
  template<typename Printer>
  Printer& Print(Printer& o) const {
    size_t count = channels_.Count ();
    o << "ChannelGroup: Size:" << count;

    for (SI4 i = 0; i < count; ++i) {
      o << Right("", 9);
      const byte* ch = channels_[i];
      CH1 c;
      do {
        c = *ch;
        o << c << ' ';
        ++ch;
      } while (c != 0);
      o << '\n';
    }
  }

 private:
  TArray<const UI1*> channels_;  //< List of Script address for the channels.

};
}  // namespace _
#endif  //< KABUKI_TOOLKIT_AV_CHANNELGROUP
#endif  //< #if SEAM >= KABUKI_TOOLKIT_AV_1

/* Kabuki Toolkit
@file    ~/source/hmi/hmi_/channel_group.h
@author  Cale McCollough <calemccollough.github.io>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough@gmail.com>;
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

#pragma once
#include <pch.h>
#if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0
#ifndef HEADER_FOR_KT_HMI_CHANNELGROUP
#define HEADER_FOR_KT_HMI_CHANNELGROUP

#include "config.h"

namespace _ {

/* A group of */
class KABUKI ChannelGroup {
 public:
  /* Constructs a group of channels. */
  ChannelGroup();

  /* Gets the Script address of the channel. */
  const byte* GetChannelAddress(uint_t channel);

  /* Sets the channel address Script device. */
  void SetChannelAddress(uint_t channel, const byte* address);

  /* Adds a new channel to the group. */
  uint_t AddChannel(const byte* address);

  /* Removes the channel at the given index if it exists.
      @return Returns false if the index is out of bounds. */
  bool RemoveChannel(uint_t index);

  /* Prints this object to a string. */
  _::Utf& Print(_::Utf& print) const;

 private:
  _::Array<const byte*> channels_;  //< List of Script address for the channels.

};  //< class ChannelGroup
}       //< namespace _
#endif  //< HEADER_FOR_KT_HMI_CHANNELGROUP
#endif  //< #if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0

/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/av/t_mixerpage.h
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
#ifndef KABUKI_TOOLKIT_AV_MIXERPAGE
#define KABUKI_TOOLKIT_AV_MIXERPAGE

namespace _ {

class SDK Mixer;
class SDK MixerChannel;

/* Class that represents a page of mixer channels on the Symmetry Live!
A MixerPage stores 8 TemplatePages in an array. The user can switch through
banks of pages on the mixer. The mixer can either be sorted in ascending
track index order or the page banks can be customized to make buses.
*/
class SDK MixerPage {
  public:
    /* Constructor. */
    MixerPage(Mixer* mixer = nullptr, SI4 index_start = 0)
      : mixer_ (mixer) {
      DASSERT (mixer);
      if (index_start < 0) {
        index_start = 0;
      }
      else {
        index_start = index_start_;
      }

      for (SI4 i = 0; i < 8; ++i) channels_[i] = 0;
    }

    /* Destructor. */
    ~MixerPage() {}

    /* Gets the name_. */
    TStrand<>& Name () { return name_; }

    /* Gets the startingIndex of this MixerPage. */
    SI4 GetStartIndex () { return index_start_; }

    /* Sets the startingIndex to the newIndex. */
    void SetStartIndex(SI4 newIndex) {
      if (index < 0) return;
      if (mixer != 0)
        if (index > mixer->channel_count_ ()) return;

      startingIndex = index;  //< The channel index starts at 1.
    }

    /* Replaces channel with theNewOne. */
    void SetChannel (SI4 channel, MixerChannel* new_channel) {
      DASSERT (new_channel);
      return channels_.Set (channel, new_channel);
    }

    /* Gets a pointer to the channel[atThisIndex]. */
    MixerChannel* GetChannel (SI4 index) {
      return channels_[index];
    }

    /* Prints this object to a terminal. */
    template<typename Printer>
    Printer& Print (Printer& o) {
      return o << "\nMixerPage:" << 
    }

  private:
    TStrand<> name_;  //< MixerPage name.
    SI4 index_start_;  //< Starting index of this page this MixerPage.
    Mixer* mixer_;     //< Pointer to the global AVMixer.
    TArray<MixerChannel*> channels_;  //< The Mixer channels.

};
}  // namespace _
#endif  //< KABUKI_TOOLKIT_AV_MIXERPAGE
#endif  //< #if SEAM >= KABUKI_TOOLKIT_AV_1

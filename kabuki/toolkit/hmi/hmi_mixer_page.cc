/* Kabuki Toolkit
    @file    ~/source/hmi/hmi_mixer_page.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
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

#include <stdafx.h>
#if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
#if MAJOR_SEAM == 2 && MINOR_SEAM == 2
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PRINT_PAUSE(message)   \
  printf("\n\n%s\n", message); \
  system("PAUSE");
#else
#define PRINTF(x, ...)
#define PRINT_PAUSE(message)
#endif

#include "mixer.h"
#include "mixer_channel.h"
#include "mixer_page.h"

namespace _ {

MixerPage::MixerPage(Mixer* m, int initStartIndex) : mixer(m) {
  if (initStartIndex < 0)  // We currently don't have a way to check the upper
                           // bounds of the intiStartIndex.
    initStartIndex = 0;
  else
    startingIndex = initStartIndex;

  channel = new MixerPage*[8];  // Init that shit yo!
  for (int i = 0; i < 8; ++i) channel[i] = 0;
}

MixerPage::~MixerPage() {
  for (int i = 0; i < 8; ++i) delete channel[i];
  delete channel;
  delete mixer;
}

int MixerPage::startIndex() { return startingIndex; }

void MixerPage::SetStartIndex(int index) {
  if (index < 0) return;
  if (mixer != 0)
    if (index > mixer->num_channels_()) return;

  startingIndex = index;  //< The channel index starts at 1.
}

byte MixerPage::GetState() { return 0; }

const char* MixerPage::SetState(byte Value) { return 0; }

const char* MixerPage::op(Expression* io, byte index) {
  switch (Index) {
    case '?':
      if (!io) return memberDevice<"MixerPage", 0, "kabuki::Mixer::Mixer">();
      break;
  }

  return invalidIndex();
}

}       //< namespace _
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2

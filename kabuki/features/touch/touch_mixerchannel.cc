/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/touch/touch_mixer_channel.cc
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

#include "component.h"
#include "mixer_channel.h"

namespace _ {

using namespace kt;

MixerChannel::MixerChannel(const CH1* initName)
    : labelText(HMI::HmiComponent::format(initName)),
      volValue(0),
      panValue(0),
      muteValue(0),
      soloValue(0),
      numAuxSends(DefaultNumAuxSends) {
  auxSend = new SI4[DefaultNumAuxSends];
  for (SI4 i = 0; i < DefaultNumAuxSends; ++i) auxSend[i] = 0;
}

MixerChannel::~MixerChannel() { delete auxSend; }

SI4 MixerChannel::GetVolume() { return volValue; }

SI4 MixerChannel::GetPan() { return panValue; }

SI4 MixerChannel::isMuted() { return muteValue; }

SI4 MixerChannel::isSoloed() { return soloValue; }

void MixerChannel::SetVolume(SI4 value) { volValue = value; }

void MixerChannel::SetPan(SI4 value) { panValue = value; }

void MixerChannel::SetMute(BOL isMuted) { muteValue = isMuted; }

void MixerChannel::SetSolo(BOL isSoloed) { soloValue = isSoloed; }

void MixerChannel::toggleMute() {
  if (muteValue)
    muteValue = false;
  else
    muteValue = true;
}

void MixerChannel::toggleSolo() {
  if (soloValue)
    soloValue = false;
  else
    soloValue = true;
}

SI4 MixerChannel::GetAux(SI4 thisAux) {
  if (thisAux < 0 || thisAux >= numAuxSends) return 0;
  return auxSend[thisAux];
}

void MixerChannel::SetAux(SI4 thisAuxSend, SI4 level) {
  if (thisAuxSend < 0 || thisAuxSend >= numAuxSends) return;
  auxSend[thisAuxSend] = level;
}

void MixerChannel::deleteAux(SI4 index) {
  if (index < 0 || index > numAuxSends)

    for (SI4 i = index; i < numAuxSends - 1; i) auxSend[i] = auxSend[++i];

  --numAuxSends;
}

void MixerChannel::insertAux(SI4 index) {
  if (index < 0 || index > numAuxSends) return;

  SI4* newAuxSendArray = new SI4[numAuxSends + 1];

  SI4 i = 0;

  for (i; i < numAuxSends - 1; i) newAuxSendArray[i] = auxSend[++i];

  for (i; i < numAuxSends - 1; i) auxSend[i] = auxSend[++i];

  ++numAuxSends;
}

const CH1* MixerChannel::op(Expression* io, byte index) {
  switch (Index) {
    case 0:
      return Script::NumMembers(0);
  }

  return Query ? Enquery("MixerChannel", "kabuki::Mixer") : InvalidIndex();
}

}  // namespace _
#endif  //< #if SEAM >= KABUKI_FEATURES_TOUCH_1

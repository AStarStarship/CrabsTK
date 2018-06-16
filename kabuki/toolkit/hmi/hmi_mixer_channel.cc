/* Kabuki Toolkit
    @file    ~/source/hmi/hmi_mixer_channel.cc
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

#include "component.h"
#include "mixer_channel.h"

namespace _ {

using namespace kt;

MixerChannel::MixerChannel(const char* initName)
    : labelText(HMI::HmiComponent::format(initName)),
      volValue(0),
      panValue(0),
      muteValue(0),
      soloValue(0),
      numAuxSends(DefaultNumAuxSends) {
  auxSend = new int[DefaultNumAuxSends];
  for (int i = 0; i < DefaultNumAuxSends; ++i) auxSend[i] = 0;
}

MixerChannel::~MixerChannel() { delete auxSend; }

int MixerChannel::GetVolume() { return volValue; }

int MixerChannel::GetPan() { return panValue; }

int MixerChannel::isMuted() { return muteValue; }

int MixerChannel::isSoloed() { return soloValue; }

void MixerChannel::SetVolume(int value) { volValue = value; }

void MixerChannel::SetPan(int value) { panValue = value; }

void MixerChannel::SetMute(bool isMuted) { muteValue = isMuted; }

void MixerChannel::SetSolo(bool isSoloed) { soloValue = isSoloed; }

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

int MixerChannel::GetAux(int thisAux) {
  if (thisAux < 0 || thisAux >= numAuxSends) return 0;
  return auxSend[thisAux];
}

void MixerChannel::SetAux(int thisAuxSend, int level) {
  if (thisAuxSend < 0 || thisAuxSend >= numAuxSends) return;
  auxSend[thisAuxSend] = level;
}

void MixerChannel::deleteAux(int index) {
  if (index < 0 || index > numAuxSends)

    for (int i = index; i < numAuxSends - 1; i) auxSend[i] = auxSend[++i];

  --numAuxSends;
}

void MixerChannel::insertAux(int index) {
  if (index < 0 || index > numAuxSends) return;

  int* newAuxSendArray = new int[numAuxSends + 1];

  int i = 0;

  for (i; i < numAuxSends - 1; i) newAuxSendArray[i] = auxSend[++i];

  for (i; i < numAuxSends - 1; i) auxSend[i] = auxSend[++i];

  ++numAuxSends;
}

const char* MixerChannel::op(Expression* io, byte index) {
  switch (Index) {
    case 0:
      return Script::NumMembers(0);
  }

  return Query ? Enquery("MixerChannel", "kabuki::Mixer") : InvalidIndex();
}

}       //< namespace _
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2

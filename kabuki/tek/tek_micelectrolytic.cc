/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/tek/tek_micelectrolytic.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include "cmicelectrolytic.h"

namespace _ {

ElectrolyticMic::ElectrolyticMic(PinName pin)
    : min_(0), max_(~0), input_(pin) {}

UI2 ElectrolyticMic::GetMin() { return min_; }

void ElectrolyticMic::SetMin(UI2 value) {
  if (value > max_) return;
  min_ = value;
}

UI2 ElectrolyticMic::GetMax() { return max_; }

void ElectrolyticMic::SetMax(UI2 value) {
  if (value < min_) return;
  max_ = value;
}

void ElectrolyticMic::BoundMinMax() {
  UI2 sample = input_.read_u16();

  if (sample < min_)
    min_ = sample;
  else if (sample > max_)
    max_ = sample;
}

UI2 ElectrolyticMic::Read() { return input_.read_u16(); }

}  // namespace _

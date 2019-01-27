/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/tek/tek_cled.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include "c_led.h"

namespace _ {

inline void TurnLedOn(UI1* spiOutBytes, offset_t bit_number) {
  offset_t bit = bit_number;
  spiOutBytes[bit >> kBitToByteShift] |= (1 << (bit & kBitNumberMask));
}

inline void TurnLedOff(UI1* spiOutBytes, offset_t bit_number) {
  offset_t bit = bit_number;
  spiOutBytes[bit >> kBitToByteShift] &= ~(1 << (bit & kBitNumberMask));
}

inline void ToggleLed(UI1* spiOutBytes, offset_t bit_number_) {
  offset_t bit = bit_number_;
  UI1* pointerToByte = spiOutBytes + (bit >> kBitToByteShift);
  UI1 UI1 = *pointerToByte, mask = 1 << (bit & (bit & kBitNumberMask));
  if (UI1 == 0) {
    *pointerToByte = UI1 | mask;
    return;
  }
  *pointerToByte = UI1 & ~mask;
}

inline BOL GetLedState(UI1* spiOutBytes, offset_t offset) {
  return spiOutBytes[offset >> kBitToByteShift] &
         (1 << (offset & kBitNumberMask));
}

inline void SetLedState(UI1* spiOutBytes, offset_t bit_number_, BOL state) {
  offset_t temp = bit_number_;
  UI1 mask = (UI1)(1 << (temp & kBitNumberMask));
  if (state) {
    spiOutBytes[temp >> kBitToByteShift] |= mask;
    return;
  }
  spiOutBytes[temp >> 3] &= ~mask;
}

Led::Led(offset_t bit, offset_t row) : bit_number_(bit), row_number_(row) {
  /// ???
}

const Operation* Led::Star(char_t index, Expr* expr) {
  static const Operation This = {"LED", NumOperations(0), FirstOperation('A'),
                                 "tek", 0};

  switch (index) {
    case '?':
      return &This;
  }

  return nullptr;
}

LedOp::LedOp(Led* object) : object_(object) {
  // Nothing to do here!
}

const Operation* LedOp::Star(char_t index, Expr* expr) {
  object_->Star(index, expr);
}

}  // namespace _

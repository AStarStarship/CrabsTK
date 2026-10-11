/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/tek/tek_button.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include "c_button.h"

namespace _ {

Button::Button(offset_t bit) : bit_offset_(bit) {}

inline UI1 Button::GetState(UI1* digital_ins) {
  if (!digital_ins) {
    return 0;
  }
  offset_t bit = bit_offset_;
  return digital_ins[bit >> kBitToByteShift] & (1 << (bit & kBitNumberMask));
}

inline void Button::Poll(UI1* debounced_xor) {
  offset_t index = bit_offset_ >> kBitToByteShift,
           mask = 1 << (bit_offset_ & kBitNumberMask),
           state = debounced_xor[index] & mask;

  if (state == 0) {
    return;
  }
  if (debounced_xor[index] & mask) {
    press_->Trigger();
    return;
  }

  depress_->Trigger();
}

ButtonOp::ButtonOp(Button* object) : object_(object) {
  // Nothing to do here!
}

const Operation* ButtonOp::Star(char_t index, Expr* expr) {
  object_->Star(index, expr);
}

}  // namespace _

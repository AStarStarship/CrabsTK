/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/tek/cbutton.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#ifndef INCLUDED_KABUKI_FEATURE_TEK_SENSOR_BUTTON
#define INCLUDED_KABUKI_FEATURE_TEK_SENSOR_BUTTON

#include "mbed.h"

namespace _ {

/* A button stored as an offset.
Buttons are composed of a unsigned integer type that stores the bit offset
from the Controller. This bit offset then gets translated to a UI1 offset
and a mask through some bit shifting.
*/
class Button {
 public:
  /* Constructs a button with the given bit offset. */
  Button(offset_t bit);

  /* Gets the state of the button. */
  inline UI1 GetState(UI1* digital_ins);

  /* Polls the state of the switch using the debounced XOR. */
  inline void Poll(UI1* debounced_xor);

  /* Script operations. */
  const _::Operation* Star(char_t index, _::Expr* expr);

 private:
  offset_t bit_offset_;  //< Offset to the packed button bits array.
  _::Event *press_,      //< Handler for button pressed event.
      *depress_,         //< Handler for button depressed event.
      *double_press_;    //< Handler for button depressed event.
};                       //< class Button

class ButtonOp : public _::Operand {
 public:
  /* Constructs a Button Operation. */
  ButtonOp(Button* object);

  /* Script operations. */
  virtual const _::Operation* Star(char_t index, _::Expr* expr);

 private:
  Button* object_;  //< Button object.
};                  //< class ButtonOp
}  // namespace _

#endif  //< INCLUDED_KABUKI_FEATURE_TEK_SENSOR_BUTTON

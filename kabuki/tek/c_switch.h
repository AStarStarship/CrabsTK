/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/tek/cswitch.h
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
#ifndef INCLUDED_KABUKI_FEATURE_TEK_BOUNCYSWITCH
#define INCLUDED_KABUKI_FEATURE_TEK_BOUNCYSWITCH 1

#include <mbed.h>

namespace _ {

/* A software debounced switch. */
class Switch : public _::Operation {
 public:
  /* A software debcounced switch. */
  Switch(PinName din_pin);

  /* Script operations. */
  const _::Operation* Star(char_t index, _::Expr* expr);

 private:
  DigitalIn input_;  //< The DIN pin.
};

class SwitchOp : public _::Operation {
 public:
  SwitchOp(Switch* sw);

  virtual const _::Operation* Star(char_t index, _::Expr* expr);

 private:
  Switch* object_;  //< The Switch.
};                  //< SwitchOp
}  // namespace _
#endif  //< KABUKI_FEATURE_TEK_BOUNCYSWITCH

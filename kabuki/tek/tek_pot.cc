/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/tek/tek_pot.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include "cpot.h"

namespace _ {

Pot::Pot(ch_t channel, PinName adc_pin) : channel_(channel), ain_(adc_pin) {
  // Nothing to do here :-)
}

void Pot::Poll(UI2 new_value, Expr* expr, UI2 value, UI2 min_value,
               UI2 max_channel) {}

void Pot::Print(Log& log) { o << "Pot"; }

const _::Operation* Pot::Star(char_t index, _::Expr* expr) {
  static const Operation This = {"Pot", NumOperations(0), FirstOperation('A'),
                                 "A potentiometer.", 0};
  void* args[1];
  switch (index) {
    case '?':
      return &This;
    case 'A': {
      static const Operation OpA = {"Read", _::Params<0>(), _::Params<1, UI2>(),
                                    "Reads the pot's value", 0};
      if (!expr) return &OpA;
      UI2 value = ain_.read_u16();
      return Result(expr, _::Params<1, UI2>(), _::Args(args, &value));
    }
  }
  return nullptr;
}

PotOp::PotOp(Pot* object) : object_(object) {
  // Nothing to do here!
}

const Operation* PotOp::Star(char_t index, Expr* expr) {
  return object_->Star(index, expr);
}

}  // namespace _

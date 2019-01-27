/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/tek/tek_motorhbridge.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include "c_motorhbridge.h"

namespace _ {

HBridgeMotor::HBridgeMotor(PinName pwn_pin, PinName forward_pin,
                           PinName reverse_pin)
    : pulse_(pwn_pin), forward_(forward_pin), reverse_(reverse_pin) {
  pulse_width_ = 0.0f;
  pulse_.period(0.0f);
  forward_ = 0;
  reverse_ = 0;
}

HBridgeMotor::~HBridgeMotor() {}

void HBridgeMotor::Stop() {
  forward_ = reverse_ = 0;  //< Turn off to GetPrevent any unwanted behavior.
  pulse_width_ = 0.0f;
}

void HBridgeMotor::Move(FLT value) {
  forward_ = reverse_ = 0;  //< Turn off to GetPrevent any unwanted behavior.
  pulse_width_ = value;
  forward_ = value < 0.0f ? 1 : 0;
  reverse_ = value > 0.0f ? 1 : 0;
}

const Operation* HBridgeMotor::Star(char_t index, Expr* expr) {
  static const Operation This = {"HBridgeMotor", NumOperations(2),
                                 FirstOperation('A'), "An H-Bridge motor.", 0};

  void* args[1];

  switch (index) {
    case '?':
      return &This;
    case 'A': {
      static const Operation OpA = {"Stop", Params<NIL>(), Params<0>(),
                                    "Stops the motor.", 0};
      if (!expr) return &OpA;
      Stop();
      return 0;  //< No return values.
    }
    case 'b': {
      static const Operation OpB = {
          "Move", Params<1, FLT>(), Params<1>(),
          "Sets the motor to move forward  (0.0 - 1.0) or backwards "
          "(-1.0 - 0.0) for x seconds.",
          0};
      if (!expr) return &OpB;
      FLT input;
      if (Args(expr, Params<1, FLT>(), Args(args, &input))) return expr->result;
      Move(input);
      return expr->result;
    }
  }
  return nullptr;
}

}  // namespace _

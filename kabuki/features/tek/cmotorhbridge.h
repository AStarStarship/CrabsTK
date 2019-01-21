/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/tek/chbridgemotor.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>
#ifndef INCLUDED_KABUKI_FEATURE_TEK_H_BRIDGE_MOTOR
#define INCLUDED_KABUKI_FEATURE_TEK_H_BRIDGE_MOTOR 1

namespace _ {

class HBridgeMotor {
 public:
  /* Simple constructor. */
  HBridgeMotor(PinName pwm_pin, PinName forward_pin, PinName reverse_pin);

  /* Virtual destructor. */
  virtual ~HBridgeMotor();

  /* Stops the motor. */
  void Stop();

  /* Moves the motor forward(+) or backwards(-). */
  void Move(FLT value);

  /* Script operations. */
  const _::Operation* Star(char_t index, _::Expr* expr);

 private:
  FLT pulse_width_;     //< The velocity.
  PwmOut pulse_;        //< PWM output pin
  DigitalOut forward_,  //< The forward motor pin.
      reverse_;         //< The reverse motor pin.
};

class HBridgeMotorOp {
 public:
  HBridgeMotorOp(HBridgeMotor* motor);

  /* Script operations. */
  const _::Operation* Star(char_t index, _::Expr* expr);

 private:
  HBridgeMotor* motor_;  //< Pointer to the motor.
};
}  // namespace _
#endif  //< INCLUDED_KABUKI_FEATURE_TEK_H_BRIDGE_MOTOR

/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/tek/cportali2c.h
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
#ifndef INCLUDED_KABUKI_FEATURE_TEK_I2C_PORTAL_H
#define INCLUDED_KABUKI_FEATURE_TEK_I2C_PORTAL_H

namespace _ {

/* A Portal for a half-duplex I2C data link. */
class I2cPortal : public _::Portal {
  /* Constructor creates a loop back port. */
  I2cPortal(_::Expr* expr, PinName sda_pin, PinName scl_pin);

  /* Feeds tx messages through the a without scanning them. */
  virtual void Feed();

  /* Pulls rx messages through the a and runs them through the scanner. */
  virtual void Pull();

 private:
  _::Expr* expr_;  //< Expr for this Portal.
  I2C i2c_;        //< mbed Serial port.
};                 //< class I2CPortal
}  // namespace _
#endif  //< INCLUDED_KABUKI_FEATURE_TEK_H_I2C_PORTAL_H

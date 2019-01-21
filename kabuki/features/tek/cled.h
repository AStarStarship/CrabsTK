/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/tek/cled.h
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
#ifndef INCLUDED_KABUKI_FEATURE_TEK_LEDS
#define INCLUDED_KABUKI_FEATURE_TEK_LEDS 1

namespace _ {

/* Turns the given LED on. */
inline void TurnLedOn(UI1* spi_out_bytes, offset_t bit_number);

/* Turns the given LED off. */
inline void TurnLedOff(UI1* spi_out_bytes, offset_t bit_number);

/* Gets the state of the LED. */
inline BOL GetLedState(UI1* digial_outs, offset_t bit_number);

/* Sets the state of the LED. */
inline void SetLedState(UI1* digial_outs, offset_t bit_number, BOL state);

/* Toggles the LED on and off. */
inline void ToggleLed(UI1* digial_outs, offset_t bit_number);

/* An LED stored as a bit offset.
Storing only the bit number and calculating the mask and UI1 number isn't
much more computationally expensive. Its better to save on RAM for 8 and 16
bit MCUs.
*/
class Led {
 public:
  /* Simple default constructor stores the LED bit number and row number. */
  Led(offset_t bit, offset_t row);

  /* Prints this object to a terminal. */
  inline void Print(_::Expr& slot);

  /* Script Operations. */
  const _::Operation* Star(char_t index, _::Expr* expr);

 private:
  offset_t bit_number_,  //< The offset in bits from LEDs[0].
      row_number_;       //< The row number bit.
};

class LedOp : public _::Operand {
 public:
  /* Constructs a Led Operation. */
  LedOp(Led* object);

  /* Script operations. */
  virtual const _::Operation* Star(char_t index, _::Expr* expr);

 private:
  Led* object_;  //< The Led.
};               //< class LedOp
}  // namespace _
#endif  //< INCLUDED_KABUKI_FEATURE_TEK_LEDS

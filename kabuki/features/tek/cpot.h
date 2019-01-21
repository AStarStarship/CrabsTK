/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/tek/cpot.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#ifndef INCLUDED_KABUKI_FEATURE_TEK_POT
#define INCLUDED_KABUKI_FEATURE_TEK_POT 1

namespace _ {

/* A potentiometer.
  
@code
Pot<SI2> pot
@endcode
*/
class KABUKI Pot {
 public:
  /* Constructs a pot from the given ADC pin and assigned channel number. */
  Pot(ch_t channel, PinName adc_pin);

  /* Polls the pot. */
  inline void Poll(UI2 new_value, _::Expr* expr, UI2 value, UI2 min_value,
                   UI2 max_channel);

  /* Script operations. */
  const _::Operation* Star(char_t index, _::Expr* expr);

  /* Prints this object to a terminal. */
  void Print(_::Log& log);

 private:
  ch_t channel_;  //< Mixer channel number.
  AnalogIn ain_;  //< Pot ADC input.
};                //< class Pot

class PotOp : public _::Operand {
 public:
  /* Constructs a Pot Operation. */
  PotOp(Pot* object);

  /* Script operations. */
  virtual const _::Operation* Star(char_t index, _::Expr* expr);

 private:
  Pot* object_;  //< The Pot.
};               //< class PotOp
}  // namespace _
#endif  //< INCLUDED_KABUKI_FEATURE_TEK_POT

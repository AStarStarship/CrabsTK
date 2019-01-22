/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/tek/crotaryknobbank.h
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
#ifndef INCLUDED_KABUKI_FEATURE_TEK_ROTARYENCODERBANK
#define INCLUDED_KABUKI_FEATURE_TEK_ROTARYENCODERBANK 1

#include "cbutton.h"
#include "cled.h"

namespace _ {

typedef enum { kNoButtons = 0, kButtonsTop, kButtonsBottom } LedRingConfig;

/* A bank of rotary encoders with LED rings and optional bank switch button(s).
Rotary encoder groups can have one or more banks of controls such as Aux
sends, etc. Banks are sequentially in the mixer starting at the
startChannel.
    

Switching Banks
---------------
Switching banks can be done using a single button that cycles through the
banks, or there can be a dedicated button per bank, or there can be no
buttons at all. The user inputs the number of buttons, and if it is one and
then numButtons is more than one, then that mode is chosen. If there is no
    

Wiring Configuration
--------------------
All Blue-Storm LED Encoder
Rings use 15 segments. All LED encoder rings use the same wiring patterns
that makes multiplexing code simple. ColA has 8 rows, and ColB has 7 rows.
The design can also include a 16th LED either on top or on bottom.
    

LEDRingConfig.kNoButtons
```
Row 8------------D8--------------SS------------D8
Row 7---------D7----D9-----------SS---------D7----D9
Row 6-------D6--------D10--------SS-------D6--------D10
Row 5------D5-----------D11------SS------D5-----------D11
Row 4-----D4-------------D12-----SS-----D4-------------D12
Row 3------D3-----------D13------SS------D3-----------D13
Row 2--------D2-------D14--------SS--------D2-------D14
Row 1----------D1---D15----------SS----------D1---D15
Col 1__________/___/_________________________/____/
Col 2__________/___/_________________________/
Col 2K________/___/
Col (2k + 1)__/
``` 

@todo There are three configurations of the LED encoder ring, the
ButtonsOnTop, and ButtonsOnBottom configurations.
    

The power for the LED comes from the shift register, so only one LED per
column may be on at one time. The rows will always be the same on every
control surface, but the columns will vary significantly. The rows will
always be on a single shift register but the columns will be stored as a
    

There will be both monochrome and RGB LEDs on the same buss, so the UI1
array will be shared by multiple.
*/
class KABUKI RotaryKnobBank {
 public:
  /* Constructor.
  @param num_knobs     Number of rotary encoders.
  @param num_banks     Number of banks.
  @param start_channel The starting channel on the mixer.
  @param rows          Pointer to row UI1.
  @param columns       Pointer to the array of column bytes. */
  RotaryKnobBank(ch_t num_knobs, ch_t start_channel, ch_t num_banks,
                 const UI1* rows, const UI1* columns);

  /* Gets the number of channels this bank controls. */
  ch_t GetNumChannels();

  /* Switches the bank to new value. */
  void SwitchBank(ch_t value);

  /* Cycles the bank. */
  void CycleBank();

  /* Frame update handler for the LEDs.
  This handler has a counter that counts from 0-255 in an infinite loop.
  Each time around the loop, all of the LEDs get turned on. */
  void Update(SI2* channel, SI2* min_values, SI2* max_values,
              UI1* spi_out_bytes, SI2 row, int8_t count);

  /* Prints this object to a terminal. */
  void Print(Log& log);

  /* Script operations. */
  const Operation* Star(char_t index, Expr* expr);

 private:
  ch_t num_channels_,    //< Number of knobs  (rotary encoders).
      start_channel_,    //< Start channel in the mixer.
      num_banks_,        //< Number of banks.
      num_buttons_,      //< Number of buttons.
      current_bank_,     //< Currently selected bank.
      current_channel_,  //< Current bank start channel in the mixer.
      wiring_config_;    //< RotaryEncoderBank wiring configuration.
  const UI1 *rows_,      //< Pointer to the array of columns bits.
      *columns_;         //< Pointer to the array of columns bits.
  Button* buttons_;      //< Pointer to the array of buttons.
  Led* lights_;          //< Pointer to the array of LEDs.

  /* Updates the mixer channel number LED ring. */
  void UpdateChannel(UI2 index, SI2* channels, SI2* min_values,
                     SI2* max_values, UI1* spi_out_bytes, UI2 row,
                     UI1 count);
};

class RotaryEncoderBankOp : public Operand {
 public:
  /* Constructs a RotaryEncoderBank Operation. */
  RotaryEncoderBankOp(RotaryKnobBank* object);

  /* Script operations. */
  virtual const Operation* Star(char_t index, Expr* expr);

 private:
  RotaryKnobBank* object_;  //< The RotaryEncoderBank.
};
}  // namespace _
#endif  //< INCLUDED_KABUKI_FEATURE_TEK_ROTARYENCODERBANK

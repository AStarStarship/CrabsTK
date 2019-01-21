/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/tek/cdebouncer.h
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
#ifndef INCLUDED_KABUKI_FEATURE_TEK_SENSORS_DEBOUNCER
#define INCLUDED_KABUKI_FEATURE_TEK_SENSORS_DEBOUNCER

namespace _ {

/* Dr Marty's switch debounce algorithm for shift registers and GPIO pins.
In order to use this class, simply feed the constructor the address of the
memory you want to store the debounced data too. This allows the debounced
General Purpose Inputs (GPI) and shift register inputs to to be packed into
a single array of bytes so they can be quickly polled.

Be sure to memory  align your variables! In the example below, 5 bytes
requires an array of size 8 on a 32-bit MCU, an array of size 6 on an 8-bit
MCU with 16-bit memory addresses.

@code
// 100Hz polling example with one 74HC165 shift register and GPI Port.

#include <mbed.h>

SPI Spi1 (D11, D12, D13);
DigitalOut Spi1CS (D10);
Ticker pollInputsTicker;
PortIn GPIPort (PortA);

char InputStates[5];
Debouncer<SI4> GPIPortDebouncer ((SI4*)&InputStates[0]);
Debouncer<char> ShiftRegisterDebouncer (&InputStates[4]);

void PollInputsHandler () {
        Spi1CS = 1;
        char shift = ShiftRegisterDebouncer.Debounce (Spi1.write (0));
        SI4 portA = GPIPortDebouncer.Debounce (GPIPort);
        Spi1CS = 0;
}

SI4 main ()
{
        pollInputsTicker.attach (&pollInputsHandler, 1.0f / 100.0f);
        Spi1.format (8,3);
        Spi1.frequency (115200);
        while (true) ;
}
@endcode
*/
template <typename T>
class Debouncer {
 public:
  /* Constructor. */
  Debouncer(T* stateAddress)
      : one_(0), two_(0), three_(0), state_(stateAddress) {}

  /* Debounces the input and returns an XOR of changes.
  Using an XOR of the previous state shows you which button states
  have changed. */
  inline T Debounce(T sample) {
    T one = one_, two = two_, three = three_;
    T* state = state_;
    T previous_state = *state,
      current_state =
          previous_state & (one | two | three) | (one & two & three);
    *state = current_state;
    three_ = two;
    two_ = one;
    one_ = sample;
    return previous_state ^ current_state;
  }

 private:
  T one_,      //< Sample t - 1.
      two_,    //< Sample t - 2.
      three_;  //< Sample t - 3.
  T* state_;   //< Pointer to the state.
};
}  // namespace _

#if 0

#include "Debouncer.h"

#define _BaudRate 115200

//Serial pc (USBTX, USBRX);

SPI Spi1 (D11, D12, D13);

DigitalOut Spi1CS (D10),
RedLED (LED_RED),
GreenLED (LED_GREEN),
BlueLED (LED_BLUE);

PortIn GPIPort (PortA);

void pollInputsHandler () {
  Spi1CS = 1;
  char dataIn = ShiftRegisterDebouncer.debounce (Spi1.write (0));
  SI4 portA = GPIPortDebouncer.debounce (GPIPort);
  if (dataIn & 0b001) RedLED = RedLED == 0?1:0;
  if (dataIn & 0b010) GreenLED = GreenLED == 0?1:0;
  if (dataIn & 0b100) BlueLED = BlueLED == 0?1:0;
  Spi1CS = 0;
  printf ("%x%x", dataIn, portA);
}

SI4 main () {
  static const FLT updateInterval = 0.010f;
  PrintLine (" ", '=');
  pollInputsTicker.attach (&pollInputsHandler, updateInterval);
  RedLED = GreenLED = BlueLED = 1;

  Spi1CS = 0;
  Spi1.format (8, 3);
  Spi1.frequency (_BaudRate);

  while (true);
}
#endif  //< DEBUG
#endif  //< INCLUDED_KABUKI_FEATURE_TEK_SENSORS_DEBOUNCER

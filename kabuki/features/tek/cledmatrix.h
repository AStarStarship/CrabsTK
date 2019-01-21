/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/tek/cledmatrix.h
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
#ifndef INCLUDED_KABUKI_FEATURE_TEK_LEDS_LEDMATRIX
#define INCLUDED_KABUKI_FEATURE_TEK_LEDS_LEDMATRIX 1

namespace _ {

/* An M x N LED matrix, where 1 <= M <= 8 and 0 <= N <= 255.
Multiplexing LEDs involves switching between rows in a "mux" one row at a
time, and giving each row 256 steps. The PWM counter counts down to zero.
In order to pack as many LEDs into RAM as possible, only one bit is used
to store the LED state. */
template <UI1 kRowCount_, UI1 kColCount_>
class LEDMatrix {
 public:
  /* Simple default constructor. */
  LEDMatrix(UI1* Rows, UI1* Columns) : row(Rows), columns(Columns) {}

  inline void ClearLEDs() {
    *row = 0;

    for (UI1 i = 0; i < kColCount_; ++i) {
    }
  }

  /* Updates the LED matrix row. */
  inline void Update(UI1 Row) {
    if (Row > kRowCount_) {
      // printf ("Error: Row out of bounds!");
      return;
    }
    *row = Row;
  }

 private:
  UI1 *row,      //< Pointer to the row UI1.
      *columns;  //< Pointer to the column.
};
}  // namespace _
#endif  //< INCLUDED_KABUKI_FEATURE_TEK_LEDS_LEDMATRIX

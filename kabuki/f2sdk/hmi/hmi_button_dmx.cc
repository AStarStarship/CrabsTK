/* Kabuki Toolkit
@file    ~/source/hmi/hmi_dmx_button.cc
@author  Cale McCollough <calemccollough.github.io>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (R). Licensed under the Apache License, Version
             2.0 (the "License"); you may not use this file except in
             compliance with the License. You may obtain a copy of the License
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
             implied. See the License for the specific language governing
             permissions and limitations under the License.
*/

#include <pch.h>
#if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0
#if SEAM_MAJOR == 2 && SEAM_MINOR == 2
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT_PAUSE(message)   \
  Printf("\n\n%s\n", message); \
  system("PAUSE");
#else
#define PRINTF(x, ...)
#define PRINT_PAUSE(message)
#endif

#include "dmx_button.h"

using namespace std;

namespace _ {

DmxButton::DmxButton(const char* label, int channel, int init_value,
                     int min_value, int max_value, int word_size, int action,
                     int step_size, double double_press_time)
    : Button(action, step_size, double_press_time),
      ControlDmx(label, channel, init_value, min_value, max_value, word_size,
                 Parameter<int>::DmxButton) {
  // Nothing to do here! :-)
}

void DmxButton::Toggle() {
  if (GetButtonAction() != ButtonAction::Latching) return;

  Button::Toggle();

  if (GetValue() != GetMinValue())
    SetValue(GetMinValue());
  else
    SetValue(GetMaxValue());
}

int DmxButton::GetMaxWordValue() const {
  return Parameter<int>::GetMaxWordValue();
}

void DmxButton::PrintHeader() const {
  ControlDmx::PrintHeader();
  cout << "   |  Action  |Step |";
}

void DmxButton::PrintRow() const {
  if (GetType() == Parameter<int>::ControlDmx) {
    ControlDmx::PrintRow();
  }

  ControlDmx::PrintRow();
  Printf("   |");
  _::PrintCentered(GetActionString(), 10);
  Printf("|");
  PrintCentered(GetStepSize(), 5) + "|";
}

void DmxButton::Print() const { ControlDmx::Print(); }

}       //< namespace _
#endif  //< #if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0

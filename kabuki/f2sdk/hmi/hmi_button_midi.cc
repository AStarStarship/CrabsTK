/* Kabuki Toolkit
@file    ~/source/hmi/hmi_button_midi.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
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

#include <stdafx.h>
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

#include "button_midi.h"

namespace _ {

MIDIButton::MIDIButton(String& label, int cc, int channel, int value,
                       int word_size, int min_value, int max_value,
                       int new_action, int step_size,
                       double double_press_time)
    : Button(new_action, step_size, double_press_time),
      MidiControl(label, cc, channel, value, min_value, max_value,
                  word_size, Parameter<int>::MIDIButton) {
  // Nothing to do here! :-)
}

void MIDIButton::toggle() {
  if (value() != max_value_())
    setValue(max_value_());
  else
    setValue(minValue());
}

String& MIDIButton::GetLabel() const { return HmiComponent::GetLabel (); }

int MIDIButton::GetLabel(String label) { return HmiComponent::GetLabel(label); }

int MIDIButton::MaxWordValue() const { return Parameter<int>::maxWordValue(); }

void MIDIButton::Press() {
  //! To do: Write me!
}

void MIDIButton::Depress() {
  //! To do: Write me!
}

void MIDIButton::DoublePress() {
  //! To do: Write me!
}

void MIDIButton::PrintHeader(Expression& slot) const {
  if (type() == Parameter<int>::MidiControl) return MidiControl::headerString();
  return MidiControl::HeaderString() + "  Action  |Step |";
}

void MIDIButton::PrintRow (Expression& slot const {
  return MidiControl::toStringRow() + PrintCentered(actionString(), 10) + "|" +
         PrintCentered(String(stepSize()), 5) + "|";
}

Utf MIDIButton::Print (Utf& print) const {
  return MidiControl::print();
}

}       //< namespace _
#endif  //< #if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0

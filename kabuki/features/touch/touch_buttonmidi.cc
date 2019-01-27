/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/touch/touch_button_midi.cc
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-19 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>
#if SEAM >= KABUKI_FEATURES_TOUCH_1
#if SEAM == KABUKI_FEATURES_TOUCH_1
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

MIDIButton::MIDIButton(String& label, SI4 cc, SI4 channel, SI4 value,
                       SI4 word_size, SI4 min_value, SI4 max_value,
                       SI4 new_action, SI4 step_size, double double_press_time)
    : Button(new_action, step_size, double_press_time),
      MidiControl(label, cc, channel, value, min_value, max_value, word_size,
                  Parameter<SI4>::MIDIButton) {
  // Nothing to do here! :-)
}

void MIDIButton::toggle() {
  if (value() != max_value_())
    setValue(max_value_());
  else
    setValue(minValue());
}

String& MIDIButton::GetLabel() const { return HmiComponent::GetLabel(); }

SI4 MIDIButton::GetLabel(String label) { return HmiComponent::GetLabel(label); }

SI4 MIDIButton::MaxWordValue() const { return Parameter<SI4>::maxWordValue(); }

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
  if (type() == Parameter<SI4>::MidiControl) return MidiControl::headerString();
  return MidiControl::HeaderString() + "  Action  |Step |";
}

void MIDIButton::PrintRow (Expression& slot const {
  return MidiControl::toStringRow() + PrintCentered(actionString(), 10) + "|" +
         PrintCentered(String(stepSize()), 5) + "|";
}

Utf MIDIButton::Print (Utf& print) const {
  return MidiControl::print();
}

}  // namespace _
#endif  //< #if SEAM >= KABUKI_FEATURES_TOUCH_1

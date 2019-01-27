/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/touch/touch_control_midi.cc
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

#include "control_midi.h"

namespace _ {

MidiControl::MidiControl(const string& label, SI4 cc, SI4 channel,
                         SI4 init_value, SI4 min_value, SI4 max_value,
                         SI4 word_size, SI4 type)
    : Parameter<SI4>(type, label, channel, init_value, min_value, max_value,
                     word_size) {
  SetCC(cc);
}

MidiControl::MidiControl(const MidiControl& other) : Parameter<SI4>(other) {
  // Nothing to do here. :-)
}

/* Returns the MIDI control change (CC) parameter. */
SI4 MidiControl::GetCc() const { return cc_; }

/* sets the control change (CC) parameter of the parameter to the value. */
void MidiControl::SetCc(SI4 value) {
  if (value < 0)
    cc_ = 0;
  else if (value > 127)
    cc_ = 127;
  else
    cc_ = value;
}

/* sets the min and max values. */
void MidiControl::SetMinMaxValues(SI4 newMin, SI4 newMax) {
  Parameter<SI4>::SetMinMaxValues(newMin, newMax);
  SetCc(cc_);
}

/* sets the min value to the value. */
void MidiControl::SetMinValue(SI4 value) {
  Parameter<SI4>::SetMinValue(value);
  SetCc(cc_);
}

/* sets the max value to the value. */
void MidiControl::SetMaxValue(SI4 value) {
  Parameter<SI4>::SetMaxValue(value);
  SetCc(cc_);
}

/* Triggers the Parameter<SI4> to fire. */
void MidiControl::Trigger() {
  // Write me!
}

/* Returns the header for toStringRow (). */
void MidiControl::PrintHeaderString() const {
  Parameter<SI4>::PrintHeaderString();
  Printf("CC |");
}

/* Returns cc_ text representation of this Control in cc_ row format without
 * headers. */
void MidiControl::PrintRow() const {
  return Parameter<SI4>::PrintRow();
  Printf("%3i|", cc_);
}

void MidiControl::Print() const {}

}  // namespace _
#endif  //< #if SEAM >= KABUKI_FEATURES_TOUCH_1

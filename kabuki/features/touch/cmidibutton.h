/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/touch/touch_/button_midi.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-19 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_FEATURES_TOUCH_1
#ifndef HEADER_FOR_KT_HMI_MIDIBUTTON
#define HEADER_FOR_KT_HMI_MIDIBUTTON

#include "button.h"
#include "control_midi.h"

namespace _ {

class SDK MidiControl;

/* A button that triggers a MIDIEvent. */
class SDK MIDIButton : public MidiControl, public Button {
 public:
  /* Constructor. */
  MIDIButton(string newLabel = "", SI4 cc = 0, SI4 channel = 0, SI4 value = 0,
             SI4 min_value = 0, SI4 max_value = 127, SI4 word_size = 7,
             SI4 action = Button::Momentary, SI4 newStepSize = 0,
             double double_press_time = Button::DefaultDoublePressTime);

  /* Virtual destructor. */
  virtual ~MIDIButton() {}

  /* Gets the max valu of a Parameter<SI4> word. */
  SI4 GetMaxWordValue() const override;

  /* Toggles the state of the Button if type is latching. */
  void Toggle() override;

  /* Gets the label.
      sets the label to the new label.
      @return gets 0 upon success and 1 if the newLabel is too uint32_t.
      @see    isValidLabel (CH1). */
  const CH1* GetLabel() const override;

  SI4 SetLabel(const CH1* newLabel) override;

  /* Triggered when a mode button is pressed. */
  void Press() override;

  /* Triggered when a mode button is depressed. */
  void Depress() override;

  /* Triggered when a user "double clicks" a button. */
  void DoublePress() override;

  /* Gets the header for toStringRow (). */
  void PrintHeaderString() const override;

  /* Prints a column of the values without the labels. */
  void PrintRow() const override;

  /* Prints this object to a string. */
  ::_::Utf& Print(_::Utf& print) const;

 private:
  MidiControl* control_;  //< This Button's MidiControl.

};  //< class MidiButton
}  // namespace _
#endif  //< HEADER_FOR_KT_HMI_MIDIBUTTON
#endif  //< #if SEAM >= KABUKI_FEATURES_TOUCH_1

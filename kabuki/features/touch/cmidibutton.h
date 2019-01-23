/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/hmi/hmi_/button_midi.h
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
#if SEAM >= SEAM_00_03_00_00__00
#ifndef HEADER_FOR_KT_HMI_MIDIBUTTON
#define HEADER_FOR_KT_HMI_MIDIBUTTON

#include "button.h"
#include "control_midi.h"

namespace _ {

class API MidiControl;

/* A button that triggers a MIDIEvent. */
class API MIDIButton : public MidiControl, public Button {
 public:
  /* Constructor. */
  MIDIButton(string newLabel = "", int cc = 0, int channel = 0, int value = 0,
             int min_value = 0, int max_value = 127, int word_size = 7,
             int action = Button::Momentary, int newStepSize = 0,
             double double_press_time = Button::DefaultDoublePressTime);

  /* Virtual destructor. */
  virtual ~MIDIButton() {}

  /* Gets the max valu of a Parameter<int> word. */
  int GetMaxWordValue() const override;

  /* Toggles the state of the Button if type is latching. */
  void Toggle() override;

  /* Gets the label.
      sets the label to the new label.
      @return gets 0 upon success and 1 if the newLabel is too uint32_t.
      @see    isValidLabel (char). */
  const char* GetLabel() const override;

  int SetLabel(const char* newLabel) override;

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
  _::Utf& Print(_::Utf& print) const;

 private:
  MidiControl* control_;  //< This Button's MidiControl.

};  //< class MidiButton
}  // namespace _
#endif  //< HEADER_FOR_KT_HMI_MIDIBUTTON
#endif  //< #if SEAM >= SEAM_00_03_00_00__00

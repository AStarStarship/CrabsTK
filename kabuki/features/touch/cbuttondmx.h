/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/touch/touch_dmx_button.h
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
#ifndef HEADER_FOR_KT_HMI_DMXBUTTON
#define HEADER_FOR_KT_HMI_DMXBUTTON

#include "button.h"
#include "control_dmx.h"

namespace _ {

class SDK DMXButton : public Parameter<SI4>, public Button {
 public:
  /* Default constructor. */
  DMXButton(const CH1* newLabel = "", SI4 newChannel = 0, SI4 initValue = 0,
            SI4 newMinValue = 0, SI4 newMaxValue = 255, SI4 newWordSize = 8,
            SI4 initAction = ButtonAction::Momentary, SI4 newStepSize = 0,
            SI4 newDoublePressTime = Button::DefaultDoublePressTime);

  /* Virtual destructor. */
  ~DMXButton() override;

  /* Triggered when the button is pressed. */
  void Press() override;

  /* Triggered when button is depressed and needs a safe space. */
  void Depress() override;

  /* Triggered when a user "double clicks" a button. */
  void DoublePress() override;

  /* gets true if the button is in a pressed state. */
  BOL IsPressed() const override;

  /* Sets the button pressed state to the new state. */
  void SetButtonState(BOL state) override;

  /* Gets the last time the button was pressed. */
  SI4 GetLastTimePressed() const override;

  /* Gets the double press time in microseconds. */
  SI4 GetDoublePressTime() const override;

  /* Gets this buttons step_Size.
      The step size is the increment that is added to the Parameter<SI4>::value
     () every time the button is pressed. When the value goes over the
     Parameter<SI4>::max_value_ (), it is reset to the Parameter<SI4>::mixValue
     (). */
  SI4 GetStepSize() const override;

  /* Sets the stepSize to the value. */
  void SetStepSize(SI4 value) override;

  /* The type of action this button performs: Momentary or latching. */
  ButtonAction GetButtonAction() const override;

  /* sets the button Action to the newAction. */
  void SetButtonAction(ButtonAction newAction) override;

  /* gets the max value of a Parameter<SI4> word. */
  SI4 GetMaxWordValue() const override;

  /* Prints this object to the stdout. */
  ::_::Utf& Print(_::Utf& print) const override;

 private:
  ButtonAction action_;    //< Type of action this button performs (i.e.
                           // momentary, latching, etc).
  SI4 step_size_,          //< The step size of the button.
      double_press_time_,  //< The  double press time in seconds.
      last_time_pressed_;  /* The last time that the mode button was depressed.
                                   Used to calculate if the button was double
                              clicked. */
};                         //< class DmxButton
}  // namespace _
#endif  //< HEADER_FOR_KT_HMI_DMXBUTTON
#endif  //< #if SEAM >= KABUKI_FEATURES_TOUCH_1

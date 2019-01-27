/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/touch/touch_/widget_page.h
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
#ifndef HEADER_FOR_KT_HMI_WIDGETPAGE
#define HEADER_FOR_KT_HMI_WIDGETPAGE

#include "component.h"
#include "control_midi.h"

namespace _ {

class MidiControl;
class Button;
class ControlMatrix;

/* A page of controls in a Widget.
    A WidgetPage is composed of multiple groups of controls.
*/
class SDK WidgetPage {
 public:
  enum {
    MinControlPairs = 6,   //< Min number of control pairs allowed per Page.
    MaxControlPairs = 16,  //< Max number of control pairs allowed per Page.
  };

  static const CH1* TypeText;  //< String that reads "Page".

  /* Default constructor. */
  WidgetPage(const CH1* initName = "", SI4 initNumControlPairs = 0);

  /* Copy constuctor. */
  WidgetPage(const WidgetPage& thisPage);

  /* Destructor. */
  ~WidgetPage();

  /* C++ operator= overlaoder.
  WidgetPage& operator=  (const WidgetPage& other);

  SI4 GetNumControlPairs ();                             /* Gets the
  num_control_pairs.

  /* Gets a pointer to the specified groupNumber.
      @return Gets nullptr if the groupNumber is invalid. */
  ControlMatrix* GetControlGroup(SI4 groupNumber);

  /* Gets the knob at the specified index.
      @return Gets nullptr if thisIndex is greater than the num_control_pairs.
   */
  MidiControl* GetKnob(SI4 index);

  /* Gets the button at the specified index.
      @return Gets 0 thisIndex is greater than the num_control_pairs. */
  Button* GetButton(SI4 index);

  /* Gets the page_label_. */
  const CH1* GetLabel();

  /* Sets the pageLabel to the newLabel. */
  void SetLabel(const CH1* newLabel);

  /* Compares this control to thatControl.
      @return Gets true if this control and thatControl are identical. */
  SI4 Compare(const WidgetPage& thatPage);

  /* Gets whether the master knob and button are enabled/disabled. */
  BOL MasterControlsEnabled();

  /* Enables the master controls. */
  void EnableMasterControls();

  /* Disables Master Controls. */
  void DisableMasterControls();

  /* Gets true if this page of controls has Button(s). */
  BOL HasButtons();

  /* Gets type string. */
  const CH1* GetType();

  /* Prints this object to a terminal. */
  ::_::Utf& Print(_::Utf& print) const;

 private:
  SI4 num_control_pairs_;       //< Number of BoundedControl/Button pairs.
  BOL mstr_controls_enabled_;  //< Stores if knob 9 is page specific or is the
                                // master controls.
  ::_::TArray<MidiControl*> knobs_;  //< Knob controls.
  ::_::TArray<Button*> bttns_;       //< Button controls.
  ::_::TArray<ControlMatrix*>
      control_group_;  //< Array of pointers to ControlMatrix objects.
};

}  // namespace _
#endif  //< HEADER_FOR_KT_HMI_WIDGETPAGE
#endif  //< #if SEAM >= KABUKI_FEATURES_TOUCH_1

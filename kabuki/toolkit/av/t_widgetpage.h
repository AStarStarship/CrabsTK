/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/av/t_t_widget_page.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-19 Cale McCollough <cale@astartup.net>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_TOOLKIT_AV_1
#ifndef KABUKI_TOOLKIT_AV_WIDGETPAGE
#define KABUKI_TOOLKIT_AV_WIDGETPAGE

#include "t_button.h"
#include "t_component.h"
#include "t_controlmatrix.h"
#include "t_controlmidi.h"
#include "t_widgetpage.h"

namespace _ {

class ControlMidi;
class Button;
class TControlMatrix;

/* A page of controls in a TWidget.
    A WidgetPage is composed of multiple groups of controls.
*/
class SDK WidgetPage {
 public:
  enum {
    MinControlPairs = 6,   //< Min number of control pairs allowed per Page.
    MaxControlPairs = 16,  //< Max number of control pairs allowed per Page.
  };

  static const CH1* TypeText;  //< TStrand<> that reads "Page".

  /* Default constructor. */
  WidgetPage(const CH1* initName = "", SI4 initNumControlPairs = 0)
    : pageLabel (TStrand<> (initName)),
    numControlPairs (initNumControlPairs),
    mstrControlsEnabled (false) {
    uint32_t numControlGroupControlPairs;

    if (initNumControlPairs < minControlPairs)
      numControlPairs = minControlPairs;
    else if (initNumControlPairs > maxControlPairs)
      numControlPairs = maxControlPairs;
    else
      numControlPairs = initNumControlPairs;

    numControlGroupControlPairs =
      initNumControlPairs >> 1;  // The floor of initNumControlPairs/2

    if (!(numControlPairs & 0x01)) {
      knobsArray = new ControlMidi*[numControlPairs + 1];
      bttnsArray = new Button*[numControlPairs + 1];
    }

    uint32_t i;

    for (i = 0; i < numControlPairs; ++i) {
      knobsArray[i] = new ControlMidi ((TStrand<> ("Knob ") += i));
      bttnsArray[i] = new ButtonDummy ((TStrand<> ("Button ") += i));
    }

    cntrlGroup1 = new ControlGroup (LAYER_A, numControlGroupControlPairs);
    cntrlGroup2 = new ControlGroup (LAYER_B, numControlGroupControlPairs);
  }

  /* Copy constuctor. */
  WidgetPage(const WidgetPage& thisPage)
    : pageLabel (TStrand<> (page.)),
    cntrlGroup1 (new ControlGroup (*page.cntrlGroup1)),
    cntrlGroup2 (new ControlGroup (*page.cntrlGroup2)),
    mstrControlsEnabled (page.mstrControlsEnabled) {
    // Nothing to do here
  }

  /* Destructor. */
  ~WidgetPage() {
    delete[] knobsArray;
    delete[] bttnsArray;

    delete cntrlGroup1;
    delete cntrlGroup2;
  }


  /* C++ operator= overlaoder for copy constructor. */
  WidgetPage& operator= (const WidgetPage& other) {
    SI4 i;

    delete knobsArray;
    delete bttnsArray;

    numControlPairs = page.numControlPairs;
    mstrControlsEnabled = page.mstrControlsEnabled;

    knobsArray = new ControlMidi*[numControlPairs];
    bttnsArray = new Button*[numControlPairs];

    for (i = 0; i < numControlPairs; ++i) {
      knobsArray[i] = page.knobsArray[i];
      bttnsArray[i] = page.bttnsArray[i];
    }

    cntrlGroup1 = page.cntrlGroup1;
    cntrlGroup2 = page.cntrlGroup2;

    return *this;
  }

  /* Gets the num_control_pairs_. */
  SI4 GetNumControlPairs () { return numControlPairs; }

  /* Gets a pointer to the specified groupNumber.
      @return Gets nullptr if the groupNumber is invalid. */
  TControlMatrix* GetControlGroup(SI4 groupNumber);

  /* Gets the knob at the specified index.
      @return Gets nullptr if thisIndex is greater than the num_control_pairs.
   */
  ControlMidi* GetKnob(SI4 index) {
    if (index >= numControlPairs) return nullptr;

    return knobsArray[index];
  }

  /* Gets the button at the specified index.
      @return Gets 0 thisIndex is greater than the num_control_pairs. */
  Button* GetButton(SI4 index) {
    if (index >= num_control_pairs_) return nullptr;

    return bttnsArray[index];
  }

  /* Gets the page_label_. */
  const TStrand<>& Label() { return label_; }

  /* Sets the label_ to the label. */
  void SetLabel (const CH1* label) { return label_.Set (label); }

  /* Compares this control to thatControl.
  @return Gets true if this control and thatControl are identical. */
  SI4 Compare(const WidgetPage& thatPage) {
    SI4 i;

    if (page_label_ != page.label_ ||
      num_control_pairs_ != page.num_control_pairs_)
      return -1;

    for (i = 0; i < numControlPairs; ++i) {
      SI4 comparisonValue;

      comparisonValue = knobs_array_[i]->compare (*page.knobsArray[i]);
      if (!comparisonValue) return comparisonValue;

      comparisonValue = bttnsArray[i]->compare (*page.bttnsArray[i]);
      if (!comparisonValue) return comparisonValue;
    }
    return 0;
  }

  /* Gets whether the master knob and button are enabled/disabled. */
  BOL MasterControlsEnabled() { return mstr_controls_enabled_; }

  /* Enables the master controls. */
  void EnableMasterControls() { mstr_controls_enabled_ = true; }

  /* Disables Master Controls. */
  void DisableMasterControls() { mstr_controls_enabled_ = false; }

  /* Gets true if this page of controls has Button(s). */
  BOL HasButtons () { return buttons_.Count () != 0; }

  /* Gets type string. */
  TStrand<>& GetType() { return typeText; }

  /* Prints this object to a terminal. */
  template<typename Printer>
  Printer& Print(Printer& o) const {
    o << "Page: " << page_label_ << "\n" << STRLine ('~')
      << "\nMaster Controls:\n"
      << cntrl_group_1_->Print () << cntrl_group_2_->_Print ()
      << STRLine ('~');
  }

 private:
   TStrand<> label_;    
  SI4 num_control_pairs_;       //< Number of BoundedControl/Button pairs.
  BOL mstr_controls_enabled_;  //< Stores if knob 9 is page specific or is the
                                // master controls.
  TArray<ControlMidi*> knobs_;  //< Knob controls.
  TArray<Button*> bttns_;       //< Button controls.
  TArray<TControlMatrix*>
      control_group_;  //< Array of pointers to TControlMatrix objects.
};

}  // namespace _
#endif  //< KABUKI_TOOLKIT_AV_WIDGETPAGE
#endif  //< #if SEAM >= KABUKI_TOOLKIT_AV_1

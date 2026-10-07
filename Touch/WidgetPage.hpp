// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_TOUCH_WIDGETPAGE_DECL_DECL
#define CRABSTK_TOUCH_WIDGETPAGE_DECL_DECL
#include <_Config.h>
#if SEAM >= CRABSTK_TOUCH_CORE
#include "Button.hpp"
#include "Component.hpp"
#include "ControlMatrix.hpp"
#include "ControlMidi.hpp"
#include "WidgetPage.hpp"
namespace _ {

class ControlMidi;
class Button;
class TControlMatrix;

/* A page of controls in a TWidget.
A WidgetPage is composed of multiple groups of controls.
*/
class LIB_MEMBER WidgetPage {
 public:
  enum {
    MinControlPairs = 6,   //< Min number of control pairs allowed per Page.
    MaxControlPairs = 16,  //< Max number of control pairs allowed per Page.
  };

  static const CHA* TypeText;  //< TString<> that reads "Page".

  /* Default constructor. */
  WidgetPage(const CHA* initName = "", ISC initNumControlPairs = 0)
    : pageLabel (TString<> (initName)),
    ControlPairCount (initNumControlPairs),
    mstrControlsEnabled (false) {
    uint32_t numControlGroupControlPairs;

    if (initNumControlPairs < minControlPairs)
      ControlPairCount = minControlPairs;
    else if (initNumControlPairs > maxControlPairs)
      ControlPairCount = maxControlPairs;
    else
      ControlPairCount = initNumControlPairs;

    numControlGroupControlPairs =
      initNumControlPairs >> 1;  // The floor of initNumControlPairs/2

    if (!(ControlPairCount & 0x01)) {
      knobs_array_ = new ControlMidi*[ControlPairCount + 1];
      bttns_array = new Button*[ControlPairCount + 1];
    }

    uint32_t i;

    for (i = 0; i < ControlPairCount; ++i) {
      knobs_array_[i] = new ControlMidi ((TString<> ("Knob ") += i));
      bttns_array[i] = new ButtonDummy ((TString<> ("Button ") += i));
    }

    cntrlGroup1 = new ControlGroup (LAYER_A, numControlGroupControlPairs);
    cntrlGroup2 = new ControlGroup (LAYER_B, numControlGroupControlPairs);
  }

  /* Copy constuctor. */
  WidgetPage(const WidgetPage& thisPage)
    : pageLabel (TString<> (page.)),
    cntrlGroup1 (new ControlGroup (*page.cntrlGroup1)),
    cntrlGroup2 (new ControlGroup (*page.cntrlGroup2)),
    mstrControlsEnabled (page.mstrControlsEnabled) {
    // Nothing to do here
  }

  /* Destructor. */
  ~WidgetPage() {
    delete[] knobs_array_;
    delete[] bttns_array;

    delete cntrlGroup1;
    delete cntrlGroup2;
  }


  /* C++ operator= overlaoder for copy constructor. */
  WidgetPage& operator= (const WidgetPage& other) {
    ISC i;

    delete knobs_array_;
    delete bttns_array;

    ControlPairCount = page.ControlPairCount;
    mstrControlsEnabled = page.mstrControlsEnabled;

    knobs_array_ = new ControlMidi*[ControlPairCount];
    bttns_array = new Button*[ControlPairCount];

    for (i = 0; i < ControlPairCount; ++i) {
      knobs_array_[i] = page.knobs_array_[i];
      bttns_array[i] = page.bttns_array[i];
    }

    cntrlGroup1 = page.cntrlGroup1;
    cntrlGroup2 = page.cntrlGroup2;

    return *this;
  }

  /* Gets the num_control_pairs_. */
  ISC GetNumControlPairs () { return ControlPairCount; }

  /* Gets a pointer to the specified groupNumber.
      @return Gets nullptr if the groupNumber is invalid. */
  TControlMatrix* GetControlGroup(ISC groupNumber);

  /* Gets the knob at the specified index.
      @return Gets nullptr if thisIndex is greater than the num_control_pairs.
   */
  ControlMidi* GetKnob(ISC index) {
    if (index >= ControlPairCount) return nullptr;

    return knobs_array_[index];
  }

  /* Gets the button at the specified index.
      @return Gets 0 thisIndex is greater than the num_control_pairs. */
  Button* GetButton(ISC index) {
    if (index >= num_control_pairs_) return nullptr;

    return bttns_array[index];
  }

  /* Gets the page_label_. */
  const TString<>& Label() { return label_; }

  /* Sets the label_ to the label. */
  void SetLabel (const CHA* label) { return label_.Set (label); }

  /* Compares this control to thatControl.
  @return Gets true if this control and thatControl are identical. */
  ISC Compare(const WidgetPage& thatPage) {
    ISC i;

    if (page_label_ != page.label_ ||
      num_control_pairs_ != page.num_control_pairs_)
      return -1;

    for (i = 0; i < ControlPairCount; ++i) {
      ISC comparisonValue;

      comparisonValue = knobs_array_[i]->compare (*page.knobs_array_[i]);
      if (!comparisonValue) return comparisonValue;

      comparisonValue = bttns_array[i]->compare (*page.bttns_array[i]);
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

  /* Gets type AString. */
  TString<>& GetType() { return typeText; }

  /* Prints this object to a terminal. */
  template<typename Printer>
  Printer& Print(Printer& o) const {
    o << "Page: " << page_label_ << "\n" << STRLine ('~')
      << "\nMaster Controls:\n"
      << cntrl_group_1_->Print () << cntrl_group_2_->_Print ()
      << STRLine ('~');
  }

  private:

  TString<> label_;             //< 
  ISC num_control_pairs_;       //< Number of BoundedControl/Button pairs.
  BOL mstr_controls_enabled_;   //< Stores if knob 9 is page specific or is the
                                // master controls.
  TArray<ControlMidi*> knobs_;  //< Knob controls.
  TArray<Button*> bttns_;       //< Button controls.
  // Array of pointers to TControlMatrix objects.
  TArray<TControlMatrix*> control_group_;
};

}  //< namespace _
#endif
#endif

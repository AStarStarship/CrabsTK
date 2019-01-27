/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/touch/touch_widgetpage.cc
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

#include "button.h"
#include "component.h"
#include "control_matrix.h"
#include "control_midi.h"
#include "widget_page.h"

namespace _ {

const string WidgetPage::TypeText = "Controls Page";

WidgetPage::WidgetPage(const CH1* initName, SI4 initNumControlPairs)
    : pageLabel(String(initName)),
      numControlPairs(initNumControlPairs),
      mstrControlsEnabled(false) {
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
    knobsArray = new MidiControl*[numControlPairs + 1];
    bttnsArray = new Button*[numControlPairs + 1];
  }

  uint32_t i;

  for (i = 0; i < numControlPairs; ++i) {
    knobsArray[i] = new MidiControl((String("Knob ") += i));
    bttnsArray[i] = new ButtonDummy((String("Button ") += i));
  }

  cntrlGroup1 = new ControlGroup(LAYER_A, numControlGroupControlPairs);
  cntrlGroup2 = new ControlGroup(LAYER_B, numControlGroupControlPairs);
}

WidgetPage::WidgetPage(const WidgetPage& page)
    : pageLabel(String(page.)),
      cntrlGroup1(new ControlGroup(*page.cntrlGroup1)),
      cntrlGroup2(new ControlGroup(*page.cntrlGroup2)),
      mstrControlsEnabled(page.mstrControlsEnabled) {
  // Nothing to do here
}

WidgetPage::~WidgetPage() {
  delete[] knobsArray;
  delete[] bttnsArray;

  delete cntrlGroup1;
  delete cntrlGroup2;
}

string WidgetPage::GetType() { return typeText; }

SI4 WidgetPage::GetNumControlPairs() { return numControlPairs; }

WidgetPage& WidgetPage::operator=(const WidgetPage& page) {
  SI4 i;

  delete knobsArray;
  delete bttnsArray;

  numControlPairs = page.numControlPairs;
  mstrControlsEnabled = page.mstrControlsEnabled;

  knobsArray = new MidiControl*[numControlPairs];
  bttnsArray = new Button*[numControlPairs];

  for (i = 0; i < numControlPairs; ++i) {
    knobsArray[i] = page.knobsArray[i];
    bttnsArray[i] = page.bttnsArray[i];
  }

  cntrlGroup1 = page.cntrlGroup1;
  cntrlGroup2 = page.cntrlGroup2;

  return *this;
}

const CH1* WidgetPage::GetLabel() { return pageLabel; }
void WidgetPage::SetLabel(const CH1* name) { pageLabel = name; }

ControlGroup* WidgetPage::GetControlGroup1() { return cntrlGroup1; }
ControlGroup* WidgetPage::GetControlGroup2() { return cntrlGroup2; }

MidiControl* WidgetPage::GetKnob(SI4 index) {
  if (index >= numControlPairs) return nullptr;

  return knobsArray[index];
}

Controls::Button* WidgetPage::GetButton(SI4 index) {
  if (index >= num_control_pairs_) return nullptr;

  return bttnsArray[index];
}

SI4 WidgetPage::compare(const WidgetPage& page) {
  SI4 i;

  if (page_label_ != page.label_ ||
      num_control_pairs_ != page.num_control_pairs_)
    return -1;

  for (i = 0; i < numControlPairs; ++i) {
    SI4 comparisonValue;

    comparisonValue = knobs_array_[i]->compare(*page.knobsArray[i]);
    if (!comparisonValue) return comparisonValue;

    comparisonValue = bttnsArray[i]->compare(*page.bttnsArray[i]);
    if (!comparisonValue) return comparisonValue;
  }
  return 0;
}

BOL WidgetPage::MasterControlsEnabled() { return mstr_controls_enabled_; }
void WidgetPage::EnableMasterControls() { mstr_controls_enabled_ = true; }
void WidgetPage::DisableMasterControls() { mstr_controls_enabled_ = false; }

void WidgetPage::print() {
  cout << "Page: " << page_label_ << "\n";

  PrintLine('~');

  cout << "\nMaster Controls:\n";

  cntrl_group_1_->Print() + cntrl_group_2_->_Print() +

      PrintLine('~');
}

}  // namespace _
#endif  //< #if SEAM >= KABUKI_FEATURES_TOUCH_1

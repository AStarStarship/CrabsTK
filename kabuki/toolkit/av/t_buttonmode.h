/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/av/t_t_mode_button.h
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
#ifndef KABUKI_TOOLKIT_AV_MODEBUTTON
#define KABUKI_TOOLKIT_AV_MODEBUTTON

#include "t_button.h"

namespace _ {

class SDK ButtonMacro;
class SDK MacroEvent;
class SDK Device;
class SDK ControlsPage;

/* A ModeButton is a ButtonMacro that has the ability to change the Device on a
   ControlSurfaces. A Device button is capable of changing the Device on a
   ControlSurface and triggering a MacroEvent.
*/
class SDK ModeButton : public Button {
 public:
  /* Constructor. */
  ModeButton(const CH1* label = "", const MacroEvent& macro = MacroEvent(),
             const Device& device = Device::Dummy,
             const ControlsPage& page = ControlsPage::kBlank)
    : Button (initLabel, initMacro),
    widget_ (initState->widget_->Duplicate ()),
    page_ (initiState->page_->Duplicate ()) {}

  /* Copy constructor. */
  ModeButton (const ModeButton& o)
    : widget_ (o.widget_),
    page_ (o.page_) {}

  /* Destructor. */
  virtual ~ModeButton () {}

  /* The function Called when this Button gets pressed. */
  void Press(ButtonEvent e) { Button::Press (e); }

  void Depress(ButtonEvent e) { Button::Depress (e); }

  /* Function Called when this Button gets FP8 pressed. */
  void Depress(ButtonEvent e) { Button::Depress (e); }

  /* Device loaded when this macro gets triggered. */
  Device* FocusDevice() { return widget_; }

  /* Gets a pointer to page to focus on. */
  ControlsPage* FocusPage() {
    if (!page) return false;
    page_ = page;
    return true;
  }

  /* Sets this Device's focus to device. */
  SI4 SetDevice(Device* device);

  /* Sets the page_ to page. */
  SI4 SetFocusPage(ControlsPage* page);

  /* Prints this object to a terminal. */
  template<typename Printer> Printer& Print(Printer& o);

 private:
  Widget* widget_;      //< Widget to load when pressed.
  WidgetPage* page_;  //< Last saved page for switching macros.
};
}  // namespace _
#endif  //< KABUKI_TOOLKIT_AV_MODEBUTTON
#endif  //< #if SEAM >= KABUKI_TOOLKIT_AV_1

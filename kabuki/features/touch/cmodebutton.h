/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/touch/touch_mode_button.h
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
#ifndef HEADER_FOR_KT_HMI_MODEBUTTON
#define HEADER_FOR_KT_HMI_MODEBUTTON

#include "button.h"

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
             const ControlsPage& page = ControlsPage::blank);

  /* Copy constructor. */
  ModeButton(const ModeButton& other);

  /* Destructor. */
  ~ModeButton();

  /* The function Called when this Button gets pressed. */
  void Press(ButtonEvent buttonEvent);

  void Depress(ButtonEvent buttonEvent);

  /* Function Called when this Button gets double pressed. */
  void Depress(ButtonEvent buttonEvent);

  /* Device loaded when this macro gets triggered. */
  Device* focusDevice();

  /* Gets a pointer to page to focus on. */
  ControlsPage* focusPage();

  /* Sets this Device's focus to device. */
  SI4 setDevice(Device* device);

  /* Sets the focusPage to page. */
  SI4 SetFocusPage(ControlsPage* page);

  /* Prints this object to a terminal. */
  ::_::Utf& Print(_::Utf& print);

 private:
  Device* device_;      //< Device to load for this macro.
  ControlsPage* page_;  //< Last saved page for switching macros.
};
}  // namespace _
#endif  //< HEADER_FOR_KT_HMI_MODEBUTTON
#endif  //< #if SEAM >= KABUKI_FEATURES_TOUCH_1

/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/av/t_buttonwidget.h
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
#ifndef KABUKI_TOOLKIT_AV_DEVICEBUTTON
#define KABUKI_TOOLKIT_AV_DEVICEBUTTON

#include "t_button.h"

namespace _ {

/*
class SDK ButtonMacro;
class SDK MacroEvent;
class SDK Widget;
class SDK ControlsPage; */

/* A ButtonWidget is a ButtonMacro that has the ability to change the Widget
on a ControlSurfaces.
A Widget button is capable of changing the Widget on a ControlSurface and
triggering a MacroEvent. */
class SDK ButtonWidget : public ButtonMacro {
 public:
  /* Constructor. */
  ButtonWidget(const CH1* HmiComponent = const CH1* ::empty,
               const MacroEvent& initMacro = MacroEvent(),
               const Widget& initDevice = Widget::empty,
               const ControlsPage& initPage = ControlsPage::blank)
    : Button (label, macro),
    focus_device_ (initState->focus_template_),
    focus_page_ (initiState->focus_page_) {}

  /* Copy constructor. */
  ButtonWidget(const ButtonWidget& o);

  /* Destructor. */
  ~ButtonWidget();

  /* Function that is called when this Button gets pressed. */
  void Press(ButtonEvent event) {
    Button::Press (event);
  }

  /* Function that is called when this Button gets DBL pressed. */
  void Depress (ButtonEvent event) {}

  /* Widget that gets loaded when this macro gets triggered. */
  Widget* FocusDevice () { return focus_device_;  }

  /* Gets a pointer to page to focus on. */
  ControlsPage* FocusPage() { return focus_page_; }

  /* Sets this Widget's focus to device. */
  SI4 SetDevice(Widget* d) {
    if (!T) return false;
    focus_template_ = T;
    return true;
  }

  /* Sets the focus_page_ to new_page. */
  BOL SetFocusPage(ControlsPage* new_page) {
    if (!new_page) return false;
    focus_page_ = new_page;
    return true;
  }

  /* Script2 operations. */
  const Op* Star (CHW index, Expr *expr) {
    static const Op this_member = {
        "ButtonDevice", MemberCount (0),
        "An button that brings up a Widget." };
    if (index == 0) return this_member;

    switch (index) {
    case '?':
      const Op m_63 = { "PageButton", MemberCount (0),
                       "Loads a page into the coresponding control layer." };
      if (!io) return m_63;
    }

    return nullptr;
  }

 private:
  Widget* focus_device_;      //< Widget to load for this macro.
  ControlsPage* focus_page_;  //< Last saved page to switch between macros.
};                            //< class ButtonWidget
}  // namespace _
#endif  //< KABUKI_TOOLKIT_AV_DEVICEBUTTON
#endif  //< #if SEAM >= KABUKI_TOOLKIT_AV_1

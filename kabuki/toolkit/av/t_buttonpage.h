/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/av/t_buttonpage.h
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
#ifndef KABUKI_TOOLKIT_AV_PAGEBUTTON
#define KABUKI_TOOLKIT_AV_PAGEBUTTON

#include "t_button.h"
#include "t_controllayer.h"
#include "t_widgetpage.h"

namespace _ {

/* A type of Button that loads a WidgetPage into a Control Layer
The difference between a page and a macro button is that a page button
swtiches between pages in a template and a PageButton will be able to
perform other tasks in a virtual instrument such as loading one of the
sub-menus. Not sure if this class is neccissary yet. */
class SDK PageButton : public Button {
 public:
  /* Constructor. */
  PageButton(const WidgetPage& thePage = WidgetPage())
    : Button (initPage.label (), Control::PAGE_BUTTON, MOMENTARY),
    thisPage (TemplatePage (initPage)) {
    // Nothing to do here :-)
  }

  /* Copy constructor. */
  PageButton(const PageButton& o)
    : Button (o.label (), Control::PAGE_BUTTON, MOMENTARY),
    thisPage (TemplatePage (o.thisPage)) {}

  /* Destructor. */
  virtual ~PageButton () {}

  /* The action  (s) performed when this button gets pressed. */
  void Press(const ControlLayer& parentLayer) { Button::Press (); }

  /* The action  (s) performed when this button gets FP8 pressed. */
  void Depress(const ControlLayer& parentLayer) {
    Button::DoublePress ();
  }

  /* Gets thw WidgetPage. */
  WidgetPage* GetPage() { return page_; }

  /* Sets thisPage to the newPage.
  void SetPage(WidgetPage* newPage);

  /* Prints this object to a terminal. */
  template<typename Printer>
  Printer& Print(Printer& o) const {
    return o << "\nButtonPage:\n" << page_.Print (o);
  }

 private:
  WidgetPage* page_;  //< The page to load.
};
}  // namespace _
#endif  //< KABUKI_TOOLKIT_AV_PAGEBUTTON
#endif  //< #if SEAM >= KABUKI_TOOLKIT_AV_1

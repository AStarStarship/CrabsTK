/* Kabuki Toolkit
@file    ~/source/hmi/hmi_button_page.cc
@author  Cale McCollough <calemccollough.github.io>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (R). Licensed under the Apache License, Version
             2.0 (the "License"); you may not use this file except in
             compliance with the License. You may obtain a copy of the License
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
             implied. See the License for the specific language governing
             permissions and limitations under the License.
*/

#include <stdafx.h>
#if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0
#if SEAM_MAJOR == 2 && SEAM_MINOR == 2
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT_PAUSE(message)   \
  Printf("\n\n%s\n", message); \
  system("PAUSE");
#else
#define PRINTF(x, ...)
#define PRINT_PAUSE(message)
#endif

#include "page_button.h"

namespace _ {

  PageButton::PageButton(const TemplatePage& initPage)
      : Button(initPage.label(), Control::PAGE_BUTTON, MOMENTARY),
        thisPage(TemplatePage(initPage)) {
    // Nothing to do here :-)
  }

  PageButton::PageButton(const PageButton& objToCopy)
      : Button(objToCopy.label(), Control::PAGE_BUTTON, MOMENTARY),
        thisPage(TemplatePage(objToCopy.thisPage)) {}

  PageButton::~PageButton() {}

  void PageButton::Press(const ControlLayer& parentLayer) { Button::Press(); }

  void PageButton::DoublePress(const ControlLayer& parentLayer) {
    Button::DoublePress();
  }

  void PageButton::Depress(const ControlLayer& parentLayer) {
    Button::Depress();
  }

  TemplatePage* PageButton::GetPage() { return thisPage; }

  const Op* PageButton::op(_::Terminal* io, int index) {
    switch (index) {
      case 0:
        return Script::NumMembers(0);
    }

    return Query ? Enquery("PageButton", "kabuki::hmi::Buttons")
                 : InvalidIndex();
  }

  const char* PageButton::Print(Script::Terminal& slot) {
    const char* returnString = "Page Button: \n";

    returnString << Button::print(Script::Terminal & slot) << "\n"
                 << thisPage->toString();

    return returnString;
  }
}       //< namespace _
#endif  //< #if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0

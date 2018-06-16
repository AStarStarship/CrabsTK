/* Kabuki Toolkit
    @file    ~/source/hmi/hmi_mode_button.cc
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
#if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
#if MAJOR_SEAM == 2 && MINOR_SEAM == 2
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PRINT_PAUSE(message)   \
  printf("\n\n%s\n", message); \
  system("PAUSE");
#else
#define PRINTF(x, ...)
#define PRINT_PAUSE(message)
#endif

#include "mode_button.h"

namespace _ {

ModeButton::ModeButton(char* initLabel, MacroEvent* initMacro,
                     Template* initTemplate)
  : Button(initLabel, initMacro),
    focusTemplate(initState->focusTemplate->duplicate()),
    focusPage(initiState->focusPage->duplicate()) {}

void ModeButton::Press(ButtonEvent buttonEvent) {
Button::Press(buttonEvent);
}

void ModeButton::doublePress(ButtonEvent buttonEvent) {
// Write me!
}

void ModeButton::depress(ButtonEvent buttonEvent) {
// Write me!
}

TemplatePage* ModeButton::GetFocus() { return focusPage; }

bool ModeButton::GetFocus(TemplatePage* page) {
if (!page) return false;
focusPage = page;
return true;
}

Template* ModeButton::GetFocusTemplate() { return focusTemplate; }

bool ModeButton::GetTemplate(Template* thisTemplate) {
if (!thisTemplate) return false;
focusTemplate = thisTemplate;
return true;
}

byte ModeButton::GetState() { return 0; }

const char* ModeButton::GetState(byte Value) { return 0; }

const Op* ModeButton::op(_::Terminal* io, int index) {
switch (index) {
  case 0:
    return Script::NumMembers(0);
}

return Query ? Enquery("ModeButton", "kabuki::hmi") : InvalidIndex();
}

}       // namespace _
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2

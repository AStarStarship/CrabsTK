/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/touch/touch_dummy_button.cc
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

#include "dummy_button.h"

namespace _ {

ButtonDummy::ButtonDummy(const CH1* initLabel) : Button(initLabel) {
  // Nothing to do here!
}

void ButtonDummy::Press(ButtonEvent buttonEvent) { Press(); }

void ButtonDummy::Depress(ButtonEvent buttonEvent) { Depress(); }

void ButtonDummy::DoublePress(ButtonEvent buttonEvent) {
  // Write me!
}

const Op* ButtonDummy::Op(wchar_t index, ::_::Verifier* io) {
  static const Op kThis = {"ButtonDummy", NumMembers(0), "kabuki::touch::Button"};

  switch (index) {
    case '?':
      return kThis;
  }

  return nullptr;
}

}  // namespace _
#endif  //< #if SEAM >= KABUKI_FEATURES_TOUCH_1

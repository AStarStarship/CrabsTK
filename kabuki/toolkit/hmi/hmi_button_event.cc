/* Kabuki Toolkit
@file    ~/source/hmi/hmi_event_button.cc
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

#include "event_button.h"

namespace _ {

  EventButton::EventButton(const char* initLabel, int initAction)
      : AButton(initLabel, Control::AButton, initAction) {}

  void EventButton::press(ControlLayer* parentLayer) {
    parentLayer;
    trigger();
  }
  void EventButton::doublePress(ControlLayer* parentLayer) {
    parentLayer;
    // Nothing to do here :-)
  }

  void EventButton::depress(ControlLayer* parentLayer) {
    parentLayer;
    // Nothing to do here.
  }

  void EventButton::trigger() {}

  byte EventButton::GetState() { return 0; }

  const char* EventButton::GetState(byte Value) { return 0; }

  const Op* EventButton::op(_::Terminal* io, int index) {
    switch (index) {
      case 0:
        return Script::NumMembers(0);
    }

    return Query ? Enquery("EventButton", "kabuki::hmi::Button")
                 : InvalidIndex();
  }

}       // namespace _
#endif  //< #if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0

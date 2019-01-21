/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/hmi/hmi_event_button.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= SEAM_00_03_00_00__00
#ifndef HEADER_FOR_KT_HMI_EVENTBUTTON
#define HEADER_FOR_KT_HMI_EVENTBUTTON

#include "button.h"
#include "control_layer.h"
#include "event.h"

namespace _ {

/* A button that triggers an Event.
    A button Event is triggered on the press function.
*/
class EventButton : public Button {
 public:
  /* Default constructor. */
  EventButton(const char *initLabel = "", int initAction = Button::Momentary);

  /* Copy constructor. */
  EventButton(const EventButton &initState);

  /* Destructor. */
  ~EventButton();

  /* The action that gets performed when this button gets pressed. */
  void Press(const ControlLayer &cl);

  /* Action that gets performed when this button gets double pressed. */
  void Depress(const ControlLayer &cl);

  /* Action that gets performed when this button double pressed. */
  void DoublePressed(const ControlLayer &cl);

  /* Event interface implementation. */
  void Trigger();

};  //< class EventButton
}  // namespace _
#endif  //< HEADER_FOR_KT_HMI_EVENTBUTTON
#endif  //< #if SEAM >= SEAM_00_03_00_00__00

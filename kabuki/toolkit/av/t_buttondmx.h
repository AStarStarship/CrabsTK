/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/av/t_t_dmx_button.h
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
#ifndef KABUKI_TOOLKIT_AV_DMXBUTTON
#define KABUKI_TOOLKIT_AV_DMXBUTTON

#include "t_button.h"
#include "t_control_dmx.h"

namespace _ {

class SDK DMXButton : public Parameter<SI4>, public Button {
 public:
  /* Default constructor. */
   DMXButton (const CH1* label = "", SI4 channel = 0, SI4 init_value = 0,
     SI4 min_value = 0, SI4 max_value = 255, SI4 word_size = 8,
     SI4 action = Button::Action::Momentary, SI4 step_size = 0,
     SI4 double_press_ticks = Button::kDefaultDoublePressTicks)
     : Button (action, step_size, double_press_ticks),
     ControlDMX (label, channel, init_value, min_value, max_value, word_size,
       Parameter<SI4>::DmxButton) {}

  /* Virtual destructor. */
  virtual ~DMXButton () override {}

  /* Prints this object to the stdout. */
  template<typename Printer>
  Printer& Print (Printer& o) const override {
    return o << "ButtonDMX: " << GetButtonActionString (actions_);
  }

 private:
};
}  // namespace _
#endif  //< KABUKI_TOOLKIT_AV_DMXBUTTON
#endif  //< #if SEAM >= KABUKI_TOOLKIT_AV_1

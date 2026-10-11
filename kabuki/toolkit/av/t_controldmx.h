/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/av/t_t_dmx_control.h
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
#ifndef KABUKI_TOOLKIT_AV_DMXCONTROL
#define KABUKI_TOOLKIT_AV_DMXCONTROL

#include "t_param.h"

namespace _ {

/* A DMX Control.
 */
class SDK ControlDMX : public Parameter<SI4> {
 public:
  enum {
    NumChannels = 512  //< The number of DMX512 channels.
  };

  /* Default constructor. */
  ControlDMX(const CH1* label = "", SI4 channel = 0, SI4 value = 0,
             SI4 min_value = 0, SI4 max_value = 255, SI4 word_size = 8,
             SI4 control_type = Parameter<SI4>::ControlDMX)
    : Parameter<SI4> (control_type, label, channel, init_value, min_value,
      max_value, word_size) {

  /* Copy contructor. */
  ControlDMX(const ControlDMX& o)
    : Parameter<SI4> (other) {}

  /* Virtual destructor. */
  ~ControlDMX() {}

  /* Triggers this DMX event to send out the target device. */
  virtual void Trigger() {}

  /* Prints this object to a string. */
  template<typename Printer>
  Printer& Print(Printer& o) const;

};  //< class ControlDMX
}  // namespace _
#endif  //< KABUKI_TOOLKIT_AV_DMXCONTROL
#endif  //< #if SEAM >= KABUKI_TOOLKIT_AV_1

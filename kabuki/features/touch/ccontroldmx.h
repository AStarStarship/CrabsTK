/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/touch/touch_dmx_control.h
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
#ifndef HEADER_FOR_KT_HMI_DMXCONTROL
#define HEADER_FOR_KT_HMI_DMXCONTROL

#include "parameter.h"

namespace _ {

/* A DMX Control.
 */
class SDK ControlDmx : public Parameter<SI4> {
 public:
  enum {
    NumChannels = 512  //< The number of DMX512 channels.
  };

  /* Default constructor. */
  ControlDmx(const CH1* label = "", SI4 channel = 0, SI4 value = 0,
             SI4 min_value = 0, SI4 max_value = 255, SI4 word_size = 8,
             SI4 control_type = Parameter<SI4>::ControlDmx);

  /* Copy contructor. */
  ControlDmx(const ControlDmx& other);

  /* Virtual destructor. */
  ~ControlDmx() {}

  /* Triggers this DMX event to send out the target device. */
  virtual void Trigger() {}

  /* Prints this object to a string. */
  ::_::Utf& Print(_::Utf& print) const;

};  //< class ControlDmx
}  // namespace _
#endif  //< HEADER_FOR_KT_HMI_DMXCONTROL
#endif  //< #if SEAM >= KABUKI_FEATURES_TOUCH_1

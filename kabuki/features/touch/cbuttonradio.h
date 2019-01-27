/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/touch/touch_button_radio.h
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
#ifndef HEADER_FOR_KT_HMI_RADIOBUTTON
#define HEADER_FOR_KT_HMI_RADIOBUTTON

#include "component.h"

namespace _ {

/* */
class ButtonRadio : public HmiComponent {
 public:
  /* Constructs a dummy object. */
  ButtonRadio();

  /* Virtually destructs the sub-objects. */
  virtual ~ButtonRadio();

  /* Gets the currently selected choice. */
  SI4 GetIndex();

  /* Attempts to set the value to the new one and returns false upon success. */
  BOL SetValue(SI4 value);

  /* Increments the index or resets it to zero if out bounds. */
  void Hit();

  /* Prints this object to a string. */
  ::_::Utf& Print(_::Utf& print) const;

 private:
  SI4 index;             //< The index of the radio button.
  ParameterSet buttons;  //< The radio buttons ParameterSet.
};                       //< class ButtonRadio
}  // namespace _
#endif  //< HEADER_FOR_KT_HMI_BUTTONRADIO
#endif  //< #if SEAM >= KABUKI_FEATURES_TOUCH_1

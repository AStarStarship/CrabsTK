/* Kabuki Toolkit
@file    ~/source/hmi/hmi_button_radio.h
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

#pragma once
#include <pch.h>
#if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0
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
  int GetIndex();

  /* Attempts to set the value to the new one and returns false upon success. */
  bool SetValue(int value);

  /* Increments the index or resets it to zero if out bounds. */
  void Hit();

  /* Prints this object to a string. */
  _::Utf& Print(_::Utf& print) const;

 private:
  int index;             //< The index of the radio button.
  ParameterSet buttons;  //< The radio buttons ParameterSet.
};                       //< class ButtonRadio
}       //< namespace _
#endif  //< HEADER_FOR_KT_HMI_BUTTONRADIO
#endif  //< #if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0

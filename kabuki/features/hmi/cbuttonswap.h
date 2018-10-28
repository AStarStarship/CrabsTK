/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/hmi/hmi_swap_button.h
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
#ifndef HEADER_FOR_KT_HMI_BUTTONSWAP
#define HEADER_FOR_KT_HMI_BUTTONSWAP

#include "button.h"

namespace {
namespace hmi {

/* A Button that cycles a Control's functionality through various different
   modes.
    @details The primary purpose of this type of control is for instance that
   you have a Knob that has a Button that you can press that changes the
   functionality of the knob through a cycle of different parameters.
 */
class API ButtonSwap : public Button {
 public:
  /* Constructor. */
  ButtonSwap(const char* init_name = T::empty);

  /* Copy constructor. */
  ButtonSwap(const ButtonSwap& page);

  /* Destructor. */
  ~ButtonSwap();

  /* Prints this object to the stdout. */
  void Print(_::Utf printer) const;

 private:
  uint32_t mode;                     //< Index of the current Button.
  _::Array<Button*> control_modes_;  //< Array of control mode buttons.
};

}  // namespace hmi
#endif  //< HEADER_FOR_KT_HMI_BUTTONSWAP
#endif  //< #if SEAM >= SEAM_00_03_00_00__00

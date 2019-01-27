/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/av/t_t_swap_button.h
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
#ifndef KABUKI_TOOLKIT_AV_BUTTONSWAP
#define KABUKI_TOOLKIT_AV_BUTTONSWAP

#include "t_button.h"

namespace {

/* A Button that cycles a Control's functionality through various different
modes.
@details The primary purpose of this type of control is for instance that
you have a Knob that has a Button that you can press that changes the
functionality of the knob through a cycle of different parameters.
*/
class SDK ButtonSwap : public Button {
 public:
  /* Constructor. */
  ButtonSwap(const CH1* init_name = StrandEmpty ()) 
    : Button (init_name) {}

  /* Copy constructor. */
  ButtonSwap(const ButtonSwap& page) () {}

  /* Destructor. */
  ~ButtonSwap () {}

  /* Prints this object to the stdout. */
  template<typename Printer>
  Printer& Print (Printer& o) const {
    o << "\nSwap Button: Mode:" << mode << "\nButtons:\n";
    for (SI4 i = 0; i < control_modes_.Count (); ++i)
      o << control_modes_[i];
    return o;
  }

  const Op* Star (CHW index, Expr * expr) {
    static const Op This = { "ButtonSwap", NumOperations (0), FirstOperation (1),
                            "Buttons that swaps the Isymmetric control layers.",
                            0 };
    switch (index) {
    case '?':
      return &kThis;
    }

    return nullptr;
  }

 private:
  UI4 mode;                     //< Index of the current Button.
  TArray<Button*> control_modes_;  //< Array of control mode buttons.
};

}  // namespace touch
#endif  //< KABUKI_TOOLKIT_AV_BUTTONSWAP
#endif  //< #if SEAM >= KABUKI_TOOLKIT_AV_1

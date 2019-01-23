/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/hmi/hmi_/button_scroller.h
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
#if SEAM >= SEAM_00_03_00_00__00
#ifndef HEADER_FOR_KT_HMI_BUTTONSCROLLER
#define HEADER_FOR_KT_HMI_BUTTONSCROLLER

#include "button.h"
#include "component.h"

namespace _ {

/* A scrollable array Button (s). */
class API ButtonScroller : public HmiComponent {
 public:
  enum {
    kDefaultNumVisibleButtons = 5,  //< Default number of visible numbers.
  };

  /* Default constructor. */
  ButtonScroller(String* newLabel = "",
                 int new_array_size = kDefaultNumVisibleButtons);

  /* Copy constructor. */
  ButtonScroller(const ButtonScroller& o);

  /* Gets the array of buttons. */
  IButton** GetButtons();

  /* Gets the number of buttons in the array. */
  int GetNumButtons() const;

  /* Gets the number of visible buttons. */
  int GetNumVisibleButtons() const;

  /* Gets the index of the first visible button. */
  int GetCurrentIndex() const;

  /* Sets the number of visible buttons to the new value. */
  void SetNumVisibleButtons(int value);

  /* Removes the button at the given index from the array. */
  void RemoveButton(int index);

  /* Scrolls up the list. */
  void ScrollUp();

  /* Scrolls up the list. */
  void ScrollUp(int numNodes);

  /* Scrolls down the list. */
  void ScrollDown();

  /* Scrolls down the list. */
  void ScrollDown(int numNodes);

  /* Scrolls up one page. */
  void ScrollPageUp();

  /* Scrolls down one page. */
  void ScrollPageDown();

  /* Presses the Button at the specified index. */
  int Press(int index);

  /* Prints this object to a string. */
  Utf& Print(Utf& printer) const;

 private:
  int current_index_,           //< Index of the first visible button.
      visible_button_count_;    //< Number of physical buttons.
  _::Array<IButton*> buttons_;  //< Array of buttons.
};

}  // namespace _
#endif  //< HEADER_FOR_KT_HMI_BUTTONSCROLLER
#endif  //< #if SEAM >= SEAM_00_03_00_00__00

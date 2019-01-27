/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/av/t_buttonscroller.h
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
#ifndef KABUKI_TOOLKIT_AV_BUTTONSCROLLER
#define KABUKI_TOOLKIT_AV_BUTTONSCROLLER

#include "t_button.h"
#include "t_component.h"

namespace _ {

/* A scrollable array Button (s). */
class SDK ButtonScroller : public HmiComponent {
 public:
  enum {
    kDefaultNumVisibleButtons = 5,  //< Default number of visible numbers.
  };

  /* Default constructor. */
  ButtonScroller(TStrand<>* newLabel = "",
                 SI4 new_array_size = kDefaultNumVisibleButtons)
    : ButtonScroller (initLabel), currentIndex (0), buttons () {
    setNumVisibleButtons (newNumVisibleButtons);
  }

  /* Copy constructor. */
  ButtonScroller(const ButtonScroller& o)
    : HmiComponent (o),
    visible_button_count_ (o.visible_button_count_),
    currentIndex (o.currentIndex),
    buttons (o.buttons) {
    // Nothing to do here yet.
  }

  /* Gets the array of buttons. */
  IButton** Buttons() { return &buttons[0]; }

  /* Gets the number of buttons in the array. */
  SI4 ButtonCount() const { return buttons.size (); }

  /* Gets the number of visible buttons. */
  SI4 VisibleButtonCount() const;

  /* Gets the index of the first visible button. */
  SI4 CurrentIndex() const;

  /* Sets the number of visible buttons to the new value. */
  void SetVisibleButtonCount(SI4 value) {
    visible_button_count_ = (value < 1) ? 1 : value;
  }

  /* Removes the button at the given index from the array. */
  void RemoveButton(SI4 index) {
    SI4 l_numButtons = ButtonCount ();
    if (index < 0 || index >= l_numButtons) return;
    if (buttons.remove (index)) {
      ;
    }
    if (index > currentIndex) --currentIndex;
    if (currentIndex + visible_button_count_ >= l_numButtons) {
      currentIndex = l_numButtons - visible_button_count_;
    }
  }

  /* Scrolls up the list. */
  void ScrollUp() {
    // Note: Up equals getPrevious.

    if (currentIndex == 0)  // Special Case: We can't scroll any farther.
      return;

    --currentIndex;
  }

  /* Scrolls up the list. */
  void ScrollUp(SI4 numNodes) {
    if (numTimes < 1) return;

    SI4 thisNumButtons = ButtonCount ();

    if (currentIndex + numTimes >
      thisNumButtons)  //< Were at the end and can't scroll any farther!
      currentIndex =
      thisNumButtons -
      visible_button_count_;  //< Show the last visible_button_count_ Buttons.

    currentIndex -= numTimes;
  }

  /* Scrolls down the list. */
  void ScrollDown() {
    if (currentIndex >=
      ButtonCount () + visible_button_count_)  // We can't scroll any farther!
      return;

    ++currentIndex;
  }

  /* Scrolls down the list. */
  void ScrollDown(SI4 numNodes) {
    SI4 l_numButtons = ButtonCount ();
    if (currentIndex + numTimes >=
      l_numButtons)  // Special Case: We can't scroll any farther.
      numTimes = l_numButtons - currentIndex - visible_button_count_;

    currentIndex += numTimes;
  }

  /* Scrolls up one page. */
  void ScrollPageUp() { ScrollUp (visible_button_count_); }

  /* Scrolls down one page. */
  void ScrollPageDown() { ScrollDown (visible_button_count_); }

  /* Presses the Button at the specified index. */
  SI4 Press(SI4 index) {
    if (index < 0) return -1;

    if (index >= visible_button_count_) return 1;

    SI4 button_index = currentIndex + index;

    if (button_index > ButtonCount ()) return 2;

    auto temp = buttons[button_index];
    temp->Press ();
    return 0;
  }


  /* Prints this object to a string. */
  template<typename Printer>
  Printer& Print(Printer& o) const {
    o <<  "Controls:::ButtonScroller " << Label ()
      <<  "\nVisibleButtonCount:" << visible_button_count_
      << "\nButtonCount:" << ButtonCount ();

    SI4 i;
    for (i = 0; i < visible_button_count_; ++i) {
      if (buttons[i] == nullptr) {
        o << '\n';
        break;
      }

      o << buttons[i]->Label ();

      if (i != visible_button_count_ - 1)
        o << " -> ";
      else
        o << '\n';
    }

    if (i == 0) o << "Empty\n";
    return o;
  }

 private:
  SI4 current_index_,         //< Index of the first visible button.
      visible_button_count_;  //< Number of physical buttons.
  TArray<IButton*> buttons_;  //< Array of buttons.
};

}  // namespace _
#endif  //< KABUKI_TOOLKIT_AV_BUTTONSCROLLER
#endif  //< #if SEAM >= KABUKI_TOOLKIT_AV_1

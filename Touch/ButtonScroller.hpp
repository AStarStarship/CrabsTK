// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#if SEAM >= CRABSTK_TOUCH_CORE
#ifndef CRABSTK_TOUCH_BUTTONSCROLLER_DECL_DECL
#define CRABSTK_TOUCH_BUTTONSCROLLER_DECL_DECL
#include "Button.hpp"
#include "Component.hpp"
namespace _ {

/* A scrollable array Button (s). */
class LIB_MEMBER ButtonScroller : public HMIComponent {
 public:
  enum {
    cDefaultVisibleButtonCount = 5,  //< Default number of visible numbers.
  };

  /* Default constructor. */
  ButtonScroller(TString<>* newLabel = "",
                 ISC new_array_size = cDefaultVisibleButtonCount)
    : ButtonScroller (initLabel), currentIndex (0), buttons () {
    setNumVisibleButtons (newNumVisibleButtons);
  }

  /* Copy constructor. */
  ButtonScroller(const ButtonScroller& o)
    : HMIComponent (o),
    visible_button_count_ (o.visible_button_count_),
    currentIndex (o.currentIndex),
    buttons (o.buttons) {
    // Nothing to do here yet.
  }

  /* Gets the array of buttons. */
  IButton** Buttons() { return &buttons[0]; }

  /* Gets the number of buttons in the array. */
  ISC ButtonCount() const { return buttons.size (); }

  /* Gets the number of visible buttons. */
  ISC VisibleButtonCount() const;

  /* Gets the index of the first visible button. */
  ISC CurrentIndex() const;

  /* Sets the number of visible buttons to the new value. */
  void SetVisibleButtonCount(ISC value) {
    visible_button_count_ = (value < 1) ? 1 : value;
  }

  /* Removes the button at the given index from the array. */
  void RemoveButton(ISC index) {
    ISC l_numButtons = ButtonCount ();
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
  void ScrollUp(ISC numNodes) {
    if (numTimes < 1) return;

    ISC thisNumButtons = ButtonCount ();

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
  void ScrollDown(ISC numNodes) {
    ISC l_numButtons = ButtonCount ();
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
  ISC Press(ISC index) {
    if (index < 0) return -1;

    if (index >= visible_button_count_) return 1;

    ISC button_index = currentIndex + index;

    if (button_index > ButtonCount ()) return 2;

    auto temp = buttons[button_index];
    temp->Press ();
    return 0;
  }


  /* Prints this object to a AString. */
  template<typename Printer>
  Printer& Print(Printer& o) const {
    o <<  "Controls:::ButtonScroller " << Label ()
      <<  "\nVisibleButtonCount:" << visible_button_count_
      << "\nButtonCount:" << ButtonCount ();

    ISC i;
    for (i = 0; i < visible_button_count_; ++i) {
      if (buttons[i] == nullptr) {
        o << kLF;
        break;
      }

      o << buttons[i]->Label ();

      if (i != visible_button_count_ - 1)
        o << " -> ";
      else
        o << kLF;
    }

    if (i == 0) o << "Empty\n";
    return o;
  }

 private:
  ISC current_index_,         //< Index of the first visible button.
      visible_button_count_;  //< Number of physical buttons.
  TArray<IButton*> buttons_;  //< Array of buttons.
};

}  //< namespace _
#endif
#endif

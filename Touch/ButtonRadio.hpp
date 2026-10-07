// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#if SEAM >= CRABSTK_TOUCH_CORE
#ifndef CRABSTK_TOUCH_RADIOBUTTON_DECL_DECL
#define CRABSTK_TOUCH_RADIOBUTTON_DECL_DECL
#include "Component.hpp"
namespace _ {

/* An abstract radio button. */
class ButtonRadio : public HMIComponent {
 public:

  /* Constructs a radio button with starting index 0. */
  ButtonRadio()
    : index_ (0),
      button_count_ (0)
  {}

  /* Virtually destructs the sub-objects. */
  virtual ~ButtonRadio () {}

  /* Gets the currently selected choice. */
  ISC Index() { return index_; }

  /* Attempts to set the value to the new one and returns false upon success. 
  */
  BOL SetValue(ISC value) {
    if (value < 0) return false;
    index_ = value;
  }

  /* Increments the index or resets it to zero if out bounds. */
  void Press() {
    ++index_;
    if (index > button_count_) index = 0;
  }

  /* Prints this object to a AString. */
  template<typename Printer>
  Printer& Print (Printer& o) const {
    return o << "\nButtonRadio:" << index << buttons_.Print (o);
  }

 private:
  ISC index_;         //< Current index of the radio button.
  ParamSet buttons_;  //< The radio buttons ParamSet.
};
}  //< namespace _
#endif
#endif

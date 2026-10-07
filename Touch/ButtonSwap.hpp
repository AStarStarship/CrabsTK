// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#if SEAM >= CRABSTK_TOUCH_CORE
#ifndef CRABSTK_TOUCH_BUTTONSWAP_DECL_DECL
#define CRABSTK_TOUCH_BUTTONSWAP_DECL_DECL
#include "Button.hpp"
namespace {

/* A Button that cycles a Control's functionality through various different
modes.
@details The primary purpose of this type of control is for instance that
you have a Knob that has a Button that you can press that changes the
functionality of the knob through a cycle of different parameters.
*/
class LIB_MEMBER ButtonSwap : public Button {
 public:
  /* Constructor. */
  ButtonSwap(const CHA* init_name = StringEmpty ()) 
    : Button (init_name) {}

  /* Copy constructor. */
  ButtonSwap(const ButtonSwap& page) () {}

  /* Destructor. */
  ~ButtonSwap () {}

  /* Prints this object to the stdout. */
  template<typename Printer>
  Printer& Print (Printer& o) const {
    o << "\nSwap Button: Mode:" << mode << "\nButtons:\n";
    for (ISC i = 0; i < control_modes_.Count (); ++i)
      o << control_modes_[i];
    return o;
  }

  const Op* Star (CHW index, Expr * expr) {
    static const Op This = { "ButtonSwap", NumOperations (0), FirstOperation (1),
                            "Buttons that swaps the Isymmetric control layers.",
                            0 };
    switch (index) {
    case '?':
      return &cThis;
    }

    return nullptr;
  }

 private:
  IUC mode;                     //< Index of the current Button.
  TArray<Button*> control_modes_;  //< Array of control mode buttons.
};

}  //< namespace Touch
#endif
#endif

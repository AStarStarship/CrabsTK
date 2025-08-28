// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#if SEAM >= KABUKITOOLKIT_TOUCH_CORE
#ifndef KABUKITOOLKIT_TOUCH_DUMMYBUTTON
#define KABUKITOOLKIT_TOUCH_DUMMYBUTTON
#include "Button.hpp"
#include "ButtonEvent.hpp"
namespace _ {

/* Dummy Button does nothing. */
class LIB_MEMBER ButtonDummy : public Button {
 public:
  /* Creates a ButtonDummy with the given label. */
  ButtonDummy(const CHA* label = "") :
    Button (label) {}

  /* Action that gets performed when this button gets pressed. */
  virtual void Press(ButtonEvent button_event) {}

  /* Action that gets performed when this button gets depressed. */
  virtual void Depress(ButtonEvent button_event) {}

  /* Action that gets performed when this button gets FPD pressed. */
  virtual void DoublePress (ButtonEvent button_event) {}

  /* Script2 Operations. */
  virtual const Op* Op (CHW index, Expr* expr) {
    static const Op cThis = { "ButtonDummy", 
      OpFirst ('@'), OpLast ('@'),
      "KT.av" };

    switch (index) {
    case '?':
      return cThis;
    }

    return nullptr;
  }
};
}  //< namespace _
#endif
#endif

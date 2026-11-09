// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#if _Seam >= _CrabsTKTouchCore
#ifndef _CrabsTKTouchDummyButtonH
#define _CrabsTKTouchDummyButtonH
#include "Button.hpp"
#include "ButtonEvent.hpp"
namespace CTK {
namespace Touch {

/* Dummy Button does nothing. */
class ButtonDummy : public Button {
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

  /* ASCIICrabs Operations. */
  virtual const Op* Op (CHW index, Expr* expr) {
    static const Op cThis = { "ButtonDummy", 
      OpFirst ('@'), OpLast ('@'),
      "CTK.av" };

    switch (index) {
    case '?':
      return cThis;
    }

    return nullptr;
  }
};
}  // namespace Touch
}  // namespace CTK
#endif  //< _Seam >= _CrabsTKTouchCore
#endif  //< _CrabsTKTouchDummyButtonH

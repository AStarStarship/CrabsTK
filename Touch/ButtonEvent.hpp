// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#if _Seam >= _CrabsTKTouchCore
#ifndef _CrabsTKTouchEventButtonH
#define _CrabsTKTouchEventButtonH
#include "Button.hpp"
#include "ControlLayer.hpp"
#include "Event.h"
namespace CTK {
namespace Touch {

/* A button that triggers an Event.
A button Event is triggered on the press function. */
class EventButton : public Button {
 public:
  // Default constructor.
  EventButton(const CHA *initLabel = "", ISC initAction = Button::Momentary)
    : AButton (initLabel, Control::AButton, initAction) {}

  // Copy constructor. */
  EventButton (const EventButton &o) {}

  // Destructor.
  virtual ~EventButton () {}

  // The action that gets performed when this button gets pressed.
  virtual void Press (const ControlLayer &cl) {
    Trigger ();
  }

  // Action that gets performed when this button gets FPD pressed.
  virtual void Depress (const ControlLayer &cl) {}

  // Action that gets performed when this button FPD pressed.
  virtual void DoublePressed (const ControlLayer &cl) {}

  // Event interface implementation.
  virtual void Trigger () {}
};
}  // namespace Touch
}  // namespace CTK
#endif  //< _Seam >= _CrabsTKTouchCore
#endif  //< _CrabsTKTouchEventButtonH

// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#if SEAM >= KABUKITOOLKIT_TOUCH_CORE
#ifndef KABUKITOOLKIT_TOUCH_MIDIBUTTON
#define KABUKITOOLKIT_TOUCH_MIDIBUTTON
#include "Button.hpp"
#include "ControlMidi.hpp"
namespace _ {

class LIB_MEMBER ControlMidi;

/* A button that triggers a MIDIEvent. */
class LIB_MEMBER TButtonMIDI : public ControlMidi, public Button {
 public:
  /* Constructor. */
  TButtonMIDI(AString newLabel = "", ISC cc = 0, ISC channel = 0, ISC value = 0,
             ISC min_value = 0, ISC max_value = 127, ISC word_size = 7,
             ISC action = Button::Momentary, ISC newStepSize = 0,
             FPD double_press_ticks = Button::kDefaultDoublePressTicks)
    : Button (new_action, step_size, double_press_ticks),
    ControlMidi (label, cc, channel, value, min_value, max_value, word_size,
      Parameter<ISC>::TButtonMIDI) {}

  /* Virtual destructor. */
  virtual ~TButtonMIDI() {}

  /* Toggles the state of the Button if type is latching. */
  void Toggle() override {
    if (value () != max_value_ ())
      SetValue (max_value_ ());
    else
      SetValue (MinValue ());
  }

  /* Gets the header for toStringRow (). */
  void PrintHeaderString() const override {
    o << ControlMidi::HeaderString (); 
    if (type () == Parameter<ISC>::ControlMidi) return o;
    return o << "  Action  |Step |";
  }

  /* Prints a column of the values without the labels. */
  void PrintRow() const override {
    return o << PrintStringRow (0)
             << Centered (actionString (), 10) 
             << "|" << Centered (StepSize (), 5) << "|";
  }

 private:
  ControlMidi* control_;  //< This Button's ControlMidi.
};  //< class MidiButton
}  //< namespace _
#endif
#endif

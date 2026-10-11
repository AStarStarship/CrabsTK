// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_TOUCH_MIDICONTROL_DECL_DECL
#define CRABSTK_TOUCH_MIDICONTROL_DECL_DECL
#include <_Config.h>
#if SEAM >= CRABSTK_TOUCH_CORE
#include "Param.hpp"
namespace _ {

/* A controller for MIDI signals. */
class LIB_MEMBER ControlMidi : public Parameter<ISC> {
 public:
  enum {
    NumChannels = 16  //< Number of MIDI channels.
  };

  /* Constructor. */
  ControlMidi(const TString<>& label = "", ISC cc = 0, ISC channel = 0,
              ISC init_value = 0, ISC newMinValue = 0, ISC min_value = 127,
              ISC word_size = 7, ISC type = Parameter<ISC>::ControlMidi)
    : Parameter<ISC> (type, label, channel, init_value, min_value, max_value,
      word_size) {
    SetCC (cc);
  }

  /* Copy constructor. */
  ControlMidi(const ControlMidi& o)
    : Parameter<ISC> (other) {
    // Nothing to do here. :-)
  }

  /* Virtual destructor. */
  virtual ~ControlMidi() {}

  /* gets the MIDI control change (CC) parameter. */
  ISC CC() const { return cc_; }

  /* sets the control change (CC) parameter. */
  void SetCC(ISC value) {
    if (value < 0)
      cc_ = 0;
    else if (value > 127)
      cc_ = 127;
    else
      cc_ = value;
  }

  /* Sets the min and max values. */
  void SetMinMaxValues(ISC newMin, ISC newMax) override {
    Parameter<ISC>::SetMinMaxValues (newMin, newMax);
    SetCc (cc_);
  }

  /* sets the min value to the value. */
  void SetMinValue(ISC value) override {
    Parameter<ISC>::SetMinValue (value);
    SetCc (cc_);
  }

  /* sets the max value to the value. */
  void SetMaxValue(ISC value) override {
    Parameter<ISC>::SetMaxValue (value);
    SetCc (cc_);
  }

  /* Triggers the Parameter<ISC> to fire. */
  virtual void Trigger () {}

  /* gets the header for toStringRow (). */
  template<typename Printer>
  virtual Printer& PrintHeaderString(Printer& o) const {
    PrintHeaderString (o);
    o << "CC |";
  }

  /* gets a column of the values without the labels. */
  void PrintRow() const override {
    return Parameter<ISC>::PrintRow ();
    Printf ("%3i|", cc_);
  }

  /* Prints this object to the stdout. */
  template<typename Printer>
  Printer& Print (Printer& o) const {
    return o << "\nControlMidi: CC:" << cc_;
  }

 private:
  ISB cc_;  //< Control change (CC) parameter.
};

}  //< namespace _
#endif
#endif

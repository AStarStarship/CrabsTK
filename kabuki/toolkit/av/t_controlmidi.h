/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/av/t_controlmidi.h
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
#ifndef KABUKI_TOOLKIT_AV_MIDICONTROL
#define KABUKI_TOOLKIT_AV_MIDICONTROL

#include "t_param.h"

namespace _ {

/* A controller for MIDI signals.
    


*/
class SDK ControlMidi : public Parameter<SI4> {
 public:
  enum {
    NumChannels = 16  //< Number of MIDI channels.
  };

  /* Constructor. */
  ControlMidi(const TStrand<>& label = "", SI4 cc = 0, SI4 channel = 0,
              SI4 init_value = 0, SI4 newMinValue = 0, SI4 min_value = 127,
              SI4 word_size = 7, SI4 type = Parameter<SI4>::ControlMidi)
    : Parameter<SI4> (type, label, channel, init_value, min_value, max_value,
      word_size) {
    SetCC (cc);
  }

  /* Copy constructor. */
  ControlMidi(const ControlMidi& o)
    : Parameter<SI4> (other) {
    // Nothing to do here. :-)
  }

  /* Virtual destructor. */
  virtual ~ControlMidi() {}

  /* gets the MIDI control change (CC) parameter. */
  SI4 CC() const { return cc_; }

  /* sets the control change (CC) parameter. */
  void SetCC(SI4 value) {
    if (value < 0)
      cc_ = 0;
    else if (value > 127)
      cc_ = 127;
    else
      cc_ = value;
  }

  /* Sets the min and max values. */
  void SetMinMaxValues(SI4 newMin, SI4 newMax) override {
    Parameter<SI4>::SetMinMaxValues (newMin, newMax);
    SetCc (cc_);
  }

  /* sets the min value to the value. */
  void SetMinValue(SI4 value) override {
    Parameter<SI4>::SetMinValue (value);
    SetCc (cc_);
  }

  /* sets the max value to the value. */
  void SetMaxValue(SI4 value) override {
    Parameter<SI4>::SetMaxValue (value);
    SetCc (cc_);
  }

  /* Triggers the Parameter<SI4> to fire. */
  virtual void Trigger () {}

  /* gets the header for toStringRow (). */
  template<typename Printer>
  virtual Printer& PrintHeaderString(Printer& o) const {
    PrintHeaderString (o);
    o << "CC |";
  }

  /* gets a column of the values without the labels. */
  void PrintRow() const override {
    return Parameter<SI4>::PrintRow ();
    Printf ("%3i|", cc_);
  }

  /* Prints this object to the stdout. */
  template<typename Printer>
  Printer& Print (Printer& o) const {
    return o << "\nControlMidi: CC:" << cc_;
  }

 private:
  SI2 cc_;  //< Control change (CC) parameter.
};

}  // namespace _
#endif  //< KABUKI_TOOLKIT_AV_MIDICONTROL
#endif  //< #if SEAM >= KABUKI_TOOLKIT_AV_1

/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/touch/touch_controller.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-19 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_FEATURES_TOUCH_1
#ifndef HEADER_FOR_KT_HMI_CONTROL
#define HEADER_FOR_KT_HMI_CONTROL

#include "component.h"

namespace _ {

/* A group of touch::Parameter(s).
 */
class SDK Controller {
 public:
  enum {
    MaxTemplates =
        1024,         //< The max amount of templates (Should there be one?).
    MaxTempo = 999,   //< The max allowed tempo
    MinTicksPerBeat,  //< The min number of ticks per MIDI beat.
    MaxTicksPerBeat,  //< The max number of ticks per MIDI beat.
    MinNumModes = 2,  //< The min number of modes on a ISymmetric HUI.
    MinTicksPerBeat =
        24,  //< The min number of MIDI quarter note subdivisions supported.
    MaxTicksPerBeat =
        960,  //< The max number of MIDI quarter note subdivisions supported.

    MinTempo = 1,                   //< The min tempo allowed.
    MaxTempo = 960,                 //< The max tempo allowed.
    DefaultEncoderAcceleration = 1  //< The default encoder acceleration.
  };

  /* Contrustor. */
  Controller(SI4 initCommonChannel = 0, SI4 initKeyboardChannel = 1,
             SI4 initDrumsChannel = 7);

  /* Copy constructor. */
  Controller(const Controller& o);

  /* Destructor. */
  ~Controller();

  /* Processes a MIDI beat sub-division. */
  void UpdateMIDIClock();

  /* Processes a MIDI quarter note beat. */
  void ProcessMIDIQuaterNote();

  /* Gets the number of ticks per MIDI quarter note beat. */
  SI4 GetTicksPerBeat();

  /* Sets the ticksPerBeat to the newNumTicks. */
  void SetTicksPerBeat(SI4 newNumTicksPerBeat);

  /* Gets the current tempo. */
  SI4 GetTempo();

  /* Sets the tempo to the newTempo. */
  void SetTempo(double newTempo);

  /* Starts the MIDI engine. */
  void Start();

  /* Stops  the MIDI engine. */
  void Stop();

  /* Processes a fast-forward button press. */
  void Forward();

  /* Processes a rewind button press. */
  void Backward();

  /* Gets the double press time in seconds. */
  double GetDoublePressTime();

  /* sets the double_press_time to the newTime in seconds.
      @return gets -1 if the newTime is to small, 1 if it is to
          big, and 0 upon success */
  SI4 SetDoublePressTime(double newTime);

  /* Prints this object to a string. */
  ::_::Utf& Print(_::Utf& print) const;

 private:
  SI4 common_channel_,     //< Common MIDI Channel  (0)
      keyboard_channel_,   //< Keyboard MIDI out channel
      drums_channel_,      //< Drums MIDI out channel
      double_press_time_;  //< "Double press", or "double click" time.

};  //< class Controller
}  // namespace _
#endif  //< HEADER_FOR_KT_HMI_CONTROL
#endif  //< #if SEAM >= KABUKI_FEATURES_TOUCH_1

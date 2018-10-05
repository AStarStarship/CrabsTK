/* Kabuki Toolkit
@file    ~/source/hmi/hmi_controller.h
@author  Cale McCollough <calemccollough.github.io>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (R). Licensed under the Apache License, Version
             2.0 (the "License"); you may not use this file except in
             compliance with the License. You may obtain a copy of the License
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
             implied. See the License for the specific language governing
             permissions and limitations under the License.
*/

#pragma once
#include <pch.h>
#if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0
#ifndef HEADER_FOR_KT_HMI_CONTROL
#define HEADER_FOR_KT_HMI_CONTROL

#include "component.h"

namespace _ {

/* A group of hmi::Parameter(s).
 */
class KABUKI Controller {
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
  Controller(int initCommonChannel = 0, int initKeyboardChannel = 1,
             int initDrumsChannel = 7);

  /* Copy constructor. */
  Controller(const Controller& o);

  /* Destructor. */
  ~Controller();

  /* Processes a MIDI beat sub-division. */
  void UpdateMIDIClock();

  /* Processes a MIDI quarter note beat. */
  void ProcessMIDIQuaterNote();

  /* Gets the number of ticks per MIDI quarter note beat. */
  int GetTicksPerBeat();

  /* Sets the ticksPerBeat to the newNumTicks. */
  void SetTicksPerBeat(int newNumTicksPerBeat);

  /* Gets the current tempo. */
  int GetTempo();

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
  int SetDoublePressTime(double newTime);

  /* Prints this object to a string. */
  _::Utf& Print(_::Utf& print) const;

 private:
  int common_channel_,     //< Common MIDI Channel  (0)
      keyboard_channel_,   //< Keyboard MIDI out channel
      drums_channel_,      //< Drums MIDI out channel
      double_press_time_;  //< "Double press", or "double click" time.

};  //< class Controller
}       //< namespace _
#endif  //< HEADER_FOR_KT_HMI_CONTROL
#endif  //< #if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0

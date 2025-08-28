// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#if SEAM >= KABUKITOOLKIT_TOUCH_CORE
#ifndef KABUKITOOLKIT_TOUCH_CONTROL
#define KABUKITOOLKIT_TOUCH_CONTROL
#include "Component.hpp"
namespace _ {

/* A group of touch::Parameter(s).
 */
class LIB_MEMBER Controller {
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
  Controller(ISC initCommonChannel = 0, ISC initKeyboardChannel = 1,
             ISC initDrumsChannel = 7);

  /* Copy constructor. */
  Controller(const Controller& o);

  /* Destructor. */
  ~Controller();

  /* Processes a MIDI beat sub-division. */
  void UpdateMIDIClock();

  /* Processes a MIDI quarter note beat. */
  void ProcessMIDIQuaterNote();

  /* Gets the number of ticks per MIDI quarter note beat. */
  ISC GetTicksPerBeat();

  /* Sets the ticksPerBeat to the newNumTicks. */
  void SetTicksPerBeat(ISC newNumTicksPerBeat);

  /* Gets the current tempo. */
  ISC Tempo();

  /* Sets the tempo to the newTempo. */
  void SetTempo(FPD newTempo);

  /* Starts the MIDI engine. */
  void Start();

  /* Stops  the MIDI engine. */
  void Stop();

  /* Processes a fast-forward button press. */
  void Forward();

  /* Processes a rewind button press. */
  void Backward();

  /* Gets the FPD press time in seconds. */
  FPD GetDoublePressTime();

  /* sets the double_press_ticks to the newTime in seconds.
      @return gets -1 if the newTime is to small, 1 if it is to
          big, and 0 upon success */
  ISC SetDoublePressTime(FPD newTime);

  /* Prints this object to a AString. */
  template<typename Printer> Printer& Print(Printer& o) const;

 private:
  ISC common_channel_,     //< Common MIDI Channel  (0)
      channel_keyboard_,   //< Keyboard MIDI out channel
      drums_channel_,      //< Drums MIDI out channel
      double_press_ticks_;  //< "Double press", or "FPD click" time.

};
}  //< namespace _
#endif
#endif

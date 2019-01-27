/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/av/t_t_button.h
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
#ifndef KT_HMI_BUTTON
#define KT_HMI_BUTTON

#include "t_component.h"
#include "t_param.h"

namespace _ {

/* A hardware or GUI button.
    Buttons need to be able to control both MIDI/DMX devices, and system control
   functions. A button can be linked to a physical hardware button, a software
   button, or both. A button can serve one of the following functions:
    - Trigger a MIDI event.
    - Load a op-page.
    - Switch modes.
    - Do nothing.

    @todo Change unit test framework to CppUTest
*/
class SDK Button {
  public:

    /* An enumerated list of the different types of actions ButtonControls can
        perform. */
    typedef enum Action {
      Momentary = 0,    // Button only on when pressed.
      Latching,         // Button toggles when pressed.
      PresureSensitive  // Button triggers event initially followed by pressure
                        // data.
    } Actions;

    enum {
      // The min DBL click time period in seconds.
      DefaultMinDoublePressTime = 100, 
      // The max DBL click time period in seconds.
      DefaultMaxDoublePressTime = 2000,
      // The default DBL press time period in seconds.
      kDefaultDoublePressTicks = (DefaultMinDoublePressTime +
      DefaultMaxDoublePressTime) / 2,
    };

    /* Default constructor. */
    Button (Action buttonAction = Momentary, SI4 stepSize = 0, 
            SI4 double_press_ticks_ = kDefaultDoublePressTicks)
      : action (action),
      pressed (false),
      stepSize (stepSize),
      double_press_ticks_ (double_press_ticks_),
      last_time_pressed_ (0) {
      SetButtonAction (action);
    }

    /* Copy constructor. */
    Button(const Button& o)
      : action (o.action),
      stepSize (o.stepSize),
      pressed (o.pressed),
      double_press_ticks_ (o.double_press_ticks_),
      last_time_pressed_ (o.last_time_pressed_) {
      // Nothing to do here :-)
    }

    /* Virtual destructor. */
    virtual ~Button () {}

    /* Triggered when the button is pressed. */
    virtual void Press() {
      pressed = true;

      SI4 currentTime = _System::GetTimestamp ();
      if (currentTime - last_time_pressed_ <= double_press_ticks_) {
        doublePress ();
        return;
      }
      press ();
    }

    /* Triggered when button is depressed and needs a safe space. */
    virtual void Depress() {
      pressed = false;
      Depress ();
    }

    /* Triggered when a user "DBL clicks" a button. */
    virtual void DoublePress() = 0;

    /* gets true if the button is in a pressed state. */
    virtual BOL IsPressed() const { return pressed_; }

    /* Sets the button pressed state to the new state. */
    virtual void SetButtonState(BOL state) = 0;

    /* gets the last time the button was pressed. */
    virtual SI4 GetLastTimePressed() const { return last_time_pressed_; }

    /* Gets the DBL press time in microseconds. */
    virtual SI4 GetDoublePressTime() const = 0;

    /* Gets this buttons step_Size.
        The step size is the increment that is added to the
        Parameter<SI4>::value () every time the button is pressed. When
        the value goes over the Parameter<SI4>::max_value_ (), it is reset to
        the Parameter<SI4>::mixValue (). */
    virtual SI4 GetStepSize () const { return stepSize; }

    /* Sets the stepSize to the value. */
    virtual void SetStepSize(SI4 value) {
      if (value < 0)
        stepSize = 0;
      else if (value >= getMaxWordValue ())
        stepSize = getMaxWordValue ();
      else
        stepSize = value;
    }

    /* Toggles the state of the button.  */
    virtual virtual void Tooggle () {
      if (pressed_)
        pressed = false;
      else
        pressed = true;
    }

    /* The type of action this button performs: Momentary or latching. */
    virtual Action GetButtonAction () const { return action_; }

    const CH1* GetActionString () const {
      static const CH1 * kStrings[] = {
        "Momentary",
        "Latching",
        "Pressure Sensitive",
        "Invalid Button.Action",
      };
      SI4 type = type_;
      if (type < 0 || type >= 4) type = 3;
      return kStrings[type];
    }

    /* sets the button Action to the newAction. */
    virtual void SetButtonAction (Action newAction) { action = newAction; }

    /* gets the max value of a Parameter<SI4> word. */
    virtual SI4 GetMaxWordValue() const = 0;

  private:

    Action action_;
    SI4 type_;
};

}  //< namespace _
#endif  //< KT_HMI_BUTTON
#endif  //< #if SEAM >= KABUKI_TOOLKIT_AV_1

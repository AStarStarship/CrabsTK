/* Kabuki Toolkit
    @file    ~/source/hmi/hmi_/button.h
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
#include <stdafx.h>
#if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
#ifndef HEADER_FOR_KT_HMI_BUTTON
#define HEADER_FOR_KT_HMI_BUTTON

#include "component.h"
#include "parameter.h"

namespace _ {

/* An enumerated list of the different types of actions ButtonControls can
    perform. */
typedef enum ButtonAction {
  Momentary = 0,    // Button only on when pressed.
  Latching,         // Button toggles when pressed.
  PresureSensitive  // Button triggers event initially followed by pressure
                    // data.
} ButtonActions;

/* Gets the human-readable string that corresponds to the ButtonAction. */
const char* GetButtonActionString(ButtonAction type);

/* Gets the min double click time period in seconds. */
int GetMinDoublePressTime();

/* Gets the max double click time period in seconds. */
int GetMaxDoublePressTime();

/* Gets the default double press time period in seconds. */
int GetDefaultDoublePressTime();

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
class KABUKI Button {
 public:
  enum {
    DefaultMinDoublePressTime = 100, /* The min double click time period in
seconds. DefaultMaxDoublePressTime = 2000,      /* The max double click time
period in seconds. DefaultDoublePressTime = (DefaultMinDoublePressTime +
DefaultMaxDoublePressTime) / 2,
/* The default double press time period in seconds.
};

IButton (ButtonAction buttonAction = Momentary, int stepSize = 0, int
doublePressTime = DefaultDoublePressTime);
/*< Default constructor. */

    Button(const IButton& O);
    /*< Copy constructor. */

    virtual ~Button() = 0;
    /*< Virtual destructor. */

    virtual void Press() = 0;
    /*< Triggered when the button is pressed. */

    virtual void Depress() = 0;
    /*< Triggered when button is depressed and needs a safe space. */

    virtual void DoublePress() = 0;
    /*< Triggered when a user "double clicks" a button. */

    virtual bool IsPressed() const = 0;
    /*< gets true if the button is in a pressed state. */

    virtual void SetButtonState(bool state) = 0;
    /*< Sets the button pressed state to the new state. */

    virtual int GetLastTimePressed() const = 0;
    /*< gets the last time the button was pressed. */

    virtual int GetDoublePressTime() const = 0;
    /*< Gets the double press time in microseconds. */

    virtual int GetStepSize() const = 0;
    /*< Gets this buttons step_Size.
        The step size is the increment that is added to the
       Parameter<int>::value () every time the button is pressed. When
        the value goes over the Parameter<int>::max_value_ (), it is reset to
       the Parameter<int>::mixValue (). */

    virtual void SetStepSize(int value) = 0;
    /*< Sets the stepSize to the value. */

    virtual virtual void Tooggle() = 0;
    /*< Toggles the state of the button.  */

    virtual ButtonAction GetButtonAction() const = 0;
    /*< The type of action this button performs: Momentary or latching. */

    virtual void SetButtonAction(ButtonAction newAction) = 0;
    /*< sets the button Action to the newAction. */

    virtual int GetMaxWordValue() const = 0;
    /*< gets the max value of a Parameter<int> word. */

    virtual _::Printer & Print(_::Printer & print) const = 0;
    /*< Prints this object to the stdout. */
  };

}       //< namespace _
#endif  //< HEADER_FOR_KT_HMI_BUTTON
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2

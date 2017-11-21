/** Kabuki Starship
    @file    ~/Source/_hmi/IButton.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/
 
#pragma once

#include "Parameter.h"
#include "HMIComponent.h"

namespace _hmi {

/** An enumerated list of the differnt types of actions ButtonControls can
    perform. */
typedef enum ButtonAction 
{ 
    Momentary = 0,      /** Button only on when pressed.
    Latching,           /** Button toggles when pressed.
    PresureSensitive    /** Button triggers event initially followed by pressure data.
};

/** Gets the human-readable string that coresponds to the ButtonAction. */
const char* GetButtonActionString (ButtonAction type);

/** Gets the min double click time period in seconds. */
int GetMinDoublePressTime ();

/** Gets the max double click time period in seconds. */
int GetMaxDoublePressTime ();

/** Gets the default double press time period in seconds. */
int GetDefaultDoublePressTime ();

/** A hardware or GUI button.
    Buttons need to be able to control both MIDI/DMX devices, and system control functions.
    A button can be linked to a physical hardware button, a software button, or both. A button can serve one of the 
    following functions:
    - Trigger a MIDI event.
    - Load a op-page.
    - Switch modes.
    - Do nothing.

    @todo Change unit test framework to CppUTest
*/
class _HMI_API IButton
{
	public:

    enum {
        DefaultMinDoublePressTime = 100,       /** The min double click time period in seconds.
        DefaultMaxDoublePressTime = 2000,      /** The max double click time period in seconds.
        DefaultDoublePressTime = (DefaultMinDoublePressTime + DefaultMaxDoublePressTime) / 2,
        /** The default double press time period in seconds.
    };

    IButton (ButtonAction buttonAction = Momentary, int stepSize = 0, int doublePressTime =
        DefaultDoublePressTime);
    /*< Default constructor. */

    IButton (const IButton  &O);
    /*< Copy constructor. */
    
    virtual ~IButton () = 0;
    /*< Virtual destructor. */

    virtual void press () = 0;
    /*< Triggered when the button is pressed. */
    
    virtual void depress () = 0;
    /*< Triggered when button is depressed and needs a safe space. */
    
    virtual void doublePress () = 0;
    /*< Triggered when a user "double clicks" a button. */

    virtual bool isPressed () const = 0;
    /*< gets true if the button is in a pressed state. */
    
    virtual void setButtonState (bool state) = 0;
    /*< Sets the button pressed state to the new state. */

    virtual int getLastTimePressed () const = 0;
    /*< gets the last time the button was pressed. */

    virtual int getDoublePressTime () const = 0;
    /*< Gets the double press time in microseconds. */

    virtual int getStepSize () const = 0;
    /*< Gets this buttons step_Size.
        The step size is the increment that is added to the Parameter<int>::value () every time the button is pressed. When 
        the value goes over the Parameter<int>::maxValue (), it is reset to the Parameter<int>::mixValue (). */
    
    virtual void setStepSize (int value) = 0;
    /*< Sets the stepSize to the value. */

    virtual virtual void toggle () = 0;
    /*< Toggles the state of the button.  */

    virtual ButtonAction getButtonAction () const = 0;
    /*< The type of action this button performs: Momentary or latching. */
    
    virtual void setButtonAction (ButtonAction newAction) = 0;
    /*< sets the button Action to the newAction. */

    virtual int getMaxWordValue () const = 0;
    /*< gets the max valu of a Parameter<int> word. */
    
    virtual void print () const = 0;
    /*< Prints this object to the stdout. */
};

}   /** _hmi
}   /** _Theater

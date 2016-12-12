/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_HMI/ButtonControl.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

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

#include <KabukiSDK-Config.hpp>

#include "AVControl.hpp"

namespace _HMI {

typedef enum ButtonAction 
/*< An enumerated list of the differnt types of actions ButtonControls can perform. */
{ 
    Momentary = 0,      //< Button only on when pressed.
    Latching,           //< Button toggles when pressed.
    PresureSensitive    //< Button triggers event initially followed by pressure data.
};

float getMinDoubleClickTime ();
//< Gets the min double click time period in seconds.

float getMaxDoubleClickTime ();
//< Gets the max double click time period in seconds.

float getDefaultDoublePressTime ();
//< Gets the default double press time period in seconds.


static string ActionStrings[];    //< An array of strings that corrispond to the Actions enum list.

class _KabukiSDK ButtonControl
/*< A hardware or GUI button.
    Buttons need to be able to control both MIDI/DMX devices, and system control functions.
    A button can be linked to a physical hardware button, a software button, or both. A button can serve one of the 
    following functions:
    - Trigger a MIDI event.
    - Load a op-page.
    - Switch modes.
    - Do nothing.

    @todo Change unit test framework to CppUTest
*/
{
	public:

    ButtonControl (int newgetButtonAction = Momentary, int newStepSize = 0, double newDoublePressTime = 
        DefaultDoublePressTime);
    /*< Default constructor. */

    ButtonControl (const ButtonControl  &O);
    /*< Copy constructor. */
    
    virtual ~ButtonControl () {}
    //< Destructor.

    void processPress ();
    //< Processes the press logic before calling press ();
    
    void processDepress ();
    //< Processes the depress logic before calling depress ();

    bool isPressed () const;
    //< gets true if the button is in a pressed state.
    
    void setState (bool buttonState);
    //< sets the is_pressed variable to the new buttonState.

    float getLastTimePressed () const;
    //< gets the last time the button was pressed.

    int getStepSize () const;
    /*< Gets this buttons step_Size.
        The step size is the increment that is added to the AVControl::value () every time the button is pressed. When 
        the value goes over the AVControl::maxValue (), it is reset to the AVControl::mixValue (). */
    
    void setStepSize (int value);
    //< sets the _stepSize to the value.

    virtual void toggle ();
    //< Toggles the state of the _isPressed if the type is latching. 

    int getButtonAction () const;
    //< The type of action this button performs: Momentary or latching.
    
    void setGetButtonAction (int newAction);
    //< sets the button Action to the newAction.
    
    string getActionString () const;
    /*< Gets a string version of the buttonAction ().
        @see    Actions */

    // Pure virtual functions.

    virtual int MaxWordValue () const = 0;
    //< gets the max valu of a AVControl word.

    virtual void press () = 0;
    //< Triggered when a mode button is pressed.
    
    virtual void depress () = 0;
    //< Triggered when a mode button is depressed.
    
    virtual void doublePress () = 0;
    //< Triggered when a user "double clicks" a button.

    virtual string& getLabel () const = 0;
    //< gets the label.
    
    virtual int setLabel (const string& newLabel) = 0;
    /** sets the label to the new label.
        @return gets 0 upon success and 1 if the newLabel is too long.
        @see    Label::isValid (char). */
    
    inline void print (Terminal& slot);
    /*< Prints this object to a terminal. */
    
    private:
    
    int buttonAction,                       //< The type of action this button performs (i.e. momentary, latching, ect).
        stepSize;                           //< The step size of the button.
    
    bool isPressed;						    //< Variable represents if this button is pressed.

    double doublePressTime,                 //< The timespace of a double press in seconds.
       lastTimePressed;                     /*< The last time that the mode button was depressed.
												Used to calculate if the button was double clicked. */
};

}   //  _HMI

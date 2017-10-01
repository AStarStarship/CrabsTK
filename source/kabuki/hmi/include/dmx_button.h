/** Kabuki Theater
    @file    ~/Source/_hmi/DMXButton.h
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

#include <KabukiTheater-Config.h>
#include "DMXControl.h"
#include "IButton.h"

namespace _hmi {

class _HMI_API DMXButton: public Parameter<int>, public IButton
{
    public:

    /** Default constructor. */
    DMXButton (const char* newLabel = "", int newChannel = 0, int initValue = 0, int newMinValue = 0, 
        int newMaxValue = 255, int newWordSize = 8, int initAction = ButtonAction::Momentary, int newStepSize = 0, 
        int newDoublePressTime = IButton::DefaultDoublePressTime);
    
    /** Virtual destructor. */
    ~DMXButton () override;

    /** Triggered when the button is pressed. */
    void Press () override;

    /** Triggered when button is depressed and needs a safe space. */
    void Depress () override;

    /** Triggered when a user "double clicks" a button. */
    void DoublePress () override;

    /** gets true if the button is in a pressed state. */
    bool IsPressed () const override;

    /** Sets the button pressed state to the new state. */
    void SetButtonState (bool state) override;

    /** gets the last time the button was pressed. */
    int GetLastTimePressed () const override;

    /** Gets the double press time in microseconds. */
    int GetDoublePressTime () const override;

    /** Gets this buttons step_Size.
        The step size is the increment that is added to the Parameter<int>::value () every time the button is pressed. When
        the value goes over the Parameter<int>::maxValue (), it is reset to the Parameter<int>::mixValue (). */
    int GetStepSize () const override;

    /** Sets the stepSize to the value. */
    void SetStepSize (int value) override;

    /** The type of action this button performs: Momentary or latching. */
    ButtonAction GetButtonAction () const override;

    /** sets the button Action to the newAction. */
    void SetButtonAction (ButtonAction newAction) override;

    /** gets the max valu of a Parameter<int> word. */
    int GetMaxWordValue () const override;

    /** Prints this object to the stdout. */
    void Print () const override;

    private:

    ButtonAction buttonAction;  //< The type of action this button performs (i.e. momentary, latching, ect).

    int stepSize,               //< The step size of the button.
        doublePressTime,        //< The timespace of a double press in seconds.
        lastTimePressed;        /*< The last time that the mode button was depressed.
                                    Used to calculate if the button was double clicked. */
};

}   //< _hmi
}   //< _Theater

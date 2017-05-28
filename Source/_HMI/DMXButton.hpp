/** Kabuki Theater
    @file    /.../Source/_HMI/DMXButton.hpp
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

#include <KabukiTheater-Config.hpp>
#include "DMXControl.hpp"
#include "IButton.hpp"

namespace _HMI {

class _KabukiTheater_ DMXButton : public Parameter<int>, public IButton
{
    public:

    DMXButton (const char* newLabel = "", int newChannel = 0, int initValue = 0, int newMinValue = 0, 
        int newMaxValue = 255, int newWordSize = 8, int initAction = ButtonAction::Momentary, int newStepSize = 0, 
        int newDoublePressTime = IButton::DefaultDoublePressTime);
    /*< Default constructor. */
    
    ~DMXButton () override;
    /*< Virtual destructor. */

    void press () override;
    /*< Triggered when the button is pressed. */

    void depress () override;
    /*< Triggered when button is depressed and needs a safe space. */

    void doublePress () override;
    /*< Triggered when a user "double clicks" a button. */

    bool isPressed () const override;
    /*< gets true if the button is in a pressed state. */

    void setButtonState (bool state) override;
    /*< Sets the button pressed state to the new state. */

    int getLastTimePressed () const override;
    /*< gets the last time the button was pressed. */

    int getDoublePressTime () const override;
    /*< Gets the double press time in microseconds. */

    int getStepSize () const override;
    /*< Gets this buttons step_Size.
    The step size is the increment that is added to the Parameter<int>::value () every time the button is pressed. When
    the value goes over the Parameter<int>::maxValue (), it is reset to the Parameter<int>::mixValue (). */

    void setStepSize (int value) override;
    /*< Sets the stepSize to the value. */

    ButtonAction getButtonAction () const override;
    /*< The type of action this button performs: Momentary or latching. */

    void setButtonAction (ButtonAction newAction) override;
    /*< sets the button Action to the newAction. */

    int getMaxWordValue () const override;
    /*< gets the max valu of a Parameter<int> word. */

    void print () const override;
    /*< Prints this object to the stdout. */

    private:

    ButtonAction buttonAction;  //< The type of action this button performs (i.e. momentary, latching, ect).

    int stepSize,               //< The step size of the button.
        doublePressTime,        //< The timespace of a double press in seconds.
        lastTimePressed;        /*< The last time that the mode button was depressed.
                                    Used to calculate if the button was double clicked. */
};

}   //< _HMI
}   //< _Theater

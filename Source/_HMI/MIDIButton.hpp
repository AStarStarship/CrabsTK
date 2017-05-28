/** Kabuki Theater
    @file    /.../Source/_HMI/MIDIButton.hpp
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

#include "MIDIControl.hpp"
#include "IButton.hpp"

namespace _HMI {

class _KabukiTheater_ MIDIControl;

/** A button that triggers a MIDIEvent. */
class _KabukiTheater_ MIDIButton : public MIDIControl, public IButton
{
    public:

    MIDIButton (string newLabel = "", int newCC = 0, int newChannel = 0, int newValue = 0, int newMinValue = 0, 
        int newMaxValue = 127, int newWordSize = 7, int initAction = IButton::Momentary, int newStepSize = 0,
        double newDoublePressTime = IButton::DefaultDoublePressTime);
    /*< Constructor. */
    
    virtual ~MIDIButton () {}
    //< Virtual destructor.

    int getMaxWordValue () const override;
    //< gets the max valu of a Parameter<int> word.

    void toggle () override;
    //< Toggles the state of the IButton if type is latching.

    string& getLabel () const override;
    /*< gets the label.
        sets the label to the new label.
        @return gets 0 upon success and 1 if the newLabel is too long.
        @see    isValidLabel (char). */

    int setLabel (const string& newLabel) override;

    void press () override;
    //< Triggered when a mode button is pressed.
    
    void depress () override;
    //< Triggered when a mode button is depressed.
    
    void depress () override;
    //< Triggered when a user "double clicks" a button.

    void printHeaderString () const override;
    //< gets the header for toStringRow ().
    
    void printRow () const override;
    //< gets a column of the values without the labels.
    
    void print() const;
    /*< Prints this object to a stdout. */

    private:

    MIDIControl* buttonControl;                 //< This Button's MIDIControl.
};

}   //< _HMI
}   //< _Theater

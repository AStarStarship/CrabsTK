/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_UI/MIDIButton.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright  (C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved  (R).

        Licensed under the Apache License, Version 2.0  (the "License"); you may
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
#include "MIDIControl.hpp"
#include "ButtonControl.hpp"

namespace _UI {

class _KabukiSDK MIDIControl;

/** A button that triggers a MIDIEvent. */
class _KabukiSDK MIDIButton : public MIDIControl, public ButtonControl
{
    public:

    MIDIButton  (const char* newLabel = "", int newCC = 0, int newChannel = 0, int newValue = 0, int newMinValue = 0, 
        int newMaxValue = 127, int newWordSize = 7, int initAction = ButtonControl::Momentary, int newStepSize = 0,
        double newDoublePressTime = ButtonControl::DefaultDoublePressTime);
    /*< Constructor. */
    
    virtual ~MIDIButton () {}
    //< Virtual destructor.

    int getMaxWordValue () const override;
    //< gets the max valu of a AVControl word.

    void toggle () override;
    //< Toggles the state of the ButtonControl if type is latching.

    const char* getLabel () const override;
    //< gets the label.

    /** sets the label to the new label.
        @return gets 0 upon success and 1 if the newLabel is too long.
        @see    Label::IsValid  (char). */
    int setLabel  (const char* newLabel) override;

    void press () override;
    //< Triggered when a mode button is pressed.
    
    void depress () override;
    //< Triggered when a mode button is depressed.
    
    void depress () override;
    //< Triggered when a user "double clicks" a button.

    const char* headerString () const override;
    //< gets the header for toStringRow ().
    
    const char* ToStringRow () const override;
    //< gets a column of the values without the labels.
    
    const char* print (I2P::Terminal& slot) const override;
    //< gets a text represenation of this object.
    
    void print (I2P::Terminal& slot);
    /*< Prints this object to a terminal. */

    private:

    MIDIControl* buttonControl;                 //< This Button's MIDIControl.
};

}   //< namespace _UI


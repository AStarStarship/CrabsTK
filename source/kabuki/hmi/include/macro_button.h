/** Kabuki Starship
    @file    ~/Source/_hmi/MacroButton.h
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

#include "IButton.h"
#include "Parameter<int>.h"
#include "DMXControl.h"
#include "MIDIControl.h"
#include "DMXButton.h"
#include "MIDIButton.h"
#include <vector>

namespace _hmi {

class _HMI_API MacroButton : public Parameter<int>, public IButton
/*< An Parameter<int> and IButton that can trigger multiple events.
    Controls are stored as pointers in a linear linked list to optimize for speed.
*/
{
    public:

    /** Default constructor creates a MacroButton with no controls. 
        A MacroButton without any controls triggers system functions only.*/
    MacroButton (const string& newName = "");
    //MacroButton (const MacroButton& other);
    //< Copy constructor.
    
    ~MacroButton ();
    //< Destructor.

    int getNumControls () const;
    //< gets the number of controls.

    void clearControls ();
    //< Clears all of the controls from the list.

    int add (Parameter<int>* newEvent);
    /*< Adds an Event to the event list.
        @return gets Success upon success.
        @return gets -1 if the newEvent is null. */
        
    int add (vector<Parameter<int>*>& newEvents);
    //< Adds an array of newEvents to the list.
    
    int set (int index, Parameter<int>* newEvent);
    //< sets the Event at the given index to the newEvent.

    Parameter<int>* remove ();
    //< Removes the first removes in the list.
    
    Parameter<int>* remove (int index);
    /*< Removes the oldEvent from the list.
        @return gets nullptr if the index was out of    
            bounds. */
        
    Parameter<int>* remove (Parameter<int>* oldEvent);
    /*< Removes the oldEvent from the list.
        @return gets nullptr if the oldEvent was null or if 
            it not in the list. */

    Parameter<int>* getControl (int index);
    /*< Gets the event at the given index.
        @return gets nullptr if the index is invalid. */
        
    Parameter<int>* getFirstControl ();
    /*< Gets the first event in the list.
        @return gets nullptr if list is empty. */

    int getMaxWordValue () const override;
    //< gets the max value of a Parameter<int> word.

    string& getLabel () const override;
    //< gets the label.

    int setLabel (const string& newLabel) override;
    /*< Sets the label to the new label.
        @return gets 0 upon success and 1 if the newLabel is
        too long.
        @see    HMIComponent::isValid (char). */
    
    void toggle () override;
    //< Toggles the state of the IButton if type is latching.

    void trigger () override;
    //< Triggers all of this macros functions.

    void press () override {}
    //< Triggered when a mode button is pressed.
    
    void depress () override {}
    //< Triggered when a mode button is depressed.
    
    void depress () override {}
    //< Triggered when a user "double clicks" a button.
    
    void printListString () const;
    //< gets a string of the row strings of the list.
    
    void printHeaderString () const override;
    //< gets the header for toStringRow ().
    
    void printRow () const override;
    //< gets a column of the values without the labels.
    
    void print() const;
    /*< Prints this object to a terminal. */

    private:
	
    std::vector<Parameter<int>*> controls;  //< A vector of Parameter<int> points.
};

}   //  _hmi



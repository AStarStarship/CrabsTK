/** Kabuki Theater
    @file    ~/Source/_hmi/Buttons/MacroButton.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

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

#include <global.h"
#include "../Button.h"
#include "../ControlLayer.h"
#include "../MacroEvent.h"

namespace _hmi { namespace Buttons {

class _HMI_API MacroButton: public IButton
/*<	Represents one of the macro buttons on the Symmetry Live!
    An MacroButton can trigger a single event, or several events at once. MacroButton  (s) do different
    things in different modes:
    -# Mix mode: the software record enables the track  (i.e. in HUI mode, ect) and switches the template.
    -# Aux Mode: The macro buttons cycle through aux sends A, B, C, ..., Z.
    -# Modes A & B: the macro buttons switch between different banks of pages.
    -# DSP Mode: This feature not yet implemented. This is where the controls auto-assign their parameters to
                 what ever object the user clicks on in the DAW application. */
{
    public:


    MacroButton  (const char* initLabel=T::empty, const MacroEvent& initMacro=MacroEvent ());
    // Constructor.
    
    MacroButton  (const MacroButton& copyMacro);
    //< Copy constructor.
    
	~MacroButton ();
	//< Deconstructor.
    

    virtual void press ();
    //< Function that is triggered when a mode button is depressed.
    
    virtual void depress ();
    //< Funcation that is triggered when a user "double clicks" a button.
    
    virtual void doublePress ();
    //< Function that is triggered when a mode button is depressed.

    const MacroEvent& macro ();
    //< Gets th buttonMacro.

    void print ();
	/*< Prints this object to the stdout. */

    private:

    MacroEvent buttonMacro;     //< The macro that this button triggers.
};

}   //< namespace Buttons
}   //< namespace _hmi


/** Kabuki Theater
    @file    /.../Source/_Theater/HMI/Buttons/MacroButton.hpp
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

#include <global.hpp"
#include "../Button.hpp"
#include "../ControlLayer.hpp"
#include "../MacroEvent.hpp"

namespace _Theater { namespace _HMI { namespace Buttons {

/*  @class _KabukiTheater_   MacroButton
    @breif   Represents one of the macro buttons on the Symmetry Live!
    @details An MacroButton can trigger a single event, or several events at once. MacroButton  (s) do different
    things in different modes:
    -# Mix mode: the software record enables the track  (i.e. in HUI mode, ect) and switches the template.
    -# Aux Mode: The macro buttons cycle through aux sends A, B, C, ..., Z.
    -# Modes A & B: the macro buttons switch between different banks of pages.
    -# DSP Mode: This feature not yet implemented. This is where the controls auto-assign their parameters to
                 what ever object the user clicks on in the DAW application. */


class _KabukiTheater_ MacroButton: public Button
{
    public:

    // An enumberated list of the different types of MacroButton  (s)
    typedef enum Type { normal_macro = 0, rec_enable_macro, keyboard_shortcut_macro, combo_macro };

    MacroButton  (const char* initLabel=T::empty, const MacroEvent& initMacro=MacroEvent ());
    // Constructor.
    
    MacroButton  (const MacroButton& copyMacro);
    //< Copy constructor.
    
   ~MacroButton ();
   //< Deconstructor.
    

    virtual void Press ();
    //< Function that is triggered when a mode button is depressed.
    
    virtual void Depress ();
    //< Funcation that is triggered when a user "double clicks" a button.
    
    virtual void Depress ();
    //< Function that is triggered when a mode button is depressed.

    const MacroEvent& macro ();
    //< Gets th buttonMacro.

    const char* ToString ();
    //< Gets a string representation of this MacroButton.

    private:

    MacroEvent buttonMacro;     //< The macro that this button triggers.
};



#ifdef __TESTING_UNIT__controls__

class _KabukiTheater_ __MacroButton__UnitTest__  : public UnitTest
{
public:
    __MacroButton__UnitTest__ ()
    : UnitTest ("Testing Controls::MacroButton")
    {
    }

    void runTest ()
    {
        logMessage  (T  (underLnBreak> << "Testing Macro class _KabukiTheater_\n\n    Creating test macro... ");
        MacroButton testMacro = MacroButton ();
        cout << "success\n\n";

        logMessage  (testMacro->ToString () << "\n\n    Testing copy constructor...\n\n");

        MacroButton testMacroCopy = MacroButton  (testMacro);
        logMessage  (testMacroCopy->ToString ());

        logMessage ("\n\nDone testing Template class _KabukiTheater_\n");
    }
};

#endif  //< __TESTING_UNIT__controls__

}   //< namespace Buttons
}   //< namespace _Theater { namespace _HMI

